#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#include "x86.h"

char* mode_name(uint mode)
{
    static char buf[64];
    x86_mode_name(buf, sizeof(buf), mode, "|");
    printf("%08x %s\n", mode, buf);
    return buf;
}

char* prefix_name(uint prefix)
{
    static char buf[64];
    x86_prefix_name(buf, sizeof(buf), prefix, "|");
    printf("%08x %s\n", prefix, buf);
    return buf;
}

char* prefix_namen(uint prefix)
{
    static char buf[64];
    x86_prefix_namen(buf, sizeof(buf), prefix, "|");
    printf("%08x %s\n", prefix, buf);
    return buf;
}

char* ord_name(uint ord)
{
    static char buf[64];
    x86_ord_name(buf, sizeof(buf), ord, "|");
    printf("%08x %s\n", ord, buf);
    return buf;
}

char* arg_name(uint arg)
{
    static char buf[64];
    x86_opr_name(buf, sizeof(buf), arg);
    printf("%08x %s\n", arg, buf);
    return buf;
}

char* enc_name(uint enc)
{
    static char buf[64];
    x86_enc_name(buf, sizeof(buf), enc);
    printf("%08x %s\n", enc, buf);
    return buf;
}

const char* reg_name(uint reg)
{
    return x86_reg_name(reg);
}

void test_x86_sizeof()
{
    printf("sizeof(x86_mem)=%zu\n", sizeof(x86_mem));
    printf("sizeof(x86_codec)=%zu\n", sizeof(x86_codec));
}

void test_x86_mode_name()
{
    printf("\n# x86_mode_name\n");
    assert(strcmp(mode_name(x86_modes_16), "16") == 0);
    assert(strcmp(mode_name(x86_modes_32), "32") == 0);
    assert(strcmp(mode_name(x86_modes_64), "64") == 0);
}


void test_x86_prefix_name()
{
    printf("\n# x86_prefix_name\n");
    assert(strcmp(prefix_name(1 << x86_pn_es), "es") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_cs), "cs") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_ss), "ss") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_ds), "ds") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_rex_b), "rex.b") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_rex_x), "rex.x") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_rex_r), "rex.r") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_rex_w), "rex.w") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_evex), "evex") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_fs), "fs") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_gs), "gs") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_osize), "osize") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_asize), "asize") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_wait), "wait") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_vex3), "vex3") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_vex2), "vex2") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_rex2), "rex2") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_lock), "lock") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_repne), "repne") == 0);
    assert(strcmp(prefix_name(1 << x86_pn_rep), "rep") == 0);
    assert(strcmp(prefix_name((1 << x86_pn_es) |
                              (1 << x86_pn_cs) |
                              (1 << x86_pn_ss) |
                              (1 << x86_pn_ds) |
                              (1 << x86_pn_fs) |
                              (1 << x86_pn_gs)), "es|cs|ss|ds|fs|gs") == 0);
}

void test_x86_prefix_namen()
{
    printf("\n# x86_prefix_namen\n");
    assert(strcmp(prefix_namen(1 << x86_pn_26), "26") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_2e), "2e") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_36), "36") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_3e), "3e") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_41), "41") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_42), "42") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_44), "44") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_48), "48") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_62), "62") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_64), "64") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_65), "65") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_66), "66") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_67), "67") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_9b), "9b") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_c4), "c4") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_c5), "c5") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_d5), "d5") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_f0), "f0") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_f2), "f2") == 0);
    assert(strcmp(prefix_namen(1 << x86_pn_f3), "f3") == 0);
    assert(strcmp(prefix_namen((1 << x86_pn_es) |
                               (1 << x86_pn_cs) |
                               (1 << x86_pn_ss) |
                               (1 << x86_pn_ds) |
                               (1 << x86_pn_fs) |
                               (1 << x86_pn_gs)), "26|2e|36|3e|64|65") == 0);
}

void test_x86_ord_name()
{
    printf("\n# x86_ord_name\n");
    assert(strcmp(ord_name(x86_ord_one), "one") == 0);
    assert(strcmp(ord_name(x86_ord_imm), "imm") == 0);
    assert(strcmp(ord_name(x86_ord_reg), "reg") == 0);
    assert(strcmp(ord_name(x86_ord_mrm), "mrm") == 0);
    assert(strcmp(ord_name(x86_ord_vec), "vec") == 0);
    assert(strcmp(ord_name(x86_ord_is4), "is4") == 0);
    assert(strcmp(ord_name(x86_ord_ime), "ime") == 0);
    assert(strcmp(ord_name(x86_ord_sib), "sib") == 0);
    assert(strcmp(ord_name(x86_ord_rax), "rax") == 0);
    assert(strcmp(ord_name(x86_ord_rcx), "rcx") == 0);
    assert(strcmp(ord_name(x86_ord_rdx), "rdx") == 0);
    assert(strcmp(ord_name(x86_ord_rbx), "rbx") == 0);
    assert(strcmp(ord_name(x86_ord_rsp), "rsp") == 0);
    assert(strcmp(ord_name(x86_ord_rbp), "rbp") == 0);
    assert(strcmp(ord_name(x86_ord_rsi), "rsi") == 0);
    assert(strcmp(ord_name(x86_ord_rdi), "rdi") == 0);
    assert(strcmp(ord_name(x86_ord_st0), "st0") == 0);
    assert(strcmp(ord_name(x86_ord_stx), "stx") == 0);
    assert(strcmp(ord_name(x86_ord_xmm0), "xmm0") == 0);
    assert(strcmp(ord_name(x86_ord_xmm0_7), "xmm0_7") == 0);
    assert(strcmp(ord_name(x86_ord_mxcsr), "mxcsr") == 0);
    assert(strcmp(ord_name(x86_ord_rflags), "rflags") == 0);
    assert(strcmp(ord_name(x86_ord_reg|x86_ord_r), "reg|r") == 0);
    assert(strcmp(ord_name(x86_ord_reg|x86_ord_w), "reg|w") == 0);
    assert(strcmp(ord_name(x86_ord_reg|x86_ord_rw), "reg|rw") == 0);
    assert(strcmp(ord_name(x86_ord_mrm|x86_ord_r), "mrm|r") == 0);
    assert(strcmp(ord_name(x86_ord_mrm|x86_ord_w), "mrm|w") == 0);
    assert(strcmp(ord_name(x86_ord_mrm|x86_ord_rw), "mrm|rw") == 0);
    assert(strcmp(ord_name(x86_ord_sib|x86_ord_r), "sib|r") == 0);
    assert(strcmp(ord_name(x86_ord_sib|x86_ord_w), "sib|w") == 0);
    assert(strcmp(ord_name(x86_ord_sib|x86_ord_rw), "sib|rw") == 0);
    assert(strcmp(ord_name(x86_ord_is4|x86_ord_r), "is4|r") == 0);
    assert(strcmp(ord_name(x86_ord_is4|x86_ord_w), "is4|w") == 0);
    assert(strcmp(ord_name(x86_ord_is4|x86_ord_rw), "is4|rw") == 0);
    assert(strcmp(ord_name(x86_ord_vec|x86_ord_r), "vec|r") == 0);
    assert(strcmp(ord_name(x86_ord_vec|x86_ord_w), "vec|w") == 0);
    assert(strcmp(ord_name(x86_ord_vec|x86_ord_rw), "vec|rw") == 0);
}

void test_x86_opr_name()
{
    printf("\n# x86_opr_name\n");
    assert(strcmp(arg_name(x86_opr_flag_er), "{er}") == 0);
    assert(strcmp(arg_name(x86_opr_flag_k), "{k}") == 0);
    assert(strcmp(arg_name(x86_opr_flag_sae), "{sae}") == 0);
    assert(strcmp(arg_name(x86_opr_flag_z), "{z}") == 0);
    assert(strcmp(arg_name(x86_opr_flag_rs2), "{rs2}") == 0);
    assert(strcmp(arg_name(x86_opr_flag_rs4), "{rs4}") == 0);
    assert(strcmp(arg_name(x86_opr_mem), "mem") == 0);
    assert(strcmp(arg_name(x86_opr_rel), "rel") == 0);
    assert(strcmp(arg_name(x86_opr_ind), "ind") == 0);
    assert(strcmp(arg_name(x86_opr_far), "far") == 0);
    assert(strcmp(arg_name(x86_opr_bcst), "bcst") == 0);
    assert(strcmp(arg_name(x86_opr_imm), "imm") == 0);
    assert(strcmp(arg_name(x86_opr_reg), "reg") == 0);
    assert(strcmp(arg_name(x86_opr_vec), "vec") == 0);
    assert(strcmp(arg_name(x86_opr_st), "st") == 0);
    assert(strcmp(arg_name(x86_opr_k), "k") == 0);
    assert(strcmp(arg_name(x86_opr_seg), "seg") == 0);
    assert(strcmp(arg_name(x86_opr_creg), "creg") == 0);
    assert(strcmp(arg_name(x86_opr_dreg), "dreg") == 0);
    assert(strcmp(arg_name(x86_opr_bnd), "bnd") == 0);
    assert(strcmp(arg_name(x86_opr_ib), "ib") == 0);
    assert(strcmp(arg_name(x86_opr_iw), "iw") == 0);
    assert(strcmp(arg_name(x86_opr_i16), "i16") == 0);
    assert(strcmp(arg_name(x86_opr_i32), "i32") == 0);
    assert(strcmp(arg_name(x86_opr_i64), "i64") == 0);
    assert(strcmp(arg_name(x86_opr_r8), "r8") == 0);
    assert(strcmp(arg_name(x86_opr_r16), "r16") == 0);
    assert(strcmp(arg_name(x86_opr_r32), "r32") == 0);
    assert(strcmp(arg_name(x86_opr_r64), "r64") == 0);
    assert(strcmp(arg_name(x86_opr_i16e), "i16e") == 0);
    assert(strcmp(arg_name(x86_opr_rw), "rw") == 0);
    assert(strcmp(arg_name(x86_opr_mm), "mm") == 0);
    assert(strcmp(arg_name(x86_opr_xmm), "xmm") == 0);
    assert(strcmp(arg_name(x86_opr_ymm), "ymm") == 0);
    assert(strcmp(arg_name(x86_opr_zmm), "zmm") == 0);
    assert(strcmp(arg_name(x86_opr_moffs), "moffs") == 0);
    assert(strcmp(arg_name(x86_opr_rel8), "rel8") == 0);
    assert(strcmp(arg_name(x86_opr_relw), "relw") == 0);
    assert(strcmp(arg_name(x86_opr_1), "1") == 0);
    assert(strcmp(arg_name(x86_opr_reg_a), "a") == 0);
    assert(strcmp(arg_name(x86_opr_reg_c), "c") == 0);
    assert(strcmp(arg_name(x86_opr_reg_d), "d") == 0);
    assert(strcmp(arg_name(x86_opr_reg_si), "si") == 0);
    assert(strcmp(arg_name(x86_opr_reg_di), "di") == 0);
    assert(strcmp(arg_name(x86_opr_reg_di), "di") == 0);
    assert(strcmp(arg_name(x86_opr_reg_ah), "ah") == 0);
    assert(strcmp(arg_name(x86_opr_reg_st0), "st0") == 0);
    assert(strcmp(arg_name(x86_opr_reg_st1), "st1") == 0);
    assert(strcmp(arg_name(x86_opr_reg_v0), "v0") == 0);
    assert(strcmp(arg_name(x86_opr_reg_al), "al") == 0);
    assert(strcmp(arg_name(x86_opr_reg_cl), "cl") == 0);
    assert(strcmp(arg_name(x86_opr_reg_ax), "ax") == 0);
    assert(strcmp(arg_name(x86_opr_reg_cx), "cx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_dx), "dx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_bx), "bx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_eax), "eax") == 0);
    assert(strcmp(arg_name(x86_opr_reg_ecx), "ecx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_edx), "edx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_ebx), "ebx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_rax), "rax") == 0);
    assert(strcmp(arg_name(x86_opr_reg_rcx), "rcx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_rdx), "rdx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_rbx), "rbx") == 0);
    assert(strcmp(arg_name(x86_opr_reg_aw), "aw") == 0);
    assert(strcmp(arg_name(x86_opr_reg_cw), "cw") == 0);
    assert(strcmp(arg_name(x86_opr_reg_dw), "dw") == 0);
    assert(strcmp(arg_name(x86_opr_reg_bw), "bw") == 0);
    assert(strcmp(arg_name(x86_opr_reg_pa), "pa") == 0);
    assert(strcmp(arg_name(x86_opr_reg_pc), "pc") == 0);
    assert(strcmp(arg_name(x86_opr_reg_pd), "pd") == 0);
    assert(strcmp(arg_name(x86_opr_reg_pb), "pb") == 0);
    assert(strcmp(arg_name(x86_opr_reg_psi), "psi") == 0);
    assert(strcmp(arg_name(x86_opr_reg_pdi), "pdi") == 0);
    assert(strcmp(arg_name(x86_opr_reg_xmm0), "xmm0") == 0);
    assert(strcmp(arg_name(x86_opr_reg_xmm0_7), "xmm0_7") == 0);
    assert(strcmp(arg_name(x86_opr_r_m8), "r/m8") == 0);
    assert(strcmp(arg_name(x86_opr_r_m16), "r/m16") == 0);
    assert(strcmp(arg_name(x86_opr_r_m32), "r/m32") == 0);
    assert(strcmp(arg_name(x86_opr_r_m64), "r/m64") == 0);
    assert(strcmp(arg_name(x86_opr_r8_m8), "r8/m8") == 0);
    assert(strcmp(arg_name(x86_opr_r16_m8), "r16/m8") == 0);
    assert(strcmp(arg_name(x86_opr_r16_m16), "r16/m16") == 0);
    assert(strcmp(arg_name(x86_opr_r32_m8), "r32/m8") == 0);
    assert(strcmp(arg_name(x86_opr_r32_m16), "r32/m16") == 0);
    assert(strcmp(arg_name(x86_opr_r32_m32), "r32/m32") == 0);
    assert(strcmp(arg_name(x86_opr_r64_m8), "r64/m8") == 0);
    assert(strcmp(arg_name(x86_opr_r64_m16), "r64/m16") == 0);
    assert(strcmp(arg_name(x86_opr_r64_m32), "r64/m32") == 0);
    assert(strcmp(arg_name(x86_opr_r64_m64), "r64/m64") == 0);
    assert(strcmp(arg_name(x86_opr_rw_mw), "rw/mw") == 0);
    assert(strcmp(arg_name(x86_opr_vm32x), "vm32x") == 0);
    assert(strcmp(arg_name(x86_opr_vm32y), "vm32y") == 0);
    assert(strcmp(arg_name(x86_opr_vm32z), "vm32z") == 0);
    assert(strcmp(arg_name(x86_opr_vm64x), "vm64x") == 0);
    assert(strcmp(arg_name(x86_opr_vm64y), "vm64y") == 0);
    assert(strcmp(arg_name(x86_opr_vm64z), "vm64z") == 0);
    assert(strcmp(arg_name(x86_opr_m16bcst), "m16bcst") == 0);
    assert(strcmp(arg_name(x86_opr_m32bcst), "m32bcst") == 0);
    assert(strcmp(arg_name(x86_opr_m64bcst), "m64bcst") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m32_m16bcst), "xmm/m32/m16bcst") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m64_m16bcst), "xmm/m64/m16bcst") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m64_m32bcst), "xmm/m64/m32bcst") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m128_m16bcst), "xmm/m128/m16bcst") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m128_m32bcst), "xmm/m128/m32bcst") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m128_m64bcst), "xmm/m128/m64bcst") == 0);
    assert(strcmp(arg_name(x86_opr_ymm_m256_m16bcst), "ymm/m256/m16bcst") == 0);
    assert(strcmp(arg_name(x86_opr_ymm_m256_m32bcst), "ymm/m256/m32bcst") == 0);
    assert(strcmp(arg_name(x86_opr_ymm_m256_m64bcst), "ymm/m256/m64bcst") == 0);
    assert(strcmp(arg_name(x86_opr_zmm_m512_m16bcst), "zmm/m512/m16bcst") == 0);
    assert(strcmp(arg_name(x86_opr_zmm_m512_m32bcst), "zmm/m512/m32bcst") == 0);
    assert(strcmp(arg_name(x86_opr_zmm_m512_m64bcst), "zmm/m512/m64bcst") == 0);
    assert(strcmp(arg_name(x86_opr_mm_m32), "mm/m32") == 0);
    assert(strcmp(arg_name(x86_opr_mm_m64), "mm/m64") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m8), "xmm/m8") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m16), "xmm/m16") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m32), "xmm/m32") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m64), "xmm/m64") == 0);
    assert(strcmp(arg_name(x86_opr_xmm_m128), "xmm/m128") == 0);
    assert(strcmp(arg_name(x86_opr_ymm_m256), "ymm/m256") == 0);
    assert(strcmp(arg_name(x86_opr_zmm_m512), "zmm/m512") == 0);
    assert(strcmp(arg_name(x86_opr_seg_es), "es") == 0);
    assert(strcmp(arg_name(x86_opr_seg_cs), "cs") == 0);
    assert(strcmp(arg_name(x86_opr_seg_ss), "ss") == 0);
    assert(strcmp(arg_name(x86_opr_seg_ds), "ds") == 0);
    assert(strcmp(arg_name(x86_opr_seg_fs), "fs") == 0);
    assert(strcmp(arg_name(x86_opr_seg_gs), "gs") == 0);
    assert(strcmp(arg_name(x86_opr_k_m8), "k/m8") == 0);
    assert(strcmp(arg_name(x86_opr_k_m16), "k/m16") == 0);
    assert(strcmp(arg_name(x86_opr_k_m32), "k/m32") == 0);
    assert(strcmp(arg_name(x86_opr_k_m64), "k/m64") == 0);
    assert(strcmp(arg_name(x86_opr_bnd_mem), "bnd/mem") == 0);
    assert(strcmp(arg_name(x86_opr_m8), "m8") == 0);
    assert(strcmp(arg_name(x86_opr_m16), "m16") == 0);
    assert(strcmp(arg_name(x86_opr_m32), "m32") == 0);
    assert(strcmp(arg_name(x86_opr_m64), "m64") == 0);
    assert(strcmp(arg_name(x86_opr_m128), "m128") == 0);
    assert(strcmp(arg_name(x86_opr_m256), "m256") == 0);
    assert(strcmp(arg_name(x86_opr_m512), "m512") == 0);
    assert(strcmp(arg_name(x86_opr_m80), "m80") == 0);
    assert(strcmp(arg_name(x86_opr_m384), "m384") == 0);
    assert(strcmp(arg_name(x86_opr_mib), "mib") == 0);
    assert(strcmp(arg_name(x86_opr_mw), "mw") == 0);
    assert(strcmp(arg_name(x86_opr_mp), "mp") == 0);
    assert(strcmp(arg_name(x86_opr_vm32), "vm32") == 0);
    assert(strcmp(arg_name(x86_opr_vm64), "vm64") == 0);
    assert(strcmp(arg_name(x86_opr_far16_16), "far16:16") == 0);
    assert(strcmp(arg_name(x86_opr_far16_32), "far16:32") == 0);
    assert(strcmp(arg_name(x86_opr_memfar16_16), "memfar16:16") == 0);
    assert(strcmp(arg_name(x86_opr_memfar16_32), "memfar16:32") == 0);
    assert(strcmp(arg_name(x86_opr_memfar16_64), "memfar16:64") == 0);
}

void test_x86_enc_name()
{
    printf("\n# x86_enc_name\n");
    assert(strcmp(enc_name(x86_enc_t_lex), ".lex") == 0);
    assert(strcmp(enc_name(x86_enc_t_vex), ".vex") == 0);
    assert(strcmp(enc_name(x86_enc_t_evex), ".evex") == 0);
    assert(strcmp(enc_name(x86_enc_t_o16), ".o16") == 0);
    assert(strcmp(enc_name(x86_enc_t_o32), ".o32") == 0);
    assert(strcmp(enc_name(x86_enc_t_o64), ".o64") == 0);
    assert(strcmp(enc_name(x86_enc_t_a16), ".a16") == 0);
    assert(strcmp(enc_name(x86_enc_t_a32), ".a32") == 0);
    assert(strcmp(enc_name(x86_enc_t_a64), ".a64") == 0);
    assert(strcmp(enc_name(x86_enc_t_lock), ".lock") == 0);
    assert(strcmp(enc_name(x86_enc_t_rep), ".rep") == 0);
    assert(strcmp(enc_name(x86_enc_t_opcode), "") == 0);
    assert(strcmp(enc_name(x86_enc_t_opcode_r), "+r") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_r), "/r") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_0), "/0") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_1), "/1") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_2), "/2") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_3), "/3") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_4), "/4") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_5), "/5") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_6), "/6") == 0);
    assert(strcmp(enc_name(x86_enc_t_modrm_7), "/7") == 0);
    assert(strcmp(enc_name(x86_enc_t_ib), "ib") == 0);
    assert(strcmp(enc_name(x86_enc_t_iw), "iw") == 0);
    assert(strcmp(enc_name(x86_enc_t_i16), "i16") == 0);
    assert(strcmp(enc_name(x86_enc_t_i32), "i32") == 0);
    assert(strcmp(enc_name(x86_enc_t_i64), "i64") == 0);
    assert(strcmp(enc_name(x86_enc_t_i16e), "i16e") == 0);
    assert(strcmp(enc_name(x86_enc_l_lz), ".lz") == 0);
    assert(strcmp(enc_name(x86_enc_l_l0), ".l0") == 0);
    assert(strcmp(enc_name(x86_enc_l_l1), ".l1") == 0);
    assert(strcmp(enc_name(x86_enc_l_128), ".128") == 0);
    assert(strcmp(enc_name(x86_enc_l_256), ".256") == 0);
    assert(strcmp(enc_name(x86_enc_l_512), ".512") == 0);
    assert(strcmp(enc_name(x86_enc_l_lig), ".lig") == 0);
    assert(strcmp(enc_name(x86_enc_p_66), ".66") == 0);
    assert(strcmp(enc_name(x86_enc_p_9b), ".9b") == 0);
    assert(strcmp(enc_name(x86_enc_p_f2), ".f2") == 0);
    assert(strcmp(enc_name(x86_enc_p_f3), ".f3") == 0);
    assert(strcmp(enc_name(x86_enc_m_0f), ".0f") == 0);
    assert(strcmp(enc_name(x86_enc_m_0f38), ".0f38") == 0);
    assert(strcmp(enc_name(x86_enc_m_0f3a), ".0f3a") == 0);
    assert(strcmp(enc_name(x86_enc_m_map4), ".map4") == 0);
    assert(strcmp(enc_name(x86_enc_m_map5), ".map5") == 0);
    assert(strcmp(enc_name(x86_enc_m_map6), ".map6") == 0);
    assert(strcmp(enc_name(x86_enc_w_w0), ".w0") == 0);
    assert(strcmp(enc_name(x86_enc_w_w1), ".w1") == 0);
    assert(strcmp(enc_name(x86_enc_w_wb), ".wb") == 0);
    assert(strcmp(enc_name(x86_enc_w_wn), ".wn") == 0);
    assert(strcmp(enc_name(x86_enc_w_wx), ".wx") == 0);
    assert(strcmp(enc_name(x86_enc_w_ww), ".ww") == 0);
    assert(strcmp(enc_name(x86_enc_w_wig), ".wig") == 0);
}

void test_x86_reg_name()
{
    assert(strcmp(reg_name(x86_al), "al") == 0);
    assert(strcmp(reg_name(x86_cl), "cl") == 0);
    assert(strcmp(reg_name(x86_dl), "dl") == 0);
    assert(strcmp(reg_name(x86_bl), "bl") == 0);
    assert(strcmp(reg_name(x86_r15b), "r15b") == 0);
    assert(strcmp(reg_name(x86_r31b), "r31b") == 0);
    assert(strcmp(reg_name(x86_ax), "ax") == 0);
    assert(strcmp(reg_name(x86_cx), "cx") == 0);
    assert(strcmp(reg_name(x86_dx), "dx") == 0);
    assert(strcmp(reg_name(x86_bx), "bx") == 0);
    assert(strcmp(reg_name(x86_r15w), "r15w") == 0);
    assert(strcmp(reg_name(x86_r31w), "r31w") == 0);
    assert(strcmp(reg_name(x86_eax), "eax") == 0);
    assert(strcmp(reg_name(x86_ecx), "ecx") == 0);
    assert(strcmp(reg_name(x86_edx), "edx") == 0);
    assert(strcmp(reg_name(x86_ebx), "ebx") == 0);
    assert(strcmp(reg_name(x86_r15d), "r15d") == 0);
    assert(strcmp(reg_name(x86_r31d), "r31d") == 0);
    assert(strcmp(reg_name(x86_rax), "rax") == 0);
    assert(strcmp(reg_name(x86_rcx), "rcx") == 0);
    assert(strcmp(reg_name(x86_rdx), "rdx") == 0);
    assert(strcmp(reg_name(x86_rbx), "rbx") == 0);
    assert(strcmp(reg_name(x86_r15), "r15") == 0);
    assert(strcmp(reg_name(x86_r31), "r31") == 0);
    assert(strcmp(reg_name(x86_mm0), "mm0") == 0);
    assert(strcmp(reg_name(x86_mm15), "mm15") == 0);
    assert(strcmp(reg_name(x86_xmm0), "xmm0") == 0);
    assert(strcmp(reg_name(x86_xmm31), "xmm31") == 0);
    assert(strcmp(reg_name(x86_ymm0), "ymm0") == 0);
    assert(strcmp(reg_name(x86_ymm31), "ymm31") == 0);
    assert(strcmp(reg_name(x86_zmm0), "zmm0") == 0);
    assert(strcmp(reg_name(x86_zmm31), "zmm31") == 0);
    assert(strcmp(reg_name(x86_st0), "st0") == 0);
    assert(strcmp(reg_name(x86_st7), "st7") == 0);
}

int main(int argc, char const *argv[])
{
    test_x86_sizeof();
    test_x86_mode_name();
    test_x86_prefix_name();
    test_x86_prefix_namen();
    test_x86_ord_name();
    test_x86_opr_name();
    test_x86_enc_name();
    test_x86_reg_name();
    return 0;
}
