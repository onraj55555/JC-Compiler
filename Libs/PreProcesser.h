//
// Created by jarno on 3/09/23.
//

#ifndef COMPILER_CPP_PREPROCESSER_H
#define COMPILER_CPP_PREPROCESSER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Tokens.h"
#include "Errors.h"
#include "Lexer.h"
#include "PreProcesserHelper.h"
#include "Helper.h"

class PreProcesser {
private:
    AbstractState * m_abstract_state;
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> m_includes;
    std::unordered_map<std::string, std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>>> m_functions;
    //std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> m_main;
    //std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> m_global_variables;
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>>& m_lexer_output;
public:
    PreProcesser(std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>>& lexer_output) : m_lexer_output(lexer_output) {
        m_abstract_state = new IdleState();
    }
    void process();
    void change_state(AbstractState & abstract_state);
    virtual ~PreProcesser() {
        delete m_abstract_state;
    }

    std::vector<std::string> get_function_names();
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> & operator[](std::string & function_name);
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> & get(std::string & function_name) {
        return this->operator[](function_name);
    }
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> & get_includes();
};

#endif //COMPILER_CPP_PREPROCESSER_H
