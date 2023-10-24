//
// Created by jarno on 3/09/23.
//

#include "PreProcesser.h"

void PreProcesser::process() {
    //std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> temp_tokens;
    std::vector<std::unique_ptr<AbstractToken>> temp_row;
    std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> * current_scope = &m_functions["global"];
    int closing_curly_count = 0;
    for(auto & row : m_lexer_output) {

        if(PreProcesserHelper::is_include(row)) {
            //std::cout << "FOUND INCLUDE!" << std::endl;
            current_scope->push_back(std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>(std::move(row)));
            continue;
        }

        int row_size = row->size();
        for(int i = 0; i < row->size(); i++) {
            std::unique_ptr<AbstractToken> & token = row->at(i);
            if(i + 2 < row_size) {
                if(HelperSpace::instanceof<LiteralToken>(row->at(i)) && HelperSpace::instanceof<OperatorToken>(row->at(i + 1)) && HelperSpace::instanceof<LiteralToken>(row->at(i + 2))) {
                    LiteralToken * left = dynamic_cast<LiteralToken *>(row->at(i).get());
                    OperatorToken * dot = dynamic_cast<OperatorToken *>(row->at(i + 1).get());
                    LiteralToken * right = dynamic_cast<LiteralToken *>(row->at(i + 2).get());

                    if(left->get_type() == Helper::get_type("int") && dot->get_operator() == Helper::get_operator_type('.') && right->get_type() == Helper::get_type("int")) {
                        std::string value(left->get_value());
                        value += '.';
                        value += right->get_value();
                        token = std::make_unique<LiteralToken>(Helper::get_type("float"), value);
                        i += 2;
                    }
                }
            }

            AbstractState * next_state = m_abstract_state->get_next_state(row->at(i));
            delete m_abstract_state;
            m_abstract_state = next_state;



            if(HelperSpace::instanceof<InvalidState>(m_abstract_state)) {
                SyntaxError * error = new SyntaxError("Invalid state!");
                throw error;
            }

            if(m_abstract_state->is_end()) {
                if(HelperSpace::instanceof<FunctionStates::FirstCurlyState>(m_abstract_state)) {
                    const std::string & function_name = dynamic_cast<NameToken *>(temp_row[4].get())->get_name();
                    if(m_functions[function_name].empty() && closing_curly_count == 0) {
                        current_scope = &m_functions[function_name];
                        closing_curly_count++;
                    }
                    else {
                        if(closing_curly_count > 0) {
                            CompilerError * error = new InformationError("Current function not ended when defining a new function");
                            throw error;
                        }
                        CompilerError * error = new RedefinitionError(function_name);
                        throw error;
                    }
                }
                temp_row.push_back(std::move(token));
                current_scope->push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(temp_row)));

                AbstractState * next_state = new IdleState();
                delete m_abstract_state;
                m_abstract_state = next_state;
                continue;
            }

            if(HelperSpace::instanceof<KeywordToken>(token)) {
                KeywordToken * keywordToken = dynamic_cast<KeywordToken *>(token.get());
                if(keywordToken->get_type() == Helper::get_bracket_type('}')) {
                    closing_curly_count--;
                    temp_row.push_back(std::move(token));
                    current_scope->push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(temp_row)));

                    if(closing_curly_count == 0) {
                        current_scope = &m_functions["global"];
                        continue;
                    }
                }
            }

            /*if(token->get_type() == "SEMI") {
                temp_row.push_back(std::move(token));
                temp_tokens.push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(temp_row)));
                continue;
            }*/
            temp_row.push_back(std::move(token));
        }
    }

    if(!temp_row.empty()) {
        current_scope->push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(temp_row)));
    }

    for(auto & function : m_functions) {
        const std::string & function_name = function.first;
        std::cout << function_name << " scope:" << std::endl;
        for(auto & row : function.second) {
            for(auto & element : *row) {
                std::cout << *element;
            }
            std::cout << std::endl;
        }
        std::cout << "End scope" << std::endl;
    }
    /*for(const auto& row : temp_tokens) {
        for(const auto& token : *row) {
            std::cout << *token;
        }
        std::cout << std::endl;
    }*/
}

void PreProcesser::change_state(AbstractState & abstract_state) {

}

std::vector<std::string> PreProcesser::get_function_names() {
    std::vector<std::string> function_names;
    function_names.reserve(m_functions.size());
    for(auto & pair : m_functions) {
        function_names.push_back(pair.first);
    }

    return function_names;
}

std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> & PreProcesser::operator[](std::string &function_name) {
    return m_functions[function_name];
}

std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> &PreProcesser::get_includes() {
    return m_includes;
}
