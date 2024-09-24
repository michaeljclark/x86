#include <stdio.h>

#include "x86.h"

void print_decode_table(x86_modes modes)
{
    x86_map_idx map = x86_table_build(modes);
    for(size_t i = 0; i < map.count; i++) {
        x86_print_map(map.map + i);
    }
    free(map.map);
}

void print_opcode_table(x86_modes modes, x86_sort sort, uint compact)
{

    x86_table_idx tab = x86_opc_table_filter(x86_opc_table_identity(), modes);
    if (sort) tab = x86_opc_table_sorted(tab, sort);
    for(size_t i = 0; i < tab.count; i++) {
        x86_print_op(tab.idx[i], compact);
    }
    free(tab.idx);
}

void print_help(const char *progname)
{
    fprintf(stderr, "%s [options]\n\n"
        "\t-a     sort opcodes alphanumerically\n"
        "\t-n     sort opcodes numerically\n"
        "\t-c     use compact order output\n"
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
    x86_modes modes = x86_modes_16 | x86_modes_32 | x86_modes_64;
    x86_sort sort = x86_sort_none;
    uint help = 0, compact = 0, generate = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) sort = x86_sort_alpha;
        else if (strcmp(argv[i], "-n") == 0) sort = x86_sort_numeric;
        else if (strcmp(argv[i], "-h") == 0) help = 1;
        else if (strcmp(argv[i], "-c") == 0) compact = 1;
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
        if (generate) print_decode_table(modes);
        else print_opcode_table(modes, sort, compact);
    }
    return 0;
}
