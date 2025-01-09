#include "x86.h"

void t1(x86_ctx *ctx)
{
    x86_opc_data m = {
        0, 0, 0, 0, x86_enc_t_lex | x86_enc_p_none | x86_enc_m_none,
        { { 0x00 } }, { { 0xff } }
    };
    x86_opc_data *r = x86_table_lookup(ctx->idx, &m);
    if (r) x86_print_op(r, 1, 1);
}

void t2(x86_ctx *ctx)
{
    x86_opc_data m = {
        0, 0, 0, 0, x86_enc_t_lex | x86_enc_p_none | x86_enc_m_none,
        { { 0x80, 0xe7 } }, { { 0xff, 0xff } }
    };
    x86_opc_data *r = x86_table_lookup(ctx->idx, &m);
    if (r) x86_print_op(r, 1, 1);
}

void t3(x86_ctx *ctx)
{
    x86_opc_data m = {
        0, 0, 0, 0, x86_enc_t_lex | x86_enc_p_rexw | x86_enc_m_0f38,
        { { 0xfc } }, { { 0xff } }
    };
    x86_opc_data *r = x86_table_lookup(ctx->idx, &m);
    if (r) x86_print_op(r, 1, 1);
}

void t4(x86_ctx *ctx)
{
    x86_opc_data m = {
        0, 0, 0, 0, x86_enc_t_lex | x86_enc_p_rexw | x86_enc_p_66 | x86_enc_m_0f38,
        { { 0xfc } }, { { 0xff } }
    };
    x86_opc_data *r = x86_table_lookup(ctx->idx, &m);
    if (r) x86_print_op(r, 1, 1);
}

int main(int argc, char **argv)
{
    x86_ctx *ctx = x86_ctx_create(x86_modes_64);
    t1(ctx);
    t2(ctx);
    t3(ctx);
    t4(ctx);
    x86_ctx_destroy(ctx);
    return 0;
}
