cmake_minimum_required(VERSION 3.16)
project(Assembler C)

set(CMAKE_C_STANDARD 90)

add_executable(Assembler assembler.c system.h utils.h utils.c tableMgmt.c firstStage.c firstStage.h secondStage.c stages.h structs.h)
