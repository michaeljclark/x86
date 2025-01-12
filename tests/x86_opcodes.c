#include <stdio.h>
#include <stdlib.h>

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

void print_opcode_stats(uint modes)
{
    printf("%4s %4s %4s", "typ", "pfx", "map");
    for (size_t j = 0; j < 16; j++) printf("  %02hhx", (uint)j << 4);
    printf("\n");

    printf("%4s %4s %4s", "----", "----", "----");
    for (size_t j = 0; j < 16; j++) printf(" ---");
    printf("\n");

    x86_ctx *ctx = x86_ctx_create(modes);
    for (uint type = 0; type < 4; type++) {
        for (uint prefix = 0; prefix < 16; prefix++) {
            for (uint map = 0; map < 8; map++) {
                const char * type_name = x86_table_type_name(type);
                const char * prefix_name = x86_table_prefix_name(prefix);
                const char * map_name = x86_table_map_name(map);
                size_t acc_page = x86_acc_page(type, prefix, map);
                size_t offset = x86_acc_offset(ctx->idx, acc_page);
                if (offset == 0) continue;
                size_t opcodes[16] = { 0 };
                for (size_t op = 0; op < 256; op++) {
                    x86_acc_entry *ent = x86_acc_lookup(ctx->idx, offset + op);
                    opcodes[op>>4] += ent->nent;
                }
                printf("%4s %4s %4s", type_name, prefix_name, map_name);
                for (size_t j = 0; j < 16; j++)
                    if (opcodes[j]) printf(" %3zu", opcodes[j]);
                    else printf("   -");
                printf("\n");
            }
        }
    }
    x86_ctx_destroy(ctx);
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
    uint help = 0, compact = 0, opcode = 0, generate = 0, stats = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) sort = x86_sort_alpha;
        else if (strcmp(argv[i], "-n") == 0) sort = x86_sort_numeric;
        else if (strcmp(argv[i], "-h") == 0) help = 1;
        else if (strcmp(argv[i], "-c") == 0) compact = 1;
        else if (strcmp(argv[i], "-o") == 0) opcode = 1;
        else if (strcmp(argv[i], "-g") == 0) generate = 1;
        else if (strcmp(argv[i], "-s") == 0) stats = 1;
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
        if (stats) print_opcode_stats(modes);
        else if (generate) print_decode_table(modes, compact, opcode);
        else print_opcode_table(modes, sort, compact, opcode);
    }
    return 0;
}
