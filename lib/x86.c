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
typedef struct x86_codec_opr x86_codec_opr;

enum x86_state
{
    x86_state_top,
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

struct x86_codec_opr
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

x86_map_str x86_prefix_names[] =
{
    { 1 << x86_pn_es,           "es"               },
    { 1 << x86_pn_cs,           "cs"               },
    { 1 << x86_pn_ss,           "ss"               },
    { 1 << x86_pn_ds,           "ds"               },
    { 1 << x86_pn_rex_b,        "rex.b"            },
    { 1 << x86_pn_rex_x,        "rex.x"            },
    { 1 << x86_pn_rex_r,        "rex.r"            },
    { 1 << x86_pn_rex_w,        "rex.w"            },
    { 1 << x86_pn_evex,         "evex"             },
    { 1 << x86_pn_fs,           "fs"               },
    { 1 << x86_pn_gs,           "gs"               },
    { 1 << x86_pn_osize,        "osize"            },
    { 1 << x86_pn_asize,        "asize"            },
    { 1 << x86_pn_wait,         "wait"             },
    { 1 << x86_pn_vex3,         "vex3"             },
    { 1 << x86_pn_vex2,         "vex2"             },
    { 1 << x86_pn_rex2,         "rex2"             },
    { 1 << x86_pn_lock,         "lock"             },
    { 1 << x86_pn_repne,        "repne"            },
    { 1 << x86_pn_rep,          "rep"              },
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
    { x86_enc_t_i16e,           "i16e"             },
    { x86_enc_t_i64,            "i64"              },
    { x86_enc_t_i32,            "i32"              },
    { x86_enc_t_i16,            "i16"              },
    { x86_enc_t_iw,             "iw"               },
    { x86_enc_t_ib,             "ib"               },
    { x86_enc_t_opcode,         ""                 },
    { x86_enc_t_opcode_r,       "+r"               },
    { x86_enc_t_modrm_7,        "/7"               },
    { x86_enc_t_modrm_6,        "/6"               },
    { x86_enc_t_modrm_5,        "/5"               },
    { x86_enc_t_modrm_4,        "/4"               },
    { x86_enc_t_modrm_3,        "/3"               },
    { x86_enc_t_modrm_2,        "/2"               },
    { x86_enc_t_modrm_1,        "/1"               },
    { x86_enc_t_modrm_0,        "/0"               },
    { x86_enc_t_modrm_r,        "/r"               },
    { x86_enc_t_rep,            ".rep"             },
    { x86_enc_t_lock,           ".lock"            },
    { x86_enc_t_a64,            ".a64"             },
    { x86_enc_t_a32,            ".a32"             },
    { x86_enc_t_a16,            ".a16"             },
    { x86_enc_t_o64,            ".o64"             },
    { x86_enc_t_o32,            ".o32"             },
    { x86_enc_t_o16,            ".o16"             },
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
    { x86_enc_p_f3,             ".f3"              },
    { x86_enc_p_f2,             ".f2"              },
    { x86_enc_p_9b,             ".9b"              },
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

size_t x86_prefix_namen(char * buf, size_t len, uint ord, const char *sep)
{
    size_t count = 0;
    int ret = 0;
    for (;;) {
        if (ord == 0) break;
        uint tz = ctz(ord);
        if (tz < sizeof(x86_prefixes)/sizeof(x86_prefixes[0])) {
            if (buf == NULL || len - count <= len) {
                ret = snprintf(buf ? buf + count : NULL,
                    buf ? len - count : INT_MAX,
                    "%s%02hhx", count == 0 ? "" : sep, x86_prefixes[tz]);
                if (ret > 0) count += ret;
            }
        }
        ord &= ~(1 << tz);
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

size_t x86_prefix_name(char * buf, size_t len, uint ord, const char *sep)
{
    return x86_name_map(x86_prefix_names, buf, len, ord, sep);
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

/*
 *  metadata filters
 */

int x86_enc_filter_rex(x86_rex prefix, x86_enc enc)
{
    uint lex =  ((uint)enc & x86_enc_t_mask) == x86_enc_t_lex;
    uint ew0 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewn =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_wn;
    uint ewb =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_wb;
    uint eww =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_ww;
    uint ewx =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_wx;
    uint ewig = ((uint)enc & x86_enc_w_mask) == x86_enc_w_wig;

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

int x86_enc_filter_rex2(x86_rex2 prefix, x86_enc enc)
{
    uint lex =  ((uint)enc & x86_enc_t_mask) == x86_enc_t_lex;
    uint ew0 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewig = ((uint)enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint em =   ((uint)enc & x86_enc_m_mask) >> x86_enc_m_shift;

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

int x86_enc_filter_vex2(x86_vex2 prefix, x86_enc enc)
{
    uint vex =  ((uint)enc & x86_enc_t_mask) == x86_enc_t_vex;
    uint ew0 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ewig = ((uint)enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint lz =   ((uint)enc & x86_enc_l_mask) == x86_enc_l_lz;
    uint l0 =   ((uint)enc & x86_enc_l_mask) == x86_enc_l_l0;
    uint l1 =   ((uint)enc & x86_enc_l_mask) == x86_enc_l_l1;
    uint l128 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_128;
    uint l256 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_256;
    uint lig =  ((uint)enc & x86_enc_l_mask) == x86_enc_l_lig;
    uint np =   ((uint)enc & x86_enc_p_mask) == x86_enc_p_none;
    uint p66 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_66;
    uint pf2 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_f2;
    uint pf3 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_f3;
    uint em =   ((uint)enc & x86_enc_m_mask) >> x86_enc_m_shift;

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

int x86_enc_filter_vex3(x86_vex3 prefix, x86_enc enc)
{
    uint vex =  ((uint)enc & x86_enc_t_mask) == x86_enc_t_vex;
    uint ew0 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewig = ((uint)enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint lz =   ((uint)enc & x86_enc_l_mask) == x86_enc_l_lz;
    uint l0 =   ((uint)enc & x86_enc_l_mask) == x86_enc_l_l0;
    uint l1 =   ((uint)enc & x86_enc_l_mask) == x86_enc_l_l1;
    uint l128 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_128;
    uint l256 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_256;
    uint lig =  ((uint)enc & x86_enc_l_mask) == x86_enc_l_lig;
    uint np =   ((uint)enc & x86_enc_p_mask) == x86_enc_p_none;
    uint p66 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_66;
    uint pf2 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_f2;
    uint pf3 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_f3;
    uint em =   ((uint)enc & x86_enc_m_mask) >> x86_enc_m_shift;

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

int x86_enc_filter_evex(x86_evex prefix, x86_enc enc)
{
    uint evex = ((uint)enc & x86_enc_t_mask) == x86_enc_t_evex;
    uint ew0 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w0;
    uint ew1 =  ((uint)enc & x86_enc_w_mask) == x86_enc_w_w1;
    uint ewig = ((uint)enc & x86_enc_w_mask) == x86_enc_w_wig;
    uint l128 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_128;
    uint l256 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_256;
    uint l512 = ((uint)enc & x86_enc_l_mask) == x86_enc_l_512;
    uint lig =  ((uint)enc & x86_enc_l_mask) == x86_enc_l_lig;
    uint np =   ((uint)enc & x86_enc_p_mask) == x86_enc_p_none;
    uint p66 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_66;
    uint pf2 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_f2;
    uint pf3 =  ((uint)enc & x86_enc_p_mask) == x86_enc_p_f3;
    uint em =   ((uint)enc & x86_enc_m_mask) >> x86_enc_m_shift;

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

static int x86_op_enc_compare_opcode(const void *p1, const void *p2)
{
    const x86_opc_data *op1 = x86_opc_table + *(size_t*)p1;
    const x86_opc_data *op2 = x86_opc_table + *(size_t*)p2;
    for (size_t i = 0; i < array_size(op1->enc); i++) {
        if (op1->enc[i] < op2->enc[i]) return -1;
        if (op1->enc[i] > op2->enc[i]) return 1;
    }
    return 0;
}

static int x86_op_enc_compare_alpha(const void *p1, const void *p2)
{
    const x86_opc_data *op1 = x86_opc_table + *(size_t*)p1;
    const x86_opc_data *op2 = x86_opc_table + *(size_t*)p2;
    return strcmp(x86_op_names[op1->op], x86_op_names[op2->op]);
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

x86_table_idx x86_opc_table_sorted(x86_table_idx tab, x86_sort sort)
{
    switch (sort) {
    case x86_sort_none:
        break;
    case x86_sort_numeric:
        qsort(tab.idx, tab.count, sizeof(size_t), x86_op_enc_compare_opcode);
        break;
    case x86_sort_alpha:
        qsort(tab.idx, tab.count, sizeof(size_t), x86_op_enc_compare_alpha);
        break;
    }
    return tab;
}

x86_table_idx x86_opc_table_filter(x86_table_idx tab, x86_modes modes)
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

static int x86_map_op_compare_opcode_masked(const void *p1, const void *p2)
{
    x86_map_op *om1 = (x86_map_op*)p1;
    x86_map_op *om2 = (x86_map_op*)p2;
    if (om1->type < om2->type) return -1;
    if (om1->type > om2->type) return 1;
    if (om1->map < om2->map) return -1;
    if (om1->map > om2->map) return 1;
    if (om1->prefix < om2->prefix) return -1;
    if (om1->prefix > om2->prefix) return 1;
    for (size_t i = 0; i < 2; i++) {
        uchar mask = om1->opm[i] & om2->opm[i];
        if ((om1->opc[i] & mask) < (om2->opc[i] & mask)) return -1;
        if ((om1->opc[i] & mask) > (om2->opc[i] & mask)) return 1;
    }
    if (om1->rec < om2->rec) return -1;
    if (om1->rec > om2->rec) return 1;
    return 0;
}

static int x86_map_op_compare_opcode(const void *p1, const void *p2)
{
    x86_map_op *om1 = (x86_map_op*)p1;
    x86_map_op *om2 = (x86_map_op*)p2;
    if (om1->type < om2->type) return -1;
    if (om1->type > om2->type) return 1;
    if (om1->map < om2->map) return -1;
    if (om1->map > om2->map) return 1;
    if (om1->prefix < om2->prefix) return -1;
    if (om1->prefix > om2->prefix) return 1;
    for (size_t i = 0; i < 2; i++) {
        if (om1->opc[i] < om2->opc[i]) return -1;
        if (om1->opc[i] > om2->opc[i]) return 1;
        /* most specific mask first for fixed modrm */
        if (om1->opm[i] < om2->opm[i]) return 1;
        if (om1->opm[i] > om2->opm[i]) return -1;
    }
    if (om1->rec < om2->rec) return -1;
    if (om1->rec > om2->rec) return 1;
    return 0;
}

static uint x86_op_major_type(const x86_opc_data *d, size_t i)
{
    uint e = d->enc[i] & x86_enc_t_mask;
    if (e == x86_enc_t_lex ||
        e == x86_enc_t_vex ||
        e == x86_enc_t_evex) return e;
    return 0;
}

static void x86_add_op_map_mod_entries(x86_map_op *op_map,
    x86_map_op rec, uint mod11p, uint mod11n)
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

static void x86_build_prefix_table(const x86_opc_data *table,
    x86_table_idx tab, x86_map_op *op_map, size_t *count)
{
    size_t n = 0;

    for(size_t i = 0; i < tab.count; i++) {
        const x86_opc_data *d = table + tab.idx[i];
        const x86_opr_data *o = x86_opr_table + d->opr;
        const x86_ord_data *p = x86_ord_table + d->ord;

        /* extract type, prefix, map and synthesize width prefixes */
        uint map = 0, type = 0, pfx = 0, pfx2 = 0, pfx3 = 0;
        uchar opc[2] = { 0 }, opm[2] = { 0 };
        for (size_t i = 0; i < array_size(d->enc) && d->enc[i]; i++) {
            switch (d->enc[i] & x86_enc_t_mask) {
            case x86_enc_t_lex:
            case x86_enc_t_vex:
            case x86_enc_t_evex:
                type = x86_op_major_type(d, i) >> x86_enc_t_shift;
                switch (d->enc[i] & x86_enc_m_mask) {
                case x86_enc_m_none: map = x86_map_none; break;
                case x86_enc_m_0f:   map = x86_map_0f; break;
                case x86_enc_m_0f38: map = x86_map_0f38; break;
                case x86_enc_m_0f3a: map = x86_map_0f3a; break;
                case x86_enc_m_map5: map = x86_map_map5; break;
                case x86_enc_m_map6: map = x86_map_map6; break;
                }
                switch (d->enc[i] & x86_enc_p_mask) {
                case x86_enc_p_66: pfx |= x86_pc_66; break;
                case x86_enc_p_9b: pfx |= x86_pc_9b; break;
                case x86_enc_p_f2: pfx |= x86_pc_f2; break;
                case x86_enc_p_f3: pfx |= x86_pc_f3; break;
                }
                switch (d->enc[i] & x86_enc_w_mask) {
                case x86_enc_w_wig:
                case x86_enc_w_wn:
                case x86_enc_w_wb:
                case x86_enc_w_w0: break;
                case x86_enc_w_w1: pfx |= x86_pc_48; break;
                case x86_enc_w_wx: pfx2 = pfx | x86_pc_48; /* fallthrough */
                case x86_enc_w_ww: pfx3 = pfx | x86_pc_66; break;
                }
                break;
            }
        }

        /* extract opcode and function */
        for (size_t i = 0; i < array_size(d->enc) && d->enc[i]; i++)
        {
            uint func;
            switch (d->enc[i] & x86_enc_t_mask) {
            case x86_enc_t_opcode:
                opc[0] = d->enc[i] & 0xff;
                opm[0] = 0xff;
                break;
            case x86_enc_t_opcode_r:
                opc[0] = d->enc[i] & 0xff;
                opm[0] = 0xf8;
                break;
            case x86_enc_t_modrm_n:
                func = (d->enc[i] & x86_enc_modrm_mask) >> x86_enc_modrm_shift;
                x86_modrm mod = x86_enc_modrm(0, func, 0);
                opc[1] = mod.data[0];
                opm[1] = 0x38;
                break;
            }
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
            x86_map_op rec = { type, pfx, map, { opc[0], opc[1] },
                { opm[0], opm[1] }, tab.idx[i] };
            x86_add_op_map_mod_entries(op_map + n, rec, mod11p, mod11n);
        }
        n += mod11n ? 3 : 1;
        if (pfx2) {
            if (op_map) {
                x86_map_op rec = { type, pfx2, map, { opc[0], opc[1] },
                    { opm[0], opm[1] }, tab.idx[i] };
                x86_add_op_map_mod_entries(op_map + n, rec, mod11p, mod11n);
            }
            n += mod11n ? 3 : 1;
        }
        if (pfx3) {
            if (op_map) {
                x86_map_op rec = { type, pfx3, map, { opc[0], opc[1] },
                    { opm[0], opm[1] }, tab.idx[i] };
                x86_add_op_map_mod_entries(op_map + n, rec, mod11p, mod11n);
            }
            n += mod11n ? 3 : 1;
        }
    }

    if (count) *count = n;
}

x86_map_idx x86_table_build(x86_modes modes)
{
    x86_map_idx map;
    x86_table_idx tab = x86_opc_table_filter(x86_opc_table_identity(), modes);
    x86_build_prefix_table(x86_opc_table, tab, NULL, &map.count);
    map.map = calloc(sizeof(x86_map_op), map.count);
    x86_build_prefix_table(x86_opc_table, tab, map.map, NULL);
    qsort(map.map, map.count, sizeof(x86_map_op), x86_map_op_compare_opcode);
    free(tab.idx);
    return map;
}

x86_map_op* x86_table_lookup(x86_map_idx map, const x86_map_op *m)
{
    size_t begin = 0, end = map.count;
    while (end != 0) {
        size_t half = (end >> 1), probe = begin + half;
        if (x86_map_op_compare_opcode_masked(m, map.map + probe) > 0) {
            begin = probe + 1;
            end -= half + 1;
        } else {
            end = half;
        }
    }
    return map.map + begin;
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

static uint x86_prefix_pc_to_pn(uint ord)
{
    uint result = 0;
    switch (ord & ~x86_pc_48) {
    case x86_pc_66: result |= (1 << x86_pn_66); break;
    case x86_pc_9b: result |= (1 << x86_pn_9b); break;
    case x86_pc_f2: result |= (1 << x86_pn_f2); break;
    case x86_pc_f3: result |= (1 << x86_pn_f3); break;
    }
    if (ord & x86_pc_48) result |= (1 << x86_pn_48);
    return result;
}

void x86_print_map(x86_map_op *m)
{
    char buf[256];
    x86_table_col cols[9];
    size_t count = 0, opclen = 0, len;
    uchar opcode[2], opm[2];

    const x86_opc_data *d = x86_opc_table + m->rec;
    uint map = m->map;
    uint type = m->type << x86_enc_t_shift;
    uint prefix = x86_prefix_pc_to_pn(m->prefix);

    buf[(len = 0)] = '\0';
    len += x86_enc_name(buf+len, sizeof(buf)-len, type);
    cols[count++] = x86_new_column(5, buf);

    buf[(len = 0)] = '\0';
    x86_prefix_namen(buf+len, sizeof(buf)-len, prefix, "|");
    cols[count++] = x86_new_column(5, buf);

    buf[(len = 0)] = '\0';
    len += x86_map_name(buf+len, sizeof(buf)-len, map, "|");
    cols[count++] = x86_new_column(4, buf);

    buf[(len = 0)] = '\0';
    for (size_t i = 0; i < 2 && m->opm[i]; i++) {
        if (i != 0) len += snprintf(buf+len, sizeof(buf)-len, " ");
        len += snprintf(buf+len, sizeof(buf)-len, "%02hhx", m->opc[i]);
    }
    cols[count++] = x86_new_column(5, buf);

    buf[(len = 0)] = '\0';
    for (size_t i = 0; i < 2 && m->opm[i]; i++) {
        if (i != 0) len += snprintf(buf+len, sizeof(buf)-len, " ");
        len += snprintf(buf+len, sizeof(buf)-len, "%02hhx", m->opm[i]);
    }
    cols[count++] = x86_new_column(5, buf);

    buf[(len = 0)] = '\0';
    len += snprintf(buf+len, sizeof(buf)-len, "%4u", m->rec);
    cols[count++] = x86_new_column(4, buf);

    buf[(len = 0)] = '\0';
    len += snprintf(buf+len, sizeof(buf)-len, "%s", x86_op_names[d->op]);
    cols[count++] = x86_new_column(18, buf);

    buf[(len = 0)] = '\0';
    for (size_t i = 0; i < array_size(d->enc) && d->enc[i]; i++) {
        if (len != 0) len += snprintf(buf+len, sizeof(buf)-len, " ");
        if ((d->enc[i] & x86_enc_t_mask) == x86_enc_t_opcode_r) {
            len += snprintf(buf+len, sizeof(buf)-len, "%02hhx", d->enc[i]&0xff);
        }
        len += x86_enc_name(buf+len, sizeof(buf)-len, d->enc[i]);
        if ((d->enc[i] & x86_enc_t_mask) == x86_enc_t_opcode) {
            len += snprintf(buf+len, sizeof(buf)-len, "%02hhx", d->enc[i]&0xff);
        }
    }
    cols[count++] = x86_new_column(30, buf);

    x86_print_row(count, cols);
}

void x86_print_op(uint rec, uint compact)
{
    char buf[256];
    x86_table_col cols[4];
    size_t count = 0, len;

    const x86_opc_data *d = x86_opc_table + rec;
    const x86_opr_data *o = x86_opr_table + d->opr;
    const x86_ord_data *p = x86_ord_table + d->ord;

    buf[(len = 0)] = '\0';
    len += snprintf(buf+len, sizeof(buf)-len, "%s ", x86_op_names[d->op]);
    for (size_t i = 0; i < array_size(o->opr) && o->opr[i]; i++) {
        if (i != 0) len += snprintf(buf+len, sizeof(buf)-len, ",");
        len += x86_opr_name(buf+len, sizeof(buf)-len, o->opr[i]);
    }
    cols[count++] = x86_new_column(52, buf);

    if (compact) {
        buf[(len = 0)] = '\0';
        len += x86_ord_mnem(buf+len, sizeof(buf)-len, p->ord);
        cols[count++] = x86_new_column(4, buf);
    }

    buf[(len = 0)] = '\0';
    for (size_t i = 0; i < array_size(d->enc) && d->enc[i]; i++) {
        if (len != 0) len += snprintf(buf+len, sizeof(buf)-len, " ");
        if ((d->enc[i] & x86_enc_t_mask) == x86_enc_t_opcode_r) {
            len += snprintf(buf+len, sizeof(buf)-len, "%02hhx", d->enc[i]&0xff);
        }
        len += x86_enc_name(buf+len, sizeof(buf)-len, d->enc[i]);
        if ((d->enc[i] & x86_enc_t_mask) == x86_enc_t_opcode) {
            len += snprintf(buf+len, sizeof(buf)-len, "%02hhx", d->enc[i]&0xff);
        }
    }
    cols[count++] = x86_new_column(30, buf);

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

static x86_modeb x86_codec_mode(x86_codec *c)
{
    uint is64 = !!(c->flags & x86_cf_amd64);
    uint is32 = !!(c->flags & x86_cf_ia32);
    uint is16 = !(is32 | is64);
    x86_modeb modeb = { is64, is32, is16 };
    return modeb;
}

static uint x86_enc_mode(x86_modeb mode)
{
    if (mode.is64) return x86_modes_64;
    else if (mode.is32) return x86_modes_32;
    else return x86_modes_16;
}

int x86_codec_write(x86_buffer *buf, x86_codec c, size_t *len)
{
    size_t nbytes = 0;

    x86_modeb mode = x86_codec_mode(&c);

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
    if (c.flags & x86_cp_wait) {
        nbytes += x86_out8(buf, x86_pb_wait);
    }
    if (c.flags & x86_cp_lock) {
        nbytes += x86_out8(buf, x86_pb_lock);
    }
    if (c.flags & x86_cp_rep) {
        nbytes += x86_out8(buf, x86_pb_rep);
    }
    if (c.flags & x86_cp_repne) {
        nbytes += x86_out8(buf, x86_pb_repne);
    }
    if (c.flags & x86_cp_osize) {
        nbytes += x86_out8(buf, x86_pb_osize);
    }
    if (c.flags & x86_cp_asize) {
        nbytes += x86_out8(buf, x86_pb_asize);
    }

    /* extended prefixes */
    switch ((c.flags & x86_ce_mask) >> x86_ce_shift) {
        case 1:
            nbytes += x86_out8(buf, c.rex.data[0]);
            break;
        case 2:
            nbytes += x86_out8(buf, x86_pb_rex2);
            nbytes += x86_out8(buf, c.rex2.data[0]);
            break;
        case 3:
            nbytes += x86_out8(buf, x86_pb_vex2);
            nbytes += x86_out8(buf, c.vex2.data[0]);
            break;
        case 4:
            nbytes += x86_out8(buf, x86_pb_vex3);
            nbytes += x86_out8(buf, c.vex3.data[0]);
            nbytes += x86_out8(buf, c.vex3.data[1]);
            break;
        case 5:
            nbytes += x86_out8(buf, x86_pb_evex);
            nbytes += x86_out8(buf, c.evex.data[0]);
            nbytes += x86_out8(buf, c.evex.data[1]);
            nbytes += x86_out8(buf, c.evex.data[2]);
            break;
    }

    /* map */
    switch ((c.flags & x86_cm_mask) >> x86_cm_shift) {
    case 0: break;
    case 1: nbytes += x86_out8(buf, 0x0f); break;
    case 2: nbytes += x86_out16(buf, 0x0f38); break;
    case 3: nbytes += x86_out16(buf, 0x0f3a); break;
    }

    /* opcode */
    for (size_t i = 0; i < c.opclen; i++) {
        nbytes += x86_out8(buf, c.opc[i]);
    }

    /* ModRM and SIB */
    if (c.flags & x86_cf_modrm) {
        nbytes += x86_out8(buf, c.modrm.data[0]);

        uchar mod = (c.modrm.data[0] >> x86_mod_shift) & x86_mod_mask;
        uchar rm = (c.modrm.data[0] >> x86_rm_shift) & x86_rm_mask;

        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            /* there is no SIB in real mode */
            if (!mode.is16 && rm == x86_rm_disp_sib) {
                nbytes += x86_out8(buf, c.sib.data[0]);
            }
            break;
        case x86_mod_reg: break;
        }
        switch (mod) {
        case x86_mod_disp0:
            /* this is RIP-relative in amd64 mode */
            if (rm == x86_rm_disp0_iw) {
                if (mode.is16) {
                    nbytes += x86_out32(buf, (u32)c.disp32);
                } else {
                    nbytes += x86_out16(buf, (u16)c.disp32);
                }
            }
            break;
        case x86_mod_disp8:
            nbytes += x86_out8(buf, (u8)c.disp32);
            break;
        case x86_mod_dispw:
            if (mode.is16) {
                nbytes += x86_out16(buf, (u16)c.disp32); break;
            } else {
                nbytes += x86_out32(buf, (u32)c.disp32); break;
            }
        case x86_mod_reg: break;
        }
    }

    /* additional immediate used by CALLF/JMPF/ENTER */
    if (c.flags & x86_cf_i16e) {
        nbytes += x86_out16(buf, (u16)c.imm16e);
    }

    uint pi = (c.flags & x86_ci_mask) >> x86_ci_shift;
    uint osize = !!(c.flags & x86_cp_osize);

    /* immediate */
    switch (pi) {
    case 1: /* iw */
        if (mode.is16 ^ osize) {
            nbytes += x86_out16(buf, (u16)c.imm32);
        } else {
            nbytes += x86_out32(buf, (u32)c.imm32);
        }
        break;
    case 2: /* ib */  nbytes += x86_out8(buf, (u8)c.imm32); break;
    case 3: /* i16 */ nbytes += x86_out16(buf, (u16)c.imm32); break;
    case 4: /* i32 */ nbytes += x86_out32(buf, (u32)c.imm32); break;
    case 5: /* i64 */ nbytes += x86_out64(buf, (u64)c.imm64); break;
    }

    *len = nbytes;
    return 0;
}

static int x86_filter_opdata(x86_codec *c, x86_map_op *k, x86_map_op *r, uint w)
{
    const x86_opc_data *d = x86_opc_table + r->rec;

    x86_modeb mode = x86_codec_mode(c);
    uint osize = !!(c->flags & x86_cp_osize);
    uint width = x86_enc_w_wig;

    if (mode.is16 && (d->mode & x86_modes_16) == 0) return -1;
    if (mode.is32 && (d->mode & x86_modes_32) == 0) return -1;
    if (mode.is64 && (d->mode & x86_modes_64) == 0) return -1;

    for (size_t i = 0; i < array_size(d->enc) && d->enc[i]; i++) {
        if (d->enc[i] & x86_enc_w_mask) {
            width = d->enc[i] & x86_enc_w_mask;
        }
        switch (d->enc[i] & x86_enc_t_mask) {
        case x86_enc_t_lex:
        case x86_enc_t_vex:
        case x86_enc_t_evex:
            switch (c->flags & x86_ce_mask) {
            case x86_ce_rex:
                if (x86_enc_filter_rex(c->rex, d->enc[i]) < 0) return -1;
                break;
            case x86_ce_rex2:
                if (x86_enc_filter_rex2(c->rex2, d->enc[i]) < 0) return -1;
                break;
            case x86_ce_vex2:
                if (x86_enc_filter_vex2(c->vex2, d->enc[i]) < 0) return -1;
                break;
            case x86_ce_vex3:
                if (x86_enc_filter_vex3(c->vex3, d->enc[i]) < 0) return -1;
                break;
            case x86_ce_evex:
                if (x86_enc_filter_evex(c->evex, d->enc[i]) < 0) return -1;
                break;
            }
            break;
        case x86_enc_t_o16:
            switch (width) {
            case x86_enc_w_ww:
            case x86_enc_w_wx:
                if (!(mode.is16 ^ osize) || w) return -1;
                break;
            }
            break;
        case x86_enc_t_o32:
            switch (width) {
            case x86_enc_w_ww:
                /* .ww means no 32-bit operands in 64-bit mode */
                if ((mode.is16 ^ osize) || mode.is64) return -1;
                break;
            case x86_enc_w_wx:
                if ((mode.is16 ^ osize) || w) return -1;
                break;
            }
            break;
        case x86_enc_t_o64:
            switch (width) {
            case x86_enc_w_ww:
                /* .ww means ignores W=1 in 64-bit mode */
                if (!mode.is64) return -1;
                break;
            case x86_enc_w_wx:
                if (!mode.is64 || !w) return -1;
                break;
            }
            break;
        }
    }

    return 0;
}

static int x86_parse_encoding(x86_buffer *buf, x86_codec *c,
    x86_map_op *r, size_t *len)
{
    const x86_opc_data *d = x86_opc_table + r->rec;
    int opcode_i = -1, opcode_j = -1, modrm_i = -1;
    x86_modeb mode = x86_codec_mode(c);
    size_t nbytes = 0;

    /* find opcode and modrm indices to check opcode length */
    for (size_t i = 0; i < array_size(d->enc) && d->enc[i]; i++) {
        switch (d->enc[i] & x86_enc_t_mask) {
        case x86_enc_t_opcode:
        case x86_enc_t_opcode_r:
            if (opcode_i == -1) opcode_i = i;
            opcode_j = i;
            break;
        case x86_enc_t_modrm_r:
        case x86_enc_t_modrm_n:
            modrm_i = i;
            c->flags |= x86_cf_modrm;
            c->modrm.data[0] = c->opc[1];
            break;
        }
    }

    /* put back byte if there is single byte opcode without modrm */
    if (modrm_i != opcode_i + 1 && opcode_i == opcode_j) {
        nbytes -= x86_buffer_unread(buf, 1);
        c->opclen = 1;
    } else {
        c->opclen = 2;
    }

    /* parse SIB and displacement */
    if (c->flags & x86_cf_modrm) {
        uchar modrm = c->modrm.data[0];
        uchar mod = (modrm >> x86_mod_shift) & x86_mod_mask;
        uchar rm = (modrm >> x86_rm_shift) & x86_rm_mask;
        switch (mod) {
        case x86_mod_disp0:
        case x86_mod_disp8:
        case x86_mod_dispw:
            /* there is no SIB in real mode */
            if (!mode.is16 && rm == x86_rm_disp_sib) {
                nbytes += x86_buffer_read(buf, c->sib.data, 1);
            }
            break;
        case x86_mod_reg:
            break;
        }
        switch (mod) {
        case x86_mod_disp0:
            /* this is RIP-relative in amd64 mode */
            if (rm == x86_rm_disp0_iw) {
                if (mode.is16) {
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
            if (mode.is16) {
                c->disp32 = (i16)x86_in16(buf); nbytes += 2;
            } else {
                c->disp32 = (i32)x86_in32(buf); nbytes += 4;
            }
        case x86_mod_reg:
            break;
        }
    }

    uint osize = !!(c->flags & x86_cp_osize);

    /* parse immediate */
    for (size_t i = opcode_j + 1; i < array_size(d->enc) && d->enc[i]; i++) {
        switch(d->enc[i] & x86_enc_t_mask) {
        case x86_enc_t_ib:
            c->imm32 = (i8)x86_in8(buf); nbytes += 1;
            c->flags |= x86_ci_ib;
            break;
        case x86_enc_t_iw:
            if (mode.is16 ^ osize) {
                c->imm32 = (i16)x86_in16(buf); nbytes += 2;
            } else {
                c->imm32 = (i32)x86_in32(buf); nbytes += 4;
            }
            c->flags |= x86_ci_iw;
            break;
        case x86_enc_t_i16:
            c->imm32 = (i16)x86_in16(buf);  nbytes += 2;
            c->flags |= x86_ci_i16;
            break;
        case x86_enc_t_i32:
            c->imm32 = (i32)x86_in32(buf);  nbytes += 4;
            c->flags |= x86_ci_i32;
            break;
        case x86_enc_t_i64:
            c->imm64 = (i64)x86_in64(buf);  nbytes += 8;
            c->flags |= x86_ci_i64;
            break;
        case x86_enc_t_i16e:
            c->imm16e = (i16)x86_in16(buf); nbytes += 2;
            c->flags |= x86_cf_i16e;
            break;
        }
    }

    *len = nbytes;
    return 0;
}

x86_codec_opr x86_extract_operands(x86_codec *c)
{
    x86_codec_opr q;
    memset(&q, 0, sizeof(q));

    x86_modeb mode = x86_codec_mode(c);

    q.osz = !!(c->flags & x86_cp_osize);

    if (c->flags & x86_cf_modrm) {
        uchar mod = (c->modrm.data[0] >> 6) & 3;
        uchar rm =  (c->modrm.data[0] >> 0) & 7;
        uchar reg = (c->modrm.data[0] >> 3) & 7;

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
            if (!mode.is16 && rm == x86_rm_disp_sib) {
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

    switch (c->flags & x86_ce_mask) {
    case x86_ce_rex:
        q.b |= ( c->rex.data[0] &    1) << 3; /* [0] -> b[3]*/
        q.x |= ( c->rex.data[0] &    2) << 2; /* [1] -> x[3]*/
        q.r |= ( c->rex.data[0] &    4) << 1; /* [2] -> r[3]*/
        q.w  = ( c->rex.data[0] &    8) >> 3;
        break;
    case x86_ce_rex2:
        q.b |= ( c->rex2.data[0] &   1) << 3; /* [0] -> b[3]*/
        q.x |= ( c->rex2.data[0] &   2) << 2; /* [1] -> x[3]*/
        q.r |= ( c->rex2.data[0] &   4) << 1; /* [2] -> r[3]*/
        q.w  = ( c->rex2.data[0] &   8) >> 3;
        q.b |= ( c->rex2.data[0] &  16) >> 0; /* [4] -> b[4]*/
        q.x |= ( c->rex2.data[0] &  32) >> 1; /* [5] -> x[4]*/
        q.r |= ( c->rex2.data[0] &  64) >> 2; /* [6] -> r[4]*/
        break;
    case x86_ce_vex2:
        q.r |= (~c->vex2.data[0] & 128) >> 4; /* [7] -> r[3] */
        q.l  = ( c->vex2.data[0] >>  2) & 1;
        q.v  = (~c->vex2.data[0] >>  3) & 15;
        q.osz = (c->vex2.data[0] & 3) == x86_pfx_66;
        break;
    case x86_ce_vex3:
        q.b |= (~c->vex3.data[0] &  32) >> 2; /* [5] -> b[3]*/
        q.x |= (~c->vex3.data[0] &  64) >> 3; /* [6] -> x[3]*/
        q.r |= (~c->vex3.data[0] & 128) >> 4; /* [7] -> r[3]*/
        q.l  = ( c->vex3.data[1] >>  2) & 1;
        q.v  = (~c->vex3.data[1] >>  3) & 15;
        q.w  = ( c->vex3.data[1] >>  7) & 1;
        q.osz = (c->vex3.data[1] & 3) == x86_pfx_66;
        break;
    case x86_ce_evex:
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

const char* x86_ptr_size_str(uint regsz, uint opr)
{
    /* todo - use operand class reg/vec */
    switch (opr & x86_opr_mem_mask) {
    case x86_opr_m8: return "byte ptr";
    case x86_opr_m16: return "word ptr";
    case x86_opr_m32: return "dword ptr";
    case x86_opr_m64: return "qword ptr";
    case x86_opr_m80: return "tword ptr";
    case x86_opr_m128: return "xmmword ptr";
    case x86_opr_m256: return "ymmword ptr";
    case x86_opr_m512: return "zmmword ptr";
    default:
        switch (regsz) {
        case x86_opr_size_8: return "byte ptr";
        case x86_opr_size_16: return "word ptr";
        case x86_opr_size_32: return "dword ptr";
        case x86_opr_size_64: return "qword ptr";
        case x86_opr_size_128: return "xmmword ptr";
        case x86_opr_size_256: return "ymmword ptr";
        case x86_opr_size_512: return "zmmword ptr";
        default: break;
        }
    }
    return "ptr";
}

const uint x86_mode_addr_size(x86_modeb mode)
{
    /* todo - handle address size prefix */
    if (mode.is16) return x86_opr_size_16;
    if (mode.is32) return x86_opr_size_32;
    if (mode.is64) return x86_opr_size_64;
    return 0;
}

const uint x86_opr_reg_size(x86_codec_opr q, x86_modeb mode, uint opr, uint enc)
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
        switch (enc & x86_enc_w_mask) {
        case x86_enc_w_wb: return x86_opr_size_8;
        case x86_enc_w_ww:
            if (mode.is16) return (q.osz ? x86_opr_size_32 : x86_opr_size_16);
            if (mode.is32) return (q.osz ? x86_opr_size_16 : x86_opr_size_32);
            if (mode.is64) return (q.osz ? x86_opr_size_16 : x86_opr_size_64);
            break;
        case x86_enc_w_wx:
            if (mode.is16) return (q.osz ? x86_opr_size_32 : x86_opr_size_16);
            if (mode.is32) return (q.osz ? x86_opr_size_16 : x86_opr_size_32);
            if (mode.is64) return (q.osz ? x86_opr_size_16 :
                                     q.w ? x86_opr_size_64 : x86_opr_size_32);
            break;
        case x86_enc_w_w0: return x86_opr_size_32;
        case x86_enc_w_w1: return x86_opr_size_64;
        default:
        }
    }

    return x86_mode_addr_size(mode);
}

uint x86_sized_gpr(x86_codec *c, uint reg, uint opr)
{
    switch (opr & x86_opr_size_mask) {
    case x86_opr_size_8:
        /* legacy encoding selects ah/cd/dh/bh instead of spl/bpl/sil/dil */
        if ((c->flags & x86_ce_mask) == x86_ce_none &&
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

uint x86_disp_scale(x86_codec *c, uint regsz)
{
    /* todo - EVEX scales needs tuple type */
    uint m = 1;
    switch (c->flags & x86_ce_mask) {
    case x86_ce_evex:
        switch (regsz) {
        case x86_opr_size_8: return 1;
        case x86_opr_size_16: return 2;
        case x86_opr_size_32: return 4;
        case x86_opr_size_64: return 8;
        case x86_opr_size_128: return 16;
        case x86_opr_size_256: return 32;
        case x86_opr_size_512: return 64;
        }
    default:
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

size_t x86_opr_mrm_str(char *buf, size_t buflen, x86_codec *c,
    x86_codec_opr q, x86_modeb mode, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(q, mode, opr, enc);
    uint addrsz = x86_mode_addr_size(mode);
    int disp = c->disp32 * x86_disp_scale(c, regsz);

    switch(q.mod) {
    case x86_mod_disp0:
        if (q.rm == x86_rm_disp0_iw) {
            if (mode.is64) {
                if (disp) {
                    return snprintf(buf, buflen, "%s [rip %s 0x%x]",
                        x86_ptr_size_str(regsz, opr),
                        disp < 0 ? "-" : "+",
                        disp < 0 ? -disp : disp);
                } else {
                    return snprintf(buf, buflen, "%s [rip]",
                        x86_ptr_size_str(regsz, opr));
                }
            } else {
                return snprintf(buf, buflen, "%s [0x%x]",
                    x86_ptr_size_str(regsz, opr), disp);
            }
        } else if (q.rm == x86_rm_disp_sib) {
            if (q.s) {
                return snprintf(buf, buflen, "%s [%s + %d*%s]",
                    x86_ptr_size_str(regsz, opr),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)), (1 << q.s),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)));
            } else {
                return snprintf(buf, buflen, "%s [%s + %s]",
                    x86_ptr_size_str(regsz, opr),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)));
            }
        } else {
            return snprintf(buf, buflen, "%s [%s]",
                x86_ptr_size_str(regsz, opr),
                x86_reg_name(x86_sized_gpr(c, q.b, addrsz)));
        }
        break;
    case x86_mod_disp8:
    case x86_mod_dispw:
        if (q.rm == x86_rm_disp_sib) {
            if (q.s) {
                return snprintf(buf, buflen, "%s [%s + %d*%s %s 0x%x]",
                    x86_ptr_size_str(regsz, opr),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)), (1 << q.s),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)),
                    disp < 0 ? "-" : "+",
                    disp < 0 ? -disp : disp);
            } else {
                return snprintf(buf, buflen, "%s [%s + %s %s 0x%x]",
                    x86_ptr_size_str(regsz, opr),
                    x86_reg_name(x86_sized_gpr(c, q.b, addrsz)),
                    x86_reg_name(x86_sized_gpr(c, q.x, addrsz)),
                    disp < 0 ? "-" : "+",
                    disp < 0 ? -disp : disp);
            }
        } else {
            return snprintf(buf, buflen, "%s [%s %s 0x%x]",
                x86_ptr_size_str(regsz, opr),
                x86_reg_name(x86_sized_gpr(c, q.b, addrsz)),
                disp < 0 ? "-" : "+",
                disp < 0 ? -disp : disp);
        }
        break;
    case x86_mod_reg:
        return snprintf(buf, buflen, "%s",
            x86_reg_name(x86_sized_gpr(c, q.b, regsz)));
    }
    return 0;
}

size_t x86_opr_reg_str(char *buf, size_t buflen, x86_codec *c,
    x86_codec_opr q, x86_modeb mode, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(q, mode, opr, enc);
    uint regn_r = x86_sized_gpr(c, q.r, regsz);
    const char* reg_r = x86_reg_name(regn_r);
    return snprintf(buf, buflen, "%s", reg_r);
}

size_t x86_opr_vec_str(char *buf, size_t buflen, x86_codec *c,
    x86_codec_opr q, x86_modeb mode, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(q, mode, opr, enc);
    uint regn_r = x86_sized_gpr(c, q.v, regsz);
    const char* reg_r = x86_reg_name(regn_r);
    return snprintf(buf, buflen, "%s", reg_r);
}

size_t x86_opr_imm_str(char *buf, size_t buflen, x86_codec *c,
    x86_codec_opr q, x86_modeb mode, uint opr, uint enc)
{
    if ((c->flags & x86_ci_mask) == x86_ci_i64) {
        llong imm = c->imm64;
        return snprintf(buf, buflen, "%s0x%llx",
            imm < 0 ? "-" : "", imm < 0 ? -imm : imm);
    } else {
        int imm = c->imm32;
        return snprintf(buf, buflen, "%s0x%x",
            imm < 0 ? "-" : "", imm < 0 ? -imm : imm);
    }
}

size_t x86_opr_const_str(char *buf, size_t buflen, x86_codec *c,
    x86_codec_opr q, x86_modeb mode, uint opr, uint enc)
{
    uint regsz = x86_opr_reg_size(q, mode, opr, enc);
    uint addrsz = x86_mode_addr_size(mode);

    switch (opr) {
    case x86_opr_1: return snprintf(buf, buflen, "1");
    case x86_opr_reg_al:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_al));
    case x86_opr_reg_cl:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_cl));
    case x86_opr_reg_ax:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_ax));
    case x86_opr_reg_cx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_cx));
    case x86_opr_reg_dx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_dx));
    case x86_opr_reg_bx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_bx));
    case x86_opr_reg_eax:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_eax));
    case x86_opr_reg_ecx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_ecx));
    case x86_opr_reg_edx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_edx));
    case x86_opr_reg_ebx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_ebx));
    case x86_opr_reg_rax:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_rax));
    case x86_opr_reg_rcx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_rcx));
    case x86_opr_reg_rdx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_rdx));
    case x86_opr_reg_rbx:
        return snprintf(buf, buflen, "%s", x86_reg_name(x86_rbx));
    case x86_opr_reg_aw:
        return snprintf(buf, buflen, "%s",
            x86_reg_name(x86_sized_gpr(c, x86_al, regsz)));
    case x86_opr_reg_cw:
        return snprintf(buf, buflen, "%s",
            x86_reg_name(x86_sized_gpr(c, x86_cl, regsz)));
    case x86_opr_reg_dw:
        return snprintf(buf, buflen, "%s",
            x86_reg_name(x86_sized_gpr(c, x86_dl, regsz)));
    case x86_opr_reg_bw:
        return snprintf(buf, buflen, "%s",
            x86_reg_name(x86_sized_gpr(c, x86_bl, regsz)));
    case x86_opr_reg_pa:
        return snprintf(buf, buflen, "[%s]",
            x86_reg_name(x86_sized_gpr(c, x86_al, addrsz)));
    case x86_opr_reg_pc:
        return snprintf(buf, buflen, "[%s]",
            x86_reg_name(x86_sized_gpr(c, x86_cl, addrsz)));
    case x86_opr_reg_pd:
        return snprintf(buf, buflen, "[%s]",
            x86_reg_name(x86_sized_gpr(c, x86_dl, addrsz)));
    case x86_opr_reg_pb:
        return snprintf(buf, buflen, "[%s]",
            x86_reg_name(x86_sized_gpr(c, x86_bl, addrsz)));
    case x86_opr_reg_psi:
        return snprintf(buf, buflen, "[%s]",
            x86_reg_name(x86_sized_gpr(c, x86_sil, addrsz)));
    case x86_opr_reg_pdi:
        return snprintf(buf, buflen, "[%s]",
            x86_reg_name(x86_sized_gpr(c, x86_dil, addrsz)));
    case x86_opr_reg_xmm0:
        return snprintf(buf, buflen, "%s", "<xmm0>");
    case x86_opr_reg_xmm0_7:
        return snprintf(buf, buflen, "%s", "<xmm0-7>");
    default:
        return snprintf(buf, buflen, "%s", "<const>");
    }
}

size_t x86_format_op(char *buf, size_t buflen, x86_codec *c)
{
    const x86_opc_data *d = x86_opc_table + c->rec;
    const x86_opr_data *o = x86_opr_table + d->opr;
    const x86_ord_data *s = x86_ord_table + d->ord;

    x86_modeb mode = x86_codec_mode(c);
    x86_codec_opr q = x86_extract_operands(c);

    size_t len = 0;
    len += snprintf(buf+len, buflen-len, "%s", x86_op_names[d->op]);

    for (size_t i = 0; i < array_size(o->opr) && o->opr[i]; i++)
    {
        len += snprintf(buf+len, buflen-len, i == 0 ? "\t" : ", ");
        switch(s->ord[i] & 7) {
        case x86_ord_const:
            len += x86_opr_const_str(buf+len, buflen-len,
                c, q, mode, o->opr[i], d->enc[0]);
            break;
        case x86_ord_imm:
            len += x86_opr_imm_str(buf+len, buflen-len,
                c, q, mode, o->opr[i], d->enc[0]);
            break;
        case x86_ord_reg:
            len += x86_opr_reg_str(buf+len, buflen-len,
                c, q, mode, o->opr[i], d->enc[0]);
            break;
        case x86_ord_mrm:
            len += x86_opr_mrm_str(buf+len, buflen-len,
                c, q, mode, o->opr[i], d->enc[0]);
            break;
        case x86_ord_vec:
            len += x86_opr_vec_str(buf+len, buflen-len,
                c, q, mode, o->opr[i], d->enc[0]);
            break;
        }
    }

    return len;
}

size_t x86_format_hex(char *buf, size_t buflen, char *data, size_t datalen)
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

int x86_codec_read(x86_buffer *buf, x86_codec *c, size_t *len, size_t limit)
{
    x86_state state = x86_state_top;
    size_t nbytes = 0;
    uchar b = 0, lastp = 0;
    uint t = 0, m = 0, w = 0, p = 0, l = 0;
    x86_map_op k = { 0 }, *r = NULL;

    static x86_map_idx idx;
    static x86_modes idx_mode;

    x86_modeb mode = x86_codec_mode(c);
    x86_modes enc_mode = x86_enc_mode(mode);

    if (!idx.map || idx_mode != enc_mode) {
        if (idx.map) free(idx.map);
        idx = x86_table_build(enc_mode);
        idx_mode = enc_mode;
    }

    while (state != x86_state_done) {
        nbytes += x86_buffer_read(buf, (void*)&b, 1);
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
                if (c->seg || lastp) goto err;
                switch (b) {
                case x86_pb_26: c->seg = x86_seg_es; break;
                case x86_pb_2e: c->seg = x86_seg_cs; break;
                case x86_pb_36: c->seg = x86_seg_ss; break;
                case x86_pb_3e: c->seg = x86_seg_ds; break;
                case x86_pb_64: c->seg = x86_seg_fs; break;
                case x86_pb_65: c->seg = x86_seg_gs; break;
                }
                break;
            case x86_pb_62:
                if (c->seg || lastp) goto err;
                nbytes += x86_buffer_read(buf, (void*)c->evex.data, 3);
                c->flags |= x86_ce_evex;
                m = (c->evex.data[0] >> 0) & 7;
                w = (c->evex.data[1] >> 7) & 1;
                p = (c->evex.data[1] >> 0) & 3;
                l = (c->evex.data[2] >> 5) & 3;
                t = x86_table_evex;
                state = x86_state_vex_opcode;
                break;
            case x86_pb_c4:
                if (c->seg || lastp) goto err;
                nbytes += x86_buffer_read(buf, (void*)c->vex3.data, 2);
                c->flags |= x86_ce_vex3;
                m = (c->vex3.data[0] >> 0) & 31;
                w = (c->vex3.data[1] >> 7) & 1;
                p = (c->vex3.data[1] >> 0) & 3;
                l = (c->vex3.data[1] >> 2) & 1;
                t = x86_table_vex;
                state = x86_state_vex_opcode;
                break;
            case x86_pb_c5:
                if (c->seg || lastp) goto err;
                nbytes += x86_buffer_read(buf, (void*)c->vex2.data, 1);
                c->flags |= x86_ce_vex2;
                m = x86_map_0f;
                p = (c->vex2.data[0] >> 0) & 3;
                l = (c->vex2.data[0] >> 2) & 1;
                t = x86_table_vex;
                state = x86_state_vex_opcode;
                break;
            case x86_pb_d5:
                if (c->seg || lastp) goto err;
                nbytes += x86_buffer_read(buf, (void*)c->rex2.data, 1);
                c->flags |= x86_ce_rex2;
                m = (c->rex2.data[0] >> 7) & 1;
                w = (c->rex2.data[0] >> 3) & 1;
                t = x86_table_lex;
                state = x86_state_lex_opcode;
                break;
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
                goto reparse;
            }
            break;
        case x86_state_rex_opcode:
            switch (b) {
            case 0x0f:
                state = x86_state_map_0f;
                break;
            default:
                state = x86_state_lex_opcode;
                goto reparse;
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
                goto reparse;
            }
            break;
        case x86_state_lex_opcode: reparse:
            /* use last prefix to get opcode map */
            k.type = t;
            k.map = m;
            switch (lastp) {
            case 0x66: k.prefix = x86_pc_66; break;
            case 0x9b: k.prefix = x86_pc_9b; break;
            case 0xf2: k.prefix = x86_pc_f2; break;
            case 0xf3: k.prefix = x86_pc_f3; break;
            }
            state = x86_state_done;
            break;
        case x86_state_vex_opcode:
            k.type = t;
            k.map = m;
            switch (p) {
            case x86_pfx_66: k.prefix = x86_pc_66; break;
            case x86_pfx_f2: k.prefix = x86_pc_f2; break;
            case x86_pfx_f3: k.prefix = x86_pc_f3; break;
            }
            state = x86_state_done;
            break;
        default:
            abort();
        }
    };

    /* populate opcode for table lookup */
    c->opc[0] = k.opc[0] = b;
    nbytes += x86_buffer_read(buf, (void*)&b, 1);
    c->opc[1] = k.opc[1] = b;
    k.opm[0] = k.opm[1] = 0xff;

    /* if REX.W=1 first attempt to lookup W=1 record */
    if (w) {
        k.prefix |= x86_pc_48;
        k.rec = 0;
        x86_debugf("table_lookup { type:%x prefix:%x map:%x "
            "opc:[%02hhx %02hhx] opm:[%02hhx %02hhx] }",
            k.type, k.prefix, k.map,
            k.opc[0], k.opc[1], k.opm[0], k.opm[1]);
        r = x86_table_lookup(idx, &k);
        while (r < idx.map + idx.count) {
            k.rec = r->rec;
            x86_debugf("checking opdata %u", r->rec);
            if (debug) x86_print_map(r);
            if (x86_map_op_compare_opcode_masked(&k, r) != 0) {
                x86_debugf("** no matches");
                r = NULL;
                break;
            }
            if (x86_filter_opdata(c, &k, r, 1) == 0) break;
            r++;
        }
    }

    /* if REX.W=0 or search failed lookup W=0/WIG record */
    if (!w || (w && !r)) {
        k.prefix &= ~x86_pc_48;
        k.rec = 0;
        x86_debugf("table_lookup { type:%x prefix:%x map:%x "
            "opc:[%02hhx %02hhx] opm:[%02hhx %02hhx] }",
            k.type, k.prefix, k.map,
            k.opc[0], k.opc[1], k.opm[0], k.opm[1]);
        r = x86_table_lookup(idx, &k);
        while (r < idx.map + idx.count) {
            k.rec = r->rec;
            x86_debugf("checking opdata %u", r->rec);
            if (debug) x86_print_map(r);
            if (x86_map_op_compare_opcode_masked(&k, r) != 0) {
                x86_debugf("** no matches");
                r = NULL;
                break;
            }
            if (x86_filter_opdata(c, &k, r, 0) == 0) break;
            r++;
        }
    }

    /* parse encoding */
    if (r) {
        size_t nread = 0;
        if (x86_parse_encoding(buf, c, r, &nread) == 0) {
            if (nbytes + nread <= limit) {
                c->rec = r->rec;
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
