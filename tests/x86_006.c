#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#include "x86.h"

void x86_disasm(x86_ctx *ctx, uchar *insn, size_t insnlen)
{
    x86_buffer buf;
    x86_codec codec;
    char str[128];
    size_t nbytes = 0, len = 0;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);

    if (x86_codec_read(ctx, &buf, &codec, &nbytes) == 0) {
        len += x86_format_hex(str+len, sizeof(str)-len, insn, nbytes);
        len += x86_format_op(str+len, sizeof(str)-len, ctx, &codec);
    } else {
        len += x86_format_hex(str+len, sizeof(str)-len, insn, nbytes);
        len += snprintf(str+len, sizeof(str)-len, "%s", "<unknown>");
    }

    printf("%s\n", str);
}

void t1(x86_ctx *ctx)
{
    uchar insn[] = { 0x66, 0x47, 0x0f, 0x7e, 0x7c, 0xee, 0xf8 };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t2(x86_ctx *ctx)
{
    uchar insn[] = { 0x66, 0x47, 0x0f, 0x6e, 0x7c, 0xee, 0xf8 };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t3(x86_ctx *ctx)
{
    uchar insn[] = { 0xc5, 0x79, 0x7e, 0x7c, 0xc1, 0xf8 };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t4(x86_ctx *ctx)
{
    uchar insn[] = { 0xc5, 0x79, 0x6e, 0x7c, 0xc1, 0xf8 };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t5(x86_ctx *ctx)
{
    uchar insn[] = { 0xc4, 0x01, 0x79, 0x7e, 0x7c, 0xf5, 0xf8 };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t6(x86_ctx *ctx)
{
    uchar insn[] = { 0xc4, 0x01, 0x79, 0x6e, 0x7c, 0xf5, 0xf8 };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t7(x86_ctx *ctx)
{
    uchar insn[] = { 0x62, 0x01, 0x7d, 0x08, 0x7e, 0x7c, 0xee, 0xfe };
    x86_disasm(ctx, insn, sizeof(insn));
}

void t8(x86_ctx *ctx)
{
    uchar insn[] = { 0x62, 0x01, 0x7d, 0x08, 0x6e, 0x7c, 0xee, 0xfe };
    x86_disasm(ctx, insn, sizeof(insn));
}

int main(int argc, char **argv)
{
    x86_ctx *ctx = x86_ctx_create(x86_modes_64);
    t1(ctx);
    t2(ctx);
    t3(ctx);
    t4(ctx);
    t5(ctx);
    t6(ctx);
    t7(ctx);
    t8(ctx);
    x86_ctx_destroy(ctx);
    return 0;
}
