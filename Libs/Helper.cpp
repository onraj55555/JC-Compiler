//
// Created by jarno on 1/09/23.
//

#include "Helper.h"
#include <string>
#include <vector>

const std::string Helper::whitespaces = " \n\t";
const char Helper::line_end = ';';
const std::string Helper::brackets = "(){}[]";
const std::vector<std::string> Helper::bracket_types{"LBRA", "RBRA", "LCUR", "RCUR", "LANG", "RANG"}; // (){}[]
const std::string Helper::allowed_variable_symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
const std::string Helper::single_operators = "+-*/<>.,=!|&%";
const std::vector<std::string> Helper::counter_operators{"++", "--"};
const std::vector<std::string> Helper::double_operators{"+=", "-=", "*=", "/=", "==", "<=", ">=", "!=", "|=", "&=", "%="};
const std::vector<std::string> Helper::single_operators_type{"PLUS", "MIN", "MUL", "DIV", "LT", "GT", "DOT", "COM", "ASGN", "NOT", "OR", "AND", "MOD"};
const std::vector<std::string> Helper::double_operators_type{"PLUSEQ", "MINEQ", "MULEQ", "DIVEQ", "EQ", "LTEQ", "GTEQ", "NEQ", "OREQ", "ANDEQ", "MODEQ"};
const std::vector<std::string> Helper::counter_operators_type{"INCR", "DECR"};
const std::vector<std::string> Helper::keywords{"#include", "return", "if", "else", "fn", "for", "while", "true", "false", "write", "read"};
const std::vector<std::string> Helper::keywords_tokens{"INCL", "RET", "IF", "ELSE", "FN", "FOR", "WHL", "TRUE", "FALSE", "WRITE", "READ"};
const std::vector<std::string> Helper::build_in_functions{"write", "read"};
const std::vector<std::string> Helper::build_in_functions_tokens{"WRITE", "READ"};
const std::vector<std::string> Helper::types{"int", "float", "char", "void"}; // Indien toevoegingen -> in PreProcesserHelper.cpp toevoegen bij checken voor for loop
const std::vector<std::string> Helper::types_keywords{"INT", "FLO", "CHA", "VOID"}; // Hier ook

bool Helper::is_whitespace(char c) {
    return Helper::whitespaces.contains(c);
}

bool Helper::is_line_end(const char c) {
    return c == Helper::line_end;
}

bool Helper::is_bracket(const char c) {
    return Helper::brackets.contains(c);
}

bool Helper::is_keyword(const std::string &str) {
    for(const std::string & keyword_str : Helper::keywords) {
        if(str == keyword_str) {
            return true;
        }
    }
    return false;
}

const std::string & Helper::get_bracket_type(const char c) {
    int index = Helper::brackets.find(c);
    if(index == std::string::npos) {
        CompilerError * error = new InformationError("Invalid bracket: \'");
        error += c;
        error += '\'';
        throw error;
    }
    return Helper::bracket_types.at(index);
}

const std::string & Helper::get_operator_type(const char c) {
    int index = Helper::single_operators.find(c);
    if(index == std::string::npos) {
        CompilerError * error = new InformationError("Invalid operator: \'");
        error += c;
        error += '\'';
        throw error;
    }
    return Helper::single_operators_type[index];
}

const std::string & Helper::get_operator_type(const std::string &str) {
    int index = -1;
    for(int i = 0; i < Helper::double_operators.size(); i++) {
        if(Helper::double_operators[i] == str) {
            index = i;
            break;
        }
    }
    if(index > -1) {
        return Helper::double_operators_type[index];
    }
    CompilerError * error = new InformationError("Invalid operator: \'");
    error->operator+=(str);
    error->operator+=("\'");
    throw error;
}

const std::string & Helper::get_keyword(const std::string &str) {
    if(!Helper::is_keyword(str)) {
        CompilerError * error = new InformationError("Invalid keyword: \'");
        error->operator+=(str);
        error->operator+=(" \'");
        throw error;
    }
    int i;
    for(i = 0; i < Helper::keywords.size(); i++) {
        if(Helper::keywords[i] == str) {
            break;
        }
    }
    return Helper::keywords_tokens[i];
}

bool Helper::is_valid_variable_name(const std::string &str) {
    if(Helper::is_digit(str[0])) {
        return false;
    }
    for(const char c : str) {
        if(!Helper::allowed_variable_symbols.contains(c)) {
            return false;
        }
    }
    return true;
}

/*bool Lexer::is_line_end(const std::string &str) {
    return (str.size() == 1)*(str[0] == ';');
}*/

bool Helper::is_digit(const char c) {
    return (c <= '9')*(c >= '0');
}

bool Helper::is_ascii_char(const std::string &str) {
    return (str.size() == 3)*(str[0] == 39)*(str[2] == 39);
}

bool Helper::is_int32(const std::string &str) {
    for(const char c : str) {
        if(!is_digit(c)) {
            return false;
        }
    }
    return true;
}

bool Helper::is_float32(const std::string &str) {
    int dot_count = 0;
    if(!Helper::is_digit(str[0])) {
        return false;
    }
    for(const char c : str) {
        if(c == '.') {
            dot_count++;
            if(dot_count > 1) {
                return false;
            }
            continue;
        }
        if(!Helper::is_digit(c)) {
            return false;
        }
    }
    return true;
}

bool Helper::is_single_operator(const char c) {
    return Helper::single_operators.contains(c);
}

bool Helper::is_single_operator(const std::string &str) {
    if(str.empty()) {
        return false;
    }
    return Helper::is_single_operator(str[0]) * (str.size() == 1);
}

bool Helper::is_double_operator(const std::string &str) {
    for(const std::string& double_operator : Helper::double_operators) {
        if(str == double_operator) {
            return true;
        }
    }
    return false;
}

bool Helper::only_operators(const std::string &str) {
    for(const char c : str) {
        if(!Helper::is_single_operator(c)) {
            std::cerr << c << " ga nie" << std::endl;
            return false;
        }
    }
    return true;
}

bool Helper::is_type(const std::string &str) {
    for(const std::string & type : Helper::types) {
        if(str == type) {
            return true;
        }
    }
    return false;
}

const std::string &Helper::get_type(const std::string &str) {
    int index;
    for(index = 0; index < Helper::types.size(); index++) {
        if(str == Helper::types[index]) {
            break;
        }
    }
    return Helper::types_keywords[index];
}

bool Helper::is_build_in_function(const std::string &str) {
    for(const std::string & function : Helper::build_in_functions) {
        if(function == str) {
            return true;
        }
    }

    for(const std::string & function : Helper::build_in_functions_tokens) {
        if(function == str) {
            return true;
        }
    }
}