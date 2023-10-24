//
// Created by jarno on 1/09/23.
//

#ifndef COMPILER_CPP_HELPER_H
#define COMPILER_CPP_HELPER_H

#include <string>
#include <vector>
#include <memory>
#include "Lexer.h"

class Helper {
public:
    static bool is_whitespace(const char c);
    static bool is_line_end(const char c);
    static bool is_bracket(const char c);
    //static bool is_operator(const char c);
    //static bool is_operator(const std::string& str);
    static bool is_single_operator(const char c);
    static bool is_single_operator(const std::string& str);
    static bool is_double_operator(const std::string& str);
    static bool only_operators(const std::string& str);
    static bool is_type(const std::string & str);

    //static bool is_line_end(const std::string& str);
    static bool is_digit(const char c);
    static bool is_ascii_char(const std::string& str); // Future proofing for unicode implementation
    static bool is_int32(const std::string& str); // Future proofing for int64, aka long long int, int is unsigned
    static bool is_float32(const std::string& str); // Future proofing for float64, aka double, float is unsigned
    static bool is_valid_variable_name(const std::string& str);
    static bool is_keyword(const std::string& str);
    static bool is_build_in_function(const std::string & str);

    static const std::string & get_type(const std::string& str);
    static const std::string & get_bracket_type(const char c);
    static const std::string & get_operator_type(const char c);
    static const std::string & get_operator_type(const std::string& str);
    static const std::string & get_keyword(const std::string& str);

    //static std::unordered_map<std::string, std::string> keywords;
    static const std::string whitespaces;
    static const char line_end;
    static const std::string brackets;
    static const std::vector<std::string> bracket_types;
    static const std::string allowed_variable_symbols;
    static const std::string single_operators;
    static const std::vector<std::string> double_operators;
    static const std::vector<std::string> counter_operators;
    static const std::vector<std::string> single_operators_type;
    static const std::vector<std::string> double_operators_type;
    static const std::vector<std::string> counter_operators_type;
    static const std::vector<std::string> keywords;
    static const std::vector<std::string> keywords_tokens;
    static const std::vector<std::string> types;
    static const std::vector<std::string> types_keywords;
    static const std::vector<std::string> build_in_functions;
    static const std::vector<std::string> build_in_functions_tokens;
};

namespace HelperSpace {
    template<typename Base, typename T>
    inline bool instanceof(const std::unique_ptr<T> &ptr) {
        return dynamic_cast<const Base *>(ptr.get()) != nullptr;
    }

    template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
        return dynamic_cast<const Base *>(ptr) != nullptr;
    }
}

#endif //COMPILER_CPP_HELPER_H
