cmake_minimum_required (VERSION 3.12)

project(x86)

find_package(Python3)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
# x86 lib
#
file(GLOB x86_metadata data/*.csv)

LIST(APPEND x86_gen_opt --print-reg-enum --print-reg-strings
    --print-opcode-enum --print-opcode-strings --print-opcode-table
    --print-operand-table --print-order-table)

LIST(APPEND x86_gen_src x86_reg_enum.inc x86_reg_strings.inc
    x86_op_enum.inc x86_op_strings.inc x86_opc_table.inc
    x86_opr_table.inc x86_ord_table.inc)

foreach(gen_opt gen_src IN ZIP_LISTS x86_gen_opt x86_gen_src)
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/gen/${gen_src}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${Python3_EXECUTABLE}
        ARGS ${CMAKE_SOURCE_DIR}/scripts/x86_tablegen.py ${gen_opt}
            > ${CMAKE_BINARY_DIR}/gen/${gen_src}
        DEPENDS ${x86_metadata} VERBATIM)
    list(APPEND x86_gen_dep ${CMAKE_BINARY_DIR}/gen/${gen_src})
endforeach()

add_custom_target(x86_generated_source DEPENDS ${x86_gen_dep})
include_directories(include ${CMAKE_BINARY_DIR}/gen)

add_library(x86 STATIC lib/x86.c)
add_dependencies(x86 x86_generated_source)

#
# x86 tests
#
foreach (test x86_001 x86_002 x86_003 x86_004 x86_005 x86_006)
    add_executable(${test} tests/${test}.c)
    target_link_libraries(${test} x86)
endforeach()

add_executable(x86_opcodes tests/x86_opcodes.c)
target_link_libraries(x86_opcodes x86)

add_executable(x86_disasm tests/x86_disasm.c)
target_link_libraries(x86_disasm x86)
