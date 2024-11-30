#include <stdio.h>
#include <limits.h>

#include "x86.h"

#include "x86_reg_strings.inc"
#include "x86_opc_table.inc"
#include "x86_opr_table.inc"
#include "x86_ord_table.inc"
#include "x86_op_strings.inc"

#define array_size(arr) sizeof(arr)/sizeof(arr[0])

typedef enum x86_state x86_state;
typedef struct x86_table_col x86_table_col;
typedef struct x86_map_str x86_map_str;
typedef struct x86_operands x86_operands;
typedef struct x86_opr_formatter x86_opr_formatter;
typedef struct x86_opr_mrm_formats x86_opr_mrm_formats;

typedef size_t (*x86_opr_str_fn)(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc);

enum x86_state
{
    x86_state_top,
    x86_state_segment,
    x86_state_legacy,
    x86_state_map_0f,
    x86_state_lex_opcode,
    x86_state_rex_opcode,
    x86_state_vex_opcode,
    x86_state_done,
};

struct x86_table_col
{
    int width;
    char *data;
};

struct x86_map_str
{
    uint ord;
    const char *str;
};

struct x86_opr_formatter
{
    x86_opr_str_fn fmt_const;
    x86_opr_str_fn fmt_imm;
    x86_opr_str_fn fmt_reg;
    x86_opr_str_fn fmt_mrm;
    x86_opr_str_fn fmt_vec;
};

struct x86_opr_mrm_formats
{
    const char *ptr_rip_disp;
    const char *ptr_rip;
    const char *ptr_disp;
    const char *ptr_sib_reg_scaled_reg;
    const char *ptr_sib_reg_reg;
    const char *ptr_reg;
    const char *ptr_sib_reg_scaled_reg_disp;
    const char *ptr_sib_reg_reg_disp;
    const char *ptr_sib_reg_disp;
    const char *reg;
};

struct x86_operands
{
    ullong mod : 3;
    ullong rm  : 3;
    ullong r   : 5;
    ullong x   : 5;
    ullong b   : 5;
    ullong s   : 2;
    ullong v   : 5;
    ullong k   : 5;
    ullong l   : 2;
    ullong w   : 1;
    ullong osz : 1;
};

extern x86_opr_formatter x86_format_intel_hex;
extern x86_opr_formatter x86_format_intel_dec;
extern x86_opr_mrm_formats x86_opr_mrm_formats_intel_hex;
extern x86_opr_mrm_formats x86_opr_mrm_formats_intel_dec;

static uint debug = 0;

void x86_set_debug(uint d) { debug = d; }

x86_map_str x86_mode_names[] =
{
    { x86_modes_64,             "64"               },
    { x86_modes_32,             "32"               },
    { x86_modes_16,             "16"               },
};

x86_map_str x86_map_names[] =
{
    { x86_map_map6,            "map6"              },
    { x86_map_map5,            "map5"              },
    { x86_map_map4,            "map4"              },
    { x86_map_0f3a,            "0f3a"              },
    { x86_map_0f38,            "0f38"              },
    { x86_map_0f,              "0f"                },
};

x86_map_str x86_ord_names[] =
{
    { x86_ord_rflags,           "rflags"           },
    { x86_ord_mxcsr,            "mxcsr"            },
    { x86_ord_xmm0_7,           "xmm0_7"           },
    { x86_ord_xmm0,             "xmm0"             },
    { x86_ord_seg,              "seg"              },
    { x86_ord_stx,              "stx"              },
    { x86_ord_st0,              "st0"              },
    { x86_ord_rdi,              "rdi"              },
    { x86_ord_rsi,              "rsi"              },
    { x86_ord_rbp,              "rbp"              },
    { x86_ord_rsp,              "rsp"              },
    { x86_ord_rbx,              "rbx"              },
    { x86_ord_rdx,              "rdx"              },
    { x86_ord_rcx,              "rcx"              },
    { x86_ord_rax,              "rax"              },
    { x86_ord_one,              "one"              },
    { x86_ord_ime,              "ime"              },
    { x86_ord_is4,              "is4"              },
    { x86_ord_sib,              "sib"              },
    { x86_ord_vec,              "vec"              },
    { x86_ord_mrm,              "mrm"              },
    { x86_ord_reg,              "reg"              },
    { x86_ord_imm,              "imm"              },
    { x86_ord_rwi,              "rwi"              },
    { x86_ord_wi,               "wi"               },
    { x86_ord_ri,               "ri"               },
    { x86_ord_rw,               "rw"               },
    { x86_ord_i,                "i"                },
    { x86_ord_r,                "r"                },
    { x86_ord_w,                "w"                },
    { 0,                        NULL               },
};

x86_map_str x86_opr_names[] =
{
    { x86_opr_memfar16_64,      "memfar16:64"      },
    { x86_opr_memfar16_32,      "memfar16:32"      },
    { x86_opr_memfar16_16,      "memfar16:16"      },
    { x86_opr_far16_32,         "far16:32"         },
    { x86_opr_far16_16,         "far16:16"         },
    { x86_opr_i16e,             "i16e"             },
    { x86_opr_bnd_mem,          "bnd/mem"          },
    { x86_opr_k_m64,            "k/m64"            },
    { x86_opr_k_m32,            "k/m32"            },
    { x86_opr_k_m16,            "k/m16"            },
    { x86_opr_k_m8,             "k/m8"             },
    { x86_opr_seg_gs,           "gs"               },
    { x86_opr_seg_fs,           "fs"               },
    { x86_opr_seg_ds,           "ds"               },
    { x86_opr_seg_ss,           "ss"               },
    { x86_opr_seg_cs,           "cs"               },
    { x86_opr_seg_es,           "es"               },
    { x86_opr_vm64z,            "vm64z"            },
    { x86_opr_vm64y,            "vm64y"            },
    { x86_opr_vm64x,            "vm64x"            },
    { x86_opr_vm32z,            "vm32z"            },
    { x86_opr_vm32y,            "vm32y"            },
    { x86_opr_vm32x,            "vm32x"            },
    { x86_opr_rw_mw,            "rw/mw"            },
    { x86_opr_r64_m64,          "r64/m64"          },
    { x86_opr_r64_m32,          "r64/m32"          },
    { x86_opr_r64_m16,          "r64/m16"          },
    { x86_opr_r64_m8,           "r64/m8"           },
    { x86_opr_r32_m32,          "r32/m32"          },
    { x86_opr_r32_m16,          "r32/m16"          },
    { x86_opr_r32_m8,           "r32/m8"           },
    { x86_opr_r16_m16,          "r16/m16"          },
    { x86_opr_r16_m8,           "r16/m8"           },
    { x86_opr_r8_m8,            "r8/m8"            },
    { x86_opr_zmm_m512_m64bcst, "zmm/m512/m64bcst" },
    { x86_opr_zmm_m512_m32bcst, "zmm/m512/m32bcst" },
    { x86_opr_zmm_m512_m16bcst, "zmm/m512/m16bcst" },
    { x86_opr_ymm_m256_m64bcst, "ymm/m256/m64bcst" },
    { x86_opr_ymm_m256_m32bcst, "ymm/m256/m32bcst" },
    { x86_opr_ymm_m256_m16bcst, "ymm/m256/m16bcst" },
    { x86_opr_xmm_m128_m64bcst, "xmm/m128/m64bcst" },
    { x86_opr_xmm_m128_m32bcst, "xmm/m128/m32bcst" },
    { x86_opr_xmm_m128_m16bcst, "xmm/m128/m16bcst" },
    { x86_opr_xmm_m64_m32bcst,  "xmm/m64/m32bcst"  },
    { x86_opr_xmm_m64_m16bcst,  "xmm/m64/m16bcst"  },
    { x86_opr_xmm_m32_m16bcst,  "xmm/m32/m16bcst"  },
    { x86_opr_zmm_m512,         "zmm/m512"         },
    { x86_opr_ymm_m256,         "ymm/m256"         },
    { x86_opr_xmm_m128,         "xmm/m128"         },
    { x86_opr_xmm_m64,          "xmm/m64"          },
    { x86_opr_xmm_m32,          "xmm/m32"          },
    { x86_opr_xmm_m16,          "xmm/m16"          },
    { x86_opr_xmm_m8,           "xmm/m8"           },
    { x86_opr_mm_m64,           "mm/m64"           },
    { x86_opr_mm_m32,           "mm/m32"           },
    { x86_opr_mp,               "mp"               },
    { x86_opr_mw,               "mw"               },
    { x86_opr_vm64,             "vm64"             },
    { x86_opr_vm32,             "vm32"             },
    { x86_opr_r_m64,            "r/m64"            },
    { x86_opr_r_m32,            "r/m32"            },
    { x86_opr_r_m16,            "r/m16"            },
    { x86_opr_r_m8,             "r/m8"             },
    { x86_opr_m64bcst,          "m64bcst"          },
    { x86_opr_m32bcst,          "m32bcst"          },
    { x86_opr_m16bcst,          "m16bcst"          },
    { x86_opr_mib,              "mib"              },
    { x86_opr_m384,             "m384"             },
    { x86_opr_m80,              "m80"              },
    { x86_opr_m512,             "m512"             },
    { x86_opr_m256,             "m256"             },
    { x86_opr_m128,             "m128"             },
    { x86_opr_m64,              "m64"              },
    { x86_opr_m32,              "m32"              },
    { x86_opr_m16,              "m16"              },
    { x86_opr_m8,               "m8"               },
    { x86_opr_reg_xmm0_7,       "xmm0_7"           },
    { x86_opr_reg_xmm0,         "xmm0"             },
    { x86_opr_reg_pdi,          "pdi"              },
    { x86_opr_reg_psi,          "psi"              },
    { x86_opr_reg_pb,           "pb"               },
    { x86_opr_reg_pd,           "pd"               },
    { x86_opr_reg_pc,           "pc"               },
    { x86_opr_reg_pa,           "pa"               },
    { x86_opr_reg_bw,           "bw"               },
    { x86_opr_reg_dw,           "dw"               },
    { x86_opr_reg_cw,           "cw"               },
    { x86_opr_reg_aw,           "aw"               },
    { x86_opr_reg_rbx,          "rbx"              },
    { x86_opr_reg_rdx,          "rdx"              },
    { x86_opr_reg_rcx,          "rcx"              },
    { x86_opr_reg_rax,          "rax"              },
    { x86_opr_reg_ebx,          "ebx"              },
    { x86_opr_reg_edx,          "edx"              },
    { x86_opr_reg_ecx,          "ecx"              },
    { x86_opr_reg_eax,          "eax"              },
    { x86_opr_reg_bx,           "bx"               },
    { x86_opr_reg_dx,           "dx"               },
    { x86_opr_reg_cx,           "cx"               },
    { x86_opr_reg_ax,           "ax"               },
    { x86_opr_reg_cl,           "cl"               },
    { x86_opr_reg_al,           "al"               },
    { x86_opr_reg_st1,          "st1"              },
    { x86_opr_reg_st0,          "st0"              },
    { x86_opr_reg_v0,           "v0"               },
    { x86_opr_reg_ah,           "ah"               },
    { x86_opr_reg_di,           "di"               },
    { x86_opr_reg_si,           "si"               },
    { x86_opr_reg_d,            "d"                },
    { x86_opr_reg_c,            "c"                },
    { x86_opr_reg_a,            "a"                },
    { x86_opr_relw,             "relw"             },
    { x86_opr_rel8,             "rel8"             },
    { x86_opr_moffs,            "moffs"            },
    { x86_opr_1,                "1"                },
    { x86_opr_zmm,              "zmm"              },
    { x86_opr_ymm,              "ymm"              },
    { x86_opr_xmm,              "xmm"              },
    { x86_opr_mm,               "mm"               },
    { x86_opr_rw,               "rw"               },
    { x86_opr_r64,              "r64"              },
    { x86_opr_r32,              "r32"              },
    { x86_opr_r16,              "r16"              },
    { x86_opr_r8,               "r8"               },
    { x86_opr_iw,               "iw"               },
    { x86_opr_i64,              "i64"              },
    { x86_opr_i32,              "i32"              },
    { x86_opr_i16,              "i16"              },
    { x86_opr_ib,               "ib"               },
    { x86_opr_bnd,              "bnd"              },
    { x86_opr_dreg,             "dreg"             },
    { x86_opr_creg,             "creg"             },
    { x86_opr_seg,              "seg"              },
    { x86_opr_k,                "k"                },
    { x86_opr_st,               "st"               },
    { x86_opr_vec,              "vec"              },
    { x86_opr_reg,              "reg"              },
    { x86_opr_imm,              "imm"              },
    { x86_opr_bcst,             "bcst"             },
    { x86_opr_far,              "far"              },
    { x86_opr_ind,              "ind"              },
    { x86_opr_rel,              "rel"              },
    { x86_opr_mem,              "mem"              },
    { x86_opr_flag_er,          "{er}"             },
    { x86_opr_flag_k,           "{k}"              },
    { x86_opr_flag_sae,         "{sae}"            },
    { x86_opr_flag_z,           "{z}"              },
    { x86_opr_flag_rs2,         "{rs2}"            },
    { x86_opr_flag_rs4,         "{rs4}"            },
    { 0,                        NULL               },
};

x86_map_str x86_enc_names[] =
{
    { x86_enc_s_a64,            " .a64"            },
    { x86_enc_s_a32,            " .a32"            },
    { x86_enc_s_a16,            " .a16"            },
    { x86_enc_s_o64,            " .o64"            },
    { x86_enc_s_o32,            " .o32"            },
    { x86_enc_s_o16,            " .o16"            },
    { x86_enc_s_rep,            " .rep"            },
    { x86_enc_s_lock,           " .lock"           },
    { x86_enc_i2_i16e,          " i16e"            },
    { x86_enc_i_i64,            " i64"             },
    { x86_enc_i_i32,            " i32"             },
    { x86_enc_i_i16,            " i16"             },
    { x86_enc_i_iw,             " iw"              },
    { x86_enc_i_ib,             " ib"              },
    { x86_enc_f_opcode_r,       ""                 },
    { x86_enc_f_opcode,         ""                 },
    { x86_enc_f_modrm_n,        ""                 },
    { x86_enc_f_modrm_r,        ""                 },
    { x86_enc_o_opcode_r,       ""                 },
    { x86_enc_o_opcode,         ""                 },
    { x86_enc_t_evex,           ".evex"            },
    { x86_enc_t_vex,            ".vex"             },
    { x86_enc_t_lex,            ".lex"             },
    { x86_enc_l_lig,            ".lig"             },
    { x86_enc_l_512,            ".512"             },
    { x86_enc_l_256,            ".256"             },
    { x86_enc_l_128,            ".128"             },
    { x86_enc_l_l1,             ".l1"              },
    { x86_enc_l_l0,             ".l0"              },
    { x86_enc_l_lz,             ".lz"              },
    { x86_enc_p_rexw,           ".w"               },
    { x86_enc_p_9b,             ".9b"              },
    { x86_enc_p_f2,             ".f2"              },
    { x86_enc_p_f3,             ".f3"              },
    { x86_enc_p_66,             ".66"              },
    { x86_enc_m_map6,           ".map6"            },
    { x86_enc_m_map5,           ".map5"            },
    { x86_enc_m_map4,           ".map4"            },
    { x86_enc_m_0f3a,           ".0f3a"            },
    { x86_enc_m_0f38,           ".0f38"            },
    { x86_enc_m_0f,             ".0f"              },
    { x86_enc_w_wig,            ".wig"             },
    { x86_enc_w_ww,             ".ww"              },
    { x86_enc_w_wx,             ".wx"              },
    { x86_enc_w_wn,             ".wn"              },
    { x86_enc_w_wb,             ".wb"              },
    { x86_enc_w_w1,             ".w1"              },
    { x86_enc_w_w0,             ".w0"              },
    { 0,                        NULL               },
};

static size_t x86_name_map(x86_map_str * p, char * buf, size_t len, uint ord,
    const char * sep)
{
    size_t count = 0;
    int ret = 0;
    for (;;) {
        while (p->str != NULL) {
            if (p->ord && (p->ord & ord) == p->ord) {
                ord = ord & ~p->ord;
                break;
            }
            p++;
        }
        if (p->str == NULL) break;
        if (buf == NULL || len - count <= len) {
            ret = snprintf(buf ? buf + count : NULL,
                buf ? len - count : INT_MAX,
                "%s%s", count == 0 ? "" : sep, p->str);
            if (ret > 0) count += ret;
        }
    }
    return count;
}

size_t x86_mode_name(char * buf, size_t len, uint mode, const char *sep)
{
    return x86_name_map(x86_mode_names, buf, len, mode, sep);
}

size_t x86_map_name(char * buf, size_t len, uint mode, const char *sep)
{
    return x86_name_map(x86_map_names, buf, len, mode, sep);
}

size_t x86_ord_name(char * buf, size_t len, uint ord, const char *sep)
{
    return x86_name_map(x86_ord_names, buf, len, ord, sep);
}

size_t x86_opr_name(char * buf, size_t len, uint opr)
{
    return x86_name_map(x86_opr_names, buf, len, opr, "");
}

size_t x86_enc_name(char * buf, size_t len, uint enc)
{
    return x86_name_map(x86_enc_names, buf, len, enc, "");
}

const char* x86_reg_name(uint reg)
{
    return (reg < 512) ? x86_reg_names[reg] : "invalid";
}

size_t x86_ord_mnem(char * buf, size_t len, const ushort *ord)
{
    const char codes[8] = " -irmv  ";
    size_t count = 0;
    for (size_t i = 0; i < array_size(x86_ord_table[0].ord) && ord[i]; i++) {
        uint type = ord[i] & 0b111;
        if (buf && count < len) {
            buf[count++] = codes[type];
        }
    }
    buf[count] = '\0';
    return count;
}

char * x86_table_type_name(uint type)
{
    switch (type) {
    case x86_table_none: return "none";
    case x86_table_lex: return "lex";
    case x86_table_vex: return "vex";
    case x86_table_evex: return "evex";
    default: return "";
    }
}

char * x86_table_map_name(uint map)
{
    switch (map) {
    case x86_map_none: return "";
    case x86_map_0f: return "0f";
    case x86_map_0f38: return "0f38";
    case x86_map_0f3a: return "0f3a";
    case x86_map_map4: return "map4";
    case x86_map_map5: return "map5";
    case x86_map_map6: return "map6";
    default: return "";
    }
}

char * x86_table_prefix_name(uint prefix)
{
    switch (prefix) {
    case x86_pfx_66: return "66";
    case x86_pfx_f3: return "f3";
    case x86_pfx_f2: return "f2";
    case x86_pfx_9b: return "9b";
    case x86_pfx_66 | x86_pfx_rexw: return "66+w";
    case x86_pfx_f3 | x86_pfx_rexw: return "f3+w";
    case x86_pfx_f2 | x86_pfx_rexw: return "f2+w";
    case x86_pfx_9b | x86_pfx_rexw: return "9b+w";
    default: return "";
    }
}

/*
 *  metadata filters
 */

int x86_enc_filter_rex(x86_rex prefix, uint enc)
{
    uint lex =  (enc & x86_enc_t_mask) == x86_enc_t_lex;
    uint ew0 =  (enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  (enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewn =  (enc & x86_enc_w_mask) == x86_enc_w_wn;
    uint ewb =  (enc & x86_enc_w_mask) == x86_enc_w_wb;
    uint eww =  (enc & x86_enc_w_mask) == x86_enc_w_ww;
    uint ewx =  (enc & x86_enc_w_mask) == x86_enc_w_wx;
    uint ewig = (enc & x86_enc_w_mask) == x86_enc_w_wig;

    uint w = (prefix.data[0] >> 3) & 1;

    if (!lex) return -1;

    switch (w) {
    case x86_vex_w0:
        if (!(ew0 || ewig || ewn || ewb || eww || ewx)) return -1;
        break;
    case x86_vex_w1:
        if (!(ew1 || ewig || ewn || ewb || eww || ewx)) return -1;
        break;
    }

    return 0;
}

int x86_enc_filter_rex2(x86_rex2 prefix, uint enc)
{
    uint lex =  (enc & x86_enc_t_mask) == x86_enc_t_lex;
    uint ew0 =  (enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  (enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewig = (enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint em =   (enc & x86_enc_m_mask) >> x86_enc_m_shift;

    uint m = (prefix.data[0] >> 7) & 1;
    uint w = (prefix.data[0] >> 3) & 1;

    if (!lex || m != em) return -1;

    switch (w) {
    case x86_vex_w0: if (!(ew0 || ewig)) return -1; break;
    case x86_vex_w1: if (!(ew1 || ewig)) return -1; break;
    default: return -1;
    }

    return 0;
}

int x86_enc_filter_vex2(x86_vex2 prefix, uint enc)
{
    uint vex =  (enc & x86_enc_t_mask) == x86_enc_t_vex;
    uint ew0 =  (enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ewig = (enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint lz =   (enc & x86_enc_l_mask) == x86_enc_l_lz;
    uint l0 =   (enc & x86_enc_l_mask) == x86_enc_l_l0;
    uint l1 =   (enc & x86_enc_l_mask) == x86_enc_l_l1;
    uint l128 = (enc & x86_enc_l_mask) == x86_enc_l_128;
    uint l256 = (enc & x86_enc_l_mask) == x86_enc_l_256;
    uint lig =  (enc & x86_enc_l_mask) == x86_enc_l_lig;
    uint np =   (enc & x86_enc_p_mask) == x86_enc_p_none;
    uint p66 =  (enc & x86_enc_p_mask) == x86_enc_p_66;
    uint pf2 =  (enc & x86_enc_p_mask) == x86_enc_p_f2;
    uint pf3 =  (enc & x86_enc_p_mask) == x86_enc_p_f3;
    uint em =   (enc & x86_enc_m_mask) >> x86_enc_m_shift;

    uint p = (prefix.data[0] >> 0) & 3;
    uint l = (prefix.data[0] >> 2) & 1;

    if (!vex || !(ew0 || ewig) || x86_map_0f != em) return -1;

    switch (l) {
    case x86_vex_l0: if (!(lig || lz || l0 || l128)) return -1; break;
    case x86_vex_l1: if (!(lig || l1 || l256)) return -1; break;
    default: return -1;
    }

    switch (p) {
    case x86_pfx_none: if (!np) return -1; break;
    case x86_pfx_66: if (!p66) return -1; break;
    case x86_pfx_f2: if (!pf2) return -1; break;
    case x86_pfx_f3: if (!pf3) return -1; break;
    default: return -1;
    }

    return 0;
}

int x86_enc_filter_vex3(x86_vex3 prefix, uint enc)
{
    uint vex =  (enc & x86_enc_t_mask) == x86_enc_t_vex;
    uint ew0 =  (enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  (enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewig = (enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint lz =   (enc & x86_enc_l_mask) == x86_enc_l_lz;
    uint l0 =   (enc & x86_enc_l_mask) == x86_enc_l_l0;
    uint l1 =   (enc & x86_enc_l_mask) == x86_enc_l_l1;
    uint l128 = (enc & x86_enc_l_mask) == x86_enc_l_128;
    uint l256 = (enc & x86_enc_l_mask) == x86_enc_l_256;
    uint lig =  (enc & x86_enc_l_mask) == x86_enc_l_lig;
    uint np =   (enc & x86_enc_p_mask) == x86_enc_p_none;
    uint p66 =  (enc & x86_enc_p_mask) == x86_enc_p_66;
    uint pf2 =  (enc & x86_enc_p_mask) == x86_enc_p_f2;
    uint pf3 =  (enc & x86_enc_p_mask) == x86_enc_p_f3;
    uint em =   (enc & x86_enc_m_mask) >> x86_enc_m_shift;

    uint m = (prefix.data[0] >> 0) & 31;
    uint w = (prefix.data[1] >> 7) & 1;
    uint p = (prefix.data[1] >> 0) & 3;
    uint l = (prefix.data[1] >> 2) & 1;

    if (!vex || m != em) return -1;

    switch (w) {
    case x86_vex_w0: if (!(ew0 || ewig)) return -1; break;
    case x86_vex_w1: if (!(ew1 || ewig)) return -1; break;
    default: return -1;
    }

    switch (l) {
    case x86_vex_l0: if (!(lig || lz || l0 || l128)) return -1; break;
    case x86_vex_l1: if (!(lig || l1 || l256)) return -1; break;
    default: return -1;
    }

    switch (p) {
    case x86_pfx_none: if (!np) return -1; break;
    case x86_pfx_66: if (!p66) return -1; break;
    case x86_pfx_f2: if (!pf2) return -1; break;
    case x86_pfx_f3: if (!pf3) return -1; break;
    default: return -1;
    }

    return 0;
}

int x86_enc_filter_evex(x86_evex prefix, uint enc)
{
    uint evex = (enc & x86_enc_t_mask) == x86_enc_t_evex;
    uint ew0 =  (enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  (enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewig = (enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint l128 = (enc & x86_enc_l_mask) == x86_enc_l_128;
    uint l256 = (enc & x86_enc_l_mask) == x86_enc_l_256;
    uint l512 = (enc & x86_enc_l_mask) == x86_enc_l_512;
    uint lig =  (enc & x86_enc_l_mask) == x86_enc_l_lig;
    uint np =   (enc & x86_enc_p_mask) == x86_enc_p_none;
    uint p66 =  (enc & x86_enc_p_mask) == x86_enc_p_66;
    uint pf2 =  (enc & x86_enc_p_mask) == x86_enc_p_f2;
    uint pf3 =  (enc & x86_enc_p_mask) == x86_enc_p_f3;
    uint em =   (enc & x86_enc_m_mask) >> x86_enc_m_shift;

    uint m = (prefix.data[0] >> 0) & 7;
    uint w = (prefix.data[1] >> 7) & 1;
    uint p = (prefix.data[1] >> 0) & 3;
    uint l = (prefix.data[2] >> 5) & 3;

    if (!evex || m != em) return -1;

    switch (w) {
    case x86_vex_w0: if (!(ew0 || ewig)) return -1; break;
    case x86_vex_w1: if (!(ew1 || ewig)) return -1; break;
    default: return -1;
    }

    switch (l) {
    case x86_vex_l0: if (!(lig || l128)) return -1; break;
    case x86_vex_l1: if (!(lig || l256)) return -1; break;
    case x86_vex_l2: if (!(lig || l512)) return -1; break;
    default: return -1;
    }

    switch (p) {
    case x86_pfx_none: if (!np) return -1; break;
    case x86_pfx_66: if (!p66) return -1; break;
    case x86_pfx_f2: if (!pf2) return -1; break;
    case x86_pfx_f3: if (!pf3) return -1; break;
    default: return -1;
    }

    return 0;
}

/*
 *  table sorting
 */

static int x86_opc_data_compare_opcode(const void *p1, const void *p2)
{
    const x86_opc_data *op1 = x86_opc_table + *(size_t*)p1;
    const x86_opc_data *op2 = x86_opc_table + *(size_t*)p2;

    /* split into prefix and suffix */
    uint mask = x86_enc_o_mask | x86_enc_f_mask | x86_enc_s_mask |
                x86_enc_i_mask | x86_enc_i2_mask;
    uint op1pre = op1->enc & ~mask;
    uint op2pre = op2->enc & ~mask;
    uint op1suf = op1->enc &  mask;
    uint op2suf = op2->enc &  mask;

    if (op1pre < op2pre) return -1;
    if (op1pre > op2pre) return 1;
    if (op1->opc[0] < op2->opc[0]) return -1;
    if (op1->opc[0] > op2->opc[0]) return 1;
    if (op1->opc[1] < op2->opc[1]) return -1;
    if (op1->opc[1] > op2->opc[1]) return 1;
    if (op1suf < op2suf) return -1;
    if (op1suf > op2suf) return 1;
    return 0;
}

static int x86_opc_data_compare_alpha(const void *p1, const void *p2)
{
    const x86_opc_data *op1 = x86_opc_table + *(size_t*)p1;
    const x86_opc_data *op2 = x86_opc_table + *(size_t*)p2;
    int alpha = strcmp(x86_op_names[op1->op], x86_op_names[op2->op]);
    if (alpha == 0) return x86_opc_data_compare_opcode(p1, p2);
    else return alpha;
}

static x86_table_idx x86_opc_table_index(size_t n)
{
    x86_table_idx tab = { n, malloc(sizeof(size_t) * n) };
    for(size_t i = 0; i < tab.count; i++) tab.idx[i] = i;
    return tab;
}

x86_table_idx x86_opc_table_identity()
{
    return x86_opc_table_index(x86_opc_table_size);
}

x86_table_idx x86_opc_table_sorted(x86_table_idx tab, uint sort)
{
    switch (sort) {
    case x86_sort_none:
        break;
    case x86_sort_numeric:
        qsort(tab.idx, tab.count, sizeof(size_t), x86_opc_data_compare_opcode);
        break;
    case x86_sort_alpha:
        qsort(tab.idx, tab.count, sizeof(size_t), x86_opc_data_compare_alpha);
        break;
    }
    return tab;
}

x86_table_idx x86_opc_table_filter(x86_table_idx tab, uint modes)
{
    size_t count = 0;
    for(size_t i = 0; i < tab.count; i++) {
        const x86_opc_data *d = x86_opc_table + tab.idx[i];
        if (d->mode & modes) count++;
    }
    x86_table_idx newtab = { count, malloc(sizeof(size_t) * count) };
    count = 0;
    for(size_t i = 0; i < tab.count; i++) {
        const x86_opc_data *d = x86_opc_table + tab.idx[i];
        if (d->mode & modes) newtab.idx[count++] = i;
    }
    free(tab.idx);
    return newtab;
}

static int x86_opc_data_compare_masked(const void *p1, const void *p2)
{
    x86_opc_data *om1 = (x86_opc_data*)p1;
    x86_opc_data *om2 = (x86_opc_data*)p2;
    uint enc1 = om1->enc, enc2 = om2->enc;
    uint pre1 = enc1 & (x86_enc_t_mask | x86_enc_m_mask | x86_enc_prexw_mask);
    uint pre2 = enc2 & (x86_enc_t_mask | x86_enc_m_mask | x86_enc_prexw_mask);
    if (pre1 < pre2) return -1;
    if (pre1 > pre2) return 1;
    for (size_t i = 0; i < 2; i++) {
        uchar mask = om1->opm[i] & om2->opm[i];
        if ((om1->opc[i] & mask) < (om2->opc[i] & mask)) return -1;
        if ((om1->opc[i] & mask) > (om2->opc[i] & mask)) return 1;
    }
    /* suffix is unnecessary for matching opcodes because it contains format*/
    return 0;
}

static int x86_opc_data_compare_build(const void *p1, const void *p2)
{
    x86_opc_data *om1 = (x86_opc_data*)p1;
    x86_opc_data *om2 = (x86_opc_data*)p2;
    uint enc1 = om1->enc, enc2 = om2->enc;
    uint pre1 = enc1 & (x86_enc_t_mask | x86_enc_m_mask | x86_enc_prexw_mask);
    uint pre2 = enc2 & (x86_enc_t_mask | x86_enc_m_mask | x86_enc_prexw_mask);
    if (pre1 < pre2) return -1;
    if (pre1 > pre2) return 1;
    for (size_t i = 0; i < 2; i++) {
        if (om1->opc[i] < om2->opc[i]) return -1;
        if (om1->opc[i] > om2->opc[i]) return 1;
        /* most specific mask first for fixed modrm */
        if (om1->opm[i] < om2->opm[i]) return 1;
        if (om1->opm[i] > om2->opm[i]) return -1;
    }
    uint suf1 = enc1 & ~(x86_enc_t_mask | x86_enc_m_mask | x86_enc_prexw_mask);
    uint suf2 = enc2 & ~(x86_enc_t_mask | x86_enc_m_mask | x86_enc_prexw_mask);
    if (suf1 < suf2) return -1;
    if (suf1 > suf2) return 1;
    return 0;
}

static void x86_add_opc_data(x86_opc_data *op_map,
    x86_opc_data rec, uint mod11p, uint mod11n)
{
    if (mod11p) {
        /* add one entry with mod == 0b11 - ModRM.rm is register */
        rec.opm[1] |= 0xc0;
        rec.opc[1] |= 0xc0;
        op_map[0] = rec;
    } else if (mod11n) {
        /* add three entries with mod != 0b11 - ModRM.rm is memory */
        rec.opm[1] |= 0xc0;
        rec.opc[1] = (rec.opc[1] & 0x3f) | 0x80;
        op_map[0] = rec;
        rec.opc[1] = (rec.opc[1] & 0x3f) | 0x40;
        op_map[1] = rec;
        rec.opc[1] = (rec.opc[1] & 0x3f);
        op_map[2] = rec;
    } else {
        /* add entry unmodified */
        op_map[0] = rec;
    }
}

static void x86_build_prefix_table(const x86_opc_data *op_table,
    x86_table_idx tab, x86_opc_data *op_map, size_t *count)
{
    size_t n = 1;

    for(size_t i = 0; i < tab.count; i++) {
        const x86_opc_data *d = op_table + tab.idx[i];
        const x86_opr_data *o = x86_opr_table + d->opr;
        const x86_ord_data *p = x86_ord_table + d->ord;

        /* extract prefix and synthesize width prefixes */
        uint pfx1 = 0, pfx2 = 0, pfx3 = 0;
        switch (x86_enc_type(d->enc)) {
        case x86_enc_t_lex:
        case x86_enc_t_vex:
        case x86_enc_t_evex:
            switch (d->enc & x86_enc_w_mask) {
            case x86_enc_w_wig:
            case x86_enc_w_wn:
            case x86_enc_w_wb:
            case x86_enc_w_w0: break;
            case x86_enc_w_w1: pfx1 |= x86_enc_p_rexw; break;
            case x86_enc_w_wx: pfx2 = pfx1 | x86_enc_p_rexw; /* fallthrough */
            case x86_enc_w_ww: pfx3 = pfx1 | x86_enc_p_66; break;
            }
            break;
         }

        /* find register or memory operand mapping to modrm.rm field
         * so that we can add mod=0b11 or mod!=0b11 to modrm mask */
        uint mod11p = 0, mod11n = 0;
        for (size_t i = 0; i < array_size(o->opr) && o->opr[i]; i++)
        {
            uint isreg = (o->opr[i] & x86_opr_type_mask) >= x86_opr_reg;
            uint ismem = (o->opr[i] & x86_opr_mem) != 0;
            uint ismrm = (p->ord[i] & x86_ord_type_mask) == x86_ord_mrm;
            if (ismrm) {
                if (isreg && !ismem) {
                    mod11p = 1; /* modb == 0b11 */
                    break;
                } else if (!isreg && ismem) {
                    mod11n = 1; /* modb != 0b11 */
                    break;
                }
            }
        }

        /* add entries to table */
        if (op_map) {
            x86_opc_data rec = *d;
            rec.enc |= pfx1;
            x86_add_opc_data(op_map + n, rec, mod11p, mod11n);
        }
        n += mod11n ? 3 : 1;
        if (pfx2) {
            if (op_map) {
                x86_opc_data rec = *d;
                rec.enc |= pfx2;
                x86_add_opc_data(op_map + n, rec, mod11p, mod11n);
            }
            n += mod11n ? 3 : 1;
        }
        if (pfx3) {
            if (op_map) {
                x86_opc_data rec = *d;
                rec.enc |= pfx3;
                x86_add_opc_data(op_map + n, rec, mod11p, mod11n);
            }
            n += mod11n ? 3 : 1;
        }
    }

    if (count) *count = n;
}

static void x86_build_accel_table(x86_acc_idx *idx,
    x86_acc_entry *acc, size_t *count)
{
    /* slot zero is one byte legacy opcodes: type:LEX, prefix:0, map:0 */
    size_t num_pages = 1;
    uint max_entries = 0;
    for (size_t i = 1; i < idx->map_count; i++) {
        const x86_opc_data *m = idx->map + i;
        uint type = x86_enc_type(m->enc) >> x86_enc_t_shift;
        uint prefix = x86_enc_prefix(m->enc) >> x86_enc_p_shift;
        uint map = x86_enc_map(m->enc) >> x86_enc_m_shift;
        size_t acc_page = x86_acc_page(type, prefix, map);
        /*
         * offset zero means the slice is not allocated but page zero is
         * preallocated as a special cased for type:LEX, prefix:0, map:0
         */
        size_t offset = 0;
        if (!acc) {
            /* counting pass assign offsets except for page zero */
            if (acc_page > 0 && idx->page_offsets[acc_page] == 0) {
                size_t page = num_pages++;
                idx->page_offsets[acc_page] = page;
                offset = page << 8;
            }
        } else if (acc) {
            /* writing pass lookup offset */
            offset = x86_acc_offset(idx, acc_page);
            uint oc = m->opc[0], msk = m->opm[0], om = oc;
            /*  (type, prefix, map, opcode) -> (index, count) */
            while ((oc & msk) == om) {
                if (acc[offset + oc].idx == 0) acc[offset + oc].idx = i;
                acc[offset + oc].nent++;
                oc++;
            }
        }
    }
    if (count) {
        *count = num_pages << 8;
    }
}

x86_acc_idx* x86_table_build(uint modes)
{
    x86_acc_idx *idx = calloc(1, sizeof(x86_acc_idx));
    x86_table_idx tab = x86_opc_table_filter(x86_opc_table_identity(), modes);
    x86_build_prefix_table(x86_opc_table, tab, NULL, &idx->map_count);
    idx->map = calloc(idx->map_count, sizeof(x86_opc_data));
    x86_build_prefix_table(x86_opc_table, tab, idx->map, NULL);
    qsort(idx->map, idx->map_count, sizeof(x86_opc_data), x86_opc_data_compare_build);
    idx->page_offsets = calloc(512, sizeof(uchar));
    x86_build_accel_table(idx, NULL, &idx->acc_count);
    idx->acc = calloc(sizeof(x86_acc_entry), idx->acc_count);
    x86_build_accel_table(idx, idx->acc, NULL);
    free(tab.idx);
    return idx;
}

static x86_opc_data* x86_table_lookup_slow(x86_acc_idx *idx, const x86_opc_data *m)
{
    size_t begin = 0, end = idx->map_count;
    while (end != 0) {
        size_t half = (end >> 1), probe = begin + half;
        if (x86_opc_data_compare_masked(m, idx->map + probe) > 0) {
            begin = probe + 1;
            end -= half + 1;
        } else {
            end = half;
        }
    }
    return idx->map + begin;
}

x86_opc_data* x86_table_lookup(x86_acc_idx *idx, const x86_opc_data *m)
{
    uint type = x86_enc_type(m->enc) >> x86_enc_t_shift;
    uint prefix = x86_enc_prefix(m->enc) >> x86_enc_p_shift;
    uint map = x86_enc_map(m->enc) >> x86_enc_m_shift;
    size_t acc_page = x86_acc_page(type, prefix, map);
    size_t offset = x86_acc_offset(idx, acc_page) + m->opc[0];
    x86_acc_entry *ent = x86_acc_lookup(idx, offset);
    x86_acc_idx new_idx = { ent->nent, idx->map + ent->idx };
    return x86_table_lookup_slow(&new_idx, m);
}

/*
 * table printing utilities
 */

static x86_table_col x86_new_column(int width, char *data)
{
    x86_table_col col = { width, strdup(data) };
    return col;
}

static void x86_print_row(size_t count, x86_table_col *cols)
{
    printf("|");
    for (size_t i = 0; i < count; i++) {
        printf(" %-*s |", cols[i].width, cols[i].data);
        free(cols[i].data);
    }
    printf("\n");
}

static size_t x86_format_enc(char * buf, size_t buflen, const x86_opc_data *d)
{
    size_t len = 0;

    uint s = x86_enc_suffix(d->enc);
    uint i = x86_enc_imm(d->enc);
    uint i2 = x86_enc_imm2(d->enc);
    uint enc = x86_enc_leading(d->enc);

    len += x86_enc_name(buf+len, buflen-len, enc);

    switch (x86_enc_opcode(enc)) {
    case x86_enc_o_opcode_r:
        len += snprintf(buf+len, buflen-len, " %02hhx+r", d->opc[0]);
        break;
    case x86_enc_o_opcode:
        len += snprintf(buf+len, buflen-len, " %02hhx", d->opc[0]);
        break;
    }

    switch (x86_enc_func(enc)) {
    case x86_enc_f_modrm_r:
        len += snprintf(buf+len, buflen-len, " /r");
        break;
    case x86_enc_f_modrm_n:
        len += snprintf(buf+len, buflen-len, " /%d", (d->opc[1] >> 3) & 7);
        break;
    case x86_enc_f_opcode_r:
        len += snprintf(buf+len, buflen-len, " %02hhx+r", d->opc[1]);
        break;
    case x86_enc_f_opcode:
        len += snprintf(buf+len, buflen-len, " %02hhx", d->opc[1]);
        break;
    }

    if (i2) {
        len += x86_enc_name(buf+len, buflen-len, i2);
    }
    if (i) {
        len += x86_enc_name(buf+len, buflen-len, i);
    }
    if (s) {
        len += x86_enc_name(buf+len, buflen-len, s);
    }

    return len;
}

void x86_print_op(const x86_opc_data *d, uint compact, uint opcode)
{
    char buf[256];
    x86_table_col cols[6];
    size_t count = 0, len;

    const x86_opr_data *o = x86_opr_table + d->opr;
    const x86_ord_data *p = x86_ord_table + d->ord;

    buf[(len = 0)] = '\0';
    if (compact) {
        len += snprintf(buf+len, sizeof(buf)-len, "%s", x86_op_names[d->op]);
        cols[count++] = x86_new_column(18, buf);
    } else {
        len += snprintf(buf+len, sizeof(buf)-len, "%s ", x86_op_names[d->op]);
        for (size_t i = 0; i < array_size(o->opr) && o->opr[i]; i++) {
            if (i != 0) len += snprintf(buf+len, sizeof(buf)-len, ",");
            len += x86_opr_name(buf+len, sizeof(buf)-len, o->opr[i]);
        }
        cols[count++] = x86_new_column(52, buf);
    }

    if (opcode) {
        buf[(len = 0)] = '\0';
        len += snprintf(buf+len, sizeof(buf)-len, "%02hhx %02hhx",
            d->opc[0], d->opc[1]);
        cols[count++] = x86_new_column(5, buf);
        buf[(len = 0)] = '\0';
        len += snprintf(buf+len, sizeof(buf)-len, "%02hhx %02hhx",
            d->opm[0], d->opm[1]);
        cols[count++] = x86_new_column(5, buf);
    }

    if (compact) {
        buf[(len = 0)] = '\0';
        len += x86_ord_mnem(buf+len, sizeof(buf)-len, p->ord);
        cols[count++] = x86_new_column(4, buf);
    }

    uint s = x86_enc_suffix(d->enc);
    uint i = x86_enc_imm(d->enc);
    uint i2 = x86_enc_imm2(d->enc);
    uint enc = x86_enc_leading(d->enc);

    buf[(len = 0)] = '\0';
    len += x86_format_enc(buf, sizeof(buf), d);
    cols[count++] = x86_new_column(31, buf);

    if (!compact) {
        buf[(len = 0)] = '\0';
        for (size_t i = 0; i < array_size(p->ord) && p->ord[i]; i++) {
            if (i != 0) len += snprintf(buf+len, sizeof(buf)-len, ",");
            len += x86_ord_name(buf+len, sizeof(buf)-len, p->ord[i], "/");
        }
        cols[count++] = x86_new_column(23, buf);
    }

    buf[(len = 0)] = '\0';
    len += x86_mode_name(buf+len, sizeof(buf)-len, d->mode, "/");
    cols[count++] = x86_new_column(8, buf);

    x86_print_row(count, cols);
}

/*
 * encoding / decoding
 */

int x86_codec_write(x86_buffer *buf, x86_codec c, size_t *len)
{
    size_t nbytes = 0;

    /* segment prefix */
    switch (c.seg) {
    case x86_seg_es: nbytes += x86_out8(buf, x86_pb_es); break;
    case x86_seg_cs: nbytes += x86_out8(buf, x86_pb_cs); break;
    case x86_seg_ss: nbytes += x86_out8(buf, x86_pb_ss); break;
    case x86_seg_ds: nbytes += x86_out8(buf, x86_pb_ds); break;
    case x86_seg_fs: nbytes += x86_out8(buf, x86_pb_fs); break;
    case x86_seg_gs: nbytes += x86_out8(buf, x86_pb_gs); break;
    }

    /* other prefixes */
    if (x86_codec_has_wait(&c)) {
        nbytes += x86_out8(buf, x86_pb_wait);
    }
    if (x86_codec_has_lock(&c)) {
        nbytes += x86_out8(buf, x86_pb_lock);
    }
    if (x86_codec_has_rep(&c)) {
        nbytes += x86_out8(buf, x86_pb_rep);
    }
    if (x86_codec_has_repne(&c)) {
        nbytes += x86_out8(buf, x86_pb_repne);
    }
    if (x86_codec_has_osize(&c)) {
        nbytes += x86_out8(buf, x86_pb_osize);
    }
    if (x86_codec_has_asize(&c)) {
        nbytes += x86_out8(buf, x86_pb_asize);
    }

    /* extended prefixes */
    switch (x86_codec_field_ce(&c) >> x86_ce_shift) {
        case x86_ce_rex >> x86_ce_shift:
            nbytes += x86_out8(buf, c.rex.data[0]);
            break;
        case x86_ce_rex2 >> x86_ce_shift:
            nbytes += x86_out8(buf, x86_pb_rex2);
            nbytes += x86_out8(buf, c.rex2.data[0]);
            break;
        case x86_ce_vex2 >> x86_ce_shift:
            nbytes += x86_out8(buf, x86_pb_vex2);
            nbytes += x86_out8(buf, c.vex2.data[0]);
            break;
        case x86_ce_vex3 >> x86_ce_shift:
            nbytes += x86_out8(buf, x86_pb_vex3);
            nbytes += x86_out8(buf, c.vex3.data[0]);
            nbytes += x86_out8(buf, c.vex3.data[1]);
            break;
        case x86_ce_evex >> x86_ce_shift:
            nbytes += x86_out8(buf, x86_pb_evex);
            nbytes += x86_out8(buf, c.evex.data[0]);
            nbytes += x86_out8(buf, c.evex.data[1]);
            nbytes += x86_out8(buf, c.evex.data[2]);
            break;
    }

    /* map */
    switch (x86_codec_field_cm(&c) >> x86_cm_shift) {
    case x86_cm_none >> x86_cm_shift:
        break;
    case x86_cm_0f   >> x86_cm_shift:
        nbytes += x86_out8(buf, 0x0f);
        break;
    case x86_cm_0f38 >> x86_cm_shift:
        nbytes += x86_out16(buf, 0x380f);
        break;
    case x86_cm_0f3a >> x86_cm_shift:
        nbytes += x86_out16(buf, 0x3a0f);
        break;
    }

    /* opcode */
    for (size_t i = 0; i < c.opclen; i++) {
        nbytes += x86_out8(buf, c.opc[i]);
    }

    /* ModRM and SIB */
    if (x86_codec_has_modrm(&c)) {
        nbytes += x86_out8(buf, c.modrm.data[0]);

        uchar mod = (c.modrm.data[0] >> x86_mod_shift) & x86_mod_mask;
        uchar rm = (c.modrm.data[0] >> x86_rm_shift) & x86_rm_mask;

        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            /* there is no SIB in real mode */
            if (!x86_codec_is16(&c) && rm == x86_rm_disp_sib) {
                nbytes += x86_out8(buf, c.sib.data[0]);
            }
            break;
        case x86_mod_reg: break;
        }
        switch (mod) {
        case x86_mod_disp0:
            /* this is RIP-relative in amd64 mode */
            if (rm == x86_rm_disp0_iw) {
                if (x86_codec_is16(&c)) {
                    nbytes += x86_out16(buf, (u16)c.disp32);
                } else {
                    nbytes += x86_out32(buf, (u32)c.disp32);
                }
            }
            break;
        case x86_mod_disp8:
            nbytes += x86_out8(buf, (u8)c.disp32);
            break;
        case x86_mod_dispw:
            if (x86_codec_is16(&c)) {
                nbytes += x86_out16(buf, (u16)c.disp32); break;
            } else {
                nbytes += x86_out32(buf, (u32)c.disp32); break;
            }
        case x86_mod_reg: break;
        }
    }

    /* additional immediate used by CALLF/JMPF/ENTER */
    if (x86_codec_has_i16e(&c)) {
        nbytes += x86_out16(buf, (u16)c.imm16e);
    }

    /* immediate */
    switch (x86_codec_field_ci(&c) >> x86_ci_shift) {
    case x86_ci_iw >> x86_ci_shift: /* iw */
        if (x86_codec_is16(&c) ^ x86_codec_has_osize(&c)) {
            nbytes += x86_out16(buf, (u16)c.imm32);
        } else {
            nbytes += x86_out32(buf, (u32)c.imm32);
        }
        break;
    case x86_ci_ib >> x86_ci_shift:
        nbytes += x86_out8(buf, (u8)c.imm32);
        break;
    case x86_ci_i16 >> x86_ci_shift:
        nbytes += x86_out16(buf, (u16)c.imm32);
        break;
    case x86_ci_i32 >> x86_ci_shift:
        nbytes += x86_out32(buf, (u32)c.imm32);
        break;
    case x86_ci_i64 >> x86_ci_shift:
        nbytes += x86_out64(buf, (u64)c.imm64);
        break;
    }

    *len = nbytes;
    return 0;
}

static int x86_filter_opdata(x86_codec *c, x86_opc_data *d, uint w)
{
    if (x86_codec_is16(c) && !x86_mode_has16(d->mode)) return -1;
    if (x86_codec_is32(c) && !x86_mode_has32(d->mode)) return -1;
    if (x86_codec_is64(c) && !x86_mode_has64(d->mode)) return -1;

    switch (x86_codec_field_ce(c) >> x86_ce_shift) {
    case x86_ce_rex >> x86_ce_shift:
        if (x86_enc_filter_rex(c->rex, d->enc) < 0) return -1;
        break;
    case x86_ce_rex2 >> x86_ce_shift:
        if (x86_enc_filter_rex2(c->rex2, d->enc) < 0) return -1;
        break;
    case x86_ce_vex2 >> x86_ce_shift:
        if (x86_enc_filter_vex2(c->vex2, d->enc) < 0) return -1;
        break;
    case x86_ce_vex3 >> x86_ce_shift:
        if (x86_enc_filter_vex3(c->vex3, d->enc) < 0) return -1;
        break;
    case x86_ce_evex >> x86_ce_shift:
        if (x86_enc_filter_evex(c->evex, d->enc) < 0) return -1;
        break;
    }

    if (x86_enc_has_o16(d->enc)) {
        switch (x86_enc_width(d->enc)) {
        case x86_enc_w_ww:
        case x86_enc_w_wx:
            if (!(x86_codec_is16(c) ^ x86_codec_has_osize(c)) || w) return -1;
            break;
        }
    }
    if (x86_enc_has_o32(d->enc)) {
        switch (x86_enc_width(d->enc)) {
        case x86_enc_w_ww:
            /* .ww means no 32-bit operands in 64-bit mode */
            if ((x86_codec_is16(c) ^ x86_codec_has_osize(c)) ||
                 x86_codec_is64(c)) return -1;
            break;
        case x86_enc_w_wx:
            if ((x86_codec_is16(c) ^ x86_codec_has_osize(c)) || w) return -1;
            break;
        }
    }
    if (x86_enc_has_o64(d->enc)) {
        switch (x86_enc_width(d->enc)) {
        case x86_enc_w_ww:
            /* .ww means ignores W=1 in 64-bit mode */
            if (!x86_codec_is64(c)) return -1;
            break;
        case x86_enc_w_wx:
            if (!x86_codec_is64(c) || !w) return -1;
            break;
        }
    }

    return 0;
}

static int x86_parse_encoding(x86_buffer *buf, x86_codec *c,
    x86_opc_data *d, size_t *len)
{
    size_t nbytes = 0;

    /* parse SIB and displacement */
    if (x86_codec_has_modrm(c)) {
        uchar modrm = c->modrm.data[0];
        uchar mod = (modrm >> x86_mod_shift) & x86_mod_mask;
        uchar rm = (modrm >> x86_rm_shift) & x86_rm_mask;
        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            /* there is no SIB in real mode */
            if (!x86_codec_is16(c) && rm == x86_rm_disp_sib) {
                c->sib.data[0] = (u8)x86_in8(buf); nbytes += 1;
            }
            break;
        case x86_mod_reg:
            break;
        }
        switch (mod) {
        case x86_mod_disp0:
            /* this is RIP-relative in amd64 mode */
            if (rm == x86_rm_disp0_iw) {
                if (x86_codec_is16(c)) {
                    c->disp32 = (i16)x86_in16(buf); nbytes += 2;
                } else {
                    c->disp32 = (i32)x86_in32(buf); nbytes += 4;
                }
            }
            break;
        case x86_mod_disp8:
            c->disp32 = (i8)x86_in8(buf); nbytes += 1;
            break;
        case x86_mod_dispw:
            if (x86_codec_is16(c)) {
                c->disp32 = (i16)x86_in16(buf); nbytes += 2;
            } else {
                c->disp32 = (i32)x86_in32(buf); nbytes += 4;
            }
        case x86_mod_reg:
            break;
        }
    }

    /* parse immediate */
    switch(x86_enc_imm2(d->enc) >> x86_enc_i2_shift) {
    case x86_enc_i2_i16e >> x86_enc_i2_shift:
        c->imm16e = (i16)x86_in16(buf); nbytes += 2;
        c->flags |= x86_cf_i16e;
        break;
    }
    switch(x86_enc_imm(d->enc) >> x86_enc_i_shift) {
    case x86_enc_i_ib >> x86_enc_i_shift:
        c->imm32 = (i8)x86_in8(buf); nbytes += 1;
        c->flags |= x86_ci_ib;
        break;
    case x86_enc_i_iw >> x86_enc_i_shift:
        if (x86_codec_is16(c) ^ x86_codec_has_osize(c)) {
            c->imm32 = (i16)x86_in16(buf); nbytes += 2;
        } else {
            c->imm32 = (i32)x86_in32(buf); nbytes += 4;
        }
        c->flags |= x86_ci_iw;
        break;
    case x86_enc_i_i16 >> x86_enc_i_shift:
        c->imm32 = (i16)x86_in16(buf);  nbytes += 2;
        c->flags |= x86_ci_i16;
        break;
    case x86_enc_i_i32 >> x86_enc_i_shift:
        c->imm32 = (i32)x86_in32(buf);  nbytes += 4;
        c->flags |= x86_ci_i32;
        break;
    case x86_enc_i_i64 >> x86_enc_i_shift:
        c->imm64 = (i64)x86_in64(buf);  nbytes += 8;
        c->flags |= x86_ci_i64;
        break;
    }

    *len = nbytes;
    return 0;
}

x86_operands x86_codec_operands(x86_codec *c)
{
    x86_operands q;
    memset(&q, 0, sizeof(q));

    q.osz = x86_codec_has_osize(c);

    if (x86_codec_has_modrm(c)) {
        uchar mod = (c->modrm.data[0] >> x86_mod_shift) & x86_mod_mask;
        uchar rm =  (c->modrm.data[0] >> x86_rm_shift) & x86_rm_mask;
        uchar reg = (c->modrm.data[0] >> x86_reg_shift) & x86_reg_mask;

        /*
         * q.rm contains unextended value from ModRM.rm
         * and is used to indicate SIB/disp encoding.
         *
         * if SIB present, copy SIB.b into q.b
         * if SIB not present, copy ModRM.rm into q.b
         *
         * q.b contains extended ModRM.rm or SIB.b
         */

        q.mod = mod;
        q.rm = rm;
        q.r = reg;

        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            if (!x86_codec_is16(c) && rm == x86_rm_disp_sib) {
                q.b = (c->sib.data[0] >> 0) & 7;
                q.x = (c->sib.data[0] >> 3) & 7;
                q.s = (c->sib.data[0] >> 6) & 3;
            } else {
                q.b = q.rm;
            }
            break;
        case x86_mod_reg:
            q.b = q.rm;
            break;
        }
    } else {
        /* if no ModRM, the register, if present, is in the
         * low 3-bits of the opcode; so stash it in q.r */
        q.r = c->opc[0] & 7;
    }

    switch (x86_codec_field_ce(c) >> x86_ce_shift) {
    case x86_ce_rex >> x86_ce_shift:
        q.b |= ( c->rex.data[0] &    1) << 3; /* [0] -> b[3]*/
        q.x |= ( c->rex.data[0] &    2) << 2; /* [1] -> x[3]*/
        q.r |= ( c->rex.data[0] &    4) << 1; /* [2] -> r[3]*/
        q.w  = ( c->rex.data[0] &    8) >> 3;
        break;
    case x86_ce_rex2 >> x86_ce_shift:
        q.b |= ( c->rex2.data[0] &   1) << 3; /* [0] -> b[3]*/
        q.x |= ( c->rex2.data[0] &   2) << 2; /* [1] -> x[3]*/
        q.r |= ( c->rex2.data[0] &   4) << 1; /* [2] -> r[3]*/
        q.w  = ( c->rex2.data[0] &   8) >> 3;
        q.b |= ( c->rex2.data[0] &  16) >> 0; /* [4] -> b[4]*/
        q.x |= ( c->rex2.data[0] &  32) >> 1; /* [5] -> x[4]*/
        q.r |= ( c->rex2.data[0] &  64) >> 2; /* [6] -> r[4]*/
        break;
    case x86_ce_vex2 >> x86_ce_shift:
        q.r |= (~c->vex2.data[0] & 128) >> 4; /* [7] -> r[3] */
        q.l  = ( c->vex2.data[0] >>  2) & 1;
        q.v  = (~c->vex2.data[0] >>  3) & 15;
        q.osz = (c->vex2.data[0] & 3) == x86_pfx_66;
        break;
    case x86_ce_vex3 >> x86_ce_shift:
        q.b |= (~c->vex3.data[0] &  32) >> 2; /* [5] -> b[3]*/
        q.x |= (~c->vex3.data[0] &  64) >> 3; /* [6] -> x[3]*/
        q.r |= (~c->vex3.data[0] & 128) >> 4; /* [7] -> r[3]*/
        q.l  = ( c->vex3.data[1] >>  2) & 1;
        q.v  = (~c->vex3.data[1] >>  3) & 15;
        q.w  = ( c->vex3.data[1] >>  7) & 1;
        q.osz = (c->vex3.data[1] & 3) == x86_pfx_66;
        break;
    case x86_ce_evex >> x86_ce_shift:
        q.b |= (~c->evex.data[0] &  32) >> 2; /* [5] -> b[3]*/
        q.x |= (~c->evex.data[0] &  64) >> 3; /* [6] -> x[3]*/
        q.r |= (~c->evex.data[0] & 128) >> 4; /* [7] -> r[3]*/
        q.b |= ( c->evex.data[0] &   8) << 1; /* [3] -> b[4]*/
        q.x |= (~c->evex.data[1] &   4) << 2; /* [2] -> x[4]*/
        q.r |= (~c->evex.data[0] &  16) >> 0; /* [4] -> r[4]*/
        q.v  = (~c->evex.data[1] >>  3) & 15;
        q.v |= (~c->evex.data[2] &   8) << 1; /* [3] -> v[4]*/
        q.k  = ( c->evex.data[2] >>  0) & 7;
        q.l  = ( c->evex.data[2] >>  5) & 3;
        q.osz = (c->evex.data[1] & 3) == x86_pfx_66;
        break;
    }

    return q;
}

uint x86_codec_addr_size(x86_codec *c)
{
    /* todo - handle address size prefix */
    if (x86_codec_is32(c)) return x86_opr_size_32;
    if (x86_codec_is64(c)) return x86_opr_size_64;
    return x86_opr_size_16;
}

const char* x86_ptr_size_str(uint sz)
{
    switch (sz) {
    case x86_opr_size_8: return "byte ptr";
    case x86_opr_size_16: return "word ptr";
    case x86_opr_size_32: return "dword ptr";
    case x86_opr_size_64: return "qword ptr";
    case x86_opr_size_80: return "tword ptr";
    case x86_opr_size_128: return "xmmword ptr";
    case x86_opr_size_256: return "ymmword ptr";
    case x86_opr_size_512: return "zmmword ptr";
    default: return "ptr";
    }
}

int x86_opr_mem_size(uint opr)
{
    switch (opr & x86_opr_mem_mask) {
    case x86_opr_m8: return x86_opr_size_8;
    case x86_opr_m16: return x86_opr_size_16;
    case x86_opr_m32: return x86_opr_size_32;
    case x86_opr_m64: return x86_opr_size_64;
    case x86_opr_m80: return x86_opr_size_80;
    case x86_opr_m128: return x86_opr_size_128;
    case x86_opr_m256: return x86_opr_size_256;
    case x86_opr_m512: return x86_opr_size_512;
    default: return x86_opr_size_word;
    }
}

uint x86_opr_reg_size(x86_codec *c, x86_operands q, uint opr, uint enc)
{
    uint oprty = (opr & x86_opr_type_mask);
    uint oprsz = (opr & x86_opr_size_mask);

    /* operand contains the register size */
    if (oprsz != 0 && oprsz != x86_opr_size_word) {
        return oprsz;
    }
    /* 'rw' deduce size from mode, operand size prefix and REX.W */
    else if (oprty == x86_opr_reg && oprsz == x86_opr_size_word)
    {
        switch (x86_enc_width(enc)) {
        case x86_enc_w_wb: return x86_opr_size_8;
        case x86_enc_w_ww:
            if (x86_codec_is16(c))
                return (q.osz ? x86_opr_size_32 : x86_opr_size_16);
            if (x86_codec_is32(c))
                return (q.osz ? x86_opr_size_16 : x86_opr_size_32);
            if (x86_codec_is64(c))
                return (q.osz ? x86_opr_size_16 : x86_opr_size_64);
            break;
        case x86_enc_w_wx:
            if (x86_codec_is16(c))
                return (q.osz ? x86_opr_size_32 : x86_opr_size_16);
            if (x86_codec_is32(c))
                return (q.osz ? x86_opr_size_16 : x86_opr_size_32);
            if (x86_codec_is64(c))
                return (q.osz ? x86_opr_size_16 :
                        q.w ? x86_opr_size_64 : x86_opr_size_32);
            break;
        case x86_enc_w_w0: return x86_opr_size_32;
        case x86_enc_w_w1: return x86_opr_size_64;
        default: break;
        }
    }

    return x86_codec_addr_size(c);
}

static uint x86_sized_gpr(x86_codec *c, uint reg, uint opr)
{
    switch (opr & x86_opr_size_mask) {
    case x86_opr_size_8:
        /* legacy encoding selects ah/cd/dh/bh instead of spl/bpl/sil/dil */
        if ((x86_codec_field_ce(c)) == x86_ce_none &&
           ((reg & 31) >=4 && (reg & 31) < 8)) return x86_reg_bl | (reg & 31);
        return x86_reg_b | (reg & 31);
    case x86_opr_size_16: return x86_reg_w | (reg & 31);
    case x86_opr_size_32: return x86_reg_d | (reg & 31);
    case x86_opr_size_64: return x86_reg_q | (reg & 31);
    case x86_opr_size_128: return x86_reg_xmm | (reg & 31);
    case x86_opr_size_256: return x86_reg_ymm | (reg & 31);
    case x86_opr_size_512: return x86_reg_zmm | (reg & 31);
    default: return reg;
    }
}

static uint x86_disp8_scale(x86_codec *c, uint regsz)
{
    /* todo - EVEX compressed displacement for disp8 needs element size
     * and tuple type. this code is a simple but broken heuristic. */
    if (x86_codec_field_ce(c) == x86_ce_evex) {
        switch (regsz) {
        case x86_opr_size_8: return 1;
        case x86_opr_size_16: return 2;
        case x86_opr_size_32: return 4;
        case x86_opr_size_64: return 8;
        case x86_opr_size_128: return 16;
        case x86_opr_size_256: return 32;
        case x86_opr_size_512: return 64;
        default: break;
        }
    }
    return 1;
}

static const struct x86_mod_data { ushort r8, rw, b, x; } x86_mod_real[8] =
{
    /* todo - handle real mode base index encoding */
    { x86_al,   x86_ax,   x86_bx,   x86_si   },
    { x86_cl,   x86_cx,   x86_bx,   x86_di   },
    { x86_dl,   x86_dx,   x86_bp,   x86_si   },
    { x86_bl,   x86_bx,   x86_bp,   x86_di   },
    { x86_ah,   x86_sp,   x86_si,   x86_none },
    { x86_ch,   x86_bp,   x86_di,   x86_none },
    { x86_dh,   x86_si,   x86_bp,   x86_none },
    { x86_bh,   x86_di,   x86_bx,   x86_none },
};

x86_opr_mrm_formats x86_opr_mrm_formats_intel_hex = {
    "%s [rip %s 0x%x]",
    "%s [rip]",
    "%s [%s0x%x]",
    "%s [%s + %d*%s]",
    "%s [%s + %s]",
    "%s [%s]",
    "%s [%s + %d*%s %s 0x%x]",
    "%s [%s + %s %s 0x%x]",
    "%s [%s %s 0x%x]",
    "%s"
};

x86_opr_mrm_formats x86_opr_mrm_formats_intel_dec = {
    "%s [rip %s %d]",
    "%s [rip]",
    "%s [%s%d]",
    "%s [%s + %d*%s]",
    "%s [%s + %s]",
    "%s [%s]",
    "%s [%s + %d*%s %s %d]",
    "%s [%s + %s %s %d]",
    "%s [%s %s %d]",
    "%s"
};

size_t x86_opr_intel_mrm_str_internal(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc, x86_opr_mrm_formats *fmt)
{
    uint regsz = x86_opr_reg_size(c, q, opr, enc);
    uint memsz = x86_opr_mem_size(opr);
    uint ptrsz = memsz == x86_opr_size_word ? regsz : memsz;
    uint addrsz = x86_codec_addr_size(c);
    int disp = c->disp32;

    switch(q.mod) {
    case x86_mod_disp0:
        if (q.rm == x86_rm_disp0_iw) {
            if (x86_codec_is64(c)) {
                if (disp) {
                    return snprintf(buf, buflen, fmt->ptr_rip_disp,
                        x86_ptr_size_str(ptrsz),
                        disp < 0 ? "-" : "+",
                        disp < 0 ? -disp : disp);
                } else {
                    return snprintf(buf, buflen, fmt->ptr_rip,
                        x86_ptr_size_str(ptrsz));
                }
            } else {
                return snprintf(buf, buflen, fmt->ptr_disp,
                    x86_ptr_size_str(ptrsz),
                    disp < 0 ? "-" : "",
                    disp < 0 ? -disp : disp);
            }
        } else if (q.rm == x86_rm_disp_sib) {
            if (q.s) {
                return snprintf(buf, buflen, fmt->ptr_sib_reg_scaled_reg,
                    x86_ptr_size_str(ptrsz),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)), (1 << q.s),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)));
            } else {
                return snprintf(buf, buflen, fmt->ptr_sib_reg_reg,
                    x86_ptr_size_str(ptrsz),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)));
            }
        } else {
            return snprintf(buf, buflen, fmt->ptr_reg,
                x86_ptr_size_str(ptrsz),
                x86_reg_name(x86_sized_gpr(c, q.b, addrsz)));
        }
        break;
    case x86_mod_disp8:
         disp *= x86_disp8_scale(c, regsz);
         /* fallthrough */
    case x86_mod_dispw:
        if (q.rm == x86_rm_disp_sib) {
            if (q.s) {
                return snprintf(buf, buflen, fmt->ptr_sib_reg_scaled_reg_disp,
                    x86_ptr_size_str(ptrsz),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)), (1 << q.s),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)),
                    disp < 0 ? "-" : "+",
                    disp < 0 ? -disp : disp);
            } else {
                return snprintf(buf, buflen, fmt->ptr_sib_reg_reg_disp,
                    x86_ptr_size_str(ptrsz),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)),
                    disp < 0 ? "-" : "+",
                    disp < 0 ? -disp : disp);
            }
        } else {
            return snprintf(buf, buflen, fmt->ptr_sib_reg_disp,
                x86_ptr_size_str(ptrsz),
                x86_reg_name(x86_sized_gpr(c, q.b, addrsz)),
                disp < 0 ? "-" : "+",
                disp < 0 ? -disp : disp);
        }
        break;
    case x86_mod_reg:
        return snprintf(buf, buflen, fmt->reg,
            x86_reg_name(x86_sized_gpr(c, q.b, regsz)));
    }
    return 0;
}

size_t x86_opr_intel_mrm_dec_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc)
{
    return x86_opr_intel_mrm_str_internal(buf, buflen, c, q, opr, enc,
        &x86_opr_mrm_formats_intel_dec);
}

size_t x86_opr_intel_mrm_hex_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc)
{
    return x86_opr_intel_mrm_str_internal(buf, buflen, c, q, opr, enc,
        &x86_opr_mrm_formats_intel_hex);
}

size_t x86_opr_intel_reg_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(c, q, opr, enc);
    uint regn_r = x86_sized_gpr(c, q.r, regsz);
    const char* reg_r = x86_reg_name(regn_r);
    return snprintf(buf, buflen, "%s", reg_r);
}

size_t x86_opr_intel_vec_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(c, q, opr, enc);
    uint regn_r = x86_sized_gpr(c, q.v, regsz);
    const char* reg_r = x86_reg_name(regn_r);
    return snprintf(buf, buflen, "%s", reg_r);
}

size_t x86_opr_intel_imm_hex_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q,  uint opr, uint enc)
{
    if ((x86_codec_field_ci(c)) == x86_ci_i64) {
        llong imm = c->imm64;
        return snprintf(buf, buflen, "%s0x%llx",
            imm < 0 ? "-" : "", imm < 0 ? -imm : imm);
    } else {
        int imm = c->imm32;
        return snprintf(buf, buflen, "%s0x%x",
            imm < 0 ? "-" : "", imm < 0 ? -imm : imm);
    }
}

size_t x86_opr_intel_imm_dec_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q,  uint opr, uint enc)
{
    if ((x86_codec_field_ci(c)) == x86_ci_i64) {
        llong imm = c->imm64;
        return snprintf(buf, buflen, "%s%lld",
            imm < 0 ? "-" : "", imm < 0 ? -imm : imm);
    } else {
        int imm = c->imm32;
        return snprintf(buf, buflen, "%s%d",
            imm < 0 ? "-" : "", imm < 0 ? -imm : imm);
    }
}

uint x86_opr_intel_const_reg(x86_codec *c, x86_operands q, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(c, q, opr, enc);
    uint addrsz = x86_codec_addr_size(c);

    switch (opr) {
    case x86_opr_reg_al: return x86_al;
    case x86_opr_reg_cl: return x86_cl;
    case x86_opr_reg_ax: return x86_ax;
    case x86_opr_reg_cx: return x86_cx;
    case x86_opr_reg_dx: return x86_dx;
    case x86_opr_reg_bx: return x86_bx;
    case x86_opr_reg_eax: return x86_eax;
    case x86_opr_reg_ecx: return x86_ecx;
    case x86_opr_reg_edx: return x86_edx;
    case x86_opr_reg_ebx: return x86_ebx;
    case x86_opr_reg_rax: return x86_rax;
    case x86_opr_reg_rcx: return x86_rcx;
    case x86_opr_reg_rdx: return x86_rdx;
    case x86_opr_reg_rbx: return x86_rbx;
    case x86_opr_reg_aw: return x86_sized_gpr(c, x86_al, regsz);
    case x86_opr_reg_cw: return x86_sized_gpr(c, x86_cl, regsz);
    case x86_opr_reg_dw: return x86_sized_gpr(c, x86_dl, regsz);
    case x86_opr_reg_bw: return x86_sized_gpr(c, x86_bl, regsz);
    case x86_opr_reg_pa: return x86_sized_gpr(c, x86_al, addrsz);
    case x86_opr_reg_pc: return x86_sized_gpr(c, x86_cl, addrsz);
    case x86_opr_reg_pd: return x86_sized_gpr(c, x86_dl, addrsz);
    case x86_opr_reg_pb: return x86_sized_gpr(c, x86_bl, addrsz);
    case x86_opr_reg_psi: return x86_sized_gpr(c, x86_sil, addrsz);
    case x86_opr_reg_pdi: return x86_sized_gpr(c, x86_dil, addrsz);
    default: break;
    }
    return -1;
}

size_t x86_opr_intel_const_str(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(c, q, opr, enc);
    uint addrsz = x86_codec_addr_size(c);
    uint regname = x86_opr_intel_const_reg(c, q, opr, enc);

    if (regname >= 0) {
        return snprintf(buf, buflen, "%s", x86_reg_name(regname));
    }

    switch (opr) {
    case x86_opr_1: return snprintf(buf, buflen, "1");
    case x86_opr_reg_xmm0: return snprintf(buf, buflen, "%s", "<xmm0>");
    case x86_opr_reg_xmm0_7: return snprintf(buf, buflen, "%s", "<xmm0-7>");
    default: return snprintf(buf, buflen, "%s", "<unknown>");
    }
}

x86_opr_formatter x86_format_intel_hex =
{
    .fmt_const = &x86_opr_intel_const_str,
    .fmt_imm = &x86_opr_intel_imm_hex_str,
    .fmt_reg = &x86_opr_intel_reg_str,
    .fmt_mrm = &x86_opr_intel_mrm_hex_str,
    .fmt_vec = &x86_opr_intel_vec_str
};

x86_opr_formatter x86_format_intel_dec =
{
    .fmt_const = &x86_opr_intel_const_str,
    .fmt_imm = &x86_opr_intel_imm_dec_str,
    .fmt_reg = &x86_opr_intel_reg_str,
    .fmt_mrm = &x86_opr_intel_mrm_dec_str,
    .fmt_vec = &x86_opr_intel_vec_str
};

static size_t x86_format_operand(char *buf, size_t buflen, x86_codec *c,
    x86_operands q, uint ord, uint opr, uint enc, x86_opr_formatter *fmt)
{
    switch(ord & x86_ord_type_mask) {
    case x86_ord_const: return fmt->fmt_const(buf, buflen, c, q, opr, enc);
    case x86_ord_imm: return fmt->fmt_imm(buf, buflen, c, q, opr, enc);
    case x86_ord_reg: return fmt->fmt_reg(buf, buflen, c, q, opr, enc);
    case x86_ord_mrm: return fmt->fmt_mrm(buf, buflen, c, q, opr, enc);
    case x86_ord_vec: return fmt->fmt_vec(buf, buflen, c, q, opr, enc);
    default: return 0;
    }
}

size_t x86_format_op(char *buf, size_t buflen, x86_ctx *ctx, x86_codec *c)
{
    const x86_opc_data *d = ctx->idx->map + c->rec;
    const x86_opr_data *o = x86_opr_table + d->opr;
    const x86_ord_data *s = x86_ord_table + d->ord;

    x86_operands q = x86_codec_operands(c);

    size_t len = 0;
    len += snprintf(buf+len, buflen-len, "%s", x86_op_names[d->op]);
    for (size_t i = 0; i < array_size(o->opr) && o->opr[i]; i++)
    {
        len += snprintf(buf+len, buflen-len, i == 0 ? "\t" : ", ");
        len += x86_format_operand(buf+len, buflen-len, c, q,
            s->ord[i], o->opr[i], d->enc, &x86_format_intel_dec);
    }

    return len;
}

size_t x86_format_hex(char *buf, size_t buflen, uchar *data, size_t datalen)
{
    size_t len = 0;
    for(size_t i = 0; i < datalen && i < 11; i++) {
        len += snprintf(buf+len, buflen-len, i == 0 ? "\t" : " ");
        len += snprintf(buf+len, buflen-len, "%02hhx", data[i]);
    }
    size_t tabs = datalen < 10 ? (40 - datalen*3) / 8 : 1;
    for (size_t i = 0; i < tabs ; i++) {
        len += snprintf(buf+len, buflen-len, "\t");
    }
    return len;
}

int x86_codec_read(x86_ctx *ctx, x86_buffer *buf, x86_codec *c,
    size_t *len, size_t limit)
{
    x86_state state = x86_state_top;
    size_t nbytes = 0;
    uint t = 0, m = 0, w = 0, p = 0, l = 0, mode = ctx->mode;
    uint sufmask = ~(x86_enc_t_mask | x86_enc_prexw_mask | x86_enc_m_mask);
    x86_opc_data k = { 0 }, *r = NULL;
    uchar b = 0, lastp = 0;

    memset(c, 0, sizeof(x86_codec));
    switch(mode) {
    case x86_modes_32: c->flags |= x86_cf_ia32; break;
    case x86_modes_64: c->flags |= x86_cf_amd64; break;
    }
 
    while (state != x86_state_done) {
        nbytes += x86_buffer_read(buf, &b, 1);
        switch (state) {
        case x86_state_top:
            switch (b) {
            case 0x40: case 0x41: case 0x42: case 0x43:
            case 0x44: case 0x45: case 0x46: case 0x47:
            case 0x48: case 0x49: case 0x4a: case 0x4b:
            case 0x4c: case 0x4d: case 0x4e: case 0x4f:
                c->rex.data[0] = b;
                c->flags |= x86_ce_rex;
                w = (c->rex.data[0] >> 3) & 1;
                t = x86_table_lex;
                state = x86_state_rex_opcode;
                break;
            case x86_pb_26:
            case x86_pb_2e:
            case x86_pb_36:
            case x86_pb_3e:
            case x86_pb_64:
            case x86_pb_65:
                state = x86_state_segment;
                goto segment_reparse;
            case x86_pb_66:
            case x86_pb_67:
            case x86_pb_9b:
            case x86_pb_f0:
            case x86_pb_f2:
            case x86_pb_f3:
                state = x86_state_legacy;
                goto legacy_reparse;
            case x86_pb_62:
                nbytes += x86_buffer_read(buf, c->evex.data, 3);
                c->flags |= x86_ce_evex;
                m = (c->evex.data[0] >> 0) & 7;
                w = (c->evex.data[1] >> 7) & 1;
                p = (c->evex.data[1] >> 0) & 3;
                l = (c->evex.data[2] >> 5) & 3;
                t = x86_table_evex;
                state = x86_state_vex_opcode;
                break;
            case x86_pb_c4:
                nbytes += x86_buffer_read(buf, c->vex3.data, 2);
                c->flags |= x86_ce_vex3;
                m = (c->vex3.data[0] >> 0) & 31;
                w = (c->vex3.data[1] >> 7) & 1;
                p = (c->vex3.data[1] >> 0) & 3;
                l = (c->vex3.data[1] >> 2) & 1;
                t = x86_table_vex;
                state = x86_state_vex_opcode;
                break;
            case x86_pb_c5:
                nbytes += x86_buffer_read(buf, c->vex2.data, 1);
                c->flags |= x86_ce_vex2;
                m = x86_map_0f;
                p = (c->vex2.data[0] >> 0) & 3;
                l = (c->vex2.data[0] >> 2) & 1;
                t = x86_table_vex;
                state = x86_state_vex_opcode;
                break;
            case x86_pb_d5:
                nbytes += x86_buffer_read(buf, c->rex2.data, 1);
                c->flags |= x86_ce_rex2;
                m = (c->rex2.data[0] >> 7) & 1;
                w = (c->rex2.data[0] >> 3) & 1;
                t = x86_table_lex;
                state = x86_state_lex_opcode;
                break;
            case 0x0f:
                t = x86_table_lex;
                state = x86_state_map_0f;
                break;
            default:
                m = x86_map_none;
                t = x86_table_lex;
                state = x86_state_lex_opcode;
                goto lex_reparse;
            }
            break;
      case x86_state_segment: segment_reparse:
            switch (b) {
            case 0x40: case 0x41: case 0x42: case 0x43:
            case 0x44: case 0x45: case 0x46: case 0x47:
            case 0x48: case 0x49: case 0x4a: case 0x4b:
            case 0x4c: case 0x4d: case 0x4e: case 0x4f:
                c->rex.data[0] = b;
                c->flags |= x86_ce_rex;
                w = (c->rex.data[0] >> 3) & 1;
                t = x86_table_lex;
                state = x86_state_rex_opcode;
                break;
            case x86_pb_26: c->seg = x86_seg_es; state = x86_state_legacy; break;
            case x86_pb_2e: c->seg = x86_seg_cs; state = x86_state_legacy; break;
            case x86_pb_36: c->seg = x86_seg_ss; state = x86_state_legacy; break;
            case x86_pb_3e: c->seg = x86_seg_ds; state = x86_state_legacy; break;
            case x86_pb_64: c->seg = x86_seg_fs; state = x86_state_legacy; break;
            case x86_pb_65: c->seg = x86_seg_gs; state = x86_state_legacy; break;
            case x86_pb_66:
            case x86_pb_67:
            case x86_pb_9b:
            case x86_pb_f0:
            case x86_pb_f2:
            case x86_pb_f3:
                state = x86_state_legacy;
                goto legacy_reparse;
            case x86_pb_62:
            case x86_pb_c4:
            case x86_pb_c5:
            case x86_pb_d5:
                goto err;
            case 0x0f:
                t = x86_table_lex;
                state = x86_state_map_0f;
                break;
            default:
                m = x86_map_none;
                t = x86_table_lex;
                state = x86_state_lex_opcode;
                goto lex_reparse;
            }
            break;
        case x86_state_legacy: legacy_reparse:
            switch (b) {
            case 0x40: case 0x41: case 0x42: case 0x43:
            case 0x44: case 0x45: case 0x46: case 0x47:
            case 0x48: case 0x49: case 0x4a: case 0x4b:
            case 0x4c: case 0x4d: case 0x4e: case 0x4f:
                c->rex.data[0] = b;
                c->flags |= x86_ce_rex;
                w = (c->rex.data[0] >> 3) & 1;
                t = x86_table_lex;
                state = x86_state_rex_opcode;
                break;
            case x86_pb_26:
            case x86_pb_2e:
            case x86_pb_36:
            case x86_pb_3e:
            case x86_pb_64:
            case x86_pb_65:
            case x86_pb_62:
            case x86_pb_c4:
            case x86_pb_c5:
            case x86_pb_d5:
                goto err;
            case x86_pb_66: lastp = b; c->flags |= x86_cp_osize; break;
            case x86_pb_67: lastp = b; c->flags |= x86_cp_asize; break;
            case x86_pb_9b: lastp = b; c->flags |= x86_cp_wait; break;
            case x86_pb_f0: lastp = b; c->flags |= x86_cp_lock; break;
            case x86_pb_f2: lastp = b; c->flags |= x86_cp_repne; break;
            case x86_pb_f3: lastp = b; c->flags |= x86_cp_rep; break;
            case 0x0f:
                t = x86_table_lex;
                state = x86_state_map_0f;
                break;
            default:
                m = x86_map_none;
                t = x86_table_lex;
                state = x86_state_lex_opcode;
                goto lex_reparse;
            }
            break;
        case x86_state_rex_opcode:
            switch (b) {
            case 0x0f:
                state = x86_state_map_0f;
                break;
            default:
                state = x86_state_lex_opcode;
                goto lex_reparse;
            }
            break;
        case x86_state_map_0f:
            switch (b) {
            case 0x38:
                c->flags |= x86_cm_0f38;
                m = x86_map_0f38;
                state = x86_state_lex_opcode;
                break;
            case 0x3a:
                c->flags |= x86_cm_0f3a;
                m = x86_map_0f3a;
                state = x86_state_lex_opcode;
                break;
            default:
                c->flags |= x86_cm_0f;
                m = x86_map_0f;
                state = x86_state_lex_opcode;
                goto lex_reparse;
            }
            break;
        case x86_state_lex_opcode: lex_reparse:
            k.enc |= ((t << x86_enc_t_shift) & x86_enc_t_mask)
                  |  ((m << x86_enc_m_shift) & x86_enc_m_mask);
            switch (lastp) {
            case 0x66: k.enc |= x86_enc_p_66; break;
            case 0x9b: k.enc |= x86_enc_p_9b; break;
            case 0xf2: k.enc |= x86_enc_p_f2; break;
            case 0xf3: k.enc |= x86_enc_p_f3; break;
            }
            state = x86_state_done;
            break;
        case x86_state_vex_opcode:
            k.enc |= ((t << x86_enc_t_shift) & x86_enc_t_mask)
                  |  ((m << x86_enc_m_shift) & x86_enc_m_mask);
            switch (p) {
            case x86_pfx_66: k.enc |= x86_enc_p_66; break;
            case x86_pfx_f2: k.enc |= x86_enc_p_f2; break;
            case x86_pfx_f3: k.enc |= x86_enc_p_f3; break;
            }
            state = x86_state_done;
            break;
        default:
            abort();
        }
    };

    /* populate opcode for table lookup */
    k.mode = mode;
    c->opc[0] = k.opc[0] = b;
    nbytes += x86_buffer_read(buf, &b, 1);
    c->opc[1] = k.opc[1] = b;
    k.opm[0] = k.opm[1] = 0xff;

    /* if REX.W=1 first attempt to lookup W=1 record */
    if (w) {
        k.enc |= x86_enc_p_rexw;
        k.enc = (k.enc & ~sufmask);
        x86_debugf("table_lookup { type:%x prefix:%x map:%x "
            "opc:[%02hhx %02hhx] opm:[%02hhx %02hhx] }",
            (k.enc & x86_enc_t_mask) >> x86_enc_t_shift,
            (k.enc & x86_enc_p_mask) >> x86_enc_p_shift,
            (k.enc & x86_enc_m_mask) >> x86_enc_m_shift,
            k.opc[0], k.opc[1], k.opm[0], k.opm[1]);
        r = x86_table_lookup(ctx->idx, &k);
        while (r < ctx->idx->map + ctx->idx->map_count) {
            k.enc = (k.enc & ~sufmask) | (r->enc & sufmask);
            k.enc = (k.enc & ~x86_enc_p_rexw) | (r->enc & x86_enc_p_rexw);
            size_t oprec = (r - ctx->idx->map);
            x86_debugf("checking opdata %zu", oprec);
            if (debug) x86_print_op(r, 1, 1);
            if (x86_opc_data_compare_masked(&k, r) != 0) {
                x86_debugf("** no matches");
                r = NULL;
                break;
            }
            if (x86_filter_opdata(c, r, 1) == 0) break;
            r++;
        }
    }

    /* if REX.W=0 or search failed lookup W=0/WIG record */
    if (!w || (w && !r)) {
        k.enc &= ~x86_enc_p_rexw;
        k.enc = (k.enc & ~sufmask);
        x86_debugf("table_lookup { type:%x prefix:%x map:%x "
            "opc:[%02hhx %02hhx] opm:[%02hhx %02hhx] }",
            (k.enc & x86_enc_t_mask) >> x86_enc_t_shift,
            (k.enc & x86_enc_p_mask) >> x86_enc_p_shift,
            (k.enc & x86_enc_m_mask) >> x86_enc_m_shift,
            k.opc[0], k.opc[1], k.opm[0], k.opm[1]);
        r = x86_table_lookup(ctx->idx, &k);
        while (r < ctx->idx->map + ctx->idx->map_count) {
            k.enc = (k.enc & ~sufmask) | (r->enc & sufmask);
            size_t oprec = (r - ctx->idx->map);
            x86_debugf("checking opdata %zu", oprec);
            if (debug) x86_print_op(r, 1, 1);
            if (x86_opc_data_compare_masked(&k, r) != 0) {
                x86_debugf("** no matches");
                r = NULL;
                break;
            }
            if (x86_filter_opdata(c, r, 0) == 0) break;
            r++;
        }
    }

    /* check if we have modrm byte */
    switch (x86_enc_func(r->enc)) {
    case x86_enc_f_modrm_r:
    case x86_enc_f_modrm_n:
        /* second byte is modrm */
        c->flags |= x86_cf_modrm;
        c->opclen = 1;
        break;
    case x86_enc_f_opcode:
    case x86_enc_f_opcode_r:
        /* two byte opcode */
        c->opclen = 2;
        break;
    default:
        /* no second opcode byte */
        nbytes -= x86_buffer_unread(buf, 1);
        c->opclen = 1;
        break;
    }

    /* parse encoding */
    if (r) {
        size_t nread = 0;
        if (x86_parse_encoding(buf, c, r, &nread) == 0) {
            if (nbytes + nread <= limit) {
                c->rec = (r - ctx->idx->map);
                nbytes += nread;
            } else if (c->opclen == 1) {
                /* special case opcode peek */
                goto err;
            }
        }
    }

    *len = nbytes;
    return c->rec ? 0 : -1;

err:
    nbytes -= x86_buffer_unread(buf, 1);
    *len = nbytes;
    return -1;
}

x86_ctx* x86_ctx_create(uint mode)
{
    x86_ctx *ctx = malloc(sizeof(x86_ctx));
    ctx->mode = mode;
    ctx->idx = x86_table_build(mode);
    return ctx;
}

void x86_ctx_destroy(x86_ctx *ctx)
{
    free(ctx->idx->page_offsets);
    free(ctx->idx->map);
    free(ctx->idx->acc);
    free(ctx);
}
