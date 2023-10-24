//
// Created by jarno on 22/08/2023.
//

#ifndef COMPILER_CPP_LEXER_H
#define COMPILER_CPP_LEXER_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include "Tokens.h"
#include "Helper.h"
#include "Errors.h"


// TODO: obfuscation als compiler optie met vars in map als key met obfuscation name als value

class Lexer {
public:
    Lexer(char * filename);
    void lex();
    void print(std::ostream& os = std::cout);
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>>& get_output();
private:
    std::string m_filename;
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> m_tokens;
    void parse_string(std::string& string, std::vector<std::unique_ptr<AbstractToken>>& row);
    void parse_operators(std::string& operator_string, std::vector<std::unique_ptr<AbstractToken>>& row);
    void parse_line(const std::string& line);
};

#endif //COMPILER_CPP_LEXER_H
