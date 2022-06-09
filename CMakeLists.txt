# CMakeList.txt : CMake project for BigInt & VarInt stuff

cmake_minimum_required (VERSION 3.8)

project (VarInt)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("submodules/googletest")

add_library (VarIntLib "VarInt.cpp" "VarInt.h")

add_executable (VarInt_Test main.cpp "VarInt_Test.cpp" "VarInt_Test.h" )
target_link_libraries(VarInt_Test VarIntLib)

enable_testing()
# add_test(NAME VarInt_GTest)
add_executable(VarInt_GTest VarInt_GTest.cpp)
target_link_libraries(VarInt_GTest VarIntLib GTest::gtest)



