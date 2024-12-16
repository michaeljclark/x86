#pragma once

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "bits.h"
#include "bytes.h"

typedef struct x86_rex x86_rex;
typedef struct x86_rex2 x86_rex2;
typedef struct x86_vex2 x86_vex2;
typedef struct x86_vex3 x86_vex3;
typedef struct x86_evex x86_evex;
typedef struct x86_modrm x86_modrm;
typedef struct x86_sib x86_sib;

typedef struct x86_mem x86_mem;
typedef struct x86_codec x86_codec;
typedef struct x86_modeb x86_modeb;
typedef struct x86_opc_data x86_opc_data;
typedef struct x86_opr_data x86_opr_data;
typedef struct x86_ord_data x86_ord_data;
typedef struct x86_table_idx x86_table_idx;
typedef struct x86_acc_idx x86_acc_idx;
typedef struct x86_acc_entry x86_acc_entry;
typedef struct x86_buffer x86_buffer;
typedef struct x86_ctx x86_ctx;

/*
 * simple debug macros
 */

#define VA_ARGS(...) , ##__VA_ARGS__
#define x86_debug(fmt,...) if (debug) printf(fmt "\n" VA_ARGS(__VA_ARGS__))
#define x86_debugf(fmt,...) if (debug) printf("%s: " fmt "\n", __func__ VA_ARGS(__VA_ARGS__))

/*
 * register groups
 */

enum
{
    x86_reg_w     = 0b0000 << 5,
    x86_reg_b     = 0b0001 << 5,
    x86_reg_d     = 0b0010 << 5,
    x86_reg_q     = 0b0011 << 5,

    x86_reg_mm    = 0b0100 << 5,
    x86_reg_xmm   = 0b0101 << 5,
    x86_reg_ymm   = 0b0110 << 5,
    x86_reg_zmm   = 0b0111 << 5,

    x86_reg_kmask = 0b1000 << 5,
    x86_reg_bl    = 0b1001 << 5,
    x86_reg_fpu   = 0b1010 << 5,
    x86_reg_bnd   = 0b1011 << 5,

    x86_reg_dreg  = 0b1100 << 5,
    x86_reg_creg  = 0b1101 << 5,
    x86_reg_sreg  = 0b1110 << 5,
    x86_reg_sys   = 0b1111 << 5,
};

/*
 * prefix byte
 */

enum x86_pb
{
    x86_pb_26    = 0x26,
    x86_pb_2e    = 0x2e,
    x86_pb_36    = 0x36,
    x86_pb_3e    = 0x3e,
    x86_pb_41    = 0x41,
    x86_pb_42    = 0x42,
    x86_pb_44    = 0x44,
    x86_pb_48    = 0x48,
    x86_pb_62    = 0x62,
    x86_pb_64    = 0x64,
    x86_pb_65    = 0x65,
    x86_pb_66    = 0x66,
    x86_pb_67    = 0x67,
    x86_pb_9b    = 0x9b,
    x86_pb_c4    = 0xc4,
    x86_pb_c5    = 0xc5,
    x86_pb_d5    = 0xd5,
    x86_pb_f0    = 0xf0,
    x86_pb_f2    = 0xf2,
    x86_pb_f3    = 0xf3,

    x86_pb_es    = x86_pb_26,
    x86_pb_cs    = x86_pb_2e, /* branch not taken with Jcc */
    x86_pb_ss    = x86_pb_36,
    x86_pb_ds    = x86_pb_3e, /* branch taken with Jcc (CET notrack) */
    x86_pb_rex_b = x86_pb_41,
    x86_pb_rex_x = x86_pb_42,
    x86_pb_rex_r = x86_pb_44,
    x86_pb_rex_w = x86_pb_48,
    x86_pb_evex  = x86_pb_62,
    x86_pb_fs    = x86_pb_64,
    x86_pb_gs    = x86_pb_65,
    x86_pb_osize = x86_pb_66,
    x86_pb_asize = x86_pb_67,
    x86_pb_wait  = x86_pb_9b, /* x87 */
    x86_pb_vex3  = x86_pb_c4,
    x86_pb_vex2  = x86_pb_c5,
    x86_pb_rex2  = x86_pb_d5,
    x86_pb_lock  = x86_pb_f0,
    x86_pb_repne = x86_pb_f2, /* REPNE/REPNZ and XACQUIRE */
    x86_pb_rep   = x86_pb_f3, /* REP/REPE/REPZ and XRELEASE */
};

/*
 * mod
 */

enum x86_mod
{
    x86_mod_disp0,
    x86_mod_disp8,
    x86_mod_dispw,
    x86_mod_reg,
};

/*
 * SIB scale
 */

enum x86_scale
{
    x86_scale_1,
    x86_scale_2,
    x86_scale_4,
    x86_scale_8,
};

/*
 * segment
 */

enum x86_seg
{
    x86_seg_none,
    x86_seg_es,
    x86_seg_cs,
    x86_seg_ss,
    x86_seg_ds,
    x86_seg_fs,
    x86_seg_gs
};

/*
 * REX flags
 */

enum
{
    x86_rex_b = 0x01,
    x86_rex_x = 0x02,
    x86_rex_r = 0x04,
    x86_rex_w = 0x08,
};

/*
 * VEX prefix
 */

enum x86_pfx
{
    x86_pfx_none,
    x86_pfx_66,
    x86_pfx_f3,
    x86_pfx_f2,
    x86_pfx_9b,
    x86_pfx_rexw = 8
};

/*
 * VEX map
 */

enum x86_map
{
    x86_map_none,
    x86_map_0f,
    x86_map_0f38,
    x86_map_0f3a,
    x86_map_map4,
    x86_map_map5,
    x86_map_map6,
};

/*
 * VEX length
 */

enum x86_vex_l
{
    x86_vex_l0 = 0b00,
    x86_vex_l1 = 0b01,
    x86_vex_l2 = 0b10,
    x86_vex_l3 = 0b11,
    x86_vex_lz = x86_vex_l0,
    x86_vex_lig = x86_vex_l3,
    x86_vex_l128 = x86_vex_l0,
    x86_vex_l256 = x86_vex_l1,
    x86_vex_l512 = x86_vex_l2,
};

/*
 * VEX width
 */

enum x86_vex_w
{
    x86_vex_w0,
    x86_vex_w1,
};

/*
 * test conditions
 */

enum x86_cond {
    /* non-signed */
    x86_never  = 0 | 0 | 0 | 0,
    x86_always = 0 | 0 | 0 | 1,
    x86_eq     = 8 | 0 | 0 | 0,
    x86_ne     = 8 | 0 | 0 | 1,
    /* signed */
    x86_lt     = 0 | 0 | 2 | 0,
    x86_ge     = 0 | 0 | 2 | 1,
    x86_le     = 8 | 0 | 2 | 0,
    x86_gt     = 8 | 0 | 2 | 1,
    /* unsigned */
    x86_ltu    = 0 | 4 | 0 | 0,
    x86_geu    = 0 | 4 | 0 | 1,
    x86_leu    = 8 | 4 | 0 | 0,
    x86_gtu    = 8 | 4 | 0 | 1,
};

/*
 * compare packed operators
 *
 * oq = ordered queit, os = ordered signalling
 * uq = unordered queit, us = unordered signalling
 */
enum x86_cmpp {
    x86_eq_oq    = 0x00,
    x86_lt_os    = 0x01,
    x86_le_os    = 0x02,
    x86_unord_q  = 0x03,
    x86_neq_uq   = 0x04,
    x86_nlt_us   = 0x05,
    x86_nle_us   = 0x06,
    x86_ord_q    = 0x07,
    x86_eq_uq    = 0x08,
    x86_nge_us   = 0x09,
    x86_ngt_us   = 0x0a,
    x86_false_oq = 0x0b,
    x86_neq_oq   = 0x0c,
    x86_ge_os    = 0x0d,
    x86_gt_os    = 0x0e,
    x86_true_uq  = 0x0f,
    x86_eq_os    = 0x10,
    x86_lt_oq    = 0x11,
    x86_le_oq    = 0x12,
    x86_unord_s  = 0x13,
    x86_neq_us   = 0x14,
    x86_nlt_uq   = 0x15,
    x86_nle_uq   = 0x16,
    x86_ord_s    = 0x17,
    x86_eq_us    = 0x18,
    x86_nge_uq   = 0x19,
    x86_ngt_uq   = 0x1a,
    x86_false_os = 0x1b,
    x86_neq_os   = 0x1c,
    x86_ge_oq    = 0x1d,
    x86_gt_oq    = 0x1e,
    x86_true_us  = 0x1f,
};

/*
 * prefix encoding structures
 *
 * note: bitfields used for documentation purposes only
 */

/*
 * ModRM
 */

struct x86_modrm
{
    union {
        uchar data[1];
        struct {
            /* [0:2] */ uchar rm  : 3;
            /* [3:5] */ uchar reg : 3;
            /* [6:7] */ uchar mod : 2;
        };
    };
};

enum
{
    x86_rm_sp_sib = 0b100,
    x86_rm_bp_disp0 = 0b101,

    x86_modrm_rm_mask = 7,
    x86_modrm_reg_shift = 3,
    x86_modrm_reg_mask = 7,
    x86_modrm_mod_shift = 6,
    x86_modrm_mod_mask = 3,
};

static inline uint x86_modrm_rm (uchar modrm) {
    return modrm & x86_modrm_rm_mask;
}
static inline uint x86_modrm_reg (uchar modrm) {
    return (modrm >> x86_modrm_reg_shift) & x86_modrm_reg_mask;
}
static inline uint x86_modrm_mod (uchar modrm) {
    return (modrm >> x86_modrm_mod_shift) & x86_modrm_mod_mask;
}

/*
 * SIB
 */

/*
 * SIB
 */

struct x86_sib
{
    union {
        uchar data[1];
        struct {
            /* [0:2] */ uchar b : 3; /* base[0:2] */
            /* [3:5] */ uchar x : 3; /* index[0:2] */
            /* [6:7] */ uchar s : 2; /* scale[0:2] */
        };
    };
};

enum
{
    x86_sib_b_mask = 7,
    x86_sib_x_shift = 3,
    x86_sib_x_mask = 7,
    x86_sib_s_shift = 6,
    x86_sib_s_mask = 3
};

static inline uint x86_sib_b (uchar sib) {
    return sib & x86_sib_b_mask;
}
static inline uint x86_sib_x (uchar sib) {
    return (sib >> x86_sib_x_shift) & x86_sib_x_mask;
}
static inline uint x86_sib_s (uchar sib) {
    return (sib >> x86_sib_s_shift) & x86_sib_s_mask;
}

/*
 * REX struct
 */

struct x86_rex
{
    union {
        uchar data[1];
        struct {
            /* [0] */ uchar b3 : 1; /* base[3] */
            /* [1] */ uchar x3 : 1; /* index[3] */
            /* [2] */ uchar r3 : 1; /* reg[3] */
            /* [3] */ uchar w  : 1; /* width */
        };
    };
};


/*
 * REX2 struct
 */

struct x86_rex2
{
    union {
        uchar data[1];
        struct {
            /* [0] */ uchar b3 : 1; /* base[3] */
            /* [1] */ uchar x3 : 1; /* index[3] */
            /* [2] */ uchar r3 : 1; /* reg[3] */
            /* [3] */ uchar w  : 1; /* width */
            /* [4] */ uchar b4 : 1; /* base[4] */
            /* [5] */ uchar x4 : 1; /* index[4] */
            /* [6] */ uchar r4 : 1; /* reg[4] */
            /* [7] */ uchar m0 : 1; /* map prefix 0x0f */
        };
    };
};


/*
 * VEX2 struct
 */

struct x86_vex2
{
    union {
        uchar data[1];
        struct {
            /* [0:1] */ uchar p   : 2; /* see x86_pfx */
            /*   [2] */ uchar l   : 1; /* len 0=128 1=256 */
            /* [3:6] */ uchar vn  : 4; /* ~vec[0:3] */
            /*   [7] */ uchar r3n : 1; /* ~reg[3] */
        };
    };
};

/*
 * VEX3 struct
 */
struct x86_vex3
{
    union {
        uchar data[2];
        struct {
            /* [0:4] */ uchar m   : 5; /* see x86_map */
            /*   [5] */ uchar b3n : 1; /* ~base[3] */
            /*   [6] */ uchar x3n : 1; /* ~index[3] */
            /*   [7] */ uchar r3n : 1; /* ~reg[3] */

            /* [0:1] */ uchar p   : 2; /* see x86_pfx */
            /*   [2] */ uchar l   : 1; /* len 0=128 1=256 */
            /* [3:6] */ uchar vn  : 4; /* ~vec[0:3] */
            /*   [7] */ uchar w   : 1; /* width */
        };
    };
};

/*
 * EVEX struct
 */
struct x86_evex
{
    union {
        uchar data[3];
        struct {
            /* [0:2] */ uchar m   : 3; /* see x86_map */
            /*   [3] */ uchar b4  : 1; /* base[4] */
            /*   [4] */ uchar r4n : 1; /* ~reg[4] */
            /*   [5] */ uchar b3n : 1; /* ~base[3] */
            /*   [6] */ uchar x3n : 1; /* ~index[3] */
            /*   [7] */ uchar r3n : 1; /* ~reg[3] */

            /* [0:1] */ uchar p   : 2; /* see x86_pfx */
            /*   [2] */ uchar x4n : 1; /* ~index[4] */
            /* [3:6] */ uchar vn  : 4; /* ~vec[0:3] */
            /*   [7] */ uchar w   : 1; /* width */

            /* [0:2] */ uchar a   : 3; /* kmask[0:2] or map4.NF[2] no-flags */
            /*   [3] */ uchar v4n : 1; /* ~vec[4] */
            /*   [4] */ uchar br  : 1; /* broadcast or map4.ND[4] new-dest */
            /* [5:6] */ uchar l   : 2; /* len 00=128 01=256, 10=512 */
            /*   [7] */ uchar z  : 1;  /* merge/zero */
        };
    };
};

/*
 * opcode encoding
 */

enum x86_enc
{
    x86_enc_w_shift          = 0,
    x86_enc_m_shift          = x86_enc_w_shift + 3,
    x86_enc_p_shift          = x86_enc_m_shift + 3,
    x86_enc_l_shift          = x86_enc_p_shift + 4,
    x86_enc_t_shift          = x86_enc_l_shift + 3,
    x86_enc_o_shift          = x86_enc_t_shift + 2,
    x86_enc_f_shift          = x86_enc_o_shift + 1,
    x86_enc_i_shift          = x86_enc_f_shift + 3,
    x86_enc_j_shift          = x86_enc_i_shift + 3,
    x86_enc_r_shift          = x86_enc_j_shift + 2,
    x86_enc_s_shift          = x86_enc_r_shift + 3,

    x86_enc_w_w0             = (1 << x86_enc_w_shift),
    x86_enc_w_w1             = (2 << x86_enc_w_shift),
    x86_enc_w_wb             = (3 << x86_enc_w_shift),
    x86_enc_w_wn             = (4 << x86_enc_w_shift),
    x86_enc_w_wx             = (5 << x86_enc_w_shift),
    x86_enc_w_ww             = (6 << x86_enc_w_shift),
    x86_enc_w_wig            = (7 << x86_enc_w_shift),
    x86_enc_w_mask           = (7 << x86_enc_w_shift),

    x86_enc_m_none           = (0 << x86_enc_m_shift),
    x86_enc_m_0f             = (1 << x86_enc_m_shift),
    x86_enc_m_0f38           = (2 << x86_enc_m_shift),
    x86_enc_m_0f3a           = (3 << x86_enc_m_shift),
    x86_enc_m_map4           = (4 << x86_enc_m_shift),
    x86_enc_m_map5           = (5 << x86_enc_m_shift),
    x86_enc_m_map6           = (6 << x86_enc_m_shift),
    x86_enc_m_mask           = (7 << x86_enc_m_shift),

    x86_enc_p_none           = (0 << x86_enc_p_shift),
    x86_enc_p_66             = (1 << x86_enc_p_shift),
    x86_enc_p_f3             = (2 << x86_enc_p_shift),
    x86_enc_p_f2             = (3 << x86_enc_p_shift),
    x86_enc_p_9b             = (4 << x86_enc_p_shift),
    x86_enc_p_rexw           = (8 << x86_enc_p_shift),
    x86_enc_p_mask           = (7 << x86_enc_p_shift),
    x86_enc_prexw_mask       = (15 << x86_enc_p_shift),

    x86_enc_l_lz             = (1 << x86_enc_l_shift),
    x86_enc_l_l0             = (2 << x86_enc_l_shift),
    x86_enc_l_l1             = (3 << x86_enc_l_shift),
    x86_enc_l_128            = (4 << x86_enc_l_shift),
    x86_enc_l_256            = (5 << x86_enc_l_shift),
    x86_enc_l_512            = (6 << x86_enc_l_shift),
    x86_enc_l_lig            = (7 << x86_enc_l_shift),
    x86_enc_l_mask           = (7 << x86_enc_l_shift),

    x86_enc_t_none           = (0 << x86_enc_t_shift),
    x86_enc_t_lex            = (1 << x86_enc_t_shift),
    x86_enc_t_vex            = (2 << x86_enc_t_shift),
    x86_enc_t_evex           = (3 << x86_enc_t_shift),
    x86_enc_t_mask           = (3 << x86_enc_t_shift),

    x86_enc_o_opcode_r       = (1 << x86_enc_o_shift), // XX+r
    x86_enc_o_mask           = (1 << x86_enc_o_shift),

    x86_enc_f_modrm_r        = (1 << x86_enc_f_shift), // /r
    x86_enc_f_modrm_n        = (2 << x86_enc_f_shift), // /N
    x86_enc_f_opcode         = (3 << x86_enc_f_shift), // XX
    x86_enc_f_opcode_r       = (4 << x86_enc_f_shift), // XX+r
    x86_enc_f_mask           = (7 << x86_enc_f_shift),

    x86_enc_i_ib             = (1 << x86_enc_i_shift),
    x86_enc_i_iw             = (2 << x86_enc_i_shift),
    x86_enc_i_iwd            = (3 << x86_enc_i_shift),
    x86_enc_i_i16            = (4 << x86_enc_i_shift),
    x86_enc_i_i32            = (5 << x86_enc_i_shift),
    x86_enc_i_i64            = (6 << x86_enc_i_shift),
    x86_enc_i_mask           = (7 << x86_enc_i_shift),

    x86_enc_j_ib             = (1 << x86_enc_j_shift),
    x86_enc_j_i16            = (2 << x86_enc_j_shift),
    x86_enc_j_mask           = (3 << x86_enc_j_shift),

    x86_enc_r_rep            = (1 << x86_enc_r_shift),
    x86_enc_r_lock           = (2 << x86_enc_r_shift),
    x86_enc_r_norexb         = (4 << x86_enc_r_shift),
    x86_enc_r_mask           = (7 << x86_enc_r_shift),

    x86_enc_s_o16            = (1 << x86_enc_s_shift),
    x86_enc_s_o32            = (2 << x86_enc_s_shift),
    x86_enc_s_o64            = (3 << x86_enc_s_shift),
    x86_enc_s_a16            = (4 << x86_enc_s_shift),
    x86_enc_s_a32            = (5 << x86_enc_s_shift),
    x86_enc_s_a64            = (6 << x86_enc_s_shift),
    x86_enc_s_mask           = (7 << x86_enc_s_shift),

    x86_enc_immediate_mask   = x86_enc_i_mask | x86_enc_j_mask,
    x86_enc_suffix_mask      = x86_enc_r_mask | x86_enc_s_mask,
    x86_enc_param_mask       = x86_enc_immediate_mask | x86_enc_suffix_mask
};

static uint x86_enc_width(uint enc) { return (enc & x86_enc_w_mask); }
static uint x86_enc_prefix(uint enc) { return (enc & x86_enc_prexw_mask); }
static uint x86_enc_length(uint enc) { return (enc & x86_enc_l_mask); }
static uint x86_enc_opcode(uint enc) { return (enc & x86_enc_o_mask); }
static uint x86_enc_func(uint enc) { return (enc & x86_enc_f_mask); }
static uint x86_enc_map(uint enc) { return (enc & x86_enc_m_mask); }
static uint x86_enc_imm(uint enc) { return (enc & x86_enc_i_mask); }
static uint x86_enc_imm2(uint enc) { return (enc & x86_enc_j_mask); }
static uint x86_enc_type(uint enc) { return (enc & x86_enc_t_mask); }
static uint x86_enc_suffix(uint enc) { return (enc & x86_enc_suffix_mask); }
static uint x86_enc_leading(uint enc) { return (enc & ~x86_enc_param_mask); }
static uint x86_enc_has_rep(uint enc) { return (enc & x86_enc_r_rep); }
static uint x86_enc_has_lock(uint enc) { return (enc & x86_enc_r_lock); }
static uint x86_enc_has_norexb(uint enc) { return (enc & x86_enc_r_norexb); }
static uint x86_enc_has_o16(uint enc) { return (enc & x86_enc_s_mask) == x86_enc_s_o16; }
static uint x86_enc_has_o32(uint enc) { return (enc & x86_enc_s_mask) == x86_enc_s_o32; }
static uint x86_enc_has_o64(uint enc) { return (enc & x86_enc_s_mask) == x86_enc_s_o64; }
static uint x86_enc_has_a16(uint enc) { return (enc & x86_enc_s_mask) == x86_enc_s_a16; }
static uint x86_enc_has_a32(uint enc) { return (enc & x86_enc_s_mask) == x86_enc_s_a32; }
static uint x86_enc_has_a64(uint enc) { return (enc & x86_enc_s_mask) == x86_enc_s_a64; }

/*
 * operand encoding
 */

enum x86_opr
{
    x86_opr_none,

    /* operand field shifts */
    x86_opr_s1               = 10,
    x86_opr_s2               = x86_opr_s1 + 4,
    x86_opr_s3               = x86_opr_s2 + 4,
    x86_opr_s4               = x86_opr_s3 + 4,

    /* operand flags */
    x86_opr_mem              = 1 << 0,
    x86_opr_rel              = 1 << 1,
    x86_opr_ind              = 1 << 2,
    x86_opr_far              = 1 << 3,
    x86_opr_bcst             = 1 << 4,
    x86_opr_flag_er          = 1 << 5,
    x86_opr_flag_k           = 1 << 6,
    x86_opr_flag_sae         = 1 << 7,
    x86_opr_flag_z           = 1 << 8,
    x86_opr_flag_rs          = 1 << 9,

    /* operand type */
    x86_opr_const            = 1 << x86_opr_s1, /* constant */
    x86_opr_imm              = 2 << x86_opr_s1, /* immediate */
    x86_opr_reg              = 3 << x86_opr_s1, /* register */
    x86_opr_vec              = 4 << x86_opr_s1, /* vector */
    x86_opr_st               = 5 << x86_opr_s1, /* x87 */
    x86_opr_k                = 6 << x86_opr_s1, /* mask */
    x86_opr_seg              = 7 << x86_opr_s1, /* segment */
    x86_opr_creg             = 8 << x86_opr_s1, /* control */
    x86_opr_dreg             = 9 << x86_opr_s1, /* debug */
    x86_opr_bnd              = 10 << x86_opr_s1, /* bound */
    x86_opr_type_mask        = 15 << x86_opr_s1,

    /* operand sizes */
    x86_opr_size_8           = 1 << x86_opr_s2,
    x86_opr_size_16          = 2 << x86_opr_s2,
    x86_opr_size_32          = 3 << x86_opr_s2,
    x86_opr_size_64          = 4 << x86_opr_s2,
    x86_opr_size_128         = 5 << x86_opr_s2,
    x86_opr_size_256         = 6 << x86_opr_s2,
    x86_opr_size_512         = 7 << x86_opr_s2,
    x86_opr_size_1024        = 8 << x86_opr_s2,
    x86_opr_size_80          = 9 << x86_opr_s2,
    x86_opr_size_wd          = 13 << x86_opr_s2,
    x86_opr_size_w           = 14 << x86_opr_s2,
    x86_opr_size_a           = 15 << x86_opr_s2,
    x86_opr_size_mask        = 15 << x86_opr_s2,

    /* sized register */
    x86_opr_r8               = x86_opr_reg | x86_opr_size_8,
    x86_opr_r16              = x86_opr_reg | x86_opr_size_16,
    x86_opr_r32              = x86_opr_reg | x86_opr_size_32,
    x86_opr_r64              = x86_opr_reg | x86_opr_size_64,
    x86_opr_rw               = x86_opr_reg | x86_opr_size_w,
    x86_opr_ra               = x86_opr_reg | x86_opr_size_a,
    x86_opr_mm               = x86_opr_vec | x86_opr_size_64,
    x86_opr_xmm              = x86_opr_vec | x86_opr_size_128,
    x86_opr_ymm              = x86_opr_vec | x86_opr_size_256,
    x86_opr_zmm              = x86_opr_vec | x86_opr_size_512,

    /* sized memory */
    x86_opr_m8               = x86_opr_mem | (1 << x86_opr_s3), /* byte */
    x86_opr_m16              = x86_opr_mem | (2 << x86_opr_s3), /* word */
    x86_opr_m32              = x86_opr_mem | (3 << x86_opr_s3), /* dword */
    x86_opr_m64              = x86_opr_mem | (4 << x86_opr_s3), /* qword */
    x86_opr_m128             = x86_opr_mem | (5 << x86_opr_s3), /* oword/xmmword */
    x86_opr_m256             = x86_opr_mem | (6 << x86_opr_s3), /* ymmword */
    x86_opr_m512             = x86_opr_mem | (7 << x86_opr_s3), /* zmmword */
    x86_opr_m1024            = x86_opr_mem | (8 << x86_opr_s3),
    x86_opr_m80              = x86_opr_mem | (9 << x86_opr_s3), /* tword/tbyte */
    x86_opr_m384             = x86_opr_mem | (10 << x86_opr_s3),
    x86_opr_mib              = x86_opr_mem | (11 << x86_opr_s3), /* bound */
    x86_opr_vm32             = x86_opr_mem | (12 << x86_opr_s3), /* vecmem32 */
    x86_opr_vm64             = x86_opr_mem | (13 << x86_opr_s3), /* vecmem64 */
    x86_opr_mw               = x86_opr_mem | (14 << x86_opr_s3), /* 16/32/64 */
    x86_opr_mp               = x86_opr_mem | (15 << x86_opr_s3), /* pointer */
    x86_opr_mem_mask         = x86_opr_mem | (15 << x86_opr_s3),

    /* unsized register / sized memory */
    x86_opr_r_m8             = x86_opr_reg | x86_opr_m8,
    x86_opr_r_m16            = x86_opr_reg | x86_opr_m16,
    x86_opr_r_m32            = x86_opr_reg | x86_opr_m32,
    x86_opr_r_m64            = x86_opr_reg | x86_opr_m64,

    /* sized register / sized memory */
    x86_opr_r8_m8            = x86_opr_r8  | x86_opr_m8,
    x86_opr_r16_m8           = x86_opr_r16 | x86_opr_m8,
    x86_opr_r16_m16          = x86_opr_r16 | x86_opr_m16,
    x86_opr_r32_m8           = x86_opr_r32 | x86_opr_m8,
    x86_opr_r32_m16          = x86_opr_r32 | x86_opr_m16,
    x86_opr_r32_m32          = x86_opr_r32 | x86_opr_m32,
    x86_opr_r64_m8           = x86_opr_r64 | x86_opr_m8,
    x86_opr_r64_m16          = x86_opr_r64 | x86_opr_m16,
    x86_opr_r64_m32          = x86_opr_r64 | x86_opr_m32,
    x86_opr_r64_m64          = x86_opr_r64 | x86_opr_m64,
    x86_opr_rw_mw            = x86_opr_rw  | x86_opr_mw,

    /* sized vector / sized memory */
    x86_opr_mm_m32           = x86_opr_mm  | x86_opr_m32,
    x86_opr_mm_m64           = x86_opr_mm  | x86_opr_m64,
    x86_opr_xmm_m8           = x86_opr_xmm | x86_opr_m8,
    x86_opr_xmm_m16          = x86_opr_xmm | x86_opr_m16,
    x86_opr_xmm_m32          = x86_opr_xmm | x86_opr_m32,
    x86_opr_xmm_m64          = x86_opr_xmm | x86_opr_m64,
    x86_opr_xmm_m128         = x86_opr_xmm | x86_opr_m128,
    x86_opr_ymm_m256         = x86_opr_ymm | x86_opr_m256,
    x86_opr_zmm_m512         = x86_opr_zmm | x86_opr_m512,

    /* sized vector memory */
    x86_opr_vm32x            = x86_opr_xmm | x86_opr_vm32,
    x86_opr_vm32y            = x86_opr_ymm | x86_opr_vm32,
    x86_opr_vm32z            = x86_opr_zmm | x86_opr_vm32,
    x86_opr_vm64x            = x86_opr_xmm | x86_opr_vm64,
    x86_opr_vm64y            = x86_opr_ymm | x86_opr_vm64,
    x86_opr_vm64z            = x86_opr_zmm | x86_opr_vm64,

    /* mask / sized memory */
    x86_opr_k_m8             = x86_opr_k   | x86_opr_m8,
    x86_opr_k_m16            = x86_opr_k   | x86_opr_m16,
    x86_opr_k_m32            = x86_opr_k   | x86_opr_m32,
    x86_opr_k_m64            = x86_opr_k   | x86_opr_m64,

    /* bound / memory */
    x86_opr_bnd_mem          = x86_opr_bnd | x86_opr_mem,

    /* memory operand alias */
    x86_opr_m16int           = x86_opr_m16,
    x86_opr_m32fp            = x86_opr_m32,
    x86_opr_m32int           = x86_opr_m32,
    x86_opr_m64fp            = x86_opr_m64,
    x86_opr_m64int           = x86_opr_m64,
    x86_opr_m80bcd           = x86_opr_m80,
    x86_opr_m80dec           = x86_opr_m80,
    x86_opr_m80fp            = x86_opr_m80,

    /* sized immediate */
    x86_opr_ib               = x86_opr_imm | x86_opr_size_8,
    x86_opr_i16              = x86_opr_imm | x86_opr_size_16,
    x86_opr_i32              = x86_opr_imm | x86_opr_size_32,
    x86_opr_i64              = x86_opr_imm | x86_opr_size_64,
    x86_opr_iwd              = x86_opr_imm | x86_opr_size_wd,
    x86_opr_iw               = x86_opr_imm | x86_opr_size_w,

    /* relative displacement */
    x86_opr_rel8             = x86_opr_imm | x86_opr_rel | x86_opr_size_8,
    x86_opr_relw             = x86_opr_imm | x86_opr_rel | x86_opr_size_w,

    /* memory offset */
    x86_opr_moffs            = x86_opr_imm | x86_opr_mem | x86_opr_size_w,

    /* constant */
    x86_opr_1                = x86_opr_const | (1 << x86_opr_s4),

    /* registers sequential */
    x86_opr_flag_rs2         = x86_opr_flag_rs | (1 << x86_opr_s4),
    x86_opr_flag_rs4         = x86_opr_flag_rs | (2 << x86_opr_s4),

    /* sized broadcast */
    x86_opr_m16bcst          = x86_opr_bcst | (1 << x86_opr_s4),
    x86_opr_m32bcst          = x86_opr_bcst | (2 << x86_opr_s4),
    x86_opr_m64bcst          = x86_opr_bcst | (3 << x86_opr_s4),

    /* sized vector / sized memory / broadcast */
    x86_opr_xmm_m32_m16bcst  = x86_opr_mm_m32   | x86_opr_m16bcst,
    x86_opr_xmm_m64_m16bcst  = x86_opr_xmm_m64  | x86_opr_m16bcst,
    x86_opr_xmm_m64_m32bcst  = x86_opr_xmm_m64  | x86_opr_m32bcst,
    x86_opr_xmm_m128_m16bcst = x86_opr_xmm_m128 | x86_opr_m16bcst,
    x86_opr_xmm_m128_m32bcst = x86_opr_xmm_m128 | x86_opr_m32bcst,
    x86_opr_xmm_m128_m64bcst = x86_opr_xmm_m128 | x86_opr_m64bcst,
    x86_opr_ymm_m256_m16bcst = x86_opr_ymm_m256 | x86_opr_m16bcst,
    x86_opr_ymm_m256_m32bcst = x86_opr_ymm_m256 | x86_opr_m32bcst,
    x86_opr_ymm_m256_m64bcst = x86_opr_ymm_m256 | x86_opr_m64bcst,
    x86_opr_zmm_m512_m16bcst = x86_opr_zmm_m512 | x86_opr_m16bcst,
    x86_opr_zmm_m512_m32bcst = x86_opr_zmm_m512 | x86_opr_m32bcst,
    x86_opr_zmm_m512_m64bcst = x86_opr_zmm_m512 | x86_opr_m64bcst,

    /* enter / far displacement */
    x86_opr_far16_16         = x86_opr_imm | x86_opr_far | (3 << x86_opr_s4),
    x86_opr_far16_32         = x86_opr_imm | x86_opr_far | (4 << x86_opr_s4),

    /* far memory indirect */
    x86_opr_memfar16_16      = x86_opr_mem | x86_opr_far | (1 << x86_opr_s4),
    x86_opr_memfar16_32      = x86_opr_mem | x86_opr_far | (2 << x86_opr_s4),
    x86_opr_memfar16_64      = x86_opr_mem | x86_opr_far | (3 << x86_opr_s4),

    /* implicit register */
    x86_opr_reg_a            = x86_opr_ind | x86_opr_reg | (0 << x86_opr_s4),
    x86_opr_reg_c            = x86_opr_ind | x86_opr_reg | (1 << x86_opr_s4),
    x86_opr_reg_d            = x86_opr_ind | x86_opr_reg | (2 << x86_opr_s4),
    x86_opr_reg_b            = x86_opr_ind | x86_opr_reg | (3 << x86_opr_s4),
    x86_opr_reg_si           = x86_opr_ind | x86_opr_reg | (4 << x86_opr_s4),
    x86_opr_reg_di           = x86_opr_ind | x86_opr_reg | (5 << x86_opr_s4),
    x86_opr_reg_ah           = x86_opr_ind | x86_opr_reg | (6 << x86_opr_s4),
    x86_opr_reg_v0           = x86_opr_ind | x86_opr_vec | (0 << x86_opr_s4),
    x86_opr_reg_st0          = x86_opr_ind | x86_opr_st  | (0 << x86_opr_s4),
    x86_opr_seg_es           = x86_opr_ind | x86_opr_seg | (0 << x86_opr_s4),
    x86_opr_seg_cs           = x86_opr_ind | x86_opr_seg | (1 << x86_opr_s4),
    x86_opr_seg_ss           = x86_opr_ind | x86_opr_seg | (2 << x86_opr_s4),
    x86_opr_seg_ds           = x86_opr_ind | x86_opr_seg | (3 << x86_opr_s4),
    x86_opr_seg_fs           = x86_opr_ind | x86_opr_seg | (4 << x86_opr_s4),
    x86_opr_seg_gs           = x86_opr_ind | x86_opr_seg | (5 << x86_opr_s4),

    /* sized implicit register */
    x86_opr_reg_al           = x86_opr_reg_a | x86_opr_size_8,
    x86_opr_reg_cl           = x86_opr_reg_c | x86_opr_size_8,
    x86_opr_reg_ax           = x86_opr_reg_a | x86_opr_size_16,
    x86_opr_reg_cx           = x86_opr_reg_c | x86_opr_size_16,
    x86_opr_reg_dx           = x86_opr_reg_d | x86_opr_size_16,
    x86_opr_reg_bx           = x86_opr_reg_b | x86_opr_size_16,
    x86_opr_reg_eax          = x86_opr_reg_a | x86_opr_size_32,
    x86_opr_reg_ecx          = x86_opr_reg_c | x86_opr_size_32,
    x86_opr_reg_edx          = x86_opr_reg_d | x86_opr_size_32,
    x86_opr_reg_ebx          = x86_opr_reg_b | x86_opr_size_32,
    x86_opr_reg_rax          = x86_opr_reg_a | x86_opr_size_64,
    x86_opr_reg_rcx          = x86_opr_reg_c | x86_opr_size_64,
    x86_opr_reg_rdx          = x86_opr_reg_d | x86_opr_size_64,
    x86_opr_reg_rbx          = x86_opr_reg_b | x86_opr_size_64,
    x86_opr_reg_aw           = x86_opr_reg_a | x86_opr_size_w,
    x86_opr_reg_cw           = x86_opr_reg_c | x86_opr_size_w,
    x86_opr_reg_dw           = x86_opr_reg_d | x86_opr_size_w,
    x86_opr_reg_bw           = x86_opr_reg_b | x86_opr_size_w,
    x86_opr_reg_pa           = x86_opr_reg_a | x86_opr_size_a,
    x86_opr_reg_pc           = x86_opr_reg_c | x86_opr_size_a,
    x86_opr_reg_pd           = x86_opr_reg_d | x86_opr_size_a,
    x86_opr_reg_pb           = x86_opr_reg_b | x86_opr_size_a,
    x86_opr_reg_psi          = x86_opr_reg_si | x86_opr_size_a,
    x86_opr_reg_pdi          = x86_opr_reg_di | x86_opr_size_a,
    x86_opr_reg_xmm0         = x86_opr_reg_v0 | x86_opr_size_128,
    x86_opr_reg_xmm0_7       = x86_opr_reg_v0 | x86_opr_size_1024,
};

/*
 * order encoding
 */

enum x86_ord
{
    x86_ord_none,

    x86_ord_s1               = 0,
    x86_ord_s2               = x86_ord_s1 + 3,
    x86_ord_s3               = x86_ord_s2 + 3,

    x86_ord_const            = 1 << x86_ord_s1, // -
    x86_ord_imm              = 2 << x86_ord_s1, // I
    x86_ord_reg              = 3 << x86_ord_s1, // R
    x86_ord_mrm              = 4 << x86_ord_s1, // M
    x86_ord_vec              = 5 << x86_ord_s1, // V
    x86_ord_opr              = 6 << x86_ord_s1, // O
    x86_ord_type_mask        = 7 << x86_ord_s1,

    x86_ord_r                = 0b001 << x86_ord_s2,
    x86_ord_w                = 0b010 << x86_ord_s2,
    x86_ord_i                = 0b100 << x86_ord_s2,
    x86_ord_rw               = 0b011 << x86_ord_s2,
    x86_ord_ri               = 0b101 << x86_ord_s2,
    x86_ord_wi               = 0b110 << x86_ord_s2,
    x86_ord_rwi              = 0b111 << x86_ord_s2,
    x86_ord_flag_mask        = 0b111 << x86_ord_s2,

    x86_ord_one              = x86_ord_const | (1 << x86_ord_s3),
    x86_ord_rax              = x86_ord_const | (2 << x86_ord_s3),
    x86_ord_rcx              = x86_ord_const | (3 << x86_ord_s3),
    x86_ord_rdx              = x86_ord_const | (4 << x86_ord_s3),
    x86_ord_rbx              = x86_ord_const | (5 << x86_ord_s3),
    x86_ord_rsp              = x86_ord_const | (6 << x86_ord_s3),
    x86_ord_rbp              = x86_ord_const | (7 << x86_ord_s3),
    x86_ord_rsi              = x86_ord_const | (8 << x86_ord_s3),
    x86_ord_rdi              = x86_ord_const | (9 << x86_ord_s3),
    x86_ord_st0              = x86_ord_const | (10 << x86_ord_s3),
    x86_ord_stx              = x86_ord_const | (11 << x86_ord_s3),
    x86_ord_seg              = x86_ord_const | (12 << x86_ord_s3),
    x86_ord_xmm0             = x86_ord_const | (13 << x86_ord_s3),
    x86_ord_xmm0_7           = x86_ord_const | (14 << x86_ord_s3),
    x86_ord_mxcsr            = x86_ord_const | (15 << x86_ord_s3),
    x86_ord_rflags           = x86_ord_const | (16 << x86_ord_s3),

    x86_ord_sib              = x86_ord_mrm | (1 << x86_ord_s3),

    x86_ord_is4              = x86_ord_imm | (1 << x86_ord_s3),
    x86_ord_ime              = x86_ord_imm | (2 << x86_ord_s3),
};

/*
 * codec flags
 */

enum x86_cf
{
    /* [0:2] encoding */
    x86_ce_shift   = 0,
    x86_ce_none    = 0 << x86_ce_shift,
    x86_ce_rex     = 1 << x86_ce_shift,
    x86_ce_rex2    = 2 << x86_ce_shift,
    x86_ce_vex2    = 3 << x86_ce_shift,
    x86_ce_vex3    = 4 << x86_ce_shift,
    x86_ce_evex    = 5 << x86_ce_shift,
    x86_ce_mask    = 7 << x86_ce_shift,

    /* [3:4] map */
    x86_cm_shift   = 3,
    x86_cm_none    = 0 << x86_cm_shift,
    x86_cm_0f      = 1 << x86_cm_shift,
    x86_cm_0f38    = 2 << x86_cm_shift,
    x86_cm_0f3a    = 3 << x86_cm_shift,
    x86_cm_mask    = 3 << x86_cm_shift,

    /* [5:7] imm */
    x86_ci_shift   = 5,
    x86_ci_none    = 0 << x86_ci_shift,
    x86_ci_ib      = 1 << x86_ci_shift,
    x86_ci_iw      = 2 << x86_ci_shift,
    x86_ci_iwd     = 3 << x86_ci_shift,
    x86_ci_i16     = 4 << x86_ci_shift,
    x86_ci_i32     = 5 << x86_ci_shift,
    x86_ci_i64     = 6 << x86_ci_shift,
    x86_ci_mask    = 7 << x86_ci_shift,

    /* [8:9] imm2 */
    x86_cj_shift   = 8,
    x86_cj_ib      = 1 << x86_cj_shift,
    x86_cj_i16     = 2 << x86_cj_shift,
    x86_cj_mask    = 3 << x86_cj_shift,

    /* [10:15] prefixes */
    x86_cp_osize   = 1 << 10, /* 0x66 */
    x86_cp_asize   = 1 << 11, /* 0x67 */
    x86_cp_wait    = 1 << 12, /* 0x9B */
    x86_cp_lock    = 1 << 13, /* 0xF0 */
    x86_cp_repne   = 1 << 14, /* 0xF2 */
    x86_cp_rep     = 1 << 15, /* 0xF3 */

    /* [16:18] flags */
    x86_cf_modrm   = 1 << 16,
    x86_cf_ia32    = 1 << 17,
    x86_cf_amd64   = 1 << 18,
};

/*
 * codec struct
 */

struct x86_codec
{
    union {
        x86_rex rex;
        x86_rex2 rex2;
        x86_vex2 vex2;
        x86_vex3 vex3;
        x86_evex evex;
    };

    union {
        uchar opc[2];
        struct {
            uchar opc0;
            x86_modrm modrm;
        };
    };

    uchar opclen : 2;
    uchar seg : 3;
    uchar spare;
    x86_sib sib;

    uint flags;
    ushort rec;
    short imm2;

    union {
        struct {
            int disp32;
            int imm32;
        };
        struct {
            ullong imm64;
        };
    };
};

/*
 * codec fields and flags
 */

static int x86_codec_field_ce(x86_codec *c) {
    return (c->flags & x86_ce_mask);
}
static int x86_codec_field_cm(x86_codec *c) {
    return (c->flags & x86_cm_mask);
}
static int x86_codec_field_ci(x86_codec *c) {
    return (c->flags & x86_ci_mask);
}
static int x86_codec_field_cj(x86_codec *c) {
    return (c->flags & x86_cj_mask);
}
static int x86_codec_has_wait(x86_codec *c) {
    return (c->flags & x86_cp_wait) != 0;
}
static int x86_codec_has_lock(x86_codec *c) {
    return (c->flags & x86_cp_lock) != 0;
}
static int x86_codec_has_rep(x86_codec *c) {
    return (c->flags & x86_cp_rep) != 0;
}
static int x86_codec_has_repne(x86_codec *c) {
    return (c->flags & x86_cp_repne) != 0;
}
static int x86_codec_has_osize(x86_codec *c) {
    return (c->flags & x86_cp_osize) != 0;
}
static int x86_codec_has_asize(x86_codec *c) {
    return (c->flags & x86_cp_asize) != 0;
}
static int x86_codec_has_modrm(x86_codec *c) {
    return (c->flags & x86_cf_modrm) != 0;
}
static int x86_codec_is16(x86_codec *c) {
    return (c->flags & (x86_cf_ia32|x86_cf_amd64)) == 0;
}
static int x86_codec_is32(x86_codec *c) {
    return (c->flags & x86_cf_ia32) != 0;
}
static int x86_codec_is64(x86_codec *c) {
    return (c->flags & x86_cf_amd64) != 0;
}

/*
 * modes
 */

enum x86_modes
{
    x86_modes_16 = (1 << 0),
    x86_modes_32 = (1 << 1),
    x86_modes_64 = (1 << 2),
};

static int x86_mode_has16(uint mode) {
    return (mode & x86_modes_16) != 0;
}
static int x86_mode_has32(uint mode) {
    return (mode & x86_modes_32) != 0;
}
static int x86_mode_has64(uint mode) {
    return (mode & x86_modes_64) != 0;
}

/*
 * memory operand
 */

struct x86_mem
{
    union {
        uint code;
        struct {
            uint base : 9;
            uint index : 9;
            uint scale : 2;
        };
    };
    int disp;
};

/*
 * opcode metadata
 *
 * type, prefix, map, opcode, mask, plus operand and order records.
 *
 * opcode - opcode number from opcode enum for name lookup
 * mode   - operating mode (16 | 32 | 64)
 * opr    - operand list (r8/m8, rw/mw, xmm/m128, etc)
 * ord    - operand order (register, immediate, regmem, etc)
 * enc    - operand encoding (type, width, prefix, map, immediate, etc)
 * opc    - opcode, ModRM function byte or second opcode byte.
 * opm    - opcode mask (f8 for XX+r), ModRM function or second byte mask.
 *
 * prefix and map are provisioned as 6 bits each to align the bitfield.
 * there are 3 types, 10 prefixes (5 * 2), and 7 maps (up to map6).
 */

struct x86_opc_data
{
    ushort op;
    ushort mode;
    ushort opr;
    ushort ord;
    uint enc;
    uchar opc[2];
    uchar opm[2];
};

struct x86_opr_data
{
    uint opr[4];
};

struct x86_ord_data
{
    ushort ord[4];
};

/*
 * invert condition
 */

static inline uint x86_invert_cond(uint c)
{
    return c ^ 1;
}

/*
 * swap condition operands
 */

static inline uint x86_swap_cond(uint c)
{
    return c & 6 ? c ^ 9 : c;
}

/*
 * ModRM encoder
 */

static inline x86_modrm x86_enc_modrm(uint mod, uint reg, uint rm)
{
    x86_modrm modrm = {
        .data = {
            /* [0:2] */ (rm & 7u) |
            /* [3:5] */ ((reg & 7u) << 3) |
            /* [6:7] */ ((mod & 3u) << 6)
        }
    };
    return modrm;
}

/*
 * SIB encoder
 */

static inline x86_sib x86_enc_sib(uint s, uint x, uint b)
{
    x86_sib sib = {
        .data = {
            /* [0:2] */ (b & 7u) |
            /* [3:5] */ ((x & 7u) << 3) |
            /* [6:7] */ (((uint)s & 3u) << 6)
        }
    };
    return sib;
}

/*
 * REX encoder
 */

static inline x86_rex x86_enc_rex(uint w, uint r, uint x, uint b)
{
    x86_rex rex = {
        .data = {
            /*   [0] */ ((b & 8u) >> 3) |
            /*   [1] */ ((x & 8u) >> 2) |
            /*   [2] */ ((r & 8u) >> 1) |
            /*   [3] */ ((w & 1u) << 3) |
            /* [4:7] */ 0x40
        }
    };
    return rex;
}

/*
 * REX2 encoder
 */

static inline x86_rex2 x86_enc_rex2(uint m, uint w, uint r, uint x, uint b)
{
    x86_rex2 rex2 = {
        .data = {
            /*   [0] */ ((b &  8u) >> 3) |
            /*   [1] */ ((x &  8u) >> 2) |
            /*   [2] */ ((r &  8u) >> 1) |
            /*   [3] */ ((w &  1u) << 3) |
            /*   [4] */ ((b & 16u) << 0) |
            /*   [5] */ ((x & 16u) << 1) |
            /*   [6] */ ((r & 16u) << 2) |
            /*   [7] */ ((m &  1u) << 7)
        }
    };
    return rex2;
}

/*
 * VEX2 encoder
 */

static inline x86_vex2 x86_enc_vex2(uint p, uint l,
    uint r, uint v)
{
    x86_vex2 vex2 = {
        .data = {
            /* [0:1] */ ((uint)p & 3u) |
            /*   [2] */ ((l & 1u) << 2) |
            /* [3:6] */ ((~v & 15u) << 3) |
            /*   [7] */ ((~r & 8u) << 4)
        }
    };
    return vex2;
}

/*
 * VEX3 encoder
 */

static inline x86_vex3 x86_enc_vex3(uint m, uint p,
    uint l, uint w, uint r, uint x, uint b, uint v)
{
    x86_vex3 vex3 = {
        .data = {
            /* [0:4] */ ((m &  31u) >> 0) |
            /*   [5] */ ((~b &  8u) << 2) |
            /*   [6] */ ((~x &  8u) << 3) |
            /*   [7] */ ((~r &  8u) << 4),
            /* [0:1] */ ((p &   3u) >> 0) |
            /*   [2] */ ((l  &  1u) << 2) |
            /* [3:6] */ ((~v & 15u) << 3) |
            /*   [7] */ ((w  &  1u) << 7)
        }
    };
    return vex3;
}

/*
 * EVEX encoder
 */

static inline x86_evex x86_enc_evex(uint m, uint p,
    uint l, uint w, uint r, uint x, uint b, uint v,
    uint k, uint brd, uint z)
{
    x86_evex evex = {
        .data = {
            /* [0:2] */ ((m &   7u) >> 0) |
            /*   [3] */ ((b &  16u) >> 1) |
            /*   [4] */ ((~r & 16u) >> 0) |
            /*   [5] */ ((~b &  8u) << 2) |
            /*   [6] */ ((~x &  8u) << 3) |
            /*   [7] */ ((~r &  8u) << 4),
            /* [0:1] */ ((p &   3u) >> 0) |
            /*   [2] */ ((~x & 16u) >> 2) |
            /* [3:6] */ ((~v & 15u) << 3) |
            /*   [7] */ ((w  &  1u) << 7),
            /* [0:2] */ ((k &   7u) >> 0) |
            /*   [3] */ ((~v & 16u) >> 1) |
            /*   [4] */ ((brd & 1u) << 4) |
            /* [5:6] */ ((l &   3u) << 5) |
            /*   [7] */ ((z &   1u) << 7)
        }
    };
    return evex;
}

/*
 * table sort types
 */

enum
{
    x86_sort_none,
    x86_sort_numeric,
    x86_sort_alpha
};

/*
 * table types that map to instruction encoding prefix types
 */

enum
{
    x86_table_none,
    x86_table_lex,
    x86_table_vex,
    x86_table_evex
};

/*
 * table sort indices array used to sort immutable opcode table
 */

struct x86_table_idx
{
    size_t count;
    size_t *idx;
};

/*
 * opcode index decode tables
 */

struct x86_acc_idx
{
    size_t map_count;
    x86_opc_data *map;
    size_t acc_count;
    x86_acc_entry *acc;
    uchar *page_offsets;
};

/*
 * opcode index acceleration entry
 */

struct x86_acc_entry
{
    uint idx : 24;
    uint nent : 8;
};

/*
 * opcode acceleration functions
 */

static uint x86_acc_page(uint type, uint prefix, uint map)
{
    return (type & 3) | ((prefix & 15) << 2) | ((map & 7) << 6);
}

static size_t x86_acc_offset(x86_acc_idx *idx, size_t acc_page)
{
    return (size_t)idx->page_offsets[acc_page] << 8;
}

static x86_acc_entry* x86_acc_lookup(x86_acc_idx *idx, size_t offset)
{
    return idx->acc + offset;
}

/*
 * bitmap utility functions
 */

static inline size_t x86_bitmap_idx(size_t i) { return i >> 6; }
static inline size_t x86_bitmap_shift(size_t i) { return (i & 63); }

static inline int x86_bitmap_get(ullong *bitmap, size_t i)
{
    return (int)((bitmap[x86_bitmap_idx(i)] >> x86_bitmap_shift(i)) & 1);
}

static inline void x86_bitmap_set(ullong *bitmap, size_t i, int value)
{
    bitmap[x86_bitmap_idx(i)] |= ((ullong)value << x86_bitmap_shift(i));
}

/*
 * context for encoder, decoder, formatter and parser
 */

struct x86_ctx
{
    uint mode;
    x86_acc_idx *idx;
};

/* simplified buffer with read (start) and write (end) cursors
 * capacity is user managed because it does no limit checking. */

struct x86_buffer
{
    uchar *data;
    size_t start;
    size_t end;
};

/*
 * buffer functions
 */

static inline void x86_buffer_init(x86_buffer *b, uchar *data)
{
    b->start = 0;
    b->end = 0;
    b->data = data;
}

static inline void x86_buffer_init_ex(x86_buffer *b, uchar *data,
    size_t start, size_t end)
{
    b->start = start;
    b->end = end;
    b->data = data;
}

static inline size_t x86_buffer_read(x86_buffer *b, uchar *buf, size_t len)
{
    memcpy(buf, b->data + b->start, len);
    b->start += len;
    return len;
}

static inline size_t x86_buffer_unread(x86_buffer *b, size_t len)
{
    b->start -= len;
    return len;
}

static inline size_t x86_buffer_write(x86_buffer *b, uchar *buf, size_t len)
{
    memcpy(b->data + b->end, buf, len);
    b->end += len;
    return len;
}

static inline size_t x86_buffer_unwrite(x86_buffer *b, size_t len)
{
    b->end -= len;
    return len;
}

static inline size_t x86_out8(x86_buffer *buf, u8 v)
{
    return x86_buffer_write(buf, (void*)&v, sizeof(u8));
}

static inline size_t x86_out16(x86_buffer *buf, u16 v)
{
    u16 t = le16(v);
    return x86_buffer_write(buf, (void*)&t, sizeof(u16));
}

static inline size_t x86_out32(x86_buffer *buf, u32 v)
{
    u32 t = le32(v);
    return x86_buffer_write(buf, (void*)&t, sizeof(u32));
}

static inline size_t x86_out64(x86_buffer *buf, u64 v)
{
    u64 t = le64(v);
    return x86_buffer_write(buf, (void*)&t, sizeof(u64));
}

static inline size_t x86_unput(x86_buffer *buf, size_t n)
{
    return x86_buffer_unwrite(buf, n);
}

static inline u8 x86_in8(x86_buffer *buf)
{
    u8 t = 0;
    x86_buffer_read(buf, (void*)&t, sizeof(u8));
    return t;
}

static inline u16 x86_in16(x86_buffer *buf)
{
    u16 t = 0;
    x86_buffer_read(buf, (void*)&t, sizeof(u16));
    return le16(t);
}

static inline u32 x86_in32(x86_buffer *buf)
{
    u32 t = 0;
    x86_buffer_read(buf, (void*)&t, sizeof(u32));
    return le32(t);
}

static inline u64 x86_in64(x86_buffer *buf)
{
    u64 t = 0;
    x86_buffer_read(buf, (void*)&t, sizeof(u64));
    return le64(t);
}

/*
 * metadata tables
 */

extern const char* x86_reg_names[];
extern const size_t x86_op_names_size;
extern const char* x86_op_names[];
extern const size_t x86_opc_table_size;
extern const x86_opc_data x86_opc_table[];
extern const size_t x86_opr_table_size;
extern const x86_opr_data x86_opr_table[];
extern const size_t x86_ord_table_size;
extern const x86_ord_data x86_ord_table[];

/*
 * encoder, decoder, table lookup, disassembly
 */

void x86_set_debug(uint d);

size_t x86_mode_name(char * buf, size_t len, uint mode, const char *sep);
size_t x86_map_name(char * buf, size_t len, uint mode, const char *sep);
size_t x86_ord_name(char * buf, size_t len, uint ord, const char *sep);
size_t x86_ord_mnem(char * buf, size_t len, const ushort *ord);
size_t x86_opr_name(char * buf, size_t len, uint opr);
size_t x86_enc_name(char * buf, size_t len, uint enc);
const char* x86_reg_name(uint reg);

char * x86_table_type_name(uint type);
char * x86_table_map_name(uint map);
char * x86_table_prefix_name(uint prefix);

int x86_enc_filter_rex(x86_rex prefix, uint enc);
int x86_enc_filter_rex2(x86_rex2 prefix, uint enc);
int x86_enc_filter_vex2(x86_vex2 prefix, uint enc);
int x86_enc_filter_vex3(x86_vex3 prefix, uint enc);
int x86_enc_filter_evex(x86_evex prefix, uint enc);

x86_table_idx x86_opc_table_identity();
x86_table_idx x86_opc_table_sorted(x86_table_idx tab, uint sort);
x86_table_idx x86_opc_table_filter(x86_table_idx tab, uint modes);
x86_opc_data* x86_table_lookup(x86_acc_idx *idx, const x86_opc_data *m);
void x86_print_op(const x86_opc_data *d, uint compact, uint opcode);
size_t x86_format_op(char *buf, size_t len, x86_ctx *ctx, x86_codec *c);
size_t x86_format_hex(char *buf, size_t len, uchar *data, size_t datalen);

x86_ctx* x86_ctx_create(uint mode);
void x86_ctx_destroy(x86_ctx *ctx);
int x86_codec_write(x86_buffer *buf, x86_codec c, size_t *len);
int x86_codec_read(x86_ctx *ctx, x86_buffer *buf, x86_codec *c, size_t *len, size_t limit);

/*
 * registers sand opcodes
 */

#include "x86_reg_enum.inc"
#include "x86_op_enum.inc"
