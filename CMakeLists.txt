cmake_minimum_required(VERSION 3.18.2)

project(encode_decode)

set(CMAKE_C_STANDARD 11) 

add_executable(encode-checksum encode-checksum.c)
add_executable(decode-checksum decode-checksum.c)
add_executable(encode-parity encode-parity.c)
add_executable(decode-parity decode-parity.c)
add_executable(encode-crc32 encode-crc32.c ioutil.c)
add_executable(decode-crc32 decode-crc32.c ioutil.c)

target_compile_options(encode-checksum PRIVATE -Wpedantic -Wall -Wextra -g)

