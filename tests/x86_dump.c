#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>

#include "x86.h"

static const int hexcols = 10;

typedef struct {
    void *data;
    size_t length;
} buffer;

static buffer parse_hex(int argc, char **argv)
{
    uchar *data = malloc(argc);
    for (size_t i = 0; i < argc; i++) {
        unsigned x = 0;
        sscanf(argv[i], "%x", &x);
        data[i] = (uchar)x;
    }
    buffer buf = { data, argc };
    return buf;
}

static buffer load_file(const char *filename)
{
    FILE *f;
    struct stat statbuf;
    char *buf;
    size_t nread;

    if ((f = fopen(filename, "r")) == NULL) {
        printf("load_file: open: %s: %s\n",
            filename, strerror(errno));
        exit(1);
    }
    if (fstat(fileno(f), &statbuf) < 0) {
        printf("load_file: stat: %s: %s\n",
            filename, strerror(errno));
        exit(1);
    }
    buf = (char*)malloc(statbuf.st_size);
    if ((nread = fread(buf, 1, statbuf.st_size, f)) != statbuf.st_size) {
        printf("load_file: fread: %s: expected %zu got %zu\n",
            filename, (size_t)statbuf.st_size, nread);
        exit(1);
    }
    return (buffer){buf, (size_t)statbuf.st_size};
}

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

static void x86_disasm(x86_ctx *ctx, uchar *insn, size_t insnlen,
    int show_off, int show_hex)
{
    x86_buffer buf;
    x86_codec codec;
    char str[128];
    size_t nbytes = 0, offset = 0;
    int ret;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);

    do {
        ret = x86_codec_read(ctx, &buf, &codec, &nbytes);
        if (ret == 0) {
            x86_format_op(str, sizeof(str), ctx, &codec);
        } else {
            break;
        }
        if (show_off || show_hex) {
            format_hex(insn, offset, nbytes, show_off, show_hex);
        }
        printf("\t%s\n", str);
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

int main(int argc, char **argv)
{
    buffer buf = { 0 };
    const char *filename = NULL;

    int help = 0, show_off = 0, show_hex = 0, i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-h") == 0) {
            help = 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            show_off = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            show_hex = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            x86_set_debug(1);
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else {
            break; /* hexidecimal */
        }
        i++;
    }

    if (!help && !filename && i < argc) {
        buf = parse_hex(argc - i, argv + i);
    }
    else if (help) {
        fprintf(stderr, "usage: %s [-o] [-x] [-d] (-f <file.bin> | <hex> ...)\n",
            argv[0]);
        return -1;
    }
    else if (filename) {
        buf = load_file(filename);
    }

    if (buf.data && buf.length) {
        x86_ctx *ctx = x86_ctx_create(x86_modes_64);
        x86_disasm(ctx, buf.data, buf.length, show_off, show_hex);
        x86_ctx_destroy(ctx);
    }

    return 0;
}
