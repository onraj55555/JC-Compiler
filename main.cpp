#include <iostream>
#include "Libs/Lexer.h"
#include "Libs/PreProcesser.h"

#define FILE "testFile.txt"

/*
 * For automated testing on windows and linux
 */
#ifdef _WIN32
#define PATH "D:\\Programming\\C++\\Compiler_CPP\\" FILE
#elif __linux__
#define PATH "/home/onraj/Documents/Programming/C++/Compiler_CPP/" FILE
#endif

int main() {
	Lexer lexer(PATH);
    try {
        lexer.lex();
    } catch (const CompilerError * compilerError) {
        std::cout << *compilerError << std::endl;
        delete compilerError;
        return 1;
    }

    std::cout << "Lexer output" << std::endl;
    lexer.print();
    PreProcesser preProcesser(lexer.get_output());
    std::cout << "Prepocesser output" << std::endl;
    try {
        preProcesser.process();
    } catch (const CompilerError * compilerError) {
        std::cout << *compilerError << std::endl;
        delete compilerError;
        return 2;
    }



    return 0;
}