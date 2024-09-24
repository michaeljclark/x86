#include <stdio.h>

#include "x86.h"

void x86_out(x86_codec codec)
{
    x86_buffer buf;
    char data[32];
    size_t nbytes = 0;

    x86_buffer_init(&buf, data);

    if (x86_codec_write(&buf, codec, &nbytes) == 0)
    {
        printf("./scripts/disasm-x86.sh");

        for (size_t i = 0; i < nbytes; i++) {
            printf(" %02hhx", data[i]);
        }
        printf("\n");
    }
}

void t1()
{
    // MOVD xmm/m32,xmm [mr: 66 0f 7e /r]
    // asm: movd DWORD PTR [r14+r13*8-0x8],xmm15

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x7e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_cp_osize;
    codec.flags |= x86_ce_rex;
    codec.flags |= x86_cm_0f;
    codec.rex = x86_enc_rex(0, x86_xmm15, x86_r13, x86_r14);
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_r13, x86_r14);
    codec.disp32 = -8;

    x86_out(codec);
}

void t2()
{
    // MOVD xmm,xmm/m32 [rm: 66 0f 6e /r]
    // asm: movd xmm15,DWORD PTR [r14+r13*8-0x8]

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x6e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_cp_osize;
    codec.flags |= x86_ce_rex;
    codec.flags |= x86_cm_0f;
    codec.rex = x86_enc_rex(0, x86_xmm15, x86_r13, x86_r14);
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_r13, x86_r14);
    codec.disp32 = -8;

    x86_out(codec);
}

void t3()
{
    // VMOVD xmm,xmm/m32 [mr: vex.128.66.0f.w0 7e /r]
    // asm: vmovd DWORD PTR [rcx+rax*8-8],xmm15

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x7e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_ce_vex2;
    codec.vex2 = x86_enc_vex2(
        x86_pfx_66, x86_vex_l128,
        /*r*/ x86_xmm15, /*v*/ 0
    );
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_rax, x86_rcx);
    codec.disp32 = -8;

    x86_out(codec);
}

void t4()
{
    // VMOVD xmm,xmm/m32 [mr: vex.128.66.0f.w0 6e /r]
    // asm: vmovd xmm15,DWORD PTR [rcx+rax*8-8]

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x6e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_ce_vex2;
    codec.vex2 = x86_enc_vex2(
        x86_pfx_66, x86_vex_l128,
        /*r*/ x86_xmm15, /*v*/ 0
    );
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_rax, x86_rcx);
    codec.disp32 = -8;

    x86_out(codec);
}

void t5()
{
    // VMOVD xmm/m32,xmm [mr: vex.128.66.0f.w0 7e /r]
    // asm: vmovd DWORD PTR [r9+r8*8-8],xmm15

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x7e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_ce_vex3;
    codec.vex3 = x86_enc_vex3(
        x86_map_0f, x86_pfx_66, x86_vex_l128, x86_vex_w0,
        /*r*/ x86_xmm15, /*x*/ x86_r14, /*b*/ x86_r13, /*v*/ 0
    );
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_r14, x86_r13);
    codec.disp32 = -8;

    x86_out(codec);
}

void t6()
{
    // VMOVD xmm,xmm/m32 [mr: vex.128.66.0f.w0 6e /r]
    // asm: vmovd xmm15,DWORD PTR [r9+r8*8-8]

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x6e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_ce_vex3;
    codec.vex3 = x86_enc_vex3(
        x86_map_0f, x86_pfx_66, x86_vex_l128, x86_vex_w0,
        /*r*/ x86_xmm15, /*x*/ x86_r14, /*b*/ x86_r13, /*v*/ 0
    );
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_r14, x86_r13);
    codec.disp32 = -8;

    x86_out(codec);
}

void t7()
{
    // VMOVD xmm/m32,xmm [mr: evex.128.66.0f.w0 7e /r]
    // asm: vmovd DWORD PTR [r14+r13*8-8],xmm31

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x7e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_ce_evex;
    codec.evex = x86_enc_evex(
        x86_map_0f, x86_pfx_66, x86_vex_l128, x86_vex_w0,
        /*r*/ x86_xmm31, /*x*/ x86_r13, /*b*/ x86_r14, /*v*/ 0,
        /*k*/ 0, /*brd*/ 0, /*z*/ 0
    );
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm15, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_r13, x86_r14);
    codec.disp32 = -2; /* EVEX scales displacement to word size */

    x86_out(codec);
}

void t8()
{
    // VMOVD xmm,xmm/m32 [rm: evex.128.66.0f.w0 6e /r]
    // asm: vmovd xmm31,DWORD PTR [r14+r13*8-8]

    x86_codec codec;

    memset(&codec, 0, sizeof(codec));
    codec.opc[0] = 0x6e;
    codec.opclen = 1;
    codec.flags |= x86_cf_amd64;
    codec.flags |= x86_cf_modrm;
    codec.flags |= x86_ce_evex;
    codec.evex = x86_enc_evex(
        x86_map_0f, x86_pfx_66, x86_vex_l128, x86_vex_w0,
        /*r*/ x86_xmm31, /*x*/ x86_r13, /*b*/ x86_r14, /*v*/ 0,
        /*k*/ 0, /*brd*/ 0, /*z*/ 0
    );
    codec.modrm = x86_enc_modrm(x86_mod_disp8, x86_xmm31, x86_rm_disp_sib);
    codec.sib = x86_enc_sib(x86_scale_8, x86_r13, x86_r14);
    codec.disp32 = -2; /* EVEX scales displacement to word size */

    x86_out(codec);
}

int main(int argc, char const *argv[])
{
    t1();
    t2();
    t3();
    t4();
    t5();
    t6();
    t7();
    t8();
    return 0;
}
