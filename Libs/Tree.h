//
// Created by jarno on 10/09/23.
//

#ifndef COMPILER_CPP_TREE_H
#define COMPILER_CPP_TREE_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "TreeNodesRevamped.h"
#include "PreProcesser.h"
#include "TreeHelper.h"

class Tree {
private:
    std::vector<std::string> m_stack;
    std::vector<AbstractNode *> m_global_scope;
    std::unordered_map<std::string, FunctionNode *> m_functions;
    PreProcesser * m_pre_processer;
    std::unordered_map<std::string, std::unordered_set<VariableNode *>> m_variables;

    void parse_scope(std::string & scope_name, std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> & scope_body);
    void parse_function_header(std::string & scope_name, std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>> & functoin_header);
    void parse_global_scope(std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> & scope_body);

    void parse_variable_declaration(std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>> & row);

    InitializeNode * parse_initialization(std::vector<std::string> & usable_scopes, std::string & current_scope, std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>> & row);

    bool is_usable_variable(std::vector<std::string> & usable_scopes, const std::string & variable_name);
    bool is_visible_function(const std::string & function_name);

    VariableNode * get_variable(std::vector<std::string> & usable_scopes, std::string & current_scope, const std::string & variable_name);
    FunctionCallNode * get_function(std::vector<std::string> & usable_scopes, std::string & current_scope, std::vector<std::unique_ptr<AbstractToken>> & row);
    MathNode * parse_math(std::vector<std::string> & usable_scopes, std::string & current_scope, std::vector<std::unique_ptr<AbstractToken>> & row);
public:
    explicit Tree(PreProcesser * pre_processer) : m_pre_processer(pre_processer) {}
    void parse();
};


#endif //COMPILER_CPP_TREE_H
