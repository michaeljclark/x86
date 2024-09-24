#include <stdio.h>

#include "x86.h"

int main(int argc, char **argv)
{
    printf("x86_mem   %2zu\n", sizeof(x86_mem));
    printf("x86_codec %2zu\n", sizeof(x86_codec));
    printf("opc_table %2zu %5zu %7zu\n",
        sizeof(x86_opc_data), x86_opc_table_size,
        sizeof(x86_opc_data) * x86_opc_table_size);
    printf("opr_table %2zu %5zu %7zu\n",
        sizeof(x86_opr_data), x86_opr_table_size,
        sizeof(x86_opr_data) * x86_opr_table_size);
    printf("ord_table %2zu %5zu %7zu\n",
        sizeof(x86_ord_data), x86_ord_table_size,
        sizeof(x86_ord_data) * x86_ord_table_size);
    x86_map_idx map = x86_table_build(x86_modes_64);
    printf("map_table %2zu %5zu %7zu\n",
        sizeof(x86_map_op), map.count,
        sizeof(x86_map_op) * map.count);
    return 0;
}
