#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "x86.h"

typedef size_t (*bench_fn)(x86_ctx *ctx, uchar *insn, size_t insnlen);

size_t x86_disasm_decode(x86_ctx *ctx, uchar *insn, size_t insnlen)
{
    x86_buffer buf;
    x86_codec codec;
    size_t nbytes = 0, offset = 0, count = 0;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);

    do {
        if (x86_codec_read(ctx, &buf, &codec, &nbytes) == 0) {
            offset += nbytes;
            count++;
        } else {
            break;
        }
    } while (offset < insnlen);
    return count;
}

size_t x86_disasm_format(x86_ctx *ctx, uchar *insn, size_t insnlen)
{
    x86_buffer buf;
    x86_codec codec;
    char str[128];
    size_t nbytes = 0, offset = 0, count = 0;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);

    do {
        if (x86_codec_read(ctx, &buf, &codec, &nbytes) == 0) {
            x86_format_op(str, sizeof(str), ctx, &codec);
            offset += nbytes;
            count++;
        } else {
            snprintf(str, sizeof(str), "%s", "<unknown>");
            break;
        }
    } while (offset < insnlen);
    return count;
}

size_t x86_disasm_test(x86_ctx *ctx, uchar *insn, size_t insnlen)
{
    x86_buffer buf;
    x86_codec codec;
    char str[128];
    size_t nbytes = 0, offset = 0, count = 0;

    x86_buffer_init_ex(&buf, insn, 0, insnlen);

    do {
        if (x86_codec_read(ctx, &buf, &codec, &nbytes) == 0) {
            x86_format_op(str, sizeof(str), ctx, &codec);
            printf("%s\n", str);
            offset += nbytes;
            count++;
        } else {
            snprintf(str, sizeof(str), "%s", "<unknown>");
            break;
        }
    } while (offset < insnlen);
    return count;
}

static void header()
{
    printf("%16s %6s %8s %8s %10s %8s %8s\n",
        "benchmark", "loops", "size", "dur(s)", "time(ns)", "MIPS", "mb/sec");
    printf("%16s %6s %8s %8s %10s %8s %8s\n",
        "----------------", "------", "--------", "--------",
        "----------", "--------", "--------");
}

void bench_exec(x86_ctx *ctx, const char *name, bench_fn fn, size_t loops,
    uchar *insn, size_t size)
{
    clock_t start, end;
    size_t sum, count = 0;
    double s, mbsec;

    start = clock();
    for (uint j = 0; j < loops; j++) {
        count += fn(ctx, insn, size);
    }
    end = clock();

    sum = loops * size;
    s = (1e9 * (end - start)) / ((double)CLOCKS_PER_SEC);
    mbsec = ((double)sum / (double)(1<<20)) / (s/1e9);

    printf("%16s %6zu %8zu %8.2f %8.2fns %8.3f %8.3f\n",
        name, loops, size, s/1e9, s/sum, (1e3 * count/s), mbsec);
}

void bench_run(x86_ctx *ctx, const char *name, bench_fn fn, size_t loops,
    uchar *insn, size_t len, size_t total)
{
    uchar *data = malloc(total + len);
    for (size_t i = 0; i < total; i+= len) {
        memcpy(data + i, insn, len);
    }
    bench_exec(ctx, name, fn, loops, data, total);
    free(data);
}

void run_benchmarks(x86_ctx *ctx, size_t loops, size_t size,
    uint decode, uint format)
{
    uchar add_reg[] = { 0x01, 0xc1 };
    uchar add_mem[] = { 0x00, 0x01 };
    uchar valignq[] = { 0x62, 0xf3, 0xfd, 0x48, 0x03, 0xd1, 0x07 };
    uchar misc[] = {
        0xf3, 0x0f, 0x1e, 0xfa, 0x55, 0x89, 0xfd, 0xbf,
        0x08, 0x00, 0x00, 0x00, 0x53, 0x48, 0x83, 0xec,
        0x08, 0xe8, 0xaa, 0xb5, 0xff, 0xff, 0x89, 0xef,
        0x48, 0x89, 0xc3, 0xe8, 0x10, 0xcb, 0xff, 0xff,
        0x48, 0x89, 0x03, 0x48, 0x83, 0xc4, 0x08, 0x48,
        0x89, 0xd8, 0x5b, 0x5d, 0xc3, 0x0f, 0x1f, 0x00
    };
    header();
    if (decode) {
        bench_run(ctx, "decode add_reg", x86_disasm_decode,
                  loops, add_reg, sizeof(add_reg), size * 1024);
        bench_run(ctx, "decode add_mem", x86_disasm_decode,
                  loops, add_mem, sizeof(add_mem), size * 1024);
        bench_run(ctx, "decode valignq", x86_disasm_decode,
                  loops, valignq, sizeof(valignq), size * 1024);
        bench_run(ctx, "decode misc", x86_disasm_decode,
                  loops, misc, sizeof(misc), size * 1024);
    }
    if (format) {
        bench_run(ctx, "format add_reg", x86_disasm_format,
                  loops, add_reg, sizeof(add_reg), size * 1024);
        bench_run(ctx, "format add_mem", x86_disasm_format,
                  loops, add_mem, sizeof(add_mem), size * 1024);
        bench_run(ctx, "format valignq", x86_disasm_format,
                  loops, valignq, sizeof(valignq), size * 1024);
        bench_run(ctx, "format misc", x86_disasm_format,
                  loops, misc, sizeof(misc), size * 1024);
    }
}

void run_tests(x86_ctx *ctx)
{
    uchar add_mem[] = { 0x00, 0x01 };
    uchar add_reg[] = { 0x01, 0xc1 };
    uchar valignq[] = { 0x62, 0xf3, 0xfd, 0x48, 0x03, 0xd1, 0x07 };
    bench_run(ctx, "test add_reg", x86_disasm_test,
              1, add_reg, sizeof(add_reg), 16);
    bench_run(ctx, "test add_mem", x86_disasm_test,
              1, add_mem, sizeof(add_mem), 16);
    bench_run(ctx, "test valignq", x86_disasm_test,
              1, valignq, sizeof(valignq), 16);
}

void print_help(const char *progname)
{
    fprintf(stderr, "%s [options]\n\n"
        "\t-l <int>    benchmark loops\n"
        "\t-s <int>    benchmark size (KiB)\n"
        "\t-d          decode benchmark\n"
        "\t-f          format benchmark\n"
        "\t-t          disassembly tests\n"
        "\t-h          help message\n"
        "\n",
    progname);
}

int main(int argc, char **argv)
{
    size_t loops = 20, size = 128;
    uint decode = 0, format = 0, test = 0, help = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) decode = 1;
        else if (strcmp(argv[i], "-f") == 0) format = 1;
        else if (strcmp(argv[i], "-t") == 0) test = 1;
        else if (strcmp(argv[i], "-h") == 0) help = 1;
        else if (strcmp(argv[i], "-l") == 0) {
            loops = strtoul(argv[++i], NULL, 10);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            size = strtoul(argv[++i], NULL, 10);
        }
        else {
            fprintf(stderr, "*** unknown option: %s\n", argv[i]);
            help = 1;
            break;
        }
    }
    if (help) {
        print_help(argv[0]);
    } else {
        x86_ctx *ctx = x86_ctx_create(x86_modes_64);
        if (test) run_tests(ctx);
        else run_benchmarks(ctx, loops, size, decode, format);
        x86_ctx_destroy(ctx);
    }
    return 0;
}
