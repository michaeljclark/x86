# x86

> metadata driven x86 instruction encoder and decoder library.

## Introduction

This x86 encoder and decoder library is a lightweight alternative x86
disassembler using revised instruction set metadata which encodes
legacy instructions using a parameterized LEX (legacy extension) format.

- metadata driven disassembler with LLVM style output.
- written in C99 for compatibility with projects written in pure C.
- low-level instruction encoder and decoder uses <= 32-bytes.
- python tablegen program to generate C tables from CSV metadata.
- metadata table tool to inspect operand encode and decode tables.
- support for REX/VEX/EVEX and preliminary support for REX2.
- carefully checked machine-readable metadata based on
  [x86-csv](https://github.com/GregoryComer/x86-csv).

This x86 encoder and decoder library has been written from scratch to be
modern and as simple as possible while also covering recent additions to
the Intel and AMD 64-bit instructions set architecture such as the EVEX
encodings for recent AVX-512 extensions and soon the REX2/EVEX encodings
for Intel APX.

***Note***: This library is a work-in-progress and is not complete,
although it features comprehensive metadata.

![x86](/images/x86.png)

_figure 1: x86 instruction set metadata organized by CPU extension._

---

## Build Instructions

The build depends on python3 to run `scripts/x86_tablegen.py`

```
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -- --verbose
```

### Interactive Disassembly Tool

This shows output from running disassembly commands in `scripts/demo.sh`:

```
$ ./build/x86_disasm  4c 13 3d ad de ad 1e
  4c 13 3d ad de ad 1e            adc r15, qword ptr [rip + 0x1eaddead]
$ ./build/x86_disasm  4c bf 01 02 03 04 05 06 07 08
  4c bf 01 02 03 04 05 06 07 08   movabs  r15, 0x807060504030201
$ ./build/x86_disasm  49 0f c7 0f
  49 0f c7 0f                     cmpxchg16b  xmmword ptr [r15]
$ ./build/x86_disasm  c5 7f e6 3d 40 84 c8 0c
  c5 7f e6 3d 40 84 c8 0c         vcvtpd2dq xmm15, ymmword ptr [rip + 0xcc88440]
$ ./build/x86_disasm  62 f2 7d 48 7e 15 11 22 33 44
  62 f2 7d 48 7e 15 11 22 33 44   vpermt2d  zmm2, zmm0, zmmword ptr [rip + 0xcc88440]
```

_figure 2: Command-line disassembly tool decoding instruction from hex._

### Disassembly Debug Tool

This shows output from running disassembly commands in `scripts/demo.sh` with debug enabled:

`$ ./build/x86_disasm -d 4c 13 3d ad de ad 1e`
```
x86_codec_read: table_lookup { type:1 prefix:8 map:0 opc:[13 3d] opm:[ff ff] }
x86_codec_read: checking opdata 12
| .lex  | 48    |      | 13    | ff    |   12 | adc       | .lex.wx 13 /r              |
  4c 13 3d ad de ad 1e            adc r15, qword ptr [rip + 0x1eaddead]
```

`$ ./build/x86_disasm -d 4c bf 01 02 03 04 05 06 07 08`
```
x86_codec_read: table_lookup { type:1 prefix:8 map:0 opc:[bf 01] opm:[ff ff] }
x86_codec_read: checking opdata 619
| .lex  | 48    |      | b8    | f8    |  619 | mov       | .lex.wx b8+r i16 .o16      |
x86_codec_read: checking opdata 620
| .lex  | 48    |      | b8    | f8    |  620 | mov       | .lex.wx b8+r i32 .o32      |
x86_codec_read: checking opdata 623
| .lex  | 48    |      | b8    | f8    |  623 | movabs    | .lex.wx b8+r i64 .o64      |
  4c bf 01 02 03 04 05 06 07 08   movabs  r15, 0x807060504030201
```

`$ ./build/x86_disasm -d 49 0f c7 0f`
```
x86_codec_read: table_lookup { type:1 prefix:8 map:1 opc:[c7 0f] opm:[ff ff] }
x86_codec_read: checking opdata 171
| .lex  | 48    | 0f   | c7 08 | ff f8 |  171 | cmpxchg16b | .lex.0f.w1 c7 /1 .lock    |
  49 0f c7 0f                     cmpxchg16b  xmmword ptr [r15]
```

`$ ./build/x86_disasm -d c5 7f e6 3d 40 84 c8 0c`
```
x86_codec_read: table_lookup { type:2 prefix:3 map:1 opc:[e6 3d] opm:[ff ff] }
x86_codec_read: checking opdata 1389
| .vex  | f2    | 0f   | e6    | ff    | 1389 | vcvtpd2dq | .vex.128.f2.0f.wig e6 /r   |
x86_codec_read: checking opdata 1390
| .vex  | f2    | 0f   | e6    | ff    | 1390 | vcvtpd2dq | .vex.256.f2.0f.wig e6 /r   |
  c5 7f e6 3d 40 84 c8 0c         vcvtpd2dq xmm15, ymmword ptr [rip + 0xcc88440]
```

`$ ./build/x86_disasm -d 62 f2 7d 48 7e 15 11 22 33 44`
```
x86_codec_read: table_lookup { type:3 prefix:1 map:2 opc:[7e 15] opm:[ff ff] }
x86_codec_read: checking opdata 2698
| .evex | 66    | 0f38 | 7e    | ff    | 2698 | vpermt2d  | .evex.128.66.0f38.w0 7e /r |
x86_codec_read: checking opdata 2699
| .evex | 66    | 0f38 | 7e    | ff    | 2699 | vpermt2d  | .evex.256.66.0f38.w0 7e /r |
x86_codec_read: checking opdata 2700
| .evex | 66    | 0f38 | 7e    | ff    | 2700 | vpermt2d  | .evex.512.66.0f38.w0 7e /r |
  62 f2 7d 48 7e 15 11 22 33 44   vpermt2d  zmm2, zmm0, zmmword ptr [rip + 0xcc88440]
```

_figure 3: Command-line disassembly tool run in debug mode._

### Metadata Table Tool

The Metadata Table tool `x86_opcodes` allows one to search the instruction
set metadata tables for varous classes of instructions. It is a useful way
to inspect the x86 metadata. Here are some example invocations:

- `$ ./build/x86_opcodes -n | grep rep` - search string instructions.
- `$ ./build/x86_opcodes -n | grep lock` - search atomic instructions.
- `$ ./build/x86_opcodes -n | grep rsp` - search stack instructions.
- `$ ./build/x86_opcodes -n | grep o64` - search 64-bit overrides.
- `$ ./build/x86_opcodes -n | grep lex` - search legacy and SSE instructions.
- `$ ./build/x86_opcodes -n | grep vex` - search VEX and EVEX instructions.

This shows sample output from the `x86_opcodes` tool being used to search
for 64-bit operand size encoding exceptions in metadata. These are special
cases where the operand size is not automatically derived from word size
synthesis rules described in [*Instruction Synthesis Notes*](#user-content-instruction-synthesis-notes):

`$ ./build/x86_opcodes -n | grep o64`

| operands             | encoding                 | order              | modes  |
|:---------------------|:-------------------------|:-------------------|:-------|
| cdqe `aw`            | `.lex.wx 98 .o64`        | `rax/rw`           | `64`   |
| cqo `dw,aw`          | `.lex.wx 99 .o64`        | `rdx/w,rax/rw`     | `64`   |
| movsq `pdi,psi`      | `.lex.wx a5 .o64 .rep`   | `rdi/r,rsi/r`      | `64`   |
| cmpsq `pdi,psi`      | `.lex.wx a7 .o64 .rep`   | `rdi/r,rsi/r`      | `64`   |
| stosq `pdi,aw`       | `.lex.wx ab .o64 .rep`   | `rdi/r,rax/r`      | `64`   |
| lodsq `aw,psi`       | `.lex.wx ad .o64 .rep`   | `rax/w,rsi/r`      | `64`   |
| scasq `pdi,aw`       | `.lex.wx af .o64 .rep`   | `rdi/r,rax/r`      | `64`   |
| iretq                | `.lex.wx cf .o64`        |                    | `64`   |
| pushfq               | `.lex.ww 9c .o64`        | `rflags/ri,rsp/rwi`| `64`   |
| popfq                | `.lex.ww 9d .o64`        | `rflags/wi,rsp/rwi`| `64`   |
| callf `memfar16:64`  | `.lex.ww ff /3 .o64`     | `mrm/r,rsp/rwi`    | `64`   |
| jmpf `memfar16:64`   | `.lex.ww ff /5 .o64`     | `mrm/r`            | `64`   |
| lss `r64,memfar16:64`| `.lex.0f.wx b2 /r .o64`  | `reg/w,mrm/r`      | `64`   |
| lfs `r64,memfar16:64`| `.lex.0f.wx b4 /r .o64`  | `reg/w,mrm/r`      | `64`   |
| lgs `r64,memfar16:64`| `.lex.0f.wx b5 /r .o64`  | `reg/w,mrm/r`      | `64`   |
| movdiri `mw,rw`      | `.lex.0f38.w1 f9 /r .o64`| `mrm/w,reg/r`      | `64/32`|

_figure 4: x86 metadata table search showing 64-bit overrides._

### Fancy Instruction Formatting Tool

The table generation tool `x86_tablegen.py` supports a mode to output the
instruction set metadata as a Markdown document organised by  sections
from the instruction descriptions in [doc/x86_desc.md](doc/x86_desc.md).

`./scripts/x86_tablegen.py --print-fancy-insn`

| opcode          | encoding                | order           | modes      |
|:----------------|:------------------------|:----------------|:-----------|
| xor `r8/m8,r8`  | `lex.wb 30 /r lock`     | `mrm/rw,reg/r`  | `64/32/16` |
| xor `rw/mw,rw`  | `lex.wx 31 /r lock`     | `mrm/rw,reg/r`  | `64/32/16` |
| xor `r8,r8/m8`  | `lex.wb 32 /r`          | `reg/rw,mrm/r`  | `64/32/16` |
| xor `rw,rw/mw`  | `lex.wx 33 /r`          | `reg/rw,mrm/r`  | `64/32/16` |
| xor `al,ib`     | `lex.wn 34 ib`          | `rax/rw,imm`    | `64/32/16` |
| xor `aw,iw`     | `lex.wx 35 iw`          | `rax/rw,imm`    | `64/32/16` |
| xor `r8/m8,ib`  | `lex.wb 80 /6 ib lock`  | `mrm/rw,imm`    | `64/32/16` |
| xor `rw/mw,iw`  | `lex.wx 81 /6 iw lock`  | `mrm/rw,imm`    | `64/32/16` |
| xor `rw/mw,ib`  | `lex.wx 83 /6 ib lock`  | `mrm/rw,imm`    | `64/32/16` |

_figure 5: Markdown x86 instruction encodings organised by section._

The fancy table format is a work-progress as it is not yet complete as it
does not yet include full descriptions or tuple type, although it makes it
easier to check instruction encodings, in addition to sorting numerically by
opcode, or alphabetically by instruction name using the metadata table tool.

---

## x86 Instruction Set Metadata

Legacy x86 instructions have been parameterized in the instruction set
metadata using a new LEX prefix for instruction encoding with abstract width
suffix codes that synthesize multiple instruction widths using combinations
of operand size prefixes and `REX.W` bits. This new LEX format makes legacy
instruction encodings consistent with VEX and EVEX encodings as well as
eliminating some redundancy in the metadata.

There are a small number of special cases for legacy instructions which need
mode-dependent overrides for cases such as, a different opcode is used for
different modes, or the instruction has a quirk where the operand size does
not follow the default rules for instruction word and address sizes:

- `.wx` is used to specify 64-bit instructions that default to 32-bit
  operands in 64-bit mode.
- `.ww` is used to specify 64-bit instructions that default to 64-bit
  operands in 64-bit mode.
- `.o16` is used to specify an instruction override specific to 16-bit mode.
- `.o32` is used to specify an instruction override specific to 32-bit mode.
- `.o32` is used to specify an instruction override specific to 64-bit mode.

### CSV File Format

The instruction set metadata in the `data` directory has the following fields
which map to instruction encoding tables in the Intel Architecture Software
Developer's Manual:

- ___Instruction___: opcode and operands from Opcode/Instruction column.
- ___Opcode___: instruction encoding from Opcode/Instruction column.
- ___Valid 64-bit___: 64-bit valid field from 64/32 bit Mode Support column.
- ___Valid 32-bit___: 32-bit valid field from 64/32 bit Mode Support column.
- ___Valid 16-bit___: 16-bit valid field from Compat/Legacy Mode column.
- ___Feature Flags___: extension name from CPUID Feature Flag column.
- ___Operand 1___: Operand 1 column from Instruction Operand Encoding table.
- ___Operand 2___: Operand 2 column from Instruction Operand Encoding table.
- ___Operand 3___: Operand 3 column from Instruction Operand Encoding table.
- ___Operand 4___: Operand 4 column from Instruction Operand Encoding table.
- ___Tuple Type___: Tuple Type column from Instruction Operand Encoding table.

The instruction set metadata in the `data` directory is derived from
[x86-csv](https://github.com/GregoryComer/x86-csv), although it has had
extensive modifications to fix transcription errors, to revise legacy
instruction encodings to conform to the new LEX format, as well as add
missing details such as missing operands or recently added AVX-512
instruction encodings and various other instruction set extensions.

### Table Generation

The appendices outline the printable form of the mnemonics used in the
generated tables to describe operands, instruction encodings and field order.
The mnemonics are referenced in the instruction set metadata files which are
translated to enums and arrays by `scripts/x86_tablegen.py` which then map to
the enum type and set definitions in `include/x86.h`:

- ___enum x86_opr___ - operand encoding enum type and set attributes.
- ___enum x86_enc___ - instruction encoding enum type and set attributes.
- ___enum x86_ord___ - operand to instruction encoding field map set attributes.

The enum values are combined together with _logical or_ combinations to
form the primary metadata tables used by the encoder and decoder library:

- ___struct x86_opc_data___ - table type for instruction opcode encodings.
- ___struct x86_opr_data___ - table type for unique sets of instruction operands.
- ___struct x86_ord_data___ - table type for unique sets of instruction field orders.

***Note***: There are some differences between the mnemonics used in the
CSV metadata and the C enums. Exceptions are described in `operand_map` and
`opcode_map` within `scripts/x86_tablegen.py`. The primary differences are
in the names used in the operand columns to indicate operand field order,
otherwise a type prefix is added, dots and brackets are omitted, and forward
slashes are translated to underscores.

---

## Appendices

This section describes the mnemonics used in the primary data structures:

- ___Appendix A - Operand Encoding___ - describes instruction operands.
- ___Appendix B - Instruction Encoding___ - describes instruction opcodes.
- ___Appendix C - Operand Order___ - describes instruction field encoding.

### Appendix A - Operand Encoding

This table outlines the operand mnemonics used in instruction operands
_(enum x86_opr)_.

| operand            | description                                           |
|:-------------------|:------------------------------------------------------|
| `r`                | integer register                                      |
| `v`                | vector register                                       |
| `k`                | mask register                                         |
| `seg`              | segment register                                      |
| `creg`             | control register                                      |
| `dreg`             | debug register                                        |
| `bnd`              | bound register                                        |
| `mem`              | memory reference                                      |
| `rw`               | integer register word (16/32/64 bit)                  |
| `mw`               | memory reference word (16/32/64 bit)                  |
| `mm`               | vector register 64-bit                                |
| `xmm`              | vector register 128-bit                               |
| `ymm`              | vector register 256-bit                               |
| `zmm`              | vector register 512-bit                               |
| `r8`               | register 8-bit                                        |
| `r16`              | register 16-bit                                       |
| `r32`              | register 32-bit                                       |
| `r64`              | register 64-bit                                       |
| `m8`               | memory reference 8-bit byte                           |
| `m16`              | memory reference 16-bit word                          |
| `m32`              | memory reference 32-bit dword                         |
| `m64`              | memory reference 64-bit qword                         |
| `m128`             | memory reference 128-bit oword/xmmword                |
| `m256`             | memory reference 256-bit ymmword                      |
| `m512`             | memory reference 512-bit zmmword                      |
| `m80`              | memory reference 80-bit tword/tbyte                   |
| `m384`             | memory reference 384-bit key locker handle            |
| `mib`              | memory reference bound                                |
| `m16bcst`          | memory reference 16-bit word broadcast                |
| `m32bcst`          | memory reference 32-bit word broadcast                |
| `m64bcst`          | memory reference 64-bit word broadcast                |
| `vm32`             | vector memory 32-bit                                  |
| `vm64`             | vector memory 64-bit                                  |
| `{er}`             | operand suffix - embedded rounding control            |
| `{k}`              | operand suffix - apply mask register                  |
| `{sae}`            | operand suffix - suppress all execptions              |
| `{z}`              | operand suffix - zero instead of merge                |
| `{rs2}`            | operand suffix - register stride 2                    |
| `{rs4}`            | operand suffix - register stride 4                    |
| `r/m8`             | register unsized memory 8-bit                         |
| `r/m16`            | register unsized memory 16-bit                        |
| `r/m32`            | register unsized memory 32-bit                        |
| `r/m64`            | register unsized memory 64-bit                        |
| `k/m8`             | mask register memory 8-bit                            |
| `k/m16`            | mask register memory 16-bit                           |
| `k/m32`            | mask register memory 32-bit                           |
| `k/m64`            | mask register memory 64-bit                           |
| `bnd/m64`          | bound register memory 64-bit                          |
| `bnd/m128`         | bound register memory 128-bit                         |
| `rw/mw`            | register or memory 16/32/64-bit (word size)           |
| `r8/m8`            | 8-bit register 8-bit memory                           |
| `r?/m?`            | N-bit register N-bit memory                           |
| `mm/m?`            | 64-bit vector N-bit memory                            |
| `xmm/m?`           | 128-bit vector N-bit memory                           |
| `ymm/m?`           | 256-bit vector N-bit memory                           |
| `zmm/m?`           | 512-bit vector N-bit memory                           |
| `xmm/m?/m?bcst`    | 128-bit vector N-bit memory N-bit broadcast           |
| `ymm/m?/m?bcst`    | 256-bit vector N-bit memory N-bit broadcast           |
| `zmm/m?/m?bcst`    | 512-bit vector N-bit memory N-bit broadcast           |
| `vm32x`            | 32-bit vector memory in xmm                           |
| `vm32y`            | 32-bit vector memory in ymm                           |
| `vm32z`            | 32-bit vector memory in zmm                           |
| `vm64x`            | 64-bit vector memory in xmm                           |
| `vm64y`            | 64-bit vector memory in ymm                           |
| `vm64z`            | 64-bit vector memory in zmm                           |
| `st0`              | implicit register st0                                 |
| `st1`              | implicit register st1                                 |
| `es`               | implicit segment es                                   |
| `cs`               | implicit segment cs                                   |
| `ss`               | implicit segment ss                                   |
| `ds`               | implicit segment ds                                   |
| `fs`               | implicit segment fs                                   |
| `gs`               | implicit segment gs                                   |
| `aw`               | implicit register (ax/eax/rax)                        |
| `cw`               | implicit register (cx/ecx/rcx)                        |
| `dw`               | implicit register (dx/edx/rdx)                        |
| `bw`               | implicit register (bx/ebx/rbx)                        |
| `pa`               | implicit indirect register (ax/eax/rax)               |
| `pc`               | implicit indirect register (cx/ecx/rcx)               |
| `pd`               | implicit indirect register (dx/edx/rdx)               |
| `pb`               | implicit indirect register (bx/ebx/rbx)               |
| `psi`              | implicit indirect register (si/esi/rsi)               |
| `pdi`              | implicit indirect register (di/edi/rdi)               |
| `xmm0`             | implicit register xmm0                                |
| `xmm0_7`           | implicit registers xmm0-xmm7                          |
| `1`                | constant 1                                            |
| `ib`               | 8-bit immediate                                       |
| `iw`               | 16-bit or 32-bit immediate (mode + operand size)      |
| `id`               | 32-bit immediate                                      |
| `iq`               | 64-bit immediate                                      |
| `rel8`             | 8-bit displacement                                    |
| `relw`             | 6-bit or 32-bit displacement (mode + operand size)    |
| `moffs`            | indirect memory offset                                |
| `far16/16`         | 16-bit seg 16-bit far displacement                    |
| `far16/32`         | 16-bit seg 32-bit far displacement                    |
| `memfar16/16`      | indirect 16-bit seg 16-bit far displacement           |
| `memfar16/32`      | indirect 16-bit seg 32-bit far displacement           |
| `memfar16/64`      | indirect 16-bit seg 64-bit far displacement           |

### Appendix B - Instruction Encoding

This table outlines the mnemonics used in instruction encodings
_(enum x86_enc)_.

| mnemonic | description                                                     |
|:---------|:----------------------------------------------------------------|
| `lex`    | legacy instruction                                              |
| `vex`    | VEX encoded instruction                                         |
| `evex`   | EVEX encoded instruction                                        |
| `.lz`    | VEX encoding L=0 and L=1 is unassigned                          |
| `.l0`    | VEX encoding L=0                                                |
| `.l1`    | VEX encoding L=1                                                |
| `.lig`   | VEX/EVEX encoding ignores length L=any                          |
| `.128`   | VEX/EVEX encoding uses 128-bit vector L=0                       |
| `.256`   | VEX/EVEX encoding uses 256-bit vector L=1                       |
| `.512`   | EVEX encoding uses 512-bit vector L=2                           |
| `.66`    | prefix byte 66 is used for opcode mapping                       |
| `.f2`    | prefix byte f2 is used for opcode mapping                       |
| `.f3`    | prefix byte f3 is used for opcode mapping                       |
| `.9b`    | prefix byte 9b is used for opcode mapping (x87 only)            |
| `.0f`    | map 0f is used in opcode                                        |
| `.0f38`  | map 0f38 is used in opcode                                      |
| `.0f3a`  | map 0f3a is used in opcode                                      |
| `.wn`    | no register extension, fixed operand size                       |
| `.wb`    | register extension, fixed operand size                          |
| `.wx`    | REX and/or operand size extension, optional 66 or REX.W0/W1     |
| `.ww`    | REX and/or operand size extension, optional 66 and REX.WIG      |
| `.w0`    | LEX/VEX/EVEX optional REX W0 with operand size used in opcode   |
| `.w1`    | LEX/VEX/EVEX mandatory REX W1 with operand size used in opcode  |
| `.wig`   | VEX/EVEX encoding width ignored                                 |
| `/r`     | ModRM byte                                                      |
| `/0../9` | ModRM byte with 'r' field used for functions 0 to 7             |
| `XX+r`   | opcode byte with 3-bit register added to the opcode             |
| `XX`     | opcode byte                                                     |
| `ib`     | 8-bit immediate                                                 |
| `iw`     | 16-bit or 32-bit immediate (real mode XOR operand size)         |
| `i16`    | 16-bit immediate                                                |
| `i32`    | 32-bit immediate                                                |
| `i64`    | 64-bit immediate                                                |
| `o16`    | encoding uses prefix 66 in 32-bit and 64-bit modes              |
| `o32`    | encoding uses prefix 66 in 16-bit mode                          |
| `o64`    | encoding is used exclusively in 64-bit mode with REX.W=1        |
| `a16`    | encoding uses prefix 67 in 32-bit and 64-bit modes              |
| `a32`    | encoding uses prefix 67 in 16-bit mode                          |
| `a64`    | encoding is used exclusively in 64-bit mode                     |
| `lock`   | memory operand encodings can be used with the LOCK prefix       |

#### Instruction Synthesis Notes

The `.wx` and `.ww` mnemonics are used to synthesize prefix combinations:

- `.wx` labels opcodes with _default 32-bit operand size in 64-bit mode_
  to synthesize 16/32/64-bit versions using REX and operand size prefix,
  or in 16/32-bit modes synthesizes 16/32-bit versions using only the
  operand size prefix. REX is used for register extension on opcodes
  with `rw` or `rw/mw` operands or fixed register operands like `aw`.
- `.ww` labels opcodes with _default 64-bit operand size in 64-bit mode_
  to synthesize 16/64-bit versions using only the operand size prefix,
  or in 16/32-bit modes. synthesizes 16/32-bit versions using only the
  operand size prefix. REX is used for register extension on opcodes
  with `rw` or `rw/mw` operands or fixed register operands like `aw`.

### Appendix C - Operand Order

This table outlines the mnemonics used to map operand field order
_(enum x86_ord)_.

| mnemonic | description                                                     |
|:---------|:----------------------------------------------------------------|
| `imm`    | ib, iw, i16, i32, i64                                           |
| `reg`    | modrm.reg                                                       |
| `mrm`    | modrm.r/m                                                       |
| `sib`    | modrm.r/m sib                                                   |
| `is4`    | register from ib                                                |
| `ime`    | i16e, far16 (special case for CALLF/JMPF/ENTER)                 |
| `vec`    | VEX.vvvv                                                        |
| `opr`    | opcode +r                                                       |
| `one`    | constant 1                                                      |
| `rax`    | constant al/ax/eax/rax                                          |
| `rcx`    | constant cl/cx/ecx/rcx                                          |
| `rdx`    | constant dl/dx/edx/rdx                                          |
| `rbx`    | constant bl/bx/ebx/rbx                                          |
| `rsp`    | constant sp/esp/rsp                                             |
| `rbp`    | constant bp/ebp/rbp                                             |
| `rsi`    | constant si/esi/rsi                                             |
| `rdi`    | constant di/edi/rdi                                             |
| `st0`    | constant st(0)                                                  |
| `stx`    | constant st(i)                                                  |
| `seg`    | constant segment                                                |
| `xmm0`   | constant xmm0                                                   |
| `xmm0_7` | constant xmm0-xmm7                                              |
| `mxcsr`  | constant mxcsr                                                  |
| `rflags` | constant rflags                                                 |
