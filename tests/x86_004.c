#include "x86.h"

void t1(x86_map_idx map)
{
    x86_map_op m = {
        x86_table_lex, x86_pc_none, x86_map_none,
        { 0x00 }, { 0xff }
    };
    x86_map_op *r = x86_table_lookup(map, &m);
    if (r) x86_print_map(r);
}

void t2(x86_map_idx map)
{
    x86_map_op m = {
        x86_table_lex, x86_pc_none, x86_map_none,
        { 0x80, 0xe7 }, { 0xff, 0xff }
    };
    x86_map_op *r = x86_table_lookup(map, &m);
    if (r) x86_print_map(r);
}

void t3(x86_map_idx map)
{
    x86_map_op m = {
        x86_table_lex, x86_pc_48, x86_map_0f38,
        { 0xfc }, { 0xff }
    };
    x86_map_op *r = x86_table_lookup(map, &m);
    if (r) x86_print_map(r);
}

void t4(x86_map_idx map)
{
    x86_map_op m = {
        x86_table_lex, x86_pc_48 | x86_pc_66, x86_map_0f38,
        { 0xfc }, { 0xff }
    };
    x86_map_op *r = x86_table_lookup(map, &m);
    if (r) x86_print_map(r);
}

int main(int argc, char **argv)
{
    x86_map_idx map = x86_table_build(x86_modes_64);
    t1(map);
    t2(map);
    t3(map);
    t4(map);
    free(map.map);
    return 0;
}
