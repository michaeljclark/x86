#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#include "x86.h"

void x86_disasm(uchar *insn, size_t insnlen)
{
    x86_buffer buf;
    x86_codec codec;
    char data[32], str[128];
    size_t nbytes = 0, len = 0;

    memset(data, 0, sizeof(data));
    memset(&codec, 0, sizeof(codec));
    codec.flags |= x86_cf_amd64;

    x86_buffer_init(&buf, data);
    x86_buffer_write(&buf, insn, insnlen);

    if (x86_codec_read(&buf, &codec, &nbytes, insnlen) == 0) {
        len += x86_format_hex(str+len, sizeof(str)-len, insn, nbytes);
        len += x86_format_op(str+len, sizeof(str)-len, &codec);
    } else {
        len += x86_format_hex(str+len, sizeof(str)-len, insn, nbytes);
        len += snprintf(str+len, sizeof(str)-len, "%s", "<unknown>");
    }

    printf("%s\n", str);
}

void t1()
{
    uchar insn[] = { 0x66, 0x47, 0x0f, 0x7e, 0x7c, 0xee, 0xf8 };
    x86_disasm(insn, sizeof(insn));
}

void t2()
{
    uchar insn[] = { 0x66, 0x47, 0x0f, 0x6e, 0x7c, 0xee, 0xf8 };
    x86_disasm(insn, sizeof(insn));
}

void t3()
{
    uchar insn[] = { 0xc5, 0x79, 0x7e, 0x7c, 0xc1, 0xf8 };
    x86_disasm(insn, sizeof(insn));
}

void t4()
{
    uchar insn[] = { 0xc5, 0x79, 0x6e, 0x7c, 0xc1, 0xf8 };
    x86_disasm(insn, sizeof(insn));
}

void t5()
{
    uchar insn[] = { 0xc4, 0x01, 0x79, 0x7e, 0x7c, 0xf5, 0xf8 };
    x86_disasm(insn, sizeof(insn));
}

void t6()
{
    uchar insn[] = { 0xc4, 0x01, 0x79, 0x6e, 0x7c, 0xf5, 0xf8 };
    x86_disasm(insn, sizeof(insn));
}

void t7()
{
    uchar insn[] = { 0x62, 0x01, 0x7d, 0x08, 0x7e, 0x7c, 0xee, 0xfe };
    x86_disasm(insn, sizeof(insn));
}

void t8()
{
    uchar insn[] = { 0x62, 0x01, 0x7d, 0x08, 0x6e, 0x7c, 0xee, 0xfe };
    x86_disasm(insn, sizeof(insn));
}

int main(int argc, char **argv)
{
    t1();
    t2();
    t3();
    t4();
    t5();
    t6();
    t7();
    t8();
    return 0;
}
