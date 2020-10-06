cmake_minimum_required(VERSION 3.18.2)

project(encode_decode)

set(CMAKE_C_STANDARD 11) 

ADD_LIBRARY(
    LibsModule 
    ioutil.c
    )

add_executable(encodeChecksum encodeChecksum.c ioutil.c)
add_executable(decodeChecksum decodeChecksum.c ioutil.c)
add_executable(encodeParity encodeParity.c ioutil.c)
add_executable(decodeParity decodeParity.c ioutil.c)
add_executable(encode-crc32 encodeCRC32.c ioutil.c)
add_executable(decode-crc32 decodeCRC32.c ioutil.c)

target_compile_options(encodeChecksum PRIVATE -Wpedantic -Wall -Wextra)

