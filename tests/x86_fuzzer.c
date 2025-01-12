#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "x86.h"

#include "llvm-c/Disassembler.h"
#include "llvm-c/Support.h"
#include "llvm-c/Target.h"

#define array_size(arr) sizeof(arr)/sizeof(arr[0])

static const int hexcols = 12;

static void format_hex(const uchar *data, size_t offset, size_t sz,
    int show_off, int show_hex)
{
    const char* hexdigits = "0123456789abcdef";
    size_t nbytes = sz < hexcols ? sz : hexcols;
    if (show_off) {
        printf("%08zx:", offset);
    }
    if (show_hex) {
        for (size_t i = 0; i < nbytes; i++) {
            printf("%c%c%c", i == 0 ? '\t' : ' ',
                hexdigits[(data[offset+i] >> 4) & 15],
                hexdigits[(data[offset+i] >> 0) & 15]);
        }
        size_t indent = (hexcols - nbytes) * 3 + 8 - (hexcols * 3) % 8;
        for (size_t i = 0; i < indent; i++) {
            printf(" ");
        }
    }
}

static int llvm_disasm_dump(LLVMDisasmContextRef ctx, const uint8_t *data,
    size_t data_len, int show_off, int show_hex)
{
    char text[128];
    size_t offset = 0;
    while (offset < data_len) {
        size_t sz = LLVMDisasmInstruction(ctx, (uint8_t *)data + offset,
            data_len - offset, 0, text, sizeof(text));
        if (sz == 0) break;
        if (show_off || show_hex) {
            format_hex(data, offset, sz, show_off, show_hex);
        }
        puts(text);
        if (show_off || show_hex) {
            while (sz > hexcols) {
                offset += hexcols; sz -= hexcols;
                format_hex(data, offset, sz, show_off, show_hex);
                puts("");
            }
        }
        offset += sz;
    };
    return 0;
}

static void x86_disasm_dump(x86_ctx *ctx, uchar *insn, size_t insnlen,
    int show_off, int show_hex)
{
    x86_buffer buf;
    x86_codec codec;
    char text[128];
    size_t nbytes = 0, offset = 0;
    int ret;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);

    do {
        ret = x86_codec_read(ctx, &buf, &codec, &nbytes);
        if (ret == 0) {
            x86_format_op(text, sizeof(text), ctx, &codec);
        } else {
            break;
        }
        if (show_off || show_hex) {
            format_hex(insn, offset, nbytes, show_off, show_hex);
        }
        printf("\t%s\n", text);
        if (show_off || show_hex) {
            while (nbytes > hexcols) {
                offset += hexcols; nbytes -= hexcols;
                format_hex(insn, offset, nbytes, show_off, show_hex);
                puts("");
            }
        }
        offset += nbytes;
    } while (offset < insnlen);
}

static size_t llvm_disasm(LLVMDisasmContextRef ctx, const uint8_t *data,
    size_t data_len, char *text, size_t text_len)
{
    return LLVMDisasmInstruction(ctx, (uint8_t *)data,
            data_len, 0, text, text_len);
}

static size_t x86_disasm(x86_ctx *ctx, uchar *insn, size_t insnlen,
    char *text, size_t text_len)
{
    x86_buffer buf;
    x86_codec codec;
    size_t nbytes = 0;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);
    if (x86_codec_read(ctx, &buf, &codec, &nbytes) == 0) {
        text[0] = '\t';
        x86_format_op(text + 1, text_len - 1, ctx, &codec);
        return nbytes;
    } else {
        text[0] = '\0';
    }

    return 0;
}

typedef struct x86_operand_gen x86_operand_gen;
typedef int (*x86_operand_pred)(x86_operand_gen *, size_t);

enum
{
    x86_gen_osize,
    x86_gen_w,
    x86_gen_i,
    x86_gen_r,
    x86_gen_mod,
    x86_gen_rm,
    x86_gen_disp,
    x86_gen_s,
    x86_gen_b,
    x86_gen_x,
    x86_gen_v,
    x86_gen_k,
    x86_gen_brd,
    x86_gen_is4,
    x86_gen_ime,
};

struct x86_operand_gen
{
    size_t idx;
    uint type;
    x86_operand_pred pred;
    ullong value;
    ullong start;
    ullong end;
    ullong step;
};

const char *x86_generator_name(uint type)
{
    switch (type) {
    case x86_gen_osize: return "osize";
    case x86_gen_w: return "w";
    case x86_gen_i: return "i";
    case x86_gen_r: return "r";
    case x86_gen_mod: return "mod";
    case x86_gen_rm: return "rm";
    case x86_gen_disp: return "disp";
    case x86_gen_s: return "s";
    case x86_gen_b: return "b";
    case x86_gen_x: return "x";
    case x86_gen_v: return "v";
    case x86_gen_k: return "k";
    case x86_gen_brd: return "brd";
    case x86_gen_is4: return "is4";
    case x86_gen_ime: return "ime";
    default: return "";
    }
}

void x86_new_generator(x86_operand_gen *gen, size_t *count, uint type,
    x86_operand_pred pred, ullong value, ullong start, ullong end, ullong step)
{
    x86_operand_gen v = { *count, type, pred, value, start, end, step };
    gen[(*count)++] = v;
}

int x86_osize_w_pred(x86_operand_gen *generators, size_t idx)
{
    int w = -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_w)
            w = (int)generators[i].value;
    }
    if (w <= 0) {
        return 1;
    } else {
        generators[idx].value = 0;
        return 0;
    }
}

int x86_mrm_sib_scale_pred(x86_operand_gen *generators, size_t idx)
{
    int mod = -1, rm = -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_mod) {
            mod = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_rm) {
            rm = (int)generators[i].value;
        }
    }
    if (mod != x86_mod_reg && rm == x86_rm_sp_sib) {
        return 1;
    } else {
        generators[idx].value = 0;
        return 0;
    }
}

int x86_mrm_sib_reg_pred(x86_operand_gen *generators, size_t idx)
{
    int mod = -1, rm = -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_mod) {
            mod = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_rm) {
            rm = (int)generators[i].value;
        }
    }
    if (mod != x86_mod_reg && rm == x86_rm_sp_sib) {
        return 1;
    } else {
        generators[idx].value = 0;
        return 0;
    }
}

int x86_mrm_disp_pred(x86_operand_gen *generators, size_t idx)
{
    int mod = -1, rm = -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_mod) {
            mod = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_rm) {
            rm = (int)generators[i].value;
        }
    }
    switch (mod) {
    case x86_mod_disp0:
        if (rm == x86_rm_bp_disp0) {
            generators[idx].step = 1ull << 30;
            generators[idx].end = 3ull << 30;
            return 1;
        } else {
            generators[idx].value = 0;
            break;
        }
    case x86_mod_disp8:
        generators[idx].step = 1ull << 6;
        generators[idx].end = 3ull << 6;
        return 1;
    case x86_mod_dispw:
        generators[idx].step = 1ull << 30;
        generators[idx].end = 3ull << 30;
        return 1;
    case x86_mod_reg:
        generators[idx].value = 0;
        break;
    }
    return 0;
}

int x86_imm_rax_pred(x86_operand_gen *generators, size_t idx)
{
    int r = -1, rm = -1, b = -1, x = -1, v= -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_r) {
            r = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_rm) {
            rm = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_b) {
            b = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_x) {
            x = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_v) {
            v = (int)generators[i].value;
        }
    }
    return (r == -1 || r == x86_ax) &&
           (rm == -1 || rm == x86_ax) &&
           (b == -1 || b == x86_ax) &&
           (x == -1 || x == x86_ax) &&
           (v == -1 || v == x86_ax);
}

int x86_imm_rax_iw_osize_pred(x86_operand_gen *generators, size_t idx)
{
    int osize = -1, r = -1, rm = -1, b = -1, x = -1, v= -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_osize) {
            osize = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_r) {
            r = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_rm) {
            rm = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_b) {
            b = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_x) {
            x = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_v) {
            v = (int)generators[i].value;
        }
    }
    if (osize == 1) {
        generators[idx].step = 1ull << 14;
        generators[idx].end = 3ull << 14;
    } else {
        generators[idx].step = 1ull << 30;
        generators[idx].end = 3ull << 30;
    }
    return (r == -1 || r == x86_ax) &&
           (rm == -1 || rm == x86_ax) &&
           (b == -1 || b == x86_ax) &&
           (x == -1 || x == x86_ax) &&
           (v == -1 || v == x86_ax);
}

int x86_brd_mem_pred(x86_operand_gen *generators, size_t idx)
{
    int mod = -1, rm = -1, brd = -1;
    for (size_t i = 0; i < idx; i++) {
        if (generators[i].type == x86_gen_mod) {
            mod = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_rm) {
            rm = (int)generators[i].value;
        }
        if (generators[i].type == x86_gen_brd) {
            brd = (int)generators[i].value;
        }
    }
    if (brd == 0 || (brd == 1 && mod != x86_mod_reg && rm == x86_rm_sp_sib)) {
        return 1;
    } else {
        generators[idx].value = 0;
        return 0;
    }
}

void x86_gen_synth(const x86_opc_data *d, x86_operand_gen *gen, size_t *count)
{
    const x86_opr_data *o = x86_opr_table + d->opr;
    const x86_ord_data *s = x86_ord_table + d->ord;

    /* currently only 64-bit mode generation is supported */

    switch (x86_enc_width(d->enc)) {
    case x86_enc_w_ww:
        if (x86_enc_has_o16(d->enc)) {
            x86_new_generator(gen, count, x86_gen_osize, NULL, 1, 1, 1, 1);
        }
        else if (x86_enc_has_o32(d->enc));
        else if (x86_enc_has_o64(d->enc));
        else {
            x86_new_generator(gen, count, x86_gen_osize, NULL, 0, 0, 1, 1);
        }
        break;
    case x86_enc_w_wx:
        if (x86_enc_has_o16(d->enc)) {
            x86_new_generator(gen, count, x86_gen_osize, NULL, 1, 1, 1, 1);
        }
        else if (x86_enc_has_o32(d->enc));
        else if (x86_enc_has_o64(d->enc)) {
            x86_new_generator(gen, count, x86_gen_w, NULL, 1, 1, 1, 1);
        }
        else {
            x86_new_generator(gen, count, x86_gen_w, NULL, 0, 0, 1, 1);
            x86_new_generator(gen, count, x86_gen_osize, x86_osize_w_pred, 0, 0, 1, 1);
        }
        break;
    case x86_enc_w_w1:
        x86_new_generator(gen, count, x86_gen_w, NULL, 1, 1, 1, 1);
        break;
    default:
        x86_new_generator(gen, count, x86_gen_w, NULL, 0, 0, 0, 1);
        break;
    }

    uint rs = 4, isreg = 0, ismem = 0, issib = 0, hask = 0;
    for (size_t i = 0; i < array_size(o->opr) && o->opr[i]; i++)
    {
        uint ord = s->ord[i], opr = o->opr[i];
        uint ord_type = ord & x86_ord_type_mask;
        uint opr_type = opr & x86_opr_type_mask;
        switch(ord_type) {
        case x86_ord_const:
            break;
        case x86_ord_imm:
            if ((ord & ~x86_ord_flag_mask) == x86_ord_is4) {
                x86_new_generator(gen, count, x86_gen_is4,
                    NULL, 0, 0, 15, rs);
            } else if ((ord & ~x86_ord_flag_mask) == x86_ord_ime) {
                switch (x86_enc_imm2(d->enc)) {
                case x86_enc_j_ib:
                    x86_new_generator(gen, count, x86_gen_ime,
                        x86_imm_rax_pred, 0, 0, 3ull << 6, 1ull << 6);
                    break;
                case x86_enc_j_i16:
                    x86_new_generator(gen, count, x86_gen_ime,
                        x86_imm_rax_pred, 0, 0, 3ull << 14, 1ull << 14);
                    break;
                }
            } else {
                switch (x86_enc_imm(d->enc)) {
                case x86_enc_i_ib:
                    x86_new_generator(gen, count, x86_gen_i,
                        x86_imm_rax_pred, 0, 0, 3ull << 6, 1ull << 6);
                    break;
                case x86_enc_i_iw:
                    x86_new_generator(gen, count, x86_gen_i,
                        x86_imm_rax_iw_osize_pred, 0, 0, 3ull << 30, 1ull << 30);
                    break;
                case x86_enc_i_iwd:
                    x86_new_generator(gen, count, x86_gen_i,
                        x86_imm_rax_pred, 0, 0, 3ull << 30, 1ull << 30);
                    break;
                case x86_enc_i_i16:
                    x86_new_generator(gen, count, x86_gen_i,
                        x86_imm_rax_pred, 0, 0, 3ull << 14, 1ull << 14);
                    break;
                case x86_enc_i_i32:
                    x86_new_generator(gen, count, x86_gen_i,
                        x86_imm_rax_pred, 0, 0, 3ull << 30, 1ull << 30);
                    break;
                case x86_enc_i_i64:
                    x86_new_generator(gen, count, x86_gen_i,
                        x86_imm_rax_pred, 0, 0, 3ull << 62, 1ull << 62);
                    break;
                }
            }
            break;
        case x86_ord_reg:
            hask = (o->opr[i] & x86_opr_flag_k) != 0;
            if (hask) {
                x86_new_generator(gen, count, x86_gen_k, NULL, 1, 1, 1, 1);
            }
            if (opr_type == x86_opr_k) {
                x86_new_generator(gen, count, x86_gen_r, NULL, 0, 0, 7, rs);
            } else {
                x86_new_generator(gen, count, x86_gen_r, NULL, 0, 0, 15, rs);
            }
            break;
        case x86_ord_opr:
            x86_new_generator(gen, count, x86_gen_b, NULL, 0, 0, 15, rs);
            break;
        case x86_ord_mrm:
            isreg = (o->opr[i] & x86_opr_type_mask) >= x86_opr_reg;
            ismem = (o->opr[i] & x86_opr_mem) != 0;
            issib = (s->ord[i] & ~x86_ord_flag_mask) == x86_ord_sib;
            hask = (o->opr[i] & x86_opr_flag_k) != 0;
            if (hask) {
                x86_new_generator(gen, count, x86_gen_k, NULL, 1, 1, 1, 1);
            }
            if (isreg && ismem && !issib) {
                x86_new_generator(gen, count, x86_gen_mod, NULL, 0, 0, 3, 1);
            } else if (isreg && !issib) {
                x86_new_generator(gen, count, x86_gen_mod, NULL, 3, 3, 3, 1);
            } else if (issib || ismem) {
                x86_new_generator(gen, count, x86_gen_mod, NULL, 0, 0, 2, 1);
            }
            if (issib) {
                x86_new_generator(gen, count, x86_gen_rm, NULL, 4, 4, 4, 1);
            } else if (opr_type == x86_opr_k) {
                x86_new_generator(gen, count, x86_gen_r, NULL, 0, 0, 7, rs);
            } else {
                x86_new_generator(gen, count, x86_gen_rm, NULL, 0, 0, 15, rs);
            }
            if (ismem) {
                x86_new_generator(gen, count, x86_gen_s,
                    x86_mrm_sib_scale_pred, 0, 0, 3, 1);
                x86_new_generator(gen, count, x86_gen_b,
                    x86_mrm_sib_reg_pred, 0, 0, 15, 1);
                x86_new_generator(gen, count, x86_gen_x,
                    x86_mrm_sib_reg_pred, 0, 0, 15, rs);
                x86_new_generator(gen, count, x86_gen_disp,
                    x86_mrm_disp_pred, 0, 0, 3ull << 30, 1ull << 30);
                }
            break;
        case x86_ord_vec:
            hask = (o->opr[i] & x86_opr_flag_k) != 0;
            if (hask) {
                x86_new_generator(gen, count, x86_gen_k, NULL, 1, 1, 1, 1);
            }
            if (opr_type == x86_opr_k) {
                x86_new_generator(gen, count, x86_gen_v, NULL, 0, 0, 7, rs);
            } else {
                x86_new_generator(gen, count, x86_gen_v, NULL, 0, 0, 15, rs);
            }
            break;
        default:
            break;
        }
        if (opr & x86_opr_bcst) {
            x86_new_generator(gen, count, x86_gen_brd, x86_brd_mem_pred, 0, 0, 1, 1);
        }
    }
}

int x86_gen_next(x86_operand_gen *gen, size_t count)
{
    for (size_t i = count-1; i < count; i--) {
        if (gen[i].pred && gen[i].pred(gen, i) == 0) continue;
        ullong value = gen[i].value;
        ullong result = value + gen[i].step;
        gen[i].value = result;
        if (gen[i].value <= gen[i].end && value < result) {
            return 1;
        } else {
            gen[i].value = gen[i].start;
        }
    }
    return 0;
}

int x86_gen_print(x86_operand_gen *gen, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        printf("%s%s=%lld",
            i > 0 ? " " : "", x86_generator_name(gen[i].type), gen[i].value);
    }
    printf("\n");
    return 0;
}

void x86_gen_codec(const x86_opc_data *d, x86_codec *c,
    x86_operand_gen *gen, size_t count)
{
    int m = 0, p = 0, l = 0, w = 0, k = 0, brd = 0, z = 0;
    int osize = 0, r = 0, mod = 0, rm = 0;
    int scale = 0, b = 0, x = 0, v = 0, disp = 0;

    memset(c, 0, sizeof(x86_codec));
    c->flags |= x86_cf_amd64;

    for (size_t i = 0; i < count; i++) {
        switch (gen[i].type) {
        case x86_gen_osize:
            osize = (int)gen[i].value;
            break;
        case x86_gen_w:
            w = (int)gen[i].value;
            break;
        case x86_gen_i:
            switch (x86_enc_imm(d->enc)) {
            case x86_enc_i_ib:
                c->imm32 = (i8)gen[i].value;
                c->flags |= x86_ci_ib;
                break;
            case x86_enc_i_iw:
                c->imm32 = (i32)gen[i].value;
                c->flags |= x86_ci_iw;
                break;
            case x86_enc_i_iwd:
                c->imm32 = (i32)gen[i].value;
                c->flags |= x86_ci_iwd;
                break;
            case x86_enc_i_i16:
                c->imm32 = (i16)gen[i].value;
                c->flags |= x86_ci_i16;
                break;
            case x86_enc_i_i32:
                c->imm32 = (i32)gen[i].value;
                c->flags |= x86_ci_i32;
                break;
            case x86_enc_i_i64:
                c->imm64 = (i64)gen[i].value;
                c->flags |= x86_ci_i64;
                break;
            default:
                break;
            }
            break;
        case x86_gen_is4:
            c->imm32 = (int)gen[i].value;
            c->flags |= x86_ci_ib;
            break;
        case x86_gen_ime:
            switch (x86_enc_imm2(d->enc)) {
            case x86_enc_j_ib:
                c->imm2 = (int)gen[i].value;
                c->flags |= x86_cj_ib;
                break;
            case x86_enc_j_i16:
                c->imm2 = (int)gen[i].value;
                c->flags |= x86_cj_i16;
                break;
            }
            break;
        case x86_gen_r:
            r = (int)gen[i].value;
            break;
        case x86_gen_mod:
            mod = (int)gen[i].value;
            break;
        case x86_gen_rm:
            rm = (int)gen[i].value;
            break;
        case x86_gen_s:
            scale = (int)gen[i].value;
            break;
        case x86_gen_b:
            b = (int)gen[i].value;
            break;
        case x86_gen_x:
            x = (int)gen[i].value;
            break;
        case x86_gen_disp:
            disp = (int)gen[i].value;
            break;
        case x86_gen_v:
            v = (int)gen[i].value;
            break;
        case x86_gen_k:
            k = (int)gen[i].value;
            break;
        case x86_gen_brd:
            brd = (int)gen[i].value;
            break;
        default:
            break;
        }
    }

    /* first opcode byte */
    switch (x86_enc_opcode(d->enc)) {
    case x86_enc_o_opcode_r:
        c->opc[0] = d->opc[0] + (b & 7);
        c->opclen = 1;
        break;
    default:
        c->opc[0] = d->opc[0];
        c->opclen = 1;
        break;
    }

    /* modrm or second opcode byte */
    switch (x86_enc_func(d->enc)) {
    case x86_enc_f_modrm_n:
        r = x86_modrm_reg(d->opc[1]);
    case x86_enc_f_modrm_r:
        c->modrm = x86_enc_modrm(mod, r, rm);
        c->flags |= x86_cf_modrm;
        break;
    case x86_enc_f_opcode:
        c->opc[1] = d->opc[1];
        c->opclen = 2;
        break;
    case x86_enc_f_opcode_r:
        c->opc[1] = d->opc[1] + (b & 7);
        c->opclen = 2;
        break;
    default:
        break;
    }

    /* final encoding */
    switch (x86_enc_type(d->enc)) {
    case x86_enc_t_lex:
        /* prefix */
        switch (x86_enc_prefix(d->enc)) {
        case x86_enc_p_66: c->flags |= x86_cp_osize; break;
        case x86_enc_p_f3: c->flags |= x86_cp_rep; break;
        case x86_enc_p_f2: c->flags |= x86_cp_repne; break;
        case x86_enc_p_9b: c->flags |= x86_cp_wait; break;
        default: break;
        }
        if (osize == 1)  c->flags |= x86_cp_osize;

        /* REX */
        switch (x86_enc_width(d->enc)) {
        case x86_enc_w_w1: w = 1; break;
        default: break;
        }
        if (w == 1 || r >= 8 || b >= 8 || x >= 8) {
            c->flags |= x86_ce_rex;
            c->rex = x86_enc_rex(w, r, x, b);
        }

        /* map */
        switch (x86_enc_map(d->enc)) {
        case x86_enc_m_0f: c->flags |= x86_cm_0f; break;
        case  x86_enc_m_0f38: c->flags |= x86_cm_0f38; break;
        case  x86_enc_m_0f3a: c->flags |= x86_cm_0f3a; break;
        default: break;
        }

        /* SIB */
        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            if (rm == x86_rm_sp_sib) {
                c->sib = x86_enc_sib(scale, x, b);
            }
            break;
        default: break;
        }

        /* displacement */
        switch (mod) {
        case x86_mod_disp0:
            if (rm == x86_rm_bp_disp0) c->disp32 = (i32)disp;
            break;
        case x86_mod_disp8: c->disp32 = (i8)disp; break;
        case x86_mod_dispw: c->disp32 = (i32)disp; break;
        default: break;
        }
        break;
    case x86_enc_t_vex:
        /* set m, p, l, w */
        p = (d->enc & x86_enc_p_mask) >> x86_enc_p_shift;
        m = (d->enc & x86_enc_m_mask) >> x86_enc_m_shift;
        switch (x86_enc_length(d->enc)) {
        case x86_enc_l_l1:
        case x86_enc_l_256: l = 1; break;
        case x86_enc_l_512: l = 2; break;
        }
        switch (x86_enc_width(d->enc)) {
        case x86_enc_w_w1: w = 1; break;
        default: break;
        }
        if (w == 0 && m == 0 && x < 8 && b < 8) {
            c->vex2 = x86_enc_vex2(p, l, r, v);
            c->flags |= x86_ce_vex2;
        } else {
            c->vex3 = x86_enc_vex3(m, p, l, w, r, x, b, v);
            c->flags |= x86_ce_vex3;
        }

        /* SIB */
        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            if (rm == x86_rm_sp_sib) {
                c->sib = x86_enc_sib(scale, x, b);
            }
            break;
        default: break;
        }

        /* displacement */
        switch (mod) {
        case x86_mod_disp0:
            if (rm == x86_rm_bp_disp0) c->disp32 = (i32)disp;
            break;
        case x86_mod_disp8: c->disp32 = (i8)disp; break;
        case x86_mod_dispw: c->disp32 = (i32)disp; break;
        default: break;
        }
        break;
    case x86_enc_t_evex:
        /* set m, p, l, w, k, brd, z */
        p = (d->enc & x86_enc_p_mask) >> x86_enc_p_shift;
        m = (d->enc & x86_enc_m_mask) >> x86_enc_m_shift;
        switch (x86_enc_length(d->enc)) {
        case x86_enc_l_l1:
        case x86_enc_l_256: l = 1; break;
        case x86_enc_l_512: l = 2; break;
        }
        switch (x86_enc_width(d->enc)) {
        case x86_enc_w_w1: w = 1; break;
        default: break;
        }
        c->evex = x86_enc_evex(m, p, l, w, r, x, b, v, k, brd, z);
        c->flags |= x86_ce_evex;

        /* SIB */
        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            if (rm == x86_rm_sp_sib) {
                c->sib = x86_enc_sib(scale, x, b);
            }
            break;
        default: break;
        }

        /* displacement */
        switch (mod) {
        case x86_mod_disp0:
            if (rm == x86_rm_bp_disp0) c->disp32 = (i32)disp;
            break;
        case x86_mod_disp8: c->disp32 = (i8)disp; break;
        case x86_mod_dispw: c->disp32 = (i32)disp; break;
        default: break;
        }
        break;
    default:
        break;
    }
}

void x86_evaluate_opcode(x86_ctx *ctx_x86, LLVMDisasmContextRef ctx_llvm,
    const x86_opc_data *d, uint debug, uint error_limit)
{
    x86_print_op(d, 1, 0);
    x86_operand_gen gen[13] = { 0 };
    size_t count = 0;
    size_t error_count = 0;
    x86_gen_synth(d, gen, &count);
    do {
        x86_codec c;
        x86_buffer b;
        uchar buf[32] = { 0 };
        char ours[128], llvm[128];
        size_t nbytes, ours_len, llvm_len;
        int pass;
        x86_buffer_init_ex(&b, buf, 0, 0);
        if (debug) {
            x86_gen_print(gen, count);
        }
        x86_gen_codec(d, &c, gen, count);
        x86_codec_write(ctx_x86, &b, c, &nbytes);
        ours_len = x86_disasm(ctx_x86, buf, nbytes, ours, sizeof(ours));
        llvm_len = llvm_disasm(ctx_llvm, buf, nbytes, llvm, sizeof(llvm));
        pass = ours_len == llvm_len && strcmp(ours, llvm) == 0;
        if (!pass) {
            error_count++;
        }
        if (!pass && error_count <= error_limit) {
            printf("llvm:");
            llvm_disasm_dump(ctx_llvm, buf, nbytes, 0, 1);
            printf("ours:");
            x86_disasm_dump(ctx_x86, buf, nbytes, 0, 1);
        }
        if (!pass && error_count == error_limit) {
            printf("[reached limit of %d errors]\n", error_limit);
            return;
        }
    } while (x86_gen_next(gen, count));
}

void x86_loop_opcodes(x86_ctx *ctx_x86, LLVMDisasmContextRef ctx_llvm,
    uint modes, uint sort, uint debug, uint error_limit)
{
    x86_table_idx tab = x86_opc_table_filter(x86_opc_table_identity(), modes);
    if (sort) tab = x86_opc_table_sorted(tab, sort);
    for(size_t i = 0; i < tab.count; i++) {
        const x86_opc_data *d = x86_opc_table + tab.idx[i];
        x86_evaluate_opcode(ctx_x86, ctx_llvm, d, debug, error_limit);
    }
    free(tab.idx);
}

void print_help(const char *progname)
{
    fprintf(stderr, "%s [-a] [-n] [-d] [-h] [-l <limit>]\n", progname);
}

int main(int argc, char **argv)
{
    uint help = 0, debug = 0;
    uint sort = x86_sort_none;
    uint limit = 25;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) sort = x86_sort_alpha;
        else if (strcmp(argv[i], "-n") == 0) sort = x86_sort_numeric;
        else if (strcmp(argv[i], "-d") == 0) debug = 1;
        else if (strcmp(argv[i], "-h") == 0) help = 1;
        else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            limit = atoi(argv[++i]);
        }
        else {
            fprintf(stderr, "*** unknown option: %s\n", argv[i]);
            help = 1;
            break;
        }
    }

    const char* args[] = { argv[0], "--x86-asm-syntax=intel" };
    LLVMParseCommandLineOptions(2, args, "");

    LLVMInitializeAllTargetInfos();
    LLVMInitializeAllTargetMCs();
    LLVMInitializeAllDisassemblers();

    if (help) {
        print_help(argv[0]);
    } else {
        x86_ctx *ctx_x86 = x86_ctx_create(x86_modes_64);
        LLVMDisasmContextRef ctx_llvm =  LLVMCreateDisasmCPUFeatures
            ("x86_64", "", "", NULL, 0, NULL, NULL);
        x86_loop_opcodes(ctx_x86, ctx_llvm, x86_modes_64, sort, debug, limit);
        LLVMDisasmDispose(ctx_llvm);
        x86_ctx_destroy(ctx_x86);
    }
}
