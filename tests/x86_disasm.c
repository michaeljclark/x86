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

    if (x86_codec_read(ctx, &buf, &codec, &nbytes, insnlen) == 0) {
        len += x86_format_hex(str+len, sizeof(str)-len, insn, nbytes);
        len += x86_format_op(str+len, sizeof(str)-len, ctx, &codec);
    } else {
        len += x86_format_hex(str+len, sizeof(str)-len, insn, nbytes);
        len += snprintf(str+len, sizeof(str)-len, "%s", "<unknown>");
    }

    printf("%s\n", str);
}

char* parse_hex(int argc, char **argv)
{
    uchar *buf = malloc(argc);
    for (size_t i = 0; i < argc; i++) {
        int x = 0;
        sscanf(argv[i], "%x", &x);
        buf[i] = (uchar)x;
    }
    return buf;
}

int main(int argc, char **argv)
{
    if (argc > 1) {
        uchar *buf = NULL;
        size_t len = 0;
        if (argc > 2 && strcmp(argv[1], "-d") == 0) {
            x86_set_debug(1);
            buf = parse_hex((len = argc - 2), argv + 2);
        } else {
            buf = parse_hex((len = argc - 1), argv + 1);
        }
        x86_ctx *ctx = x86_ctx_create(x86_modes_64);
        x86_disasm(ctx, buf, len);
        x86_ctx_destroy(ctx);
        free(buf);
    } else {
        fprintf(stderr, "usage: %s [-d] [hexbytes]\n", argv[0]);
    }
}