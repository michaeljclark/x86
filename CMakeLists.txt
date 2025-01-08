cmake_minimum_required (VERSION 3.12)

project(x86)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

LIST(APPEND x86_gen_opt --print-opcode-enums --print-opcode-tables)
LIST(APPEND x86_gen_src x86_opc_enums.inc x86_opc_tables.inc)

add_custom_target(gen_dir ALL COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_BINARY_DIR}/gen/${gen_src})
foreach(gen_opt gen_src IN ZIP_LISTS x86_gen_opt x86_gen_src)
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/gen/${gen_src}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${Python3_EXECUTABLE}
        ARGS ${CMAKE_SOURCE_DIR}/scripts/x86-tablegen.py ${gen_opt}
            > ${CMAKE_BINARY_DIR}/gen/${gen_src}
        DEPENDS ${x86_metadata} gen_dir VERBATIM)
    list(APPEND x86_gen_dep ${CMAKE_BINARY_DIR}/gen/${gen_src})
endforeach()

add_custom_target(x86_generated_source DEPENDS ${x86_gen_dep})

include_directories(include ${CMAKE_BINARY_DIR}/gen)
add_library(x86 STATIC lib/x86-core.c)
add_dependencies(x86 x86_generated_source)
add_executable(x86_bench tests/x86_bench.c)
add_executable(x86_dump tests/x86_dump.c)
add_executable(x86_opcodes tests/x86_opcodes.c)
target_link_libraries(x86_bench x86)
target_link_libraries(x86_dump x86)
target_link_libraries(x86_opcodes x86)
