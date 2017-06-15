
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=core-avx2")
include_directories (${CMAKE_CURRENT_LIST_DIR})
set(FASTNOISE_SIMD_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD.h
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_avx2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_avx512.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_internal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_neon.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_sse2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FastNoiseSIMD_sse41.cpp
)
