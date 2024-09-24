#pragma once

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "bits.h"
#include "bytes.h"

typedef enum x86_reg x86_reg;
typedef enum x86_op x86_op;
typedef enum x86_opr x86_opr;
typedef enum x86_modes x86_modes;
typedef enum x86_enc x86_enc;
typedef enum x86_ord x86_ord;
typedef enum x86_pb x86_pb;
typedef enum x86_pn x86_pn;
typedef enum x86_pc x86_pc;
typedef enum x86_rm x86_rm;
typedef enum x86_mod x86_mod;
typedef enum x86_seg x86_seg;
typedef enum x86_cf x86_cf;
typedef enum x86_scale x86_scale;
typedef enum x86_pfx x86_pfx;
typedef enum x86_map x86_map;
typedef enum x86_vex_l x86_vex_l;
typedef enum x86_vex_w x86_vex_w;
typedef enum x86_cond x86_cond;

typedef enum x86_sort x86_sort;
typedef enum x86_table x86_table;

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
typedef struct x86_mod_data x86_mod_data;
typedef struct x86_table_idx x86_table_idx;
typedef struct x86_map_idx x86_map_idx;
typedef struct x86_map_op x86_map_op;
typedef struct x86_buffer x86_buffer;

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
    x86_reg_csr   = 0b1011 << 5,

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
 * prefix number
 */

enum x86_pn
{
    x86_pn_26,
    x86_pn_2e,
    x86_pn_36,
    x86_pn_3e,
    x86_pn_41,
    x86_pn_42,
    x86_pn_44,
    x86_pn_48,
    x86_pn_62,
    x86_pn_64,
    x86_pn_65,
    x86_pn_66,
    x86_pn_67,
    x86_pn_9b,
    x86_pn_c4,
    x86_pn_c5,
    x86_pn_d5,
    x86_pn_f0,
    x86_pn_f2,
    x86_pn_f3,

    x86_pn_es      = x86_pn_26,
    x86_pn_cs      = x86_pn_2e,
    x86_pn_ss      = x86_pn_36,
    x86_pn_ds      = x86_pn_3e,
    x86_pn_rex_b   = x86_pn_41,
    x86_pn_rex_x   = x86_pn_42,
    x86_pn_rex_r   = x86_pn_44,
    x86_pn_rex_w   = x86_pn_48,
    x86_pn_evex    = x86_pn_62,
    x86_pn_fs      = x86_pn_64,
    x86_pn_gs      = x86_pn_65,
    x86_pn_osize   = x86_pn_66,
    x86_pn_asize   = x86_pn_67,
    x86_pn_wait    = x86_pn_9b,
    x86_pn_vex3    = x86_pn_c4,
    x86_pn_vex2    = x86_pn_c5,
    x86_pn_rex2    = x86_pn_d5,
    x86_pn_lock    = x86_pn_f0,
    x86_pn_repne   = x86_pn_f2,
    x86_pn_rep     = x86_pn_f3,
};

enum x86_pc
{
    x86_pc_none = 0b0000,
    x86_pc_66   = 0b0001,
    x86_pc_9b   = 0b0010,
    x86_pc_f2   = 0b0011,
    x86_pc_f3   = 0b0100,
    x86_pc_48   = 0b1000
};

/*
 * prefix table
 */

static const uchar x86_prefixes[21] =
{
    [x86_pn_es]    = x86_pb_es,
    [x86_pn_cs]    = x86_pb_cs,
    [x86_pn_ss]    = x86_pb_ss,
    [x86_pn_ds]    = x86_pb_ds,
    [x86_pn_rex_b] = x86_pb_rex_b,
    [x86_pn_rex_x] = x86_pb_rex_x,
    [x86_pn_rex_r] = x86_pb_rex_r,
    [x86_pn_rex_w] = x86_pb_rex_w,
    [x86_pn_evex]  = x86_pb_evex,
    [x86_pn_fs]    = x86_pb_fs,
    [x86_pn_gs]    = x86_pb_gs,
    [x86_pn_osize] = x86_pb_osize,
    [x86_pn_asize] = x86_pb_asize,
    [x86_pn_wait]  = x86_pb_wait,
    [x86_pn_vex3]  = x86_pb_vex3,
    [x86_pn_vex2]  = x86_pb_vex2,
    [x86_pn_rex2]  = x86_pb_rex2,
    [x86_pn_lock]  = x86_pb_lock,
    [x86_pn_repne] = x86_pb_repne,
    [x86_pn_rep]   = x86_pb_rep,
};

/*
 * r/m
 */

enum x86_rm
{
    x86_rm_disp_sib = 0b100,
    x86_rm_disp0_iw = 0b101,

    x86_rm_shift = 0,
    x86_rm_mask = 7,

    x86_reg_shift = 3,
    x86_reg_mask = 7,

    x86_mod_shift = 6,
    x86_mod_mask = 3
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
    x86_enc_w_shift          = 8,
    x86_enc_m_shift          = x86_enc_w_shift + 3,
    x86_enc_p_shift          = x86_enc_m_shift + 3,
    x86_enc_l_shift          = x86_enc_p_shift + 3,
    x86_enc_t_shift          = x86_enc_l_shift + 3,
    x86_enc_modrm_shift      = x86_enc_t_shift + 5,

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
    x86_enc_p_9b             = (2 << x86_enc_p_shift),
    x86_enc_p_f2             = (3 << x86_enc_p_shift),
    x86_enc_p_f3             = (4 << x86_enc_p_shift),
    x86_enc_p_mask           = (7 << x86_enc_p_shift),

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
    x86_enc_t_o16            = (4 << x86_enc_t_shift),
    x86_enc_t_o32            = (5 << x86_enc_t_shift),
    x86_enc_t_o64            = (6 << x86_enc_t_shift),
    x86_enc_t_a16            = (7 << x86_enc_t_shift),
    x86_enc_t_a32            = (8 << x86_enc_t_shift),
    x86_enc_t_a64            = (9 << x86_enc_t_shift),
    x86_enc_t_lock           = (10 << x86_enc_t_shift),
    x86_enc_t_rep            = (11 << x86_enc_t_shift),
    x86_enc_t_modrm_r        = (12 << x86_enc_t_shift), // /r
    x86_enc_t_modrm_n        = (13 << x86_enc_t_shift), // /N
    x86_enc_t_opcode_r       = (14 << x86_enc_t_shift), // +r
    x86_enc_t_opcode         = (15 << x86_enc_t_shift), // XX
    x86_enc_t_ib             = (16 << x86_enc_t_shift),
    x86_enc_t_iw             = (17 << x86_enc_t_shift),
    x86_enc_t_i16            = (18 << x86_enc_t_shift),
    x86_enc_t_i32            = (19 << x86_enc_t_shift),
    x86_enc_t_i64            = (20 << x86_enc_t_shift),
    x86_enc_t_i16e           = (21 << x86_enc_t_shift),
    x86_enc_t_mask           = (31 << x86_enc_t_shift),

    x86_enc_t_modrm_0        = x86_enc_t_modrm_n | (0 << x86_enc_modrm_shift),
    x86_enc_t_modrm_1        = x86_enc_t_modrm_n | (1 << x86_enc_modrm_shift),
    x86_enc_t_modrm_2        = x86_enc_t_modrm_n | (2 << x86_enc_modrm_shift),
    x86_enc_t_modrm_3        = x86_enc_t_modrm_n | (3 << x86_enc_modrm_shift),
    x86_enc_t_modrm_4        = x86_enc_t_modrm_n | (4 << x86_enc_modrm_shift),
    x86_enc_t_modrm_5        = x86_enc_t_modrm_n | (5 << x86_enc_modrm_shift),
    x86_enc_t_modrm_6        = x86_enc_t_modrm_n | (6 << x86_enc_modrm_shift),
    x86_enc_t_modrm_7        = x86_enc_t_modrm_n | (7 << x86_enc_modrm_shift),
    x86_enc_modrm_mask       = (7 << x86_enc_modrm_shift),
};

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
    x86_opr_size_word        = 14 << x86_opr_s2,
    x86_opr_size_addr        = 15 << x86_opr_s2,
    x86_opr_size_mask        = 15 << x86_opr_s2,

    /* sized register */
    x86_opr_r8               = x86_opr_reg | x86_opr_size_8,
    x86_opr_r16              = x86_opr_reg | x86_opr_size_16,
    x86_opr_r32              = x86_opr_reg | x86_opr_size_32,
    x86_opr_r64              = x86_opr_reg | x86_opr_size_64,
    x86_opr_rw               = x86_opr_reg | x86_opr_size_word,
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
    x86_opr_iw               = x86_opr_imm | x86_opr_size_word,

    /* relative displacement */
    x86_opr_rel8             = x86_opr_imm | x86_opr_rel | x86_opr_size_8,
    x86_opr_relw             = x86_opr_imm | x86_opr_rel | x86_opr_size_word,

    /* memory offset */
    x86_opr_moffs            = x86_opr_imm | x86_opr_mem | x86_opr_size_addr,

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
    x86_opr_i16e             = x86_opr_imm | x86_opr_far | (1 << x86_opr_s4),
    x86_opr_far16_16         = x86_opr_imm | x86_opr_far | (2 << x86_opr_s4),
    x86_opr_far16_32         = x86_opr_imm | x86_opr_far | (3 << x86_opr_s4),

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
    x86_opr_reg_st1          = x86_opr_ind | x86_opr_st  | (1 << x86_opr_s4),
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
    x86_opr_reg_aw           = x86_opr_reg_a | x86_opr_size_word,
    x86_opr_reg_cw           = x86_opr_reg_c | x86_opr_size_word,
    x86_opr_reg_dw           = x86_opr_reg_d | x86_opr_size_word,
    x86_opr_reg_bw           = x86_opr_reg_b | x86_opr_size_word,
    x86_opr_reg_pa           = x86_opr_reg_a | x86_opr_size_addr,
    x86_opr_reg_pc           = x86_opr_reg_c | x86_opr_size_addr,
    x86_opr_reg_pd           = x86_opr_reg_d | x86_opr_size_addr,
    x86_opr_reg_pb           = x86_opr_reg_b | x86_opr_size_addr,
    x86_opr_reg_psi          = x86_opr_reg_si | x86_opr_size_addr,
    x86_opr_reg_pdi          = x86_opr_reg_di | x86_opr_size_addr,
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
    x86_ord_type_mask        = 7 << x86_ord_s1,

    x86_ord_r                = 0b001 << x86_ord_s2,
    x86_ord_w                = 0b010 << x86_ord_s2,
    x86_ord_i                = 0b100 << x86_ord_s2,
    x86_ord_rw               = 0b011 << x86_ord_s2,
    x86_ord_ri               = 0b101 << x86_ord_s2,
    x86_ord_wi               = 0b110 << x86_ord_s2,
    x86_ord_rwi              = 0b111 << x86_ord_s2,

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
    x86_ci_iw      = 1 << x86_ci_shift,
    x86_ci_ib      = 2 << x86_ci_shift,
    x86_ci_i16     = 3 << x86_ci_shift,
    x86_ci_i32     = 4 << x86_ci_shift,
    x86_ci_i64     = 5 << x86_ci_shift,
    x86_ci_mask    = 7 << x86_ci_shift,

    /* [8:13] prefixes */
    x86_cp_osize   = 1 << 8,  /* 0x66 */
    x86_cp_asize   = 1 << 9,  /* 0x67 */
    x86_cp_wait    = 1 << 10, /* 0x9B */
    x86_cp_lock    = 1 << 11, /* 0xF0 */
    x86_cp_repne   = 1 << 12, /* 0xF2 */
    x86_cp_rep     = 1 << 13, /* 0xF3 */

    /* [14:17] flags */
    x86_cf_modrm   = 1 << 14,
    x86_cf_ia32    = 1 << 15,
    x86_cf_amd64   = 1 << 16,
    x86_cf_i16e    = 1 << 17,
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
    ushort imm16e;

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
 * modes
 */

enum x86_modes
{
    x86_modes_16 = (1 << 0),
    x86_modes_32 = (1 << 1),
    x86_modes_64 = (1 << 2),
};

/*
 * mode bits
 */

struct x86_modeb
{
    uchar is64 : 1;
    uchar is32 : 1;
    uchar is16 : 1;
};

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
 */

struct x86_opc_data
{
    ushort op : 12;
    ushort mode : 4;
    ushort opr : 9;
    ushort ord : 7;
    const uint enc[5];
};

struct x86_opr_data
{
    const uint opr[4];
};

struct x86_ord_data
{
    const ushort ord[4];
};

/*
 * invert condition
 */

static inline x86_cond x86_invert_cond(x86_cond c)
{
    return (x86_cond)(c ^ 1);
}

/*
 * swap condition operands
 */

static inline x86_cond x86_swap_cond(x86_cond c)
{
    return c & 6 ? (x86_cond)(c ^ 9) : c;
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

static inline x86_sib x86_enc_sib(x86_scale s, uint x, uint b)
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

static inline x86_vex2 x86_enc_vex2(x86_pfx p, x86_vex_l l,
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

static inline x86_vex3 x86_enc_vex3(x86_map m, x86_pfx p,
    x86_vex_l l, x86_vex_w w, uint r, uint x, uint b, uint v)
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

static inline x86_evex x86_enc_evex(x86_map m, x86_pfx p,
    x86_vex_l l, x86_vex_w w, uint r, uint x, uint b, uint v,
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
 * table generation
 */

enum x86_sort
{
    x86_sort_none,
    x86_sort_numeric,
    x86_sort_alpha
};

enum x86_table
{
    x86_table_none,
    x86_table_lex,
    x86_table_vex,
    x86_table_evex
};

struct x86_table_idx
{
    size_t count;
    size_t *idx;
};

struct x86_map_idx
{
    size_t count;
    x86_map_op *map;
};

struct x86_map_op
{
    uchar type   : 2;
    uchar prefix : 6;
    uchar map    : 6;
    uchar opc[2];
    uchar opm[2];
    ushort rec;
};

struct x86_buffer
{
    char *data;
    size_t start;
    size_t end;
};

/*
 * buffer functions
 */

static inline void x86_buffer_init(x86_buffer *b, char *data)
{
    b->start = 0;
    b->end = 0;
    b->data = data;
}

static inline size_t x86_buffer_read(x86_buffer *b, char *buf, size_t len)
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

static inline size_t x86_buffer_write(x86_buffer *b, char *buf, size_t len)
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
size_t x86_prefix_name(char * buf, size_t len, uint ord, const char *sep);
size_t x86_prefix_namen(char * buf, size_t len, uint ord, const char *sep);
size_t x86_ord_name(char * buf, size_t len, uint ord, const char *sep);
size_t x86_ord_mnem(char * buf, size_t len, const ushort *ord);
size_t x86_opr_name(char * buf, size_t len, uint opr);
size_t x86_enc_name(char * buf, size_t len, uint enc);
const char* x86_reg_name(uint reg);

int x86_enc_filter_rex(x86_rex prefix, x86_enc enc);
int x86_enc_filter_rex2(x86_rex2 prefix, x86_enc enc);
int x86_enc_filter_vex2(x86_vex2 prefix, x86_enc enc);
int x86_enc_filter_vex3(x86_vex3 prefix, x86_enc enc);
int x86_enc_filter_evex(x86_evex prefix, x86_enc enc);

int x86_codec_write(x86_buffer *buf, x86_codec c, size_t *len);
int x86_codec_read(x86_buffer *buf, x86_codec *c, size_t *len, size_t limit);

x86_table_idx x86_opc_table_identity();
x86_table_idx x86_opc_table_sorted(x86_table_idx tab, x86_sort sort);
x86_table_idx x86_opc_table_filter(x86_table_idx tab, x86_modes modes);
x86_map_idx x86_table_build(x86_modes modes);
x86_map_op* x86_table_lookup(x86_map_idx map, const x86_map_op *m);
void x86_print_map(x86_map_op *m);
void x86_print_op(uint opcrec, uint compact);
size_t x86_format_op(char *buf, size_t len, x86_codec *c);
size_t x86_format_hex(char *buf, size_t len, char *data, size_t datalen);

/*
 * registers sand opcodes
 */

#include "x86_reg_enum.inc"
#include "x86_op_enum.inc"
