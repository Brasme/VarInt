# CMakeList.txt : CMake project for BigInt & VarInt stuff

cmake_minimum_required (VERSION 3.8)

project (BigInt)

add_library (BigInt "BigIntTest.cpp" "BigIntTest.h" "BigInt.cpp" "BigInt.h")
add_library (VarInt "VarIntTest.cpp" "VarIntTest.h" "VarInt.cpp" "VarInt.h")

add_executable (BigAndVarIntTest main.cpp)
target_link_libraries(BigAndVarIntTest BigInt VarInt)

