# x86 instructions

## Base

### General
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| AND                | Logical AND                                                                      |
| OR                 | Logical Inclusive OR                                                             |
| XOR                | Logical Exclusive OR                                                             |
| ADD                | Add                                                                              |
| ADC                | Add with Carry                                                                   |
| SUB                | Subtract                                                                         |
| SBB                | Subtract with Borrow                                                             |
| NEG                | Two's Complement Negation                                                        |
| INC                | Increment by 1                                                                   |
| DEC                | Decrement by 1                                                                   |
| NOT                | One's Complement Negation                                                        |
| MUL                | Unsigned Multiply                                                                |
| DIV                | Unsigned Divide                                                                  |
| IDIV               | Signed Divide                                                                    |
| IMUL               | Signed Multiply                                                                  |
| BT                 | Bit Test                                                                         |
| BTC                | Bit Test and Complement                                                          |
| BTR                | Bit Test and Reset                                                               |
| BTS                | Bit Test and Set                                                                 |
| CMP                | Compare Two Operands                                                             |
| TEST               | Logical Compare                                                                  |
| SETcc              | Set Byte on Condition                                                            |
| CBW                | Convert Byte to Word                                                             |
| CWD                | Convert Word to Doubleword                                                       |
| CWDE               | Convert Word to Doubleword                                                       |
| CDQ                | Convert Doubleword to Quadword                                                   |
| CDQE               | Convert Doubleword to Quadword                                                   |
| CQO                | Convert Quadword to Octword                                                      |
| CMOVcc             | Conditional Move                                                                 |
| LEA                | Load Effective Address                                                           |
| MOV                | Move                                                                             |
| MOVSXD             | Move with Sign-Extension Doubleword to Quadword                                  |
| MOVSX              | Move with Sign-Extension                                                         |
| MOVZX              | Move with Zero-Extension                                                         |
| NOP                | No Operation                                                                     |
| SAR                | Shift Arithmetic Right                                                           |
| SHLD               | Shift Left Double                                                                |
| SHL                | Shift Logical Left                                                               |
| SHRD               | Shift Right Double                                                               |
| SHR                | Shift Logical Right                                                              |
| RCL                | Rotate Left Through Carry                                                        |
| RCR                | Rotate Right Through Carry                                                       |
| ROL                | Rotate Left                                                                      |
| ROR                | Rotate Right                                                                     |

### Extended
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| ANDN               | Logical AND NOT                                                                  |
| ADCX               | Unsigned Integer Addition of Two Operands with Carry Flag                        |
| ADOX               | Unsigned Integer Addition of Two Operands with Overflow Flag                     |
| MULX               | Unsigned Multiply Without Affecting Flags                                        |
| SARX               | Shift Arithmetic Right Without Affecting Flags                                   |
| SHLX               | Shift Logical Left Without Affecting Flags                                       |
| SHRX               | Shift Logical Right Without Affecting Flags                                      |
| RORX               | Rotate Right Logical Without Affecting Flags                                     |

### Control
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CALL               | Call Procedure                                                                   |
| RET                | Return from Procedure                                                            |
| Jcc                | Jump if Condition Is Met                                                         |
| JCXZ               | Jump register CX zero                                                            |
| JECXZ              | Jump register ECX zero                                                           |
| JRCXZ              | Jump register RCX zero                                                           |
| JMP                | Jump                                                                             |
| ENTER              | Make Stack Frame for Procedure Parameters                                        |
| LEAVE              | High Level Procedure Exit                                                        |
| INT                | Call to Interrupt Procedure                                                      |
| INT1               | Call to Interrupt Procedure Debug                                                |
| INT3               | Call to Interrupt Procedure Breakpoint                                           |
| LOOPE              | Loop According to ECX Counter If Equal                                           |
| LOOP               | Loop According to ECX Counter                                                    |
| LOOPNE             | Loop According to ECX Counter If Not Equal                                       |
| LOOPNZ             | Loop According to ECX Counter If Not Zero                                        |
| LOOPZ              | Loop According to ECX Counter If Zero                                            |
| UD1                | Undefined instruction 2                                                          |
| UD2                | Undefined instruction 3                                                          |

### Stack
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| PUSHA              | Push All General-Purpose Registers                                               |
| PUSHAW             | Push All General-Purpose Registers Int16                                         |
| PUSHAD             | Push All General-Purpose Registers Int32                                         |
| PUSHF              | Push EFLAGS Register onto the Stack                                              |
| PUSHFW             | Push EFLAGS Register onto the Stack Int16                                        |
| PUSHFD             | Push EFLAGS Register onto the Stack Int32                                        |
| PUSHFQ             | Push EFLAGS Register onto the Stack Int64                                        |
| PUSH               | Push Integer Onto the Stack                                                      |
| POPA               | Pop All General-Purpose Registers                                                |
| POPAW              | Pop All General-Purpose Registers Int16                                          |
| POPAD              | Pop All General-Purpose Registers Int32                                          |
| POP                | Pop a Value from the Stack                                                       |
| POPF               | Pop Stack into EFLAGS Register                                                   |
| POPFW              | Pop Stack into EFLAGS Register Int16                                             |
| POPFD              | Pop Stack into EFLAGS Register Int32                                             |
| POPFQ              | Pop Stack into EFLAGS Register Int64                                             |

### String
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CMPSB              | Compare String Operands Int8                                                     |
| CMPSW              | Compare String Operands Int16                                                    |
| CMPSD              | Compare String Operands Int32                                                    |
| CMPSQ              | Compare String Operands Int64                                                    |
| LODSB              | Load String Int8                                                                 |
| LODSW              | Load String Int16                                                                |
| LODSD              | Load String Int32                                                                |
| LODSQ              | Load String Int64                                                                |
| MOVSB              | Move Data from String to String Int8                                             |
| MOVSW              | Move Data from String to String Int16                                            |
| MOVSD              | Move Data from String to String Int32                                            |
| MOVSQ              | Move Data from String to String Int64                                            |
| SCASB              | Scan String Int8                                                                 |
| SCASW              | Scan String Int16                                                                |
| SCASD              | Scan String Int32                                                                |
| SCASQ              | Scan String Int64                                                                |
| STOSB              | Store String Int8                                                                |
| STOSW              | Store String Int16                                                               |
| STOSD              | Store String Int32                                                               |
| STOSQ              | Store String Int64                                                               |

### Memory
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| AADD               | Atomically Add                                                                   |
| AAND               | Atomically AND                                                                   |
| AOR                | Atomically OR                                                                    |
| AXOR               | Atomically XOR                                                                   |
| XADD               | Exchange and Add                                                                 |
| XCHG               | Exchange Register/Memory with Register                                           |
| CMPXCHG16B         | Compare and Exchange Sixteen Bytes                                               |
| CMPXCHG8B          | Compare and Exchange Eight Bytes                                                 |
| CMPXCHG            | Compare and Exchange                                                             |
| CMPccXADD          | Compare, Exchange and Add                                                        |
| LFENCE             | Load Fence                                                                       |
| SFENCE             | Store Fence                                                                      |
| MFENCE             | Memory Fence                                                                     |
| PAUSE              | Spin Loop Hint                                                                   |
| SERIALIZE          | Serialize Instruction Execution                                                  |
| MOVDIRI            | Move Integer as Direct Store                                                     |
| MOVDIR64B          | Move 64 Bytes as Direct Store                                                    |
| MOVNTI             | Store Int32 Using Non-Temporal Hint                                              |
| PREFETCHW          | Prefetch Data into Caches in Anticipation of a Write                             |
| PREFETCHWT1        | Prefetch Data into Caches in Anticipation of a Write With Hint T1                |
| PREFETCHT0         | Prefetch Temporal to All Cache Levels                                            |
| PREFETCHT1         | Prefetch Temporal to First Level Cache                                           |
| PREFETCHT2         | Prefetch Temporal to Second Level Cache                                          |
| PREFETCHNTA        | Prefetch Non-Temporal to All Cache Levels                                        |

### Flags
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CLC                | Clear Carry Flag                                                                 |
| CLD                | Clear Direction Flag                                                             |
| CMC                | Complement Carry Flag                                                            |
| STC                | Set Carry Flag                                                                   |
| STD                | Set Direction Flag                                                               |
| LAHF               | Load Flags into AH                                                               |
| SAHF               | Store AH into Flags                                                              |

### Bitmanip
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| BEXTR              | Bit Field Extract                                                                |
| BLSI               | Extract Lowest Set Isolated Bit                                                  |
| BLSMSK             | Get Mask Up to Lowest Set Bit                                                    |
| BLSR               | Reset Lowest Set Bit                                                             |
| BSF                | Bit Scan Forward                                                                 |
| BSR                | Bit Scan Reverse                                                                 |
| BSWAP              | Byte Swap                                                                        |
| BZHI               | Zero High Bits Starting with Specified Bit Position                              |
| CRC32              | Accumulate CRC32 Values                                                          |
| MOVBE              | Move Data After Swapping Bytes                                                   |
| LZCNT              | Count Leading Zero Bits                                                          |
| TZCNT              | Count Trailing Zero Bits                                                         |
| PDEP               | Parallel Bits Deposit                                                            |
| PEXT               | Parallel Bits Extract                                                            |
| POPCNT             | Count of Number of Bits Set to 1                                                 |

## AVX

### AVX State
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| EMMS               | Empty MMX Technology State                                                       |
| vLDMXCSR           | Load MXCSR Register State                                                        |
| vSTMXCSR           | Store MXCSR Register State                                                       |
| VZEROALL           | Zero XMM, YMM and ZMM Registers                                                  |
| VZEROUPPER         | Zero Upper Bits of YMM and ZMM Registers                                         |
| XGETBV             | Get Value of Extended Control Register                                           |
| XRSTOR             | Restore Processor Extended States                                                |
| XSAVEC             | Save Processor Extended States with Compaction                                   |
| XSAVEOPT           | Save Processor Extended States Optimized                                         |
| XSAVE              | Save Processor Extended States                                                   |

### AVX Scalar Int Move
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vLDDQU             | Load Unaligned Int128 Value                                                      |
| VMOVW              | Move Int16 Value between Register/Memory and Vector                              |
| vMOVD              | Move Int32 Value between Register/Memory and Vector                              |
| vMOVQ              | Move Int64 Value between Register/Memory and Vector                              |
| MOVDQ2Q            | Move Int64 Value from XMM to MMX Register                                        |
| MOVQ2DQ            | Move Int64 Value from MMX to XMM Register                                        |
| MOVNTQ             | Store Int64 Using Non-Temporal Hint                                              |

### AVX Scalar FP Move
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vMOVSD             | Move or Merge Scalar Float64 Values                                              |
| vMOVSS             | Move or Merge Scalar Float32 Values                                              |
| VMOVSH             | Move or Merge Scalar Float16 Values                                              |

### AVX Scalar FP Math
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vCMPSD             | Compare Scalar Float64 Values                                                    |
| vCMPSS             | Compare Scalar Float32 Values                                                    |
| VCMPSH             | Compare Scalar Float16 Values                                                    |
| vMINSD             | Minimum Scalar Float64 Values                                                    |
| vMINSS             | Minimum Scalar Float32 Values                                                    |
| VMINSH             | Minimum Scalar Float16 Values                                                    |
| vMAXSD             | Maximum Scalar Float64 Values                                                    |
| vMAXSS             | Maximum Scalar Float32 Values                                                    |
| VMAXSH             | Maximum Scalar Float16 Values                                                    |
| vADDSD             | Add Scalar Float64 Values                                                        |
| vADDSS             | Add Scalar Float32 Values                                                        |
| VADDSH             | Add Scalar Float16 Values                                                        |
| vSUBSD             | Subtract Scalar Float64 Values                                                   |
| vSUBSS             | Subtract Scalar Float32 Values                                                   |
| VSUBSH             | Subtract Scalar Float16 Values                                                   |
| vMULSD             | Multiply Scalar Float64 Values                                                   |
| vMULSS             | Multiply Scalar Float32 Values                                                   |
| VMULSH             | Multiply Scalar Float16 Values                                                   |
| vDIVSD             | Divide Scalar Float64 Values                                                     |
| vDIVSS             | Divide Scalar Float32 Values                                                     |
| VDIVSH             | Divide Scalar Float16 Values                                                     |
| VFIXUPIMMSD        | Fix Up Special Scalar Float64 Values                                             |
| VFIXUPIMMSS        | Fix Up Special Scalar Float32 Values                                             |
| VFPCLASSSD         | Tests Types Of a Scalar Float64 Values                                           |
| VFPCLASSSS         | Tests Types Of a Scalar Float32 Values                                           |
| VFPCLASSSH         | Tests Types Of a Scalar Float16 Values                                           |
| VGETEXPSD          | Convert Exponents of Scalar Float64 Values to Float64 Values                     |
| VGETEXPSS          | Convert Exponents of Scalar Float32 Values to Float32 Values                     |
| VGETEXPSH          | Convert Exponents of Scalar Float16 Values to Float32 Values                     |
| VGETMANTSD         | Extract Float64 Scalar of Normalized Mantissas from Float64 Scalar               |
| VGETMANTSS         | Extract Float32 Scalar of Normalized Mantissas from Float32 Scalar               |
| VGETMANTSH         | Extract Float32 Scalar of Normalized Mantissas from Float16 Scalar               |
| VRANGESD           | Range Restriction Calculation From a pair of Scalar Float64 Values               |
| VRANGESS           | Range Restriction Calculation From a Pair of Scalar Float32 Values               |
| VRCP14SD           | Compute Approximate Reciprocal of Scalar Float64 Values                          |
| VRCP14SS           | Compute Approximate Reciprocal of Scalar Float32 Values                          |
| vRCPSS             | Compute Reciprocal of Scalar Float32 Values                                      |
| VRCPSH             | Compute Reciprocal of Scalar Float16 Values                                      |
| VREDUCESD          | Perform a Reduction Transformation on a Scalar Float64 Values                    |
| VREDUCESS          | Perform a Reduction Transformation on a Scalar Float32 Values                    |
| VREDUCESH          | Perform a Reduction Transformation on a Scalar Float16 Values                    |
| VRNDSCALESD        | Round Scalar Float64 Value To Include A Given Number Of Fraction Bits            |
| VRNDSCALESS        | Round Scalar Float32 Value To Include A Given Number Of Fraction Bits            |
| VRNDSCALESH        | Round Scalar Float16 Value To Include A Given Number Of Fraction Bits            |
| VRSQRT14SD         | Compute Approximate Reciprocal of Square Root of Scalar Float64 Values           |
| VRSQRT14SS         | Compute Approximate Reciprocal of Square Root of Scalar Float32 Values           |
| vRSQRTSS           | Compute Reciprocal of Square Root of Scalar Float32 Values                       |
| VRSQRTSH           | Compute Reciprocal of Square Root of Scalar Float16 Values                       |
| vROUNDSD           | Round Scalar Float64 Values                                                      |
| vROUNDSS           | Round Scalar Float32 Values                                                      |
| VSCALEFSD          | Scale Scalar Float64 Values With Float64 Values                                  |
| VSCALEFSS          | Scale Scalar Float32 Values With Float32 Values                                  |
| VSCALEFSH          | Scale Scalar Float16 Values With Float16 Values                                  |
| vSQRTSD            | Square Root of Scalar Float64 Values                                             |
| vSQRTSS            | Square Root of Scalar Float32 Values                                             |
| VSQRTSH            | Square Root of Scalar Float16 Values                                             |

### AVX Scalar FP Fused Multiply Add
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VFMADD132SD        | Fused Multiply-Add of Scalar Float64 Values (r1=(r1*r3)+r2)                      |
| VFMADD132SS        | Fused Multiply-Add of Scalar Float32 Values (r1=(r1*r3)+r2)                      |
| VFMADD132SH        | Fused Multiply-Add of Scalar Float16 Values (r1=(r1*r3)+r2)                      |
| VFMADD213SD        | Fused Multiply-Add of Scalar Float64 Values (r1=(r2*r1)+r3)                      |
| VFMADD213SS        | Fused Multiply-Add of Scalar Float32 Values (r1=(r2*r1)+r3)                      |
| VFMADD213SH        | Fused Multiply-Add of Scalar Float16 Values (r1=(r2*r1)+r3)                      |
| VFMADD231SD        | Fused Multiply-Add of Scalar Float64 Values (r1-(r2*r3)+r1)                      |
| VFMADD231SS        | Fused Multiply-Add of Scalar Float32 Values (r1-(r2*r3)+r1)                      |
| VFMADD231SH        | Fused Multiply-Add of Scalar Float16 Values (r1-(r2*r3)+r1)                      |
| VFMSUB132SD        | Fused Multiply-Sub of Scalar Float64 Values (r1=r1*r3-r2)                        |
| VFMSUB132SS        | Fused Multiply-Sub of Scalar Float32 Values (r1=r1*r3-r2)                        |
| VFMSUB132SH        | Fused Multiply-Sub of Scalar Float16 Values (r1=r1*r3-r2)                        |
| VFMSUB213SD        | Fused Multiply-Sub of Scalar Float64 Values (r1=r2*r1-r3)                        |
| VFMSUB213SS        | Fused Multiply-Sub of Scalar Float32 Values (r1=r2*r1-r3)                        |
| VFMSUB213SH        | Fused Multiply-Sub of Scalar Float16 Values (r1=r2*r1-r3)                        |
| VFMSUB231SD        | Fused Multiply-Sub of Scalar Float64 Values (r1=r2*r3-r1)                        |
| VFMSUB231SS        | Fused Multiply-Sub of Scalar Float32 Values (r1=r2*r3-r1)                        |
| VFMSUB231SH        | Fused Multiply-Sub of Scalar Float16 Values (r1=r2*r3-r1)                        |
| VFNMADD132SD       | Fused Negative Multiply-Add of Scalar Float64 Values (r1=-(r1*r3)+r2)            |
| VFNMADD132SS       | Fused Negative Multiply-Add of Scalar Float32 Values (r1=-(r1*r3)+r2)            |
| VFNMADD132SH       | Fused Negative Multiply-Add of Scalar Float16 Values (r1=-(r1*r3)+r2)            |
| VFNMADD213SD       | Fused Negative Multiply-Add of Scalar Float64 Values (r1=-(r2*r1)+r3)            |
| VFNMADD213SS       | Fused Negative Multiply-Add of Scalar Float32 Values (r1=-(r2*r1)+r3)            |
| VFNMADD213SH       | Fused Negative Multiply-Add of Scalar Float16 Values (r1=-(r2*r1)+r3)            |
| VFNMADD231SD       | Fused Negative Multiply-Add of Scalar Float64 Values (r1=-(r2*r3)+r1)            |
| VFNMADD231SS       | Fused Negative Multiply-Add of Scalar Float32 Values (r1=-(r2*r3)+r1)            |
| VFNMADD231SH       | Fused Negative Multiply-Add of Scalar Float16 Values (r1=-(r2*r3)+r1)            |
| VFNMSUB132SD       | Fused Negative Multiply-Sub of Scalar Float64 Values (r1=-(r1*r3)-r2)            |
| VFNMSUB132SS       | Fused Negative Multiply-Sub of Scalar Float32 Values (r1=-(r1*r3)-r2)            |
| VFNMSUB132SH       | Fused Negative Multiply-Sub of Scalar Float16 Values (r1=-(r1*r3)-r2)            |
| VFNMSUB213SD       | Fused Negative Multiply-Sub of Scalar Float64 Values (r1=-(r2*r1)-r3)            |
| VFNMSUB213SS       | Fused Negative Multiply-Sub of Scalar Float32 Values (r1=-(r2*r1)-r3)            |
| VFNMSUB213SH       | Fused Negative Multiply-Sub of Scalar Float16 Values (r1=-(r2*r1)-r3)            |
| VFNMSUB231SD       | Fused Negative Multiply-Sub of Scalar Float64 Values (r1=-(r2*r3)-r1)            |
| VFNMSUB231SS       | Fused Negative Multiply-Sub of Scalar Float32 Values (r1=-(r2*r3)-r1)            |
| VFNMSUB231SH       | Fused Negative Multiply-Sub of Scalar Float16 Values (r1=-(r2*r3)-r1)            |
| VFCMADDCSH         | Complex Multiply and Accumulate Scalar Float16 Values                            |
| VFMADDCSH          | Complex Multiply and Accumulate Scalar Float16 Values                            |
| VFCMULCSH          | Complex Multiply Scalar Float16 Values                                           |
| VFMULCSH           | Complex Multiply Scalar Float16 Values                                           |

### AVX Scalar FP Int Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vCVTSD2SI          | Convert Scalar Float64 Value to Scalar Signed Integer Value                      |
| vCVTSD2SS          | Convert Scalar Float64 Value to Scalar Float32 Value                             |
| VCVTSD2USI         | Convert Scalar Float64 Value to Scalar Unsigned Integer Value                    |
| vCVTSS2SD          | Convert Scalar Float32 Value to Scalar Float64 Value                             |
| vCVTSS2SI          | Convert Scalar Float32 Value to Scalar Signed Integer Value                      |
| VCVTSS2USI         | Convert Scalar Float32 Value to Scalar Unsigned Integer Value                    |
| vCVTTSD2SI         | Convert with Truncation Scalar Float64 Value to Scalar Signed Integer Value      |
| VCVTTSD2USI        | Convert with Truncation Scalar Float64 Value to Scalar Unsigned Integer Value    |
| vCVTTSS2SI         | Convert with Truncation Scalar Float32 Value to Scalar Signed Integer Value      |
| VCVTTSS2USI        | Convert with Truncation Scalar Float32 Value to Scalar Unsigned Integer Value    |

### AVX Scalar Int FP Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VCVTSI2SH          | Convert Scalar Signed Int32 Value to Scalar Float16 Value                        |
| vCVTSI2SS          | Convert Scalar Signed Int32 Value to Scalar Float32 Value                        |
| vCVTSI2SD          | Convert Scalar Signed Int32 Value to Scalar Float64 Value                        |
| VCVTUSI2SD         | Convert Scalar Unsigned Int32 Value to Scalar Float64 Value                      |
| VCVTUSI2SS         | Convert Scalar Unsigned Int32 Value to Scalar Float32 Value                      |
| VCVTUSI2SH         | Convert Scalar Unsigned Int32 Value to Scalar Float16 Value                      |
| VCVTTSH2SI         | Convert with Truncation Scalar Float16 Value to Scalar Signed Integer Value      |
| VCVTTSH2USI        | Convert with Truncation Scalar Float16 Value to Scalar Unsigned Integer Value    |

### AVX Scalar FP FP Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VCVTSS2SH          | Convert Scalar Float32 Value to Scalar Float16 Value                             |
| VCVTSD2SH          | Convert Scalar Float64 Value to Scalar Float16 Value                             |
| VCVTSH2SS          | Convert Scalar Float16 Value to Scalar Float32 Value                             |
| VCVTSH2SD          | Convert Scalar Float16 Value to Scalar Float64 Value                             |
| VCVTSH2SI          | Convert Scalar Float16 Value to Scalar Signed Integer                            |
| VCVTSH2USI         | Convert Scalar Float16 Value to Scalar Unsigned Integer                          |

### AVX Packed Int Move
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vMASKMOVDQU        | Masked Conditional Move of Bytes from Int128 Value                               |
| MASKMOVQ           | Masked Conditional Move of Bytes from Int64 Value                                |
| VMOVDQA32          | Move Aligned Packed Int32 Values                                                 |
| VMOVDQA64          | Move Aligned Packed Int64 Values                                                 |
| VMOVDQU8           | Move Unaligned Packed Int8 Values                                                |
| VMOVDQU16          | Move Unaligned Packed Int16 Values                                               |
| VMOVDQU32          | Move Unaligned Packed Int32 Values                                               |
| VMOVDQU64          | Move Unaligned Packed Int64 Values                                               |
| vMOVDQA            | Move Aligned Packed Integer Values                                               |
| vMOVDQU            | Move Unaligned Packed Integer Values                                             |
| vMOVNTDQ           | Store Packed Int64 Using Non-Temporal Hint                                       |
| vMOVNTDQA          | Load Double Int64 Using Non-Temporal Aligned Hint                                |
| VPMASKMOVD         | Masked Conditional Move of Packed Int32 Values                                   |
| VPMASKMOVQ         | Masked Conditional Move of Packed Int64 Values                                   |
| vPMOVMSKB          | Move Byte Mask to Register                                                       |
| vPMOVSXBW          | Packed Move with Sign Extension Int8 to Int16                                    |
| vPMOVSXBD          | Packed Move with Sign Extension Int8 to Int32                                    |
| vPMOVSXBQ          | Packed Move with Sign Extension Int8 to Int64                                    |
| vPMOVSXWD          | Packed Move with Sign Extension Int16 to Int32                                   |
| vPMOVSXWQ          | Packed Move with Sign Extension Int16 to Int64                                   |
| vPMOVSXDQ          | Packed Move with Sign Extension Int32 to Int64                                   |
| vPMOVZXBW          | Packed Move with Zero Extension Int8 to Int16                                    |
| vPMOVZXBD          | Packed Move with Zero Extension Int8 to Int32                                    |
| vPMOVZXBQ          | Packed Move with Zero Extension Int8 to Int64                                    |
| vPMOVZXWD          | Packed Move with Zero Extension Int16 to Int32                                   |
| vPMOVZXWQ          | Packed Move with Zero Extension Int16 to Int64                                   |
| vPMOVZXDQ          | Packed Move with Zero Extension Int32 to Int64                                   |
| VPMULTISHIFTQB     | Masked Concatenation of Unaligned Bytes from Int64 Values                        |

### AVX Packed Int Math
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VDBPSADBW          | Double Block Packed Sum Absolute Differences on Unsigned Bytes                   |
| vMPSADBW           | Compute Multiple Packed Sums of Absolute Difference                              |
| vPADDB             | Add Packed Int8 Values                                                           |
| vPADDW             | Add Packed Int16 Values                                                          |
| vPADDD             | Add Packed Int32 Values                                                          |
| vPADDQ             | Add Packed Int64 Values                                                          |
| vPADDSB            | Add Packed Signed Int8 Values with Signed Saturation                             |
| vPADDSW            | Add Packed Signed Int16 Values with Signed Saturation                            |
| vPADDUSB           | Add Packed Unsigned Int8 Values with Unsigned Saturation                         |
| vPADDUSW           | Add Packed Unsigned Int16 Values with Unsigned Saturation                        |
| vPOR               | Logical OR                                                                       |
| VPORD              | Logical OR of Packed Int32 Values                                                |
| VPORQ              | Logical OR of Packed Int64 Values                                                |
| vPAND              | Logical AND                                                                      |
| VPANDD             | Logical AND of Packed Int32 Values                                               |
| VPANDQ             | Logical AND of Packed Int64 Values                                               |
| vPANDN             | Logical AND NOT                                                                  |
| VPANDND            | Logical AND NOT of Packed Int32 Values                                           |
| VPANDNQ            | Logical AND NOT of Packed Int64 Values                                           |
| vPXOR              | Logical Exclusive OR                                                             |
| VPXORD             | Logical Exclusive OR of Packed Int32 Values                                      |
| VPXORQ             | Logical Exclusive OR of Packed Int64 Values                                      |
| vPTEST             | Logical Compare                                                                  |
| vPABSB             | Packed Absolute Int8 Values                                                      |
| vPABSW             | Packed Absolute Int16 Values                                                     |
| vPABSD             | Packed Absolute Int32 Values                                                     |
| VPABSQ             | Packed Absolute Int64 Values                                                     |
| vPACKSSWB          | Pack Int16 Values to Int8 Values with Signed Saturation                          |
| vPACKSSDW          | Pack Int32 Values to Int16 Values with Signed Saturation                         |
| vPACKUSWB          | Pack Int16 Values to Int8 Values with Unsigned Saturation                        |
| vPACKUSDW          | Pack Int32 Values to Int16 Values with Unsigned Saturation                       |
| vPAVGB             | Average Packed Int8 Values                                                       |
| vPAVGW             | Average Packed Int16 Values                                                      |
| VPDPBUSD           | Multiply Unsigned and Signed Int8 Values and Add to Int32 Result                 |
| VPDPBUSDS          | Multiply Unsigned and Signed Int8 Values and Add to Int32 Result with Saturation |
| VPDPWSSD           | Multiply Signed Int16 Values and Add to Int32 Result                             |
| VPDPWSSDS          | Multiply Signed Int16 Values and Add to Int32 Result with Saturation             |
| vPEXTRB            | Extract Int8 Values                                                              |
| vPEXTRD            | Extract Int32 Values                                                             |
| vPEXTRQ            | Extract Int64 Values                                                             |
| vPEXTRW            | Extract Int16 Values                                                             |
| vPINSRB            | Insert Int8 Values                                                               |
| vPINSRD            | Insert Int32 Values                                                              |
| vPINSRQ            | Insert Int64 Values                                                              |
| vPINSRW            | Insert Int16 Values                                                              |
| vPMAXSB            | Maximum of Packed Signed Int8 Values                                             |
| vPMAXSW            | Maximum of Packed Signed Int16 Values                                            |
| vPMAXSD            | Maximum of Packed Signed Int32 Values                                            |
| VPMAXSQ            | Maximum of Packed Signed Int64 Values                                            |
| vPMAXUB            | Maximum of Packed Unsigned Int8 Values                                           |
| vPMAXUW            | Maximum of Packed Unsigned Int16 Values                                          |
| vPMAXUD            | Maximum of Packed Unsigned Int32 Values                                          |
| VPMAXUQ            | Maximum of Packed Unsigned Int64 Values                                          |
| vPMINSB            | Minimum of Packed Signed Int8 Values                                             |
| vPMINSW            | Minimum of Packed Signed Int16 Values                                            |
| vPMINSD            | Minimum of Packed Signed Int32 Values                                            |
| VPMINSQ            | Minimum of Packed Signed Int64 Values                                            |
| vPMINUB            | Minimum of Packed Unsigned Int8 Values                                           |
| vPMINUW            | Minimum of Packed Unsigned Int16 Values                                          |
| vPMINUD            | Minimum of Packed Unsigned Int32 Values                                          |
| VPMINUQ            | Minimum of Packed Unsigned Int64 Values                                          |
| vPMADDUBSW         | Multiply and Add Packed Signed and Unsigned Bytes                                |
| vPMADDWD           | Multiply and Add Packed Int16 Values and Store Int32 Values                      |
| vPMULDQ            | Multiply Packed Int32 Values                                                     |
| vPMULHRSW          | Packed Multiply High with Round and Scale                                        |
| vPMULHUW           | Multiply Packed Unsigned Int16 Values and Store High Result                      |
| vPMULHW            | Multiply Packed Signed Int16 Values and Store High Result                        |
| vPMULLW            | Multiply Packed Signed Int16 Values and Store Low Result                         |
| vPMULLD            | Multiply Packed Signed Int32 Values and Store Low Result                         |
| VPMULLQ            | Multiply Packed Signed Int64 Values and Store Low Result                         |
| vPMULUDQ           | Multiply Packed Unsigned Int32 Values                                            |
| vPSADBW            | Compute Sum of Absolute Differences                                              |
| vPSIGNB            | Packed SIGN Int8 Values                                                          |
| vPSIGNW            | Packed SIGN Int16 Values                                                         |
| vPSIGND            | Packed SIGN Int32 Values                                                         |
| vPSUBB             | Subtract Packed Int8 Values                                                      |
| vPSUBW             | Subtract Packed Int16 Values                                                     |
| vPSUBD             | Subtract Packed Int32 Values                                                     |
| vPSUBQ             | Subtract Packed Int64 Values                                                     |
| vPSUBSB            | Subtract Packed Signed Int8 Values with Signed Saturation                        |
| vPSUBSW            | Subtract Packed Signed Int16 Values with Signed Saturation                       |
| vPSUBUSB           | Subtract Packed Unsigned Int8 Values with Unsigned Saturation                    |
| vPSUBUSW           | Subtract Packed Unsigned Int16 Values with Unsigned Saturation                   |
| vPHADDW            | Packed Horizontal Add Int16 Values                                               |
| vPHADDD            | Packed Horizontal Add Int32 Values                                               |
| vPHADDSW           | Packed Horizontal Add and Saturate Signed Int16 Values                           |
| vPHMINPOSUW        | Packed Horizontal Word Minimum Unsigned Int16 Values                             |
| vPHSUBW            | Packed Horizontal Subtract Int16 Values                                          |
| vPHSUBD            | Packed Horizontal Subtract Int32 Values                                          |
| vPHSUBSW           | Packed Horizontal Subtract and Saturate Signed Int16 Values                      |

### AVX Packed Int Fused Multiply Add
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPMADD52HUQ        | Packed Multiply Unsigned 52-bit Integers and Add High 52-bit Products to Int64   |
| VPMADD52LUQ        | Packed Multiply Unsigned 52-bit Integers and Add Low 52-bit Products to Int64    |

### AVX Packed Int Comparison
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPCMPccB           | Compare Packed Signed Int8 Values Into Mask                                      |
| VPCMPccW           | Compare Packed Signed Int16 Values Into Mask                                     |
| VPCMPccD           | Compare Packed Signed Int32 Values into Mask                                     |
| VPCMPccQ           | Compare Packed Signed Int64 Values into Mask                                     |
| VPCMPccUB          | Compare Packed Unsigned Int8 Values Into Mask                                    |
| VPCMPccUW          | Compare Packed Unsigned Int16 Values Into Mask                                   |
| VPCMPccUD          | Compare Packed Unsigned Int32 Values into Mask                                   |
| VPCMPccUQ          | Compare Packed Unsigned Int64 Values into Mask                                   |
| vPCMPEQB           | Compare Packed Int8 Values for Equality                                          |
| vPCMPEQW           | Compare Packed Int16 Values for Equality                                         |
| vPCMPEQD           | Compare Packed Int32 Values for Equality                                         |
| vPCMPEQQ           | Compare Packed Int64 Values for Equality                                         |
| vPCMPGTB           | Compare Packed Signed Int8 Values for Greater Than                               |
| vPCMPGTW           | Compare Packed Signed Int16 Values for Greater Than                              |
| vPCMPGTD           | Compare Packed Signed Int32 Values for Greater Than                              |
| vPCMPGTQ           | Compare Packed Signed Int64 Values for Greater Than                              |

### AVX Packed String Comparison
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vPCMPESTRI         | Packed Compare Explicit Length Strings, Return Index                             |
| vPCMPESTRM         | Packed Compare Explicit Length Strings, Return Mask                              |
| vPCMPISTRI         | Packed Compare Implicit Length Strings, Return Index                             |
| vPCMPISTRM         | Packed Compare Implicit Length Strings, Return Mask                              |

### AVX Packed FP Move
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VMASKMOVPS         | Masked Conditional Move of Packed Float32 Values                                 |
| VMASKMOVPD         | Masked Conditional Move of Packed Float64 Values                                 |
| vMOVAPD            | Move Aligned Packed Float64 Values                                               |
| vMOVAPS            | Move Aligned Packed Float32 Values                                               |
| vMOVHLPS           | Move Packed Float32 Values High to Low                                           |
| vMOVLHPS           | Move Packed Float32 Values Low to High                                           |
| vMOVHPD            | Move High Packed Float64 Values                                                  |
| vMOVHPS            | Move High Packed Float32 Values                                                  |
| vMOVLPD            | Move Low Packed Float64 Values                                                   |
| vMOVLPS            | Move Low Packed Float32 Values                                                   |
| vMOVMSKPD          | Extract Packed Float64 Sign Mask                                                 |
| vMOVMSKPS          | Extract Packed Float32 Sign Mask                                                 |
| vMOVNTPD           | Store Packed Float64 Values Using Non-Temporal Hint                              |
| vMOVNTPS           | Store Packed Float32 Values Using Non-Temporal Hint                              |
| vMOVSHDUP          | Replicate Float32 Values                                                         |
| vMOVSLDUP          | Replicate Float32 Values                                                         |
| vMOVDDUP           | Replicate Float64 Values                                                         |
| vMOVUPD            | Move Unaligned Packed Float64 Values                                             |
| vMOVUPS            | Move Unaligned Packed Float32 Values                                             |

### AVX Packed FP Math
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vANDNPD            | Bitwise Logical AND NOT of Packed Float64 Values                                 |
| vANDNPS            | Bitwise Logical AND NOT of Packed Float32 Values                                 |
| vANDPD             | Bitwise Logical AND of Packed Float64 Values                                     |
| vANDPS             | Bitwise Logical AND of Packed Float32 Values                                     |
| vORPD              | Bitwise Logical OR of Packed Float64 Values                                      |
| vORPS              | Bitwise Logical OR of Packed Float32 Values                                      |
| vXORPD             | Bitwise Logical XOR of Packed Float64 Values                                     |
| vXORPS             | Bitwise Logical XOR of Packed Float32 Values                                     |
| VTESTPD            | Bitwise Logical Test of Packed Float64 Values                                    |
| VTESTPS            | Bitwise Logical Test of Packed Float32 Values                                    |
| vCMPPD             | Compare Packed Float64 Values                                                    |
| vCMPPS             | Compare Packed Float32 Values                                                    |
| VCMPPH             | Compare Packed Float16 Values                                                    |
| vCOMISD            | Compare Scalar Ordered Float64 Values and Set EFLAGS                             |
| vCOMISS            | Compare Scalar Ordered Float32 Values and Set EFLAGS                             |
| VCOMISH            | Compare Scalar Ordered Float16 Values and Set EFLAGS                             |
| vUCOMISD           | Unordered Compare Scalar Float64 Values and Set EFLAGS                           |
| vUCOMISS           | Unordered Compare Scalar Float32 Values and Set EFLAGS                           |
| VUCOMISH           | Unordered Compare Scalar Float16 Values and Set EFLAGS                           |
| vMINPD             | Minimum Packed Float64 Values                                                    |
| vMINPS             | Minimum Packed Float32 Values                                                    |
| VMINPH             | Minimum Packed Float16 Values                                                    |
| vMAXPD             | Maximum Packed Float64 Values                                                    |
| vMAXPS             | Maximum Packed Float32 Values                                                    |
| VMAXPH             | Maximum Packed Float16 Values                                                    |
| vADDPD             | Add Packed Float64 Values                                                        |
| vADDPS             | Add Packed Float32 Values                                                        |
| VADDPH             | Add Packed Float16 Values                                                        |
| vSUBPD             | Subtract Packed Float64 Values                                                   |
| vSUBPS             | Subtract Packed Float32 Values                                                   |
| VSUBPH             | Subtract Packed Float16 Values                                                   |
| vADDSUBPD          | Packed Float64 Add/Sub                                                           |
| vADDSUBPS          | Packed Float32 Add/Sub                                                           |
| vMULPD             | Multiply Packed Float64 Values                                                   |
| vMULPS             | Multiply Packed Float32 Values                                                   |
| VMULPH             | Multiply Packed Float16 Values                                                   |
| vDIVPD             | Divide Packed Float64 Values                                                     |
| vDIVPS             | Divide Packed Float32 Values                                                     |
| VDIVPH             | Divide Packed Float16 Values                                                     |
| vDPPD              | Dot Product of Packed Float64 Values                                             |
| vDPPS              | Dot Product of Packed Float32 Values                                             |
| VFIXUPIMMPD        | Fix Up Special Packed Float64 Values                                             |
| VFIXUPIMMPS        | Fix Up Special Packed Float32 Values                                             |
| VFPCLASSPD         | Tests Types Of a Packed Float64 Values                                           |
| VFPCLASSPS         | Tests Types Of a Packed Float32 Values                                           |
| VFPCLASSPH         | Tests Types Of a Packed Float16 Values                                           |
| VGETEXPPD          | Convert Exponents of Packed Float64 Values to Float64 Values                     |
| VGETEXPPS          | Convert Exponents of Packed Float32 Values to Float32 Values                     |
| VGETEXPPH          | Convert Exponents of Packed Float16 Values to Float32 Values                     |
| VGETMANTPD         | Extract Float64 Vector of Normalized Mantissas from Float64 Vector               |
| VGETMANTPS         | Extract Float32 Vector of Normalized Mantissas from Float32 Vector               |
| VGETMANTPH         | Extract Float16 Vector of Normalized Mantissas from Float32 Vector               |
| VRANGEPD           | Range Restriction Calculation For Packed Pairs of Float64 Values                 |
| VRANGEPS           | Range Restriction Calculation For Packed Pairs of Float32 Values                 |
| VRCP14PD           | Compute Approximate Reciprocals of Packed Float64 Values                         |
| VRCP14PS           | Compute Approximate Reciprocals of Packed Float32 Values                         |
| VRCPPH             | Compute Reciprocals of Packed Float16 Values                                     |
| vRCPPS             | Compute Reciprocals of Packed Float32 Values                                     |
| VREDUCEPD          | Perform Reduction Transformation on Packed Float64 Values                        |
| VREDUCEPS          | Perform Reduction Transformation on Packed Float32 Values                        |
| VREDUCEPH          | Perform Reduction Transformation on Packed Float16 Values                        |
| VRNDSCALEPD        | Round Packed Float64 Values To Include A Given Number Of Fraction Bits           |
| VRNDSCALEPS        | Round Packed Float32 Values To Include A Given Number Of Fraction Bits           |
| VRNDSCALEPH        | Round Packed Float16 Values To Include A Given Number Of Fraction Bits           |
| VRSQRT14PD         | Compute Approximate Reciprocals of Square Roots of Packed Float64 Values         |
| VRSQRT14PS         | Compute Approximate Reciprocals of Square Roots of Packed Float32 Values         |
| vROUNDPD           | Round Packed Float64 Values                                                      |
| vROUNDPS           | Round Packed Float32 Values                                                      |
| VSCALEFPD          | Scale Packed Float64 Values With Float64 Values                                  |
| VSCALEFPS          | Scale Packed Float32 Values With Float32 Values                                  |
| VSCALEFPH          | Scale Packed Float16 Values With Float16 Values                                  |
| vRSQRTPS           | Compute Reciprocals of Square Roots of Packed Float32 Values                     |
| VRSQRTPH           | Compute Reciprocals of Square Roots of Packed Float16 Values                     |
| vSQRTPD            | Square Root of Packed Float64 Values                                             |
| vSQRTPS            | Square Root of Packed Float32 Values                                             |
| VSQRTPH            | Square Root of Packed Float16 Values                                             |
| vHADDPD            | Packed Float64 Horizontal Add                                                    |
| vHADDPS            | Packed Float32 Horizontal Add                                                    |
| vHSUBPD            | Packed Float64 Horizontal Subtract                                               |
| vHSUBPS            | Packed Float32 Horizontal Subtract                                               |

### AVX Packed FP Fused Multiply Add
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VFMADD132PD        | Fused Multiply-Add of Packed Float64 Values (r1=(r1*r3)+r2)                      |
| VFMADD132PS        | Fused Multiply-Add of Packed Float32 Values (r1=(r1*r3)+r2)                      |
| VFMADD132PH        | Fused Multiply-Add of Packed Float16 Values (r1=(r1*r3)+r2)                      |
| VFMADD213PD        | Fused Multiply-Add of Packed Float64 Values (r1=(r2*r1)+r3)                      |
| VFMADD213PS        | Fused Multiply-Add of Packed Float32 Values (r1=(r2*r1)+r3)                      |
| VFMADD213PH        | Fused Multiply-Add of Packed Float16 Values (r1=(r2*r1)+r3)                      |
| VFMADD231PD        | Fused Multiply-Add of Packed Float64 Values (r1-(r2*r3)+r1)                      |
| VFMADD231PS        | Fused Multiply-Add of Packed Float32 Values (r1-(r2*r3)+r1)                      |
| VFMADD231PH        | Fused Multiply-Add of Packed Float16 Values (r1-(r2*r3)+r1)                      |
| VFMADDSUB132PD     | Fused Multiply-Alternating Add/Sub of Packed Float64 Values (r1=(r1*r3)±r2)      |
| VFMADDSUB132PS     | Fused Multiply-Alternating Add/Sub of Packed Float32 Values (r1=(r1*r3)±r2)      |
| VFMADDSUB132PH     | Fused Multiply-Alternating Add/Sub of Packed Float16 Values (r1=(r1*r3)±r2)      |
| VFMADDSUB213PD     | Fused Multiply-Alternating Add/Sub of Packed Float64 Values (r1=(r2*r1)±r3)      |
| VFMADDSUB213PS     | Fused Multiply-Alternating Add/Sub of Packed Float32 Values (r1=(r2*r1)±r3)      |
| VFMADDSUB213PH     | Fused Multiply-Alternating Add/Sub of Packed Float16 Values (r1=(r2*r1)±r3)      |
| VFMADDSUB231PD     | Fused Multiply-Alternating Add/Sub of Packed Float64 Values (r1=(r2*r3)±r1)      |
| VFMADDSUB231PS     | Fused Multiply-Alternating Add/Sub of Packed Float32 Values (r1=(r2*r3)±r1)      |
| VFMADDSUB231PH     | Fused Multiply-Alternating Add/Sub of Packed Float16 Values (r1=(r2*r3)±r1)      |
| VFMSUB132PD        | Fused Multiply-Sub of Packed Float64 Values (r1=r1*r3-r2)                        |
| VFMSUB132PS        | Fused Multiply-Sub of Packed Float32 Values (r1=r1*r3-r2)                        |
| VFMSUB132PH        | Fused Multiply-Sub of Packed Float16 Values (r1=r1*r3-r2)                        |
| VFMSUB213PD        | Fused Multiply-Sub of Packed Float64 Values (r1=r2*r1-r3)                        |
| VFMSUB213PS        | Fused Multiply-Sub of Packed Float32 Values (r1=r2*r1-r3)                        |
| VFMSUB213PH        | Fused Multiply-Sub of Packed Float16 Values (r1=r2*r1-r3)                        |
| VFMSUB231PD        | Fused Multiply-Sub of Packed Float64 Values (r1=r2*r3-r1)                        |
| VFMSUB231PS        | Fused Multiply-Sub of Packed Float32 Values (r1=r2*r3-r1)                        |
| VFMSUB231PH        | Fused Multiply-Sub of Packed Float16 Values (r1=r2*r3-r1)                        |
| VFMSUBADD132PD     | Fused Multiply-Alternating Sub/Add of Packed Float64 Values (r1=(r1*r3)∓r2)      |
| VFMSUBADD132PS     | Fused Multiply-Alternating Sub/Add of Packed Float32 Values (r1=(r1*r3)∓r2)      |
| VFMSUBADD132PH     | Fused Multiply-Alternating Sub/Add of Packed Float16 Values (r1=(r1*r3)∓r2)      |
| VFMSUBADD213PD     | Fused Multiply-Alternating Sub/Add of Packed Float64 Values (r1=(r2*r1)∓r3)      |
| VFMSUBADD213PS     | Fused Multiply-Alternating Sub/Add of Packed Float32 Values (r1=(r2*r1)∓r3)      |
| VFMSUBADD213PH     | Fused Multiply-Alternating Sub/Add of Packed Float16 Values (r1=(r2*r1)∓r3)      |
| VFMSUBADD231PD     | Fused Multiply-Alternating Sub/Add of Packed Float64 Values (r1=(r2*r3)∓r1)      |
| VFMSUBADD231PS     | Fused Multiply-Alternating Sub/Add of Packed Float32 Values (r1=(r2*r3)∓r1)      |
| VFMSUBADD231PH     | Fused Multiply-Alternating Sub/Add of Packed Float16 Values (r1=(r2*r3)∓r1)      |
| VFNMADD132PD       | Fused Negative Multiply-Add of Packed Float64 Values (r1=-(r1*r3)+r2)            |
| VFNMADD132PS       | Fused Negative Multiply-Add of Packed Float32 Values (r1=-(r1*r3)+r2)            |
| VFNMADD132PH       | Fused Negative Multiply-Add of Packed Float16 Values (r1=-(r1*r3)+r2)            |
| VFNMADD213PD       | Fused Negative Multiply-Add of Packed Float64 Values (r1=-(r2*r1)+r3)            |
| VFNMADD213PS       | Fused Negative Multiply-Add of Packed Float32 Values (r1=-(r2*r1)+r3)            |
| VFNMADD213PH       | Fused Negative Multiply-Add of Packed Float16 Values (r1=-(r2*r1)+r3)            |
| VFNMADD231PD       | Fused Negative Multiply-Add of Packed Float64 Values (r1=-(r2*r3)+r1)            |
| VFNMADD231PS       | Fused Negative Multiply-Add of Packed Float32 Values (r1=-(r2*r3)+r1)            |
| VFNMADD231PH       | Fused Negative Multiply-Add of Packed Float16 Values (r1=-(r2*r3)+r1)            |
| VFNMSUB132PD       | Fused Negative Multiply-Sub of Packed Float64 Values (r1=-(r1*r3)-r2)            |
| VFNMSUB132PS       | Fused Negative Multiply-Sub of Packed Float32 Values (r1=-(r1*r3)-r2)            |
| VFNMSUB132PH       | Fused Negative Multiply-Sub of Packed Float16 Values (r1=-(r1*r3)-r2)            |
| VFNMSUB213PD       | Fused Negative Multiply-Sub of Packed Float64 Values (r1=-(r2*r1)-r3)            |
| VFNMSUB213PS       | Fused Negative Multiply-Sub of Packed Float32 Values (r1=-(r2*r1)-r3)            |
| VFNMSUB213PH       | Fused Negative Multiply-Sub of Packed Float16 Values (r1=-(r2*r1)-r3)            |
| VFNMSUB231PD       | Fused Negative Multiply-Sub of Packed Float64 Values (r1=-(r2*r3)-r1)            |
| VFNMSUB231PS       | Fused Negative Multiply-Sub of Packed Float32 Values (r1=-(r2*r3)-r1)            |
| VFNMSUB231PH       | Fused Negative Multiply-Sub of Packed Float16 Values (r1=-(r2*r3)-r1)            |
| VFCMADDCPH         | Complex Multiply and Accumulate Float16 Values                                   |
| VFMADDCPH          | Complex Multiply and Accumulate Float16 Values                                   |
| VFCMULCPH          | Complex Multiply Float16 Values                                                  |
| VFMULCPH           | Complex Multiply Float16 Values                                                  |

### AVX Packed Int Int Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPMOVWB            | Down Convert Int16 to Int8                                                       |
| VPMOVDB            | Down Convert Int32 to Int8                                                       |
| VPMOVDW            | Down Convert Int32 to Int16                                                      |
| VPMOVQB            | Down Convert Int64 to Int8                                                       |
| VPMOVQW            | Down Convert Int64 to Int16                                                      |
| VPMOVQD            | Down Convert Int64 to Int32                                                      |
| VPMOVSWB           | Down Convert Signed Int16 With Signed Saturation to Int8                         |
| VPMOVSDB           | Down Convert Signed Int32 With Signed Saturation to Int8                         |
| VPMOVSDW           | Down Convert Signed Int32 With Signed Saturation to Int16                        |
| VPMOVSQB           | Down Convert Signed Int64 With Signed Saturation to Int8                         |
| VPMOVSQW           | Down Convert Signed Int64 With Signed Saturation to Int16                        |
| VPMOVSQD           | Down Convert Signed Int64 With Signed Saturation to Int32                        |
| VPMOVUSWB          | Down Convert Unsigned Int16 With Unsigned Saturation to Int8                     |
| VPMOVUSDB          | Down Convert Unsigned Int32 With Unsigned Saturation to Int8                     |
| VPMOVUSDW          | Down Convert Unsigned Int32 With Unsigned Saturation to Int16                    |
| VPMOVUSQB          | Down Convert Unsigned Int64 With Unsigned Saturation to Int8                     |
| VPMOVUSQW          | Down Convert Unsigned Int64 With Unsigned Saturation to Int16                    |
| VPMOVUSQD          | Down Convert Unsigned Int64 With Unsigned Saturation to Int32                    |

### AVX Packed FP Int Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CVTPS2PI           | Convert Packed Float32 Values to Packed Signed Int32 Values                      |
| CVTPD2PI           | Convert Packed Float64 Values to Packed Signed Int32 Values                      |
| VCVTPH2W           | Convert Packed Float16 Values to Packed Signed Int16 Values                      |
| VCVTPH2DQ          | Convert Packed Float16 Values to Packed Signed Int32 Values                      |
| VCVTPH2QQ          | Convert Packed Float16 Values to Packed Signed Int64 Values                      |
| vCVTPS2DQ          | Convert Packed Float32 Values to Packed Signed Int32 Values                      |
| VCVTPS2QQ          | Convert Packed Float32 Values to Packed Signed Int64 Values                      |
| vCVTPD2DQ          | Convert Packed Float64 Values to Packed Signed Int32 Values                      |
| VCVTPD2QQ          | Convert Packed Float64 Values to Packed Signed Int64 Values                      |
| VCVTPH2UW          | Convert Packed Float16 Values to Packed Unsigned Int16 Values                    |
| VCVTPH2UDQ         | Convert Packed Float16 Values to Packed Unsigned Int32 Values                    |
| VCVTPH2UQQ         | Convert Packed Float16 Values to Packed Unsigned Int64 Values                    |
| VCVTPS2UDQ         | Convert Packed Float32 Values to Packed Unsigned Int32 Values                    |
| VCVTPS2UQQ         | Convert Packed Float32 Values to Packed Unsigned Int64 Values                    |
| VCVTPD2UDQ         | Convert Packed Float64 Values to Packed Unsigned Int32 Values                    |
| VCVTPD2UQQ         | Convert Packed Float64 Values to Packed Unsigned Int64 Values                    |
| CVTTPD2PI          | Convert with Truncation Packed Float64 Values to Packed Signed Int32 Values      |
| CVTTPS2PI          | Convert with Truncation Packed Float32 Values to Packed Signed Int32 Values      |
| VCVTTPH2W          | Convert with Truncation Packed Float16 Values to Packed Signed Int16 Values      |
| VCVTTPH2DQ         | Convert with Truncation Packed Float16 Values to Packed Signed Int32 Values      |
| VCVTTPH2QQ         | Convert with Truncation Packed Float16 Values to Packed Signed Int64 Values      |
| vCVTTPS2DQ         | Convert with Truncation Packed Float32 Values to Packed Signed Int32 Values      |
| VCVTTPS2QQ         | Convert with Truncation Packed Float32 Values to Packed Signed Int64 Values      |
| vCVTTPD2DQ         | Convert with Truncation Packed Float64 Values to Packed Signed Int32 Values      |
| VCVTTPD2QQ         | Convert with Truncation Packed Float64 Values to Packed Signed Int64 Values      |
| VCVTTPH2UW         | Convert with Truncation Packed Float16 Values to Packed Unsigned Int16 Values    |
| VCVTTPH2UDQ        | Convert with Truncation Packed Float16 Values to Packed Unsigned Int32 Values    |
| VCVTTPH2UQQ        | Convert with Truncation Packed Float16 Values to Packed Unsigned Int64 Values    |
| VCVTTPD2UDQ        | Convert with Truncation Packed Float64 Values to Packed Unsigned Int32 Values    |
| VCVTTPD2UQQ        | Convert with Truncation Packed Float64 Values to Packed Unsigned Int64 Values    |
| VCVTTPS2UDQ        | Convert with Truncation Packed Float32 Values to Packed Unsigned Int32 Values    |
| VCVTTPS2UQQ        | Convert with Truncation Packed Float32 Values to Packed Unsigned Int64 Values    |

### AVX Packed Int FP Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CVTPI2PS           | Convert Packed Signed Int32 Values to Packed Float32 Values                      |
| CVTPI2PD           | Convert Packed Signed Int32 Values to Packed Float64 Values                      |
| VCVTW2PH           | Convert Packed Signed Int16 Values to Packed Float16 Values                      |
| VCVTDQ2PH          | Convert Packed Signed Int32 Values to Packed Float16 Values                      |
| vCVTDQ2PS          | Convert Packed Signed Int32 Values to Packed Float32 Values                      |
| vCVTDQ2PD          | Convert Packed Signed Int32 Values to Packed Float64 Values                      |
| VCVTQQ2PH          | Convert Packed Signed Int64 Values to Packed Float16 Values                      |
| VCVTQQ2PS          | Convert Packed Signed Int64 Values to Packed Float32 Values                      |
| VCVTQQ2PD          | Convert Packed Signed Int64 Values to Packed Float64 Values                      |
| VCVTUW2PH          | Convert Packed Unsigned Int16 Values to Packed Float16 Values                    |
| VCVTUDQ2PH         | Convert Packed Unsigned Int32 Values to Packed Float16 Values                    |
| VCVTUDQ2PS         | Convert Packed Unsigned Int32 Values to Packed Float32 Values                    |
| VCVTUDQ2PD         | Convert Packed Unsigned Int32 Values to Packed Float64 Values                    |
| VCVTUQQ2PH         | Convert Packed Unsigned Int64 Values to Packed Float16 Values                    |
| VCVTUQQ2PS         | Convert Packed Unsigned Int64 Values to Packed Float32 Values                    |
| VCVTUQQ2PD         | Convert Packed Unsigned Int64 Values to Packed Float64 Values                    |

### AVX Packed FP FP Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VCVTPH2PS          | Convert Packed Float16 Values to Packed Float32 Values                           |
| VCVTPS2PH          | Convert Packed Float32 Values to Packed Float16 Values                           |
| VCVTPH2PSX         | Convert Packed Float16 Values to Packed Float32 Values                           |
| VCVTPH2PD          | Convert Packed Float16 Values to Packed Float64 Values                           |
| VCVTPS2PHX         | Convert Packed Float32 Values to Packed Float16 Values                           |
| vCVTPS2PD          | Convert Packed Float32 Values to Packed Float64 Values                           |
| VCVTPD2PH          | Convert Packed Float64 Values to Packed Float16 Values                           |
| vCVTPD2PS          | Convert Packed Float64 Values to Packed Float32 Values                           |

### AVX Shift and Align
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vPALIGNR           | Concatenate and Align Packed Int128 Values Right Bytes                           |
| VALIGND            | Concatenate and Align Packed Int32 Values                                        |
| VALIGNQ            | Concatenate and Align Packed Int64 Values                                        |
| vPSLLDQ            | Shift Packed Int128 Values Left by Bytes Logical                                 |
| vPSRLDQ            | Shift Packed Int128 Values Right by Bytes Logical                                |
| vPSLLW             | Shift Packed Int16 Values Left Logical                                           |
| vPSLLD             | Shift Packed Int32 Values Left Logical                                           |
| vPSLLQ             | Shift Packed Int64 Values Left Logical                                           |
| vPSRAW             | Shift Packed Int16 Values Right Arithmetic                                       |
| vPSRAD             | Shift Packed Int32 Values Right Arithmetic                                       |
| VPSRAQ             | Shift Packed Int64 Values Right Arithmetic                                       |
| vPSRLW             | Shift Packed Int16 Values Right Logical                                          |
| vPSRLD             | Shift Packed Int32 Values Right Logical                                          |
| vPSRLQ             | Shift Packed Int64 Values Right Logical                                          |
| VPROLD             | Bit Rotate Left Int32 Values                                                     |
| VPROLQ             | Bit Rotate Left Int64 Values                                                     |
| VPROLVD            | Bit Rotate Left Int32 Values using Vector Shift Values                           |
| VPROLVQ            | Bit Rotate Left Int64 Values using Vector Shift Values                           |
| VPRORD             | Bit Rotate Right Int32 Values                                                    |
| VPRORQ             | Bit Rotate Right Int64 Values                                                    |
| VPRORVD            | Bit Rotate Right Int32 Values using Vector Shift Values                          |
| VPRORVQ            | Bit Rotate Right Int64 Values using Vector Shift Values                          |
| VPSHLDW            | Concatenate and Shift Packed Int16 Values Left Logical                           |
| VPSHLDD            | Concatenate and Shift Packed Int32 Values Left Logical                           |
| VPSHLDQ            | Concatenate and Shift Packed Int64 Values Left Logical                           |
| VPSHLDVW           | Concatenate and Variable Shift Packed Int16 Values Left Logical                  |
| VPSHLDVD           | Concatenate and Variable Shift Packed Int32 Values Left Logical                  |
| VPSHLDVQ           | Concatenate and Variable Shift Packed Int64 Values Left Logical                  |
| VPSHRDW            | Concatenate and Shift Packed Int16 Values Right Logical                          |
| VPSHRDD            | Concatenate and Shift Packed Int32 Values Right Logical                          |
| VPSHRDQ            | Concatenate and Shift Packed Int64 Values Right Logical                          |
| VPSHRDVW           | Concatenate and Variable Shift Packed Int16 Values Right Logical                 |
| VPSHRDVD           | Concatenate and Variable Shift Packed Int32 Values Right Logical                 |
| VPSHRDVQ           | Concatenate and Variable Shift Packed Int64 Values Right Logical                 |
| VPSLLVW            | Variable Bit Shift Left Logical Int16 Values                                     |
| VPSLLVD            | Variable Bit Shift Left Logical Int32 Values                                     |
| VPSLLVQ            | Variable Bit Shift Left Logical Int64 Values                                     |
| VPSRAVW            | Variable Bit Shift Right Arithmetic Int16 Values                                 |
| VPSRAVD            | Variable Bit Shift Right Arithmetic Int32 Values                                 |
| VPSRAVQ            | Variable Bit Shift Right Arithmetic Int64 Values                                 |
| VPSRLVW            | Variable Bit Shift Right Logical Int16 Values                                    |
| VPSRLVD            | Variable Bit Shift Right Logical Int32 Values                                    |
| VPSRLVQ            | Variable Bit Shift Right Logical Int64 Values                                    |

### AVX Shuffle Blend FP
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vSHUFPD            | Packed Interleave Shuffle of Pairs of Float64 Values                             |
| vSHUFPS            | Packed Interleave Shuffle of Quadruplets of Float32 Values                       |
| VSHUFF32X4         | Shuffle Packed Float32 Values at 128-bit Granularity                             |
| VSHUFF64X2         | Shuffle Packed Float64 Values at 128-bit Granularity                             |
| vBLENDPD           | Blend Packed Float64 Values                                                      |
| vBLENDPS           | Blend Packed Float32 Values                                                      |
| vBLENDVPD          | Variable Blend Packed Float64 Values                                             |
| vBLENDVPS          | Variable Blend Packed Float32 Values                                             |
| VBLENDMPD          | Blend Float64 Vectors Using an OpMask Control                                    |
| VBLENDMPS          | Blend Float32 Vectors Using an OpMask Control                                    |

### AVX Shuffle Blend Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vPSHUFB            | Shuffle Packed Int8 Vectors                                                      |
| PSHUFW             | Shuffle Packed Int16 Vectors                                                     |
| vPSHUFD            | Shuffle Packed Int32 Vectors                                                     |
| vPSHUFHW           | Shuffle Packed High Words                                                        |
| vPSHUFLW           | Shuffle Packed Low Words                                                         |
| VSHUFI32X4         | Shuffle Packed Int32 Values at 128-bit Granularity                               |
| VSHUFI64X2         | Shuffle Packed Int64 Values at 128-bit Granularity                               |
| VPSHUFBITQMB       | Shuffle Bits from Quadword Elements Using Byte Indexes into Mask                 |
| vPBLENDVB          | Variable Blend Packed Bytes                                                      |
| vPBLENDW           | Blend Packed Int16 Vectors                                                       |
| VPBLENDD           | Blend Packed Int32 Vectors                                                       |
| VPBLENDMB          | Blend Packed Int8 Vectors Using an Opmask Control                                |
| VPBLENDMD          | Blend Packed Int32 Vectors Using an OpMask Control                               |
| VPBLENDMQ          | Blend Packed Int64 Vectors Using an OpMask Control                               |
| VPBLENDMW          | Blend Packed Int16 Vectors Using an Opmask Control                               |

### AVX Broadcast FP
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VBROADCASTSS       | Broadcast Float32 Values                                                         |
| VBROADCASTSD       | Broadcast Float64 Values                                                         |
| VBROADCASTF32X2    | Broadcast Two Float32 Values                                                     |
| VBROADCASTF32X4    | Broadcast Four Float32 Values                                                    |
| VBROADCASTF32X8    | Broadcast Eight Float32 Values                                                   |
| VBROADCASTF64X2    | Broadcast Two Float64 Values                                                     |
| VBROADCASTF64X4    | Broadcast Four Float64 Values                                                    |
| VBROADCASTF128     | Broadcast 128-bits of FP Data                                                    |

### AVX Broadcast Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPBROADCASTB       | Broadcast Int8 Values from General Purpose Register                              |
| VPBROADCASTW       | Broadcast Int16 Values from General Purpose Register                             |
| VPBROADCASTD       | Broadcast Int32 Values from General Purpose Register                             |
| VPBROADCASTQ       | Broadcast Int64 Values from General Purpose Register                             |
| VBROADCASTI32X2    | Broadcast Two Int32 Values                                                       |
| VBROADCASTI32X4    | Broadcast Four Int32 Values                                                      |
| VBROADCASTI32X8    | Broadcast Eight Int32 Values                                                     |
| VBROADCASTI64X2    | Broadcast Two Int64 Values                                                       |
| VBROADCASTI64X4    | Broadcast Four Int64 Values                                                      |
| VBROADCASTI128     | Broadcast 128-bits of Int Data                                                   |
| VPBROADCASTMB2Q    | Broadcast Int8 to Zero Extended Int64                                            |
| VPBROADCASTMW2D    | Broadcast Int16 to Zero Extended Int32                                           |

### AVX Compress Expand FP
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VCOMPRESSPD        | Store Sparse Packed Float64 Values into Dense Memory                             |
| VCOMPRESSPS        | Store Sparse Packed Float32 Values into Dense Memory                             |
| VEXPANDPD          | Load Sparse Packed Float64 Values from Dense Memory                              |
| VEXPANDPS          | Load Sparse Packed Float32 Values from Dense Memory                              |

### AVX Compress Expand Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPCOMPRESSB        | Store Sparse Packed Int8 Values                                                  |
| VPCOMPRESSW        | Store Sparse Packed Int16 Values                                                 |
| VPCOMPRESSD        | Store Sparse Packed Int32 Values                                                 |
| VPCOMPRESSQ        | Store Sparse Packed Int64 Values                                                 |
| VPEXPANDB          | Expand Packed Int8 Values from Dense Memory/Register                             |
| VPEXPANDD          | Expand Packed Int32 Values from Dense Memory/Register                            |
| VPEXPANDQ          | Expand Packed Int64 Values from Dense Memory/Register                            |
| VPEXPANDW          | Expand Packed Int16 Values from Dense Memory/Register                            |

### AVX Insert Extract FP
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vINSERTPS          | Insert into Packed Float32 Values                                                |
| VINSERTF128        | Insert Packed 128-bit FP Values                                                  |
| VINSERTF32X4       | Insert 4 Packed Float32 Values                                                   |
| VINSERTF32X8       | Insert 8 Packed Float32 Values                                                   |
| VINSERTF64X2       | Insert 2 Packed Float64 Values                                                   |
| VINSERTF64X4       | Insert 4 Packed Float64 Values                                                   |
| VEXTRACTF128       | Extract Packed 128-bit FP Values                                                 |
| VEXTRACTF32X4      | Extract 4 Packed Float32 Values                                                  |
| VEXTRACTF32X8      | Extract 8 Packed Float32 Values                                                  |
| VEXTRACTF64X2      | Extract 2 Packed Float64 Values                                                  |
| VEXTRACTF64X4      | Extract 4 Packed Float64 Values                                                  |
| vEXTRACTPS         | Extract from Packed Float32 Values                                               |

### AVX Insert Extract Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VINSERTI128        | Insert Packed 128-bit Integer Values                                             |
| VINSERTI32X4       | Insert 4 Packed Int32 Values                                                     |
| VINSERTI32X8       | Insert 8 Packed Int32 Values                                                     |
| VINSERTI64X2       | Insert 2 Packed Int64 Values                                                     |
| VINSERTI64X4       | Insert 4 Packed Int64 Values                                                     |
| VEXTRACTI128       | Extract packed 128-bit Integer Values                                            |
| VEXTRACTI32X4      | Extract 4 Packed Int32 Values                                                    |
| VEXTRACTI32X8      | Extract 8 Packed Int32 Values                                                    |
| VEXTRACTI64X2      | Extract 2 Packed Int64 Values                                                    |
| VEXTRACTI64X4      | Extract 4 Packed Int64 Values                                                    |

### AVX Permute FP
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPERM2F128         | Permute FP Values                                                                |
| VPERMI2PD          | Permute of Float64 From Two Tables Overwriting the Index                         |
| VPERMI2PS          | Permute of Float32 From Two Tables Overwriting the Index                         |
| VPERMILPD          | Permute In-Lane of Pairs of Float64 Values                                       |
| VPERMILPS          | Permute In-Lane of Quadruples of Float32 Values                                  |
| VPERMPD            | Permute of Float64 Values                                                        |
| VPERMPS            | Permute of Float32 Values                                                        |
| VPERMT2PD          | Permute of Float64 from Two Tables Overwriting one Table                         |
| VPERMT2PS          | Permute of Float32 from Two Tables Overwriting one Table                         |

### AVX Permute Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPERMB             | Permute of Int8 Values                                                           |
| VPERMW             | Permute of Int16 Values                                                          |
| VPERMD             | Permute of Int32 Values                                                          |
| VPERMQ             | Permute of Int64 Values                                                          |
| VPERM2I128         | Permute of Two Int128 Values                                                     |
| VPERMI2B           | Permute of Int8 from Two Tables Overwriting the Index                            |
| VPERMI2W           | Permute of Int16 From Two Tables Overwriting the Index                           |
| VPERMI2D           | Permute of Int32 From Two Tables Overwriting the Index                           |
| VPERMI2Q           | Permute of Int64 From Two Tables Overwriting the Index                           |
| VPERMT2B           | Permute of Int8 from Two Tables Overwriting one Table                            |
| VPERMT2W           | Permute of Int16 from Two Tables Overwriting one Table                           |
| VPERMT2D           | Permute of Int32 from Two Tables Overwriting one Table                           |
| VPERMT2Q           | Permute of Int64 from Two Tables Overwriting one Table                           |

### AVX Scatter Gather FP
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VSCATTERDPD        | Scatter Packed Float64 Values with Signed Int32 Indices                          |
| VSCATTERDPS        | Scatter Packed Float32 Values with Signed Int32 Indices                          |
| VSCATTERQPD        | Scatter Packed Float64 Values with Signed Int64 Indices                          |
| VSCATTERQPS        | Scatter Packed Float32 Values with Signed Int64 Indices                          |
| VGATHERDPD         | Gather Packed Float64 Values Using Signed Int32 Indices                          |
| VGATHERDPS         | Gather Packed Float32 Values Using Signed Int32 Indices                          |
| VGATHERQPD         | Gather Packed Float64 Values Using Signed Int64 Indices                          |
| VGATHERQPS         | Gather Packed Float32 Values Using Signed Int64 Indices                          |

### AVX Scatter Gather Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPSCATTERDD        | Scatter Packed Int32 Values with Int32 Indices                                   |
| VPSCATTERDQ        | Scatter Packed Int64 Values with Int32 Indices                                   |
| VPSCATTERQD        | Scatter Packed Int32 Values with Int64 Indices                                   |
| VPSCATTERQQ        | Scatter Packed Int64 Values with Int64 Indices                                   |
| VPGATHERDD         | Gather Packed Int32 Values Using Signed Int32 Indices                            |
| VPGATHERDQ         | Gather Packed Int64 Values Using Signed Int32 Indices                            |
| VPGATHERQD         | Gather Packed Int32 Values Using Signed Int64 Indices                            |
| VPGATHERQQ         | Gather Packed Int64 Values Using Signed Int64 Indices                            |

### AVX Pack Unpack Int
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vPUNPCKHBW         | Unpack High Data Int8 Values to Int16 Values                                     |
| vPUNPCKHWD         | Unpack High Data Int16 Values to Int32 Values                                    |
| vPUNPCKHDQ         | Unpack High Data Int32 Values to Int64 Values                                    |
| vPUNPCKHQDQ        | Unpack High Data Int64 Values to Int128 Values                                   |
| vPUNPCKLBW         | Unpack Low Data Int8 Values to Int16 Values                                      |
| vPUNPCKLWD         | Unpack Low Data Int16 Values to Int32 Values                                     |
| vPUNPCKLDQ         | Unpack Low Data Int32 Values to Int64 Values                                     |
| vPUNPCKLQDQ        | Unpack Low Data Int64 Values to Int128 Values                                    |
| vUNPCKHPD          | Unpack and Interleave High Packed Float64 Values                                 |
| vUNPCKHPS          | Unpack and Interleave High Packed Float32 Values                                 |
| vUNPCKLPD          | Unpack and Interleave Low Packed Float64 Values                                  |
| vUNPCKLPS          | Unpack and Interleave Low Packed Float32 Values                                  |

### AVX Mask Logic
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| KADDB              | ADD Two Int8 Masks                                                               |
| KADDW              | ADD Two Int16 Masks                                                              |
| KADDD              | ADD Two Int32 Masks                                                              |
| KADDQ              | ADD Two Int64 Masks                                                              |
| KANDNB             | Bitwise AND NOT Int8 Masks                                                       |
| KANDNW             | Bitwise AND NOT Int16 Masks                                                      |
| KANDND             | Bitwise AND NOT Int32 Masks                                                      |
| KANDNQ             | Bitwise AND NOT Int64 Masks                                                      |
| KANDB              | Bitwise AND Int8 Masks                                                           |
| KANDW              | Bitwise AND Int16 Masks                                                          |
| KANDD              | Bitwise AND Int32 Masks                                                          |
| KANDQ              | Bitwise AND Int64 Masks                                                          |
| KMOVB              | Move from and to Int8 Masks                                                      |
| KMOVW              | Move from and to Int16 Masks                                                     |
| KMOVD              | Move from and to Int32 Masks                                                     |
| KMOVQ              | Move from and to Int64 Masks                                                     |
| KNOTB              | Bitwise NOT Int8 Masks                                                           |
| KNOTW              | Bitwise NOT Int16 Masks                                                          |
| KNOTD              | Bitwise NOT Int32 Masks                                                          |
| KNOTQ              | Bitwise NOT Int64 Masks                                                          |
| KORB               | Bitwise OR Int8 Masks                                                            |
| KORW               | Bitwise OR Int16 Masks                                                           |
| KORD               | Bitwise OR Int32 Masks                                                           |
| KORQ               | Bitwise OR Int64 Masks                                                           |
| KORTESTB           | Bitwise OR Int8 Masks and Set Flags                                              |
| KORTESTW           | Bitwise OR Int16 Masks and Set Flags                                             |
| KORTESTD           | Bitwise OR Int32 Masks and Set Flags                                             |
| KORTESTQ           | Bitwise OR Int64 Masks and Set Flags                                             |
| KSHIFTLB           | Shift Left Int8 Masks                                                            |
| KSHIFTLW           | Shift Left Int16 Masks                                                           |
| KSHIFTLD           | Shift Left Int32 Masks                                                           |
| KSHIFTLQ           | Shift Left Int64 Masks                                                           |
| KSHIFTRB           | Shift Right Int8 Masks                                                           |
| KSHIFTRW           | Shift Right Int16 Masks                                                          |
| KSHIFTRD           | Shift Right Int32 Masks                                                          |
| KSHIFTRQ           | Shift Right Int64 Masks                                                          |
| KTESTB             | Bit Test Int8 Masks and Set Flags                                                |
| KTESTW             | Bit Test Int16 Masks and Set Flags                                               |
| KTESTD             | Bit Test Int32 Masks and Set Flags                                               |
| KTESTQ             | Bit Test Int64 Masks and Set Flags                                               |
| KUNPCKBW           | Unpack Int8 Masks to Int16 Masks                                                 |
| KUNPCKWD           | Unpack Int16 Masks to Int32 Masks                                                |
| KUNPCKDQ           | Unpack Int32 Masks to Int64 Masks                                                |
| KXNORB             | Bitwise XNOR Int8 Masks                                                          |
| KXNORW             | Bitwise XNOR Int16 Masks                                                         |
| KXNORD             | Bitwise XNOR Int32 Masks                                                         |
| KXNORQ             | Bitwise XNOR Int64 Masks                                                         |
| KXORB              | Bitwise XOR Int8 Masks                                                           |
| KXORW              | Bitwise XOR Int16 Masks                                                          |
| KXORD              | Bitwise XOR Int32 Masks                                                          |
| KXORQ              | Bitwise XOR Int64 Masks                                                          |

### AVX Mask Conversion
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPMOVB2M           | Convert an Int8 Vector Register to a Mask                                        |
| VPMOVW2M           | Convert an Int16 Vector Register to a Mask                                       |
| VPMOVD2M           | Convert an Int32 Vector Register to a Mask                                       |
| VPMOVQ2M           | Convert an Int64 Vector Register to a Mask                                       |
| VPMOVM2B           | Move Mask Register into Int8 Vector Register                                     |
| VPMOVM2D           | Move Mask Register into Int32 Vector Register                                    |
| VPMOVM2Q           | Move Mask Register into Int64 Vector Register                                    |
| VPMOVM2W           | Move Mask Register into Int16 Vector Register                                    |
| VPTESTMB           | Logical AND of Packed Int8 Values and Set Mask If Non Zero                       |
| VPTESTMW           | Logical AND of Packed Int16 Values and Set Mask If Non Zero                      |
| VPTESTMD           | Logical AND of Packed Int32 Values and Set Mask If Non Zero                      |
| VPTESTMQ           | Logical AND of Packed Int64 Values and Set Mask If Non Zero                      |
| VPTESTNMB          | Logical NAND of Packed Int8 Values and Set Mask If Non Zero                      |
| VPTESTNMW          | Logical NAND of Packed Int16 Values and Set Mask If Non Zero                     |
| VPTESTNMD          | Logical NAND of Packed Int32 Values and Set Mask If Non Zero                     |
| VPTESTNMQ          | Logical NAND of Packed Int64 Values and Set Mask If Non Zero                     |
| VP2INTERSECTD      | Compute Intersection Between Int32 to a Pair of Masks                            |
| VP2INTERSECTQ      | Compute Intersection Between Int64 to a Pair of Masks                            |

### AVX Bitmanip
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPLZCNTD           | Count Leading Zero Bits for Packed Int32 Values                                  |
| VPLZCNTQ           | Count Leading Zero Bits for Packed Int64 Values                                  |
| VPOPCNTB           | Count of Number of Bits Set to 1 in Packed Int8 Values                           |
| VPOPCNTW           | Count of Number of Bits Set to 1 in Packed Int16 Values                          |
| VPOPCNTD           | Count of Number of Bits Set to 1 in Packed Int32 Values                          |
| VPOPCNTQ           | Count of Number of Bits Set to 1 in Packed Int64 Values                          |

### AVX Conflict
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPCONFLICTD        | Detect Conflicts Within a Vector of Packed Int32 Values                          |
| VPCONFLICTQ        | Detect Conflicts Within a Vector of Packed Int64 Values                          |

### AVX Ternary Logic
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPTERNLOGD         | Bitwise Ternary Logic Int32 Granularity                                          |
| VPTERNLOGQ         | Bitwise Ternary Logic Int64 Granularity                                          |

### AVX VNNI
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VPDPBSUD           | Multiply and Add Signed Int8 and Unsigned Int8                                   |
| VPDPBSUDS          | Multiply and Add Signed Int8 and Unsigned Int8 With Signed Saturation            |
| VPDPBUUD           | Multiply and Add Unsigned Int8 and Unsigned Int8                                 |
| VPDPBUUDS          | Multiply and Add Unsigned Int8 and Unsigned Int8 With Unsigned Saturation        |
| VPDPBSSD           | Multiply and Add Signed Int8 and Signed Int8                                     |
| VPDPBSSDS          | Multiply and Add Signed Int8 and Signed Int8 With Signed Saturation              |

### AVX BF16
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VBCSTNESH2PS       | Load FP16 Element and Convert to Float32 Element with Broadcast                  |
| VBCSTNEBF162PS     | Load BF16 Element and Convert to Float32 Element With Broadcast                  |
| VCVTNE2PS2BF16     | Convert Two Registers of Packed Float32 Values to Packed BF16 Values             |
| VCVTNEEBF162PS     | Convert Even Elements of Packed BF16 Values to Packed Float32 Values             |
| VCVTNEEPH2PS       | Convert Even Elements of Packed FP16 Values to Packed Float32 Values             |
| VCVTNEOBF162PS     | Convert Odd Elements of Packed BF16 Values to Packed Float32 Values              |
| VCVTNEOPH2PS       | Convert Odd Elements of Packed FP16 Values to Packed Float32 Values              |
| VCVTNEPS2BF16      | Convert Packed Float32 Values to Packed BF16 Values                              |
| VDPBF16PS          | Dot Product of Packed BF16 Pairs Accumulated Into Packed Float32 Values          |

### AVX Xeon Phi
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VEXP2PD            | Approximation to the Exponential 2^x of Packed Float64 Values                    |
| VEXP2PS            | Approximation to the Exponential 2^x of Packed Float32 Values                    |
| VGATHERPF0DPS      | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T0 Hint |
| VGATHERPF0QPS      | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T0 Hint |
| VGATHERPF0DPD      | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T0 Hint |
| VGATHERPF0QPD      | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T0 Hint |
| VGATHERPF1DPS      | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T1 Hint |
| VGATHERPF1QPS      | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T1 Hint |
| VGATHERPF1DPD      | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T1 Hint |
| VGATHERPF1QPD      | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T1 Hint |
| VP4DPWSSDS         | Dot Product of Signed Int16 With Int32 Accumulation and Saturation (4-Iterations)|
| VP4DPWSSD          | Dot Product of Signed Int16 With Int32 Accumulation (4-Iterations)               |
| V4FMADDPS          | Packed Fused Multiply and Add of Float32 Values (4-Iterations)                   |
| V4FNMADDPS         | Packed Fused Multiply Negate and Add of Float32 Values (4-Iterations)            |
| V4FMADDSS          | Scalar Fused Multiply and Add of Float32 Values (4-Iterations)                   |
| V4FNMADDSS         | Scalar Fused Multiply Negate and Add of Float32 Values (4-Iterations)            |
| VRCP28PD           | Approximation to the Reciprocal of Packed Float64 Values                         |
| VRCP28PS           | Approximation to the Reciprocal of Packed Float32 Values                         |
| VRCP28SD           | Approximation to the Reciprocal of Scalar Float64 Value                          |
| VRCP28SS           | Approximation to the Reciprocal of Scalar Float32 Value                          |
| VRSQRT28PD         | Approximation to the Reciprocal Square Root of Packed Float64 Values             |
| VRSQRT28SD         | Approximation to the Reciprocal Square Root of Scalar Float64 Value              |
| VRSQRT28PS         | Approximation to the Reciprocal Square Root of Packed Float32 Values             |
| VRSQRT28SS         | Approximation to the Reciprocal Square Root of Scalar Float32 Value              |
| VSCATTERPF0DPS     | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T0 Hint |
| VSCATTERPF0QPS     | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T0 Hint |
| VSCATTERPF0DPD     | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T0 Hint |
| VSCATTERPF0QPD     | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T0 Hint |
| VSCATTERPF1DPS     | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T1 Hint |
| VSCATTERPF1QPS     | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T1 Hint |
| VSCATTERPF1DPD     | Sparse Prefetch Packed SP/DP Data Values With Signed Int32 Indices Using T1 Hint |
| VSCATTERPF1QPD     | Sparse Prefetch Packed SP/DP Data Values With Signed Int64 Indices Using T1 Hint |

## Supervisor

### CPUID
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CPUID              | CPU Identification                                                               |
| RDRAND             | Read Random Number                                                               |
| RDSEED             | Read Random SEED                                                                 |
| RDTSC              | Read Time-Stamp Counter                                                          |
| RDTSCP             | Read Time-Stamp Counter and Processor ID                                         |

### Privileged
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CLAC               | Clear AC Flag in EFLAGS Register                                                 |
| CLI                | Clear Interrupt Flag                                                             |
| CLTS               | Clear Task-Switched Flag in CR0                                                  |
| CLDEMOTE           | Cache Line Demote                                                                |
| CLFLUSH            | Flush Cache Line                                                                 |
| CLFLUSHOPT         | Flush Cache Line Optimized                                                       |
| CLWB               | Cache Line Write Back                                                            |
| INVD               | Invalidate Internal Caches                                                       |
| INVEPT             | Invalidate Translations Derived from EPT                                         |
| INVLPG             | Invalidate TLB Entries                                                           |
| INVPCID            | Invalidate Process-Context Identifier                                            |
| INVVPID            | Invalidate Translations Based on VPID                                            |
| HLT                | Halt                                                                             |
| HRESET             | History Reset                                                                    |
| IN                 | Input from Port                                                                  |
| INSB               | Input from Port to String Int8                                                   |
| INSW               | Input from Port to String Int16                                                  |
| INSD               | Input from Port to String Int32                                                  |
| IRET               | Interrupt Return                                                                 |
| IRETW              | Interrupt Return Int16                                                           |
| IRETD              | Interrupt Return Int32                                                           |
| IRETQ              | Interrupt Return Int64                                                           |
| LAR                | Load Access Rights Byte                                                          |
| LSS                | Load Far Pointer SS                                                              |
| LDS                | Load Far Pointer DS                                                              |
| LES                | Load Far Pointer ES                                                              |
| LFS                | Load Far Pointer FS                                                              |
| LGS                | Load Far Pointer GS                                                              |
| LGDT               | Load Global Descriptor Table Register                                            |
| LIDT               | Load Interrupt Descriptor Table Register                                         |
| LLDT               | Load Local Descriptor Table Register                                             |
| LMSW               | Load Machine Status Word                                                         |
| LSL                | Load Segment Limit                                                               |
| LTR                | Load Task Register                                                               |
| MOV                | Move                                                                             |
| MONITOR            | Set Up Monitor Address                                                           |
| MWAIT              | Monitor Wait                                                                     |
| OUT                | Output to Port                                                                   |
| OUTSB              | Output String to Port Int8                                                       |
| OUTSW              | Output String to Port Int16                                                      |
| OUTSD              | Output String to Port Int32                                                      |
| PCONFIG            | Platform Configuration                                                           |
| PTWRITE            | Write Data to a Processor Trace Packet                                           |
| RDFSBASE           | Read FS Segment Base                                                             |
| RDGSBASE           | Read GS Segment Base                                                             |
| RDMSR              | Read from Model Specific Register                                                |
| RDMSRLIST          | Read List of Model Specific Registers                                            |
| RDPID              | Read Processor ID                                                                |
| RDPKRU             | Read Protection Key Rights for User Pages                                        |
| RDPMC              | Read Performance-Monitoring Counters                                             |
| RSM                | Resume from System Management Mode                                               |
| SGDT               | Store Global Descriptor Table Register                                           |
| SIDT               | Store Interrupt Descriptor Table Register                                        |
| SLDT               | Store Local Descriptor Table Register                                            |
| SMSW               | Store Machine Status Word                                                        |
| STAC               | Set AC Flag in EFLAGS Register                                                   |
| STI                | Set Interrupt Flag                                                               |
| STR                | Store Task Register                                                              |
| SWAPGS             | Swap GS Base Register                                                            |
| SYSCALL            | Fast System Call (Intel Long Mode)                                               |
| SYSRET             | Return From Fast System Call (Intel Long Mode)                                   |
| SYSENTER           | Fast System Call (AMD Legacy Mode)                                               |
| SYSEXIT            | Return from Fast System Call (AMD Legacy Mode)                                   |
| VERR               | Verify a Segment for Reading                                                     |
| VERW               | Verify a Segment for Writing                                                     |
| WBINVD             | Write Back and Invalidate Cache                                                  |
| WBNOINVD           | Write Back and Do Not Invalidate Cache                                           |
| WRFSBASE           | Write FS Segment Base                                                            |
| WRGSBASE           | Write GS Segment Base                                                            |
| WRMSR              | Write to Model Specific Register                                                 |
| WRMSRNS            | Non-Serializing Write to Model Specific Register                                 |
| WRMSRLIST          | Write List of Model Specific Registers                                           |
| WRPKRU             | Write Protection Key Rights for User Pages                                       |
| XRSTORS            | Restore Processor Extended States Supervisor                                     |
| XSAVES             | Save Processor Extended States Supervisor                                        |
| XSETBV             | Set Extended Control Register                                                    |

## Extensions

### AES
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vAESDEC            | Perform One Round of an AES Decryption Flow                                      |
| vAESDECLAST        | Perform Last Round of an AES Decryption Flow                                     |
| vAESENC            | Perform One Round of an AES Encryption Flow                                      |
| vAESENCLAST        | Perform Last Round of an AES Encryption Flow                                     |
| vAESIMC            | Perform the AES InvMixColumn Transformation                                      |
| vAESKEYGENASSIST   | AES Round Key Generation Assist                                                  |

### AES Key Locker
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| AESDEC128KL        | Perform Ten Rounds of Key Locker AES Decryption Using 128-Bit Key                |
| AESDEC256KL        | Perform 14 Rounds of Key Locker AES Decryption Using 256-Bit Key                 |
| AESDECWIDE128KL    | Perform Ten Rounds of Key Locker AES Decryption on 8 Blocks Using 128-Bit Key    |
| AESDECWIDE256KL    | Perform 14 Rounds of Key Locker AES Decryption on 8 Blocks Using 256-Bit Key     |

### SHA
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| SHA1MSG1           | Perform Intermediate Calculation for the Next Four SHA1 Message Dwords           |
| SHA1MSG2           | Perform Final Calculation for the Next Four SHA1 Message Dwords                  |
| SHA1NEXTE          | Calculate SHA1 State Variable E after Four Rounds                                |
| SHA1RNDS4          | Perform Four Rounds of SHA1 Operation                                            |
| SHA256MSG1         | Perform Intermediate Calculation for the Next Four SHA256 Message Dwords         |
| SHA256MSG2         | Perform Final Calculation for the Next Four SHA256 Message Dwords                |
| SHA256RNDS2        | Perform Two Rounds of SHA256 Operation                                           |

### PCLMULQDQ
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vPCLMULQDQ         | Carry-Less Multiplication Quadword                                               |

### GFNI
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| vGF2P8AFFINEINVQB  | Galois Field Affine Transformation Inverse                                       |
| vGF2P8AFFINEQB     | Galois Field Affine Transformation                                               |
| vGF2P8MULB         | Galois Field Multiply Bytes                                                      |

### CET
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| ENDBR32            | Terminate an Indirect Branch in 32-bit and Compatibility Mode                    |
| ENDBR64            | Terminate an Indirect Branch in 64-bit Mode                                      |
| CLRSSBSY           | Clear Busy Flag in a Supervisor Shadow Stack Token                               |
| INCSSPD            | Increment Shadow Stack Pointer Int32                                             |
| INCSSPQ            | Increment Shadow Stack Pointer Int64                                             |
| RDSSPD             | Read Shadow Stack Pointer Int32                                                  |
| RDSSPQ             | Read Shadow Stack Pointer Int64                                                  |
| RSTORSSP           | Restore Saved Shadow Stack Pointer                                               |
| SAVEPREVSSP        | Save Previous Shadow Stack Pointer                                               |
| SETSSBSY           | Mark Shadow Stack Busy                                                           |
| WRSSD              | Write to Shadow Stack Int32                                                      |
| WRSSQ              | Write to Shadow Stack Int64                                                      |
| WRUSSD             | Write to User Shadow Stack Int32                                                 |
| WRUSSQ             | Write to User Shadow Stack Int64                                                 |

### AMX
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| TILELOADD          | Load Tile                                                                        |
| TILELOADDT1        | Load Tile With Data Caching                                                      |
| TILERELEASE        | Release Tile                                                                     |
| TILESTORED         | Store Tile                                                                       |
| TILEZERO           | Zero Tile                                                                        |
| LDTILECFG          | Load tile configuration                                                          |
| STTILECFG          | Store tile configuration                                                         |
| TDPBF16PS          | Tiled Dot Product of BF16 Tiles Accumulated into Packed Float32 Tile             |
| TDPBSSD            | Tiled Dot product of Signed Int8 and Signed Int8 with Int32 accumulation         |
| TDPBSUD            | Tiled Dot product of Signed Int8 and Unigned Int8 bytes with Int32 accumulation  |
| TDPBUSD            | Tiled Dot product of Unsigned Int8 and Signed Int8 with Int32 accumulation       |
| TDPBUUD            | Tiled Dot product of Unsigned Int8 and Unsigned Int8 with Int32 accumulation     |

### VMX
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| VMCALL             | Call Virtual Machine Monitor                                                     |
| VMCLEAR            | Clear Virtual-Machine Control Structure                                          |
| VMFUNC             | Invoke Virtual Machine Function                                                  |
| VMLAUNCH           | Launch Virtual Machine                                                           |
| VMRESUME           | Resume Virtual Machine                                                           |
| VMPTRLD            | Load Pointer to Virtual-Machine Control Structure                                |
| VMPTRST            | Store Pointer to Virtual-Machine Control Structure                               |
| VMREAD             | Read Field from Virtual-Machine Control Structure                                |
| VMWRITE            | Write Field to Virtual-Machine Control Structure                                 |
| VMXOFF             | Leave VMX Operation                                                              |
| VMXON              | Enter VMX Operation                                                              |

### User Interrupts
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CLUI               | Clear user interrupt flag                                                        |
| STUI               | Set user interrupt flag                                                          |
| SENDUIPI           | Send user interprocessor interrupt                                               |
| TESTUI             | Determine user interrupt flag                                                    |
| TPAUSE             | Timed PAUSE                                                                      |
| UMONITOR           | User Level Set Up Monitor Address                                                |
| UMWAIT             | User Level Monitor Wait                                                          |
| UIRET              | User-interrupt return                                                            |

### Enqueue Store
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| ENQCMD             | Enqueue command                                                                  |
| ENQCMDS            | Enqueue command supervisor                                                       |

### AMD
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| BLCFILL            | Fill From Lowest Clear Bit                                                       |
| BLSFILL            | Fill From Lowest Set Bit                                                         |
| BLCI               | Isolate Lowest Clear Bit                                                         |
| BLCIC              | Bit Lowest Clear Isolate Complemented                                            |
| BLCS               | Set Lowest Clear Bit                                                             |
| BLSIC              | Isolate Lowest Set Bit and Complement                                            |
| BLCMSK             | Mask From Lowest Clear Bit                                                       |
| EXTRQ              | Extract Field                                                                    |
| INSERTQ            | Insert Field                                                                     |
| T1MSKC             | Inverse Mask From Trailing Ones                                                  |
| TZMSK              | Mask From Trailing Zeros                                                         |
| LLWPCB             | Load Lightweight Profiling Control Block Address                                 |
| SLWPCB             | Store Lightweight Profiling Control Block Address                                |
| LWPVAL             | Insert Value Sample in Lightweight Profiling Ring Buffer                         |
| LWPINS             | Insert User Event Record in Lightweight Profiling Ring Buffer                    |
| MOVNTSD            | Move Non-Temporal Scalar Float64 Value                                           |
| MOVNTSS            | Move Non-Temporal Scalar Float32 Value                                           |
| MONITORX           | Setup Monitor Address                                                            |
| MWAITX             | Monitor Wait with Timeout                                                        |
| PAVGUSB            | Average Packed Unsigned Packed Int8 Values                                       |
| PF2IW              | Convert Packed Float32 Values to Int16 Values                                    |
| PF2ID              | Convert Packed Float32 Values to Int32 Values                                    |
| PFACC              | Accumulate Packed Float32 Values                                                 |
| PFNACC             | Negative Accumulate Packed Float32 Values                                        |
| PFPNACC            | Positive-Negative Accumulate Packed Float32 Values                               |
| PFADD              | Add Packed Float32 Values                                                        |
| PFSUB              | Subtract Packed Float32 Values                                                   |
| PFSUBR             | Subtract Reverse Packed Float32 Values                                           |
| PFMUL              | Multiply Packed Float32 Values                                                   |
| PFRCP              | Reciprocal Approximation Packed Float32 Values                                   |
| PFRCPIT1           | Reciprocal Packed Float32 Values, Iteration 1                                    |
| PFRCPIT2           | Reciprocal or Reciprocal Square Root Packed Float32 Values, Iteration 2          |
| PFRSQRT            | Reciprocal Square Root Approximation Packed Float32 Values                       |
| PFRSQIT1           | Reciprocal Square Root Packed Float32 Values, Iteration 1                        |
| PFCMPEQ            | Compare Equal Packed Float32 Values                                              |
| PFCMPGT            | Compare Greater Than Packed Float32 Values                                       |
| PFCMPGE            | Compare Greater or Equal Packed Float32 Values                                   |
| PFMAX              | Maximum Packed Float32 Values                                                    |
| PFMIN              | Minimum Packed Float32 Values                                                    |
| PI2FW              | Convert Two Packed Int16 Values To Float32 Values                                |
| PI2FD              | Convert Two Packed Int32 Values To Float32 Values                                |
| PREFETCH           | Prefetch Data Into Caches                                                        |
| PSWAPD             | Swap Packed Int32 Values                                                         |
| VFMADDSD           | Fused Multiply-Add of Scalar Float64 Values                                      |
| VFMADDSS           | Fused Multiply-Add of Scalar Float32 Values                                      |
| VFMSUBSD           | Fused Multiply-Sub of Scalar Float64 Values                                      |
| VFMSUBSS           | Fused Multiply-Sub of Scalar Float32 Values                                      |
| VFNMADDSD          | Fused Negative Multiply-Add of Scalar Float64 Values                             |
| VFNMADDSS          | Fused Negative Multiply-Add of Scalar Float32 Values                             |
| VFNMSUBSD          | Fused Negative Multiply-Sub of Scalar Float64 Values                             |
| VFNMSUBSS          | Fused Negative Multiply-Sub of Scalar Float32 Values                             |
| VFMADDPD           | Fused Multiply-Add of Packed Float64 Values                                      |
| VFMADDPS           | Fused Multiply-Add of Packed Float32 Values                                      |
| VFMSUBPD           | Fused Multiply-Sub of Packed Float64 Values                                      |
| VFMSUBPS           | Fused Multiply-Sub of Packed Float32 Values                                      |
| VFMADDSUBPD        | Fused Multiply-Alternating Add/Sub of Packed Float64 Values                      |
| VFMADDSUBPS        | Fused Multiply-Alternating Add/Sub of Packed Float32 Values                      |
| VFMSUBADDPD        | Fused Multiply-Alternating Sub/Add of Packed Float64 Values                      |
| VFMSUBADDPS        | Fused Multiply-Alternating Sub/Add of Packed Float32 Values                      |
| VFNMADDPD          | Fused Negative Multiply-Add of Packed Float64 Values                             |
| VFNMADDPS          | Fused Negative Multiply-Add of Packed Float32 Values                             |
| VFNMSUBPD          | Fused Negative Multiply-Sub of Packed Float64 Values                             |
| VFNMSUBPS          | Fused Negative Multiply-Sub of Packed Float32 Values                             |
| VFRCZPD            | Extract Fraction Packed Float64 Values                                           |
| VFRCZPS            | Extract Fraction Packed Float32 Values                                           |
| VFRCZSD            | Extract Fraction Scalar Float64 Value                                            |
| VFRCZSS            | Extract Fraction Scalar Float32 Value                                            |
| VPCMOV             | Vector Conditional Move                                                          |
| VPCOMB             | Compare Packed Signed Int8 Values                                                |
| VPCOMW             | Compare Packed Signed Int16 Values                                               |
| VPCOMD             | Compare Packed Signed Int32 Values                                               |
| VPCOMQ             | Compare Packed Signed Int64 Values                                               |
| VPCOMUB            | Compare Packed Unsigned Int8 Values                                              |
| VPCOMUW            | Compare Packed Unsigned Int16 Values                                             |
| VPCOMUD            | Compare Packed Unsigned Int32 Values                                             |
| VPCOMUQ            | Compare Packed Unsigned Int64 Values                                             |
| VPHADDBW           | Packed Horizontal Add Signed Int8 to Signed Int16                                |
| VPHADDBD           | Packed Horizontal Add Signed Int8 to Signed Int32                                |
| VPHADDBQ           | Packed Horizontal Add Signed Int8 to Signed Int64                                |
| VPHADDDQ           | Packed Horizontal Add Signed Int32 to Signed Int64                               |
| VPHADDUBW          | Packed Horizontal Add Unsigned Int8 to Int16                                     |
| VPHADDUBD          | Packed Horizontal Add Unsigned Int8 to Int32                                     |
| VPHADDUBQ          | Packed Horizontal Add Unsigned Int8 to Int64                                     |
| VPHADDUWD          | Packed Horizontal Add Unsigned Int16 to Int32                                    |
| VPHADDUWQ          | Packed Horizontal Add Unsigned Int16 to Int64                                    |
| VPHADDUDQ          | Packed Horizontal Add Unsigned Int32 to Int64                                    |
| VPHADDWD           | Packed Horizontal Add Signed Int16 to Signed Int32                               |
| VPHADDWQ           | Packed Horizontal Add Signed Int16 to Signed Int64                               |
| VPHSUBBW           | Packed Horizontal Subtract Signed Int8 to Int16 Values                           |
| VPHSUBWD           | Packed Horizontal Subtract Signed Int16 to Int32 Values                          |
| VPHSUBDQ           | Packed Horizontal Subtract Signed Int32 to Int64 Values                          |
| VPMACSSWW          | Packed Multiply Accumulate Signed Int16 to Int16 Values with Saturation          |
| VPMACSWW           | Packed Multiply Accumulate Signed Int16 to Int16 Values                          |
| VPMACSSWD          | Packed Multiply Accumulate Signed Int16 to Int32 Values with Saturation          |
| VPMACSWD           | Packed Multiply Accumulate Signed Int16 to Int32 Values                          |
| VPMACSSDD          | Packed Multiply Accumulate Signed Int32 to Int32 Values with Saturation          |
| VPMACSDD           | Packed Multiply Accumulate Signed Int32 to Int32 Values                          |
| VPMACSSDQL         | Packed Multiply Accumulate Signed Low Int32 to Int64 Values with Saturation      |
| VPMACSSDQH         | Packed Multiply Accumulate Signed High Int32 to Int64 Values with Saturation     |
| VPMACSDQL          | Packed Multiply Accumulate Signed Low Int32 to Int64 Values                      |
| VPMACSDQH          | Packed Multiply Accumulate Signed High Int32 to Int64 Values                     |
| VPPERM             | Packed Permute Int8 Values                                                       |
| VPROTB             | Packed Rotate Int8 Values                                                        |
| VPROTW             | Packed Rotate Int16 Values                                                       |
| VPROTD             | Packed Rotate Int32 Values                                                       |
| VPROTQ             | Packed Rotate Int64 Values                                                       |
| VPMADCSSWD         | Packed Multiply Add and Accumulate Signed Int16 to Int32 Values with Saturation  |
| VPMADCSWD          | Packed Multiply Add and Accumulate Signed Int16 to Int32 Values                  |
| VPSHLB             | Packed Shift Logical Int8 Values                                                 |
| VPSHLW             | Packed Shift Logical Int16 Values                                                |
| VPSHLD             | Packed Shift Logical Int32 Values                                                |
| VPSHLQ             | Packed Shift Logical Int64 Values                                                |
| VPSHAB             | Packed Shift Arithmetic Int8 Values                                              |
| VPSHAW             | Packed Shift Arithmetic Int16 Values                                             |
| VPSHAD             | Packed Shift Arithmetic Int32 Values                                             |
| VPSHAQ             | Packed Shift Arithmetic Int64 Values                                             |

### AMD Privileged
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| INVLPGA            | Invalidate TLB Entry in a Specified ASID                                         |

### AMD VMX
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| CLGI               | Clear Global Interrupt Flag                                                      |
| STGI               | Set Global Interrupt Flag                                                        |
| PVALIDATE          | Page Validate                                                                    |
| RMPADJUST          | Adjust RMP Permissions                                                           |
| VMGEXIT            | Virtual Machine Global Exit                                                      |
| VMLOAD             | Load State from Virtual Machine Control Block                                    |
| VMMCALL            | Call Virtual Machine Monitor                                                     |
| VMRUN              | Run Virtual Machine                                                              |
| VMSAVE             | Save State to Virtual Machine Control Block                                      |

## Legacy

### x87
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| F2XM1              | FP Compute 2x–1                                                                  |
| FABS               | FP Absolute                                                                      |
| FADD               | FP Add                                                                           |
| FADDP              | FP Add and Pop                                                                   |
| FBLD               | FP Load Binary Coded Decimal                                                     |
| FBSTP              | FP Store BCD Integer and Pop                                                     |
| FCHS               | FP Complement Sign                                                               |
| FCLEX              | FP Clear Exceptions After Checking Pending Unmasked Exceptions                   |
| FCMOVBE            | FP Conditional Move If Below Equal                                               |
| FCMOVB             | FP Conditional Move If Below                                                     |
| FCMOVE             | FP Conditional Move If Equal                                                     |
| FCMOVNBE           | FP Conditional Move If Not Below Equal                                           |
| FCMOVNB            | FP Conditional Move If Not Below                                                 |
| FCMOVNE            | FP Conditional Move If Not Equal                                                 |
| FCMOVNU            | FP Conditional Move If Not Unordered                                             |
| FCMOVU             | FP Conditional Move If Unordered                                                 |
| FCOM               | FP Compare                                                                       |
| FCOMI              | FP Compare Unordered and Set EFLAGS                                              |
| FCOMIP             | FP Compare Unordered, Pop and Set EFLAGS                                         |
| FCOMP              | FP Compare and Pop                                                               |
| FCOMPP             | FP Compare and Pop Twice                                                         |
| FCOS               | FP Cosine                                                                        |
| FDECSTP            | FP Decrement Stack-Top Pointer                                                   |
| FDIV               | FP Divide                                                                        |
| FDIVP              | FP Divide and Pop                                                                |
| FDIVR              | FP Reverse Divide                                                                |
| FDIVRP             | FP Reverse Divide and Pop                                                        |
| FFREE              | FP Free Register                                                                 |
| FIADD              | FP Add Integer                                                                   |
| FICOM              | FP Compare Integer                                                               |
| FICOMP             | FP Compare Integer and Pop                                                       |
| FIDIV              | FP Divide Integer                                                                |
| FIDIVR             | FP Reverse Divide Integer                                                        |
| FILD               | FP Load Integer                                                                  |
| FIMUL              | FP Multiply Integer                                                              |
| FINCSTP            | FP Increment Stack-Top Pointer                                                   |
| FINIT              | FP Initialize Unit After Checking Pending Unmasked Exceptions                    |
| FIST               | FP Store FP Integer                                                              |
| FISTP              | FP Store FP Integer and Pop                                                      |
| FISTTP             | FP Store Integer with Truncation                                                 |
| FISUB              | FP Subtract Integer                                                              |
| FISUBR             | FP Reverse Subtract Integer                                                      |
| FLD1               | FP Load Constant One                                                             |
| FLDCW              | FP Load Control Word                                                             |
| FLDENV             | FP Load Environment                                                              |
| FLD                | FP Load                                                                          |
| FLDL2E             | FP Load Constant Log2 e                                                          |
| FLDL2T             | FP Load Constant Log2 Ten                                                        |
| FLDLG2             | FP Load Constant Log10 Two                                                       |
| FLDLN2             | FP Load Constant Loge Two                                                        |
| FLDPI              | FP Load Constant Pi                                                              |
| FLDZ               | FP Load Constant Zero                                                            |
| FMUL               | FP Multiply                                                                      |
| FMULP              | FP Multiply and Pop                                                              |
| FNCLEX             | FP Clear Exceptions Without Checking Pending Unmasked Exceptions                 |
| FNINIT             | FP Initialize Unit Without Checking Pending Unmasked Exceptions                  |
| FNOP               | FP No Operation                                                                  |
| FNSAVE             | FP Store State Without Checking Pending Unmasked Exceptions                      |
| FNSTCW             | FP Store Control Word Without Checking Pending Unmasked Exceptions               |
| FNSTENV            | FP Store Environment Without Checking Pending Unmasked Exceptions                |
| FNSTSW             | FP Store Status Word Without Checking Pending Unmasked Exceptions                |
| FPATAN             | FP Partial Arctangent                                                            |
| FPREM1             | FP Partial Remainder                                                             |
| FPREM              | FP Partial Remainder                                                             |
| FPTAN              | FP Partial Tangent                                                               |
| FRNDINT            | FP Round to Integer                                                              |
| FRSTOR             | FP Restore State                                                                 |
| FSAVE              | FP Store State After Checking Pending Unmasked Exceptions                        |
| FSCALE             | FP Scale                                                                         |
| FSINCOS            | FP Sine and Cosine                                                               |
| FSIN               | FP Sine                                                                          |
| FSQRT              | FP Square Root                                                                   |
| FSTCW              | FP Store Control Word After Checking Pending Unmasked Exceptions                 |
| FSTENV             | FP Store Environment After Checking Pending Unmasked Exceptions                  |
| FST                | FP Store                                                                         |
| FSTP               | FP Store and Pop                                                                 |
| FSTSW              | FP Store Status Word After Checking Pending Unmasked Exceptions                  |
| FSUB               | FP Subtract                                                                      |
| FSUBP              | FP Subtract and Pop                                                              |
| FSUBR              | FP Reverse Subtract                                                              |
| FSUBRP             | FP Reverse Subtract and Pop                                                      |
| FTST               | FP Test                                                                          |
| FUCOM              | FP Unordered Compare Values                                                      |
| FUCOMI             | FP Compare Ordered and Set EFLAGS                                                |
| FUCOMIP            | FP Compare Ordered, Pop and Set EFLAGS                                           |
| FUCOMP             | FP Unordered Compare Values and Pop                                              |
| FUCOMPP            | FP Unordered Compare Values and Pop Twice                                        |
| FWAIT              | FP Wait Unmasked Exceptions                                                      |
| FXAM               | FP Examine and Classify Values                                                   |
| FXCH               | FP Exchange Register Contents                                                    |
| FXRSTOR            | FP Restore FPU, MMX, XMM, and MXCSR State                                        |
| FXSAVE             | FP Save FPU, MMX, XMM and MXCSR State                                            |
| FXTRACT            | FP Extract Exponent and Significand                                              |
| FYL2X              | FP Compute y * log2x                                                             |
| FYL2XP1            | FP Compute y * log2(x +1)                                                        |

### MPX
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| BNDCL              | Check Lower Bound                                                                |
| BNDCN              | Check Upper Bound Not Ones Complement                                            |
| BNDCU              | Check Upper Bound Ones Complement                                                |
| BNDLDX             | Load Extended Bounds Using Address Translation                                   |
| BNDMK              | Make Bounds                                                                      |
| BNDMOV             | Move Bounds                                                                      |
| BNDSTX             | Store Extended Bounds Using Address Translation                                  |

### SGX
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| ENCLU              | Execute an Enclave User Function of Specified Leaf Number                        |
| ENCLS              | Execute an Enclave System Function of Specified Leaf Number                      |
| ENCLV              | Execute an Enclave Virtual Function of Specified Leaf Number                     |

### TSX
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| XABORT             | Transactional Abort                                                              |
| XBEGIN             | Transactional Begin                                                              |
| XEND               | Transactional End                                                                |
| XACQUIRE           | Hardware Lock Elision Acquire Hint                                               |
| XRELEASE           | Hardware Lock Elision Release Hint                                               |
| XTEST              | Test If In Transactional Execution                                               |
| XRESLDTRK          | Resume tracking load addresses                                                   |
| XSUSLDTRK          | Suspend tracking load addresses                                                  |

### Obsolete
| opcode             | description                                                                      |
|:-------------------|:---------------------------------------------------------------------------------|
| AAA                | ASCII Adjust After Addition                                                      |
| AAD                | Adjust AX Before Division                                                        |
| AAM                | ASCII Adjust AX After Multiply                                                   |
| AAS                | ASCII Adjust AL After Subtraction                                                |
| ARPL               | Adjust RPL Field of Segment Selector                                             |
| BOUND              | Check Array Index Against Bounds                                                 |
| DAA                | Decimal Adjust AL after Addition                                                 |
| DAS                | Decimal Adjust AL after Subtraction                                              |
| INTO               | Call to Interrupt Procedure If Overflow                                          |
| UD0                | Undefined instruction 1                                                          |
| XLATB              | Table Look-up Translation Byte                                                   |
| XLAT               | Table Look-up Translation Word                                                   |
