#include <stdio.h>

#include "x86.h"

void print_decode_table(uint modes, uint compact, uint opcode)
{
    x86_ctx *ctx = x86_ctx_create(modes);
    for(size_t i = 0; i < ctx->idx->map_count; i++) {
        x86_print_op(ctx->idx->map + i, compact, opcode);
    }
    x86_ctx_destroy(ctx);
}

void print_opcode_table(uint modes, uint sort, uint compact, uint opcode)
{

    x86_table_idx tab = x86_opc_table_filter(x86_opc_table_identity(), modes);
    if (sort) tab = x86_opc_table_sorted(tab, sort);
    for(size_t i = 0; i < tab.count; i++) {
        x86_print_op(x86_opc_table + tab.idx[i], compact, opcode);
    }
    free(tab.idx);
}

void print_help(const char *progname)
{
    fprintf(stderr, "%s [options]\n\n"
        "\t-a     sort opcodes alphanumerically\n"
        "\t-n     sort opcodes numerically\n"
        "\t-c     print compact\n"
        "\t-o     print opcode\n"
        "\t-g     generate opcode decode table\n"
        "\t-16    remove only 16-bit\n"
        "\t-32    remove only 32-bit\n"
        "\t-64    remove only 64-bit\n"
        "\t=16    include only 16-bit\n"
        "\t=32    include only 32-bit\n"
        "\t=64    include only 64-bit\n"
        "\n",
    progname);
}

int main(int argc, char **argv)
{
    uint modes = x86_modes_16 | x86_modes_32 | x86_modes_64;
    uint sort = x86_sort_none;
    uint help = 0, compact = 0, opcode = 0, generate = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) sort = x86_sort_alpha;
        else if (strcmp(argv[i], "-n") == 0) sort = x86_sort_numeric;
        else if (strcmp(argv[i], "-h") == 0) help = 1;
        else if (strcmp(argv[i], "-c") == 0) compact = 1;
        else if (strcmp(argv[i], "-o") == 0) opcode = 1;
        else if (strcmp(argv[i], "-g") == 0) generate = 1;
        else if (strcmp(argv[i], "-16") == 0) modes &= ~x86_modes_16;
        else if (strcmp(argv[i], "-32") == 0) modes &= ~x86_modes_32;
        else if (strcmp(argv[i], "-64") == 0) modes &= ~x86_modes_64;
        else if (strcmp(argv[i], "=16") == 0) modes = x86_modes_16;
        else if (strcmp(argv[i], "=32") == 0) modes = x86_modes_32;
        else if (strcmp(argv[i], "=64") == 0) modes = x86_modes_64;
        else {
            fprintf(stderr, "*** unknown option: %s\n", argv[i]);
            help = 1;
            break;
        }
    }
    if (help) {
        print_help(argv[0]);
    } else {
        if (generate) print_decode_table(modes, compact, opcode);
        else print_opcode_table(modes, sort, compact, opcode);
    }
    return 0;
}
