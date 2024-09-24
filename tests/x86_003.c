#undef NDEBUG
#include <assert.h>

#include "x86.h"

void t1()
{
    assert(x86_enc_filter_vex2(x86_enc_vex2(
        x86_pfx_66, x86_vex_l0, x86_vex_w0, 0
    ), x86_enc_t_vex|x86_enc_l_l0|x86_enc_p_66|x86_enc_m_0f|x86_enc_w_w0) == 0);

    assert(x86_enc_filter_vex3(x86_enc_vex3(
        x86_map_0f, x86_pfx_66, x86_vex_l128, x86_vex_w0, 0, 0, 0, 0
    ), x86_enc_t_vex|x86_enc_l_l0|x86_enc_p_66|x86_enc_m_0f|x86_enc_w_w0) == 0);

    assert(x86_enc_filter_evex(x86_enc_evex(
        x86_map_0f, x86_pfx_66, x86_vex_l128, x86_vex_w0, 0, 0, 0, 0, 0, 0, 0
    ), x86_enc_t_evex|x86_enc_l_128|x86_enc_p_66|x86_enc_m_0f|x86_enc_w_w0) == 0);
}

int main(int argc, char const *argv[])
{
    t1();
    return 0;
}
