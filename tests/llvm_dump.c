#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>

#include "llvm-c/Disassembler.h"
#include "llvm-c/Support.h"
#include "llvm-c/Target.h"

static const int hexcols = 10;

typedef struct {
    void *data;
    size_t length;
} buffer;

static buffer parse_hex(int argc, char **argv)
{
    uint8_t *data = malloc(argc);
    for (size_t i = 0; i < argc; i++) {
        unsigned x = 0;
        sscanf(argv[i], "%x", &x);
        data[i] = (uint8_t)x;
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
        printf("load_file: open: %s: %s",
            filename, strerror(errno));
        exit(1);
    }
    if (fstat(fileno(f), &statbuf) < 0) {
        printf("load_file: stat: %s: %s",
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

static void format_hex(const uint8_t *data, size_t offset, size_t sz,
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

static int disasm_dump(LLVMDisasmContextRef ctx, const uint8_t *data,
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

int main(int argc, char **argv)
{
    buffer buf = { 0 };
    const char *filename = NULL;
    const char *triple = "x86_64";

    int help = 0, show_off = 0, show_hex = 0, i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-h") == 0) {
            help = 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            show_off = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            show_hex = 1;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            triple = argv[++i];
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
        fprintf(stderr,
            "usage: %s [-o] [-x] [-t <triple>] (-f <file.bin> | <hex> ...)\n",
            argv[0]);
        return -1;
    }
    else if (filename) {
        buf = load_file(filename);
    }

    const char* args[] = { argv[0], "--x86-asm-syntax=intel" };
    LLVMParseCommandLineOptions(2, args, "");

    LLVMInitializeAllTargetInfos();
    LLVMInitializeAllTargetMCs();
    LLVMInitializeAllDisassemblers();

    int ret = 0;
    if (buf.data && buf.length) {
        LLVMDisasmContextRef ctx =  LLVMCreateDisasmCPUFeatures
            (triple, "", "", NULL, 0, NULL, NULL);
        ret = disasm_dump(ctx, buf.data, buf.length, show_off, show_hex);
        LLVMDisasmDispose(ctx);
    }

    return ret;
}
