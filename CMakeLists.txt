cmake_minimum_required(VERSION 3.25)
project(Compiler_CPP)

set(CMAKE_CXX_STANDARD 23)

add_executable(Compiler_CPP main.cpp
        Libs/Lexer.cpp
        Libs/Lexer.h
        Libs/Helper.cpp
        Libs/Helper.h
        Libs/Tokens.cpp
        Libs/Tokens.h
        Libs/Errors.cpp
        Libs/Errors.h
        Libs/PreProcesser.cpp
        Libs/PreProcesser.h
        Libs/PreProcesserHelper.cpp
        Libs/PreProcesserHelper.h
        #Libs/Tree.cpp
        #Libs/Tree.h
        #Libs/TreeNodesRevamped.cpp
        #Libs/TreeNodesRevamped.h
        #Libs/TreeHelper.cpp
        #Libs/TreeHelper.h
)
