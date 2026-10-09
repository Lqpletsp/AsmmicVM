cmake_minimum_required(VERSION 3.10)

project(ASMMICVM C)

# Force C11 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json for LSP autocompletion and diagnostic tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Decoder
)

set(SOURCES
    main.c
    VM.c
    Decoder/Decoder.c
)

add_executable(ASMMICVM ${SOURCES})
