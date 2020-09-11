cmake_minimum_required(VERSION 3.17)
project(assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assembler
        assembler.c
        firstStage.c
        secondStage.c
        stages.h
        structs.h
        system.h
        tableMgmt.c
        utils.c
        utils.h)
