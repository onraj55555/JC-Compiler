#include "Tree.h"

void Tree::parse() {
    for(std::string & function_name : m_pre_processer->get_function_names()) {
        if(function_name == "global") {
            parse_global_scope(m_pre_processer->get(function_name));
            continue;
        }
    }
}

void Tree::parse_global_scope(std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> &scope_body) {
    std::vector<std::string> usable_scopes{"global"};
    std::string current_scope = "global";
    for(std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>> & row : scope_body) {
        for(int i = 0; i < row->size(); i++) {
            if(HelperSpace::instanceof<TypeToken>((*row)[i])) {
                InitializeNode * initialize_node = parse_initialization(usable_scopes, current_scope, row);
            }
        }
    }
}

InitializeNode * Tree::parse_initialization(std::vector<std::string> & usable_scopes, std::string & current_scope, std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>> &row) {

    InitializeNode * initialize_node = new InitializeNode();
    std::string type = dynamic_cast<TypeToken *>((*row)[0].get())->get_type();
    std::string name = dynamic_cast<NameToken *>((*row)[0].get())->get_name();
    int size = TreeHelper::get_data_type_size(type);

    initialize_node->set_type(type);
    initialize_node->set_name(name);
    initialize_node->set_size(size);

    VariableNode * me_became_usable = new VariableNode();
    me_became_usable->set_size(size);
    me_became_usable->set_name(name);
    me_became_usable->set_type(type);

    m_variables[current_scope].insert(me_became_usable);

    // int i;
    if(row->size() == 3) {
        initialize_node->set_value(nullptr);
        return initialize_node;
    }

    // int i = ...;
    if(row->size() == 5) {

        // int i;
        if(HelperSpace::instanceof<NameToken>((*row)[3].get())) {

            std::string value_name = dynamic_cast<NameToken *>((*row)[3].get())->get_name();
            if(!is_usable_variable(usable_scopes, value_name)) {
                InformationError * error = new InformationError(value_name);
                error->operator+=(" is not accessable in scope ").operator+=(current_scope);
                throw error;
            }

            VariableNode * value = get_variable(usable_scopes, current_scope, value_name);
            initialize_node->set_value(value);
            return initialize_node;
        }

        if(HelperSpace::instanceof<LiteralToken>((*row)[3].get())) {
            std::string value = dynamic_cast<LiteralToken *>((*row)[3].get())->get_value();

            LiteralNode * literal = new LiteralNode();
            literal->set_value(value);

            initialize_node->set_value(literal);
            return initialize_node;
        }
    }

    std::vector<std::unique_ptr<AbstractToken>> rest_of_line;

    for(int i = 0; i < row->size(); i++) {
        rest_of_line.push_back(std::move((*row)[i]));
    }

    int last = rest_of_line.size() - 1;
    int first_rbra = 0;

    for(int i = 0; i < rest_of_line.size(); i++) {
        if(HelperSpace::instanceof<KeywordToken>(rest_of_line[i].get())) {
            first_rbra = i;
            break;
        }
    }

    if(first_rbra == last && HelperSpace::instanceof<NameToken>(rest_of_line[0].get()) && HelperSpace::instanceof<KeywordToken>(rest_of_line[last].get())) {
        NameToken * function_name = dynamic_cast<NameToken *>(rest_of_line[0].get());
        KeywordToken * rbra = dynamic_cast<KeywordToken *>(rest_of_line[last].get());

        if(function_name->get_name() == Helper::get_keyword("fn") && rbra->get_type() == Helper::get_bracket_type(')')) {

        }
    }

    for(int i = 0; i < rest_of_line.size(); i++) {
        if(HelperSpace::instanceof<NameToken>(rest_of_line[i].get())) {

        }
    }
}

bool Tree::is_usable_variable(std::vector<std::string> &usable_scopes, const std::string &variable_name) {
    for(std::string & scope_name : usable_scopes) {
        for(VariableNode * variable_node : m_variables[scope_name]) {
            if(variable_name == variable_node->get_name()) {
                return true;
            }
        }
    }

    return false;
}

bool Tree::is_visible_function(const std::string &function_name) {
    if(Helper::is_build_in_function(function_name)) {
        return true;
    }

    for(auto & pair : m_functions) {
        if(pair.first == function_name) {
            return true;
        }
    }

    return false;
}

VariableNode * Tree::get_variable(std::vector<std::string> &usable_scopes, std::string &current_scope, const std::string &variable_name) {
    for(VariableNode * variable : m_variables[current_scope]) {
        if(variable->get_name() == variable_name) {
            return variable;
        }
    }

    for(std::string & scope : usable_scopes) {
        if(scope != current_scope) {
            for(VariableNode * variable : m_variables[scope]) {
                if(variable->get_name() == variable_name) {
                    return variable;
                }
            }
        }
    }

    return nullptr;
}

FunctionCallNode *Tree::get_function(std::vector<std::string> & usable_scopes, std::string & current_scope, std::vector<std::unique_ptr<AbstractToken>> & row) {
    const std::string & function_name = dynamic_cast<NameToken *>(row[0].get())->get_name();

    FunctionCallNode * function_call_node = nullptr;

    if(is_visible_function(function_name)) {
        function_call_node = new FunctionCallNode();
        function_call_node->set_name(function_name);

        for(int i = 2; i < row.size(); i++) {
            if(HelperSpace::instanceof<LiteralToken>(row[i].get())) {
                LiteralToken * literal = dynamic_cast<LiteralToken *>(row[i].get());

                LiteralNode * literal_node = new LiteralNode();
                literal_node->set_value(literal->get_value());
                literal_node->set_size(TreeHelper::get_data_type_size(literal->get_type()));

                function_call_node->add_parameter(literal_node);
            }

            if(HelperSpace::instanceof<NameToken>(row[i].get())) {
                NameToken * variable = dynamic_cast<NameToken *>(row[i].get());

                if(!is_usable_variable(usable_scopes, variable->get_name())) {
                    VariableNode * variable_node = new VariableNode(Tree::get_variable(usable_scopes, current_scope, variable->get_name()));

                    function_call_node->add_parameter(variable_node);
                }
            }
        }
    }

    return function_call_node;
}

MathNode *Tree::parse_math(std::vector<std::string> &usable_scopes, std::string &current_scope, std::vector<std::unique_ptr<AbstractToken>> &row) {
    MathNode * math_node = new MathNode();

    if(row.size() == 3) {
        if(HelperSpace::instanceof<OperatorToken>(row[1].get())) {
            if((HelperSpace::instanceof<LiteralToken>(row[0].get()))) {
                LiteralToken * literal_token = dynamic_cast<LiteralToken *>(row[0].get());

                LiteralNode * literal_node = new LiteralNode();
                literal_node->set_value(literal_token->get_value());
                literal_node->set_size(TreeHelper::get_data_type_size(literal_token->get_type()));

                math_node->set_left_operand(literal_node);
            }

            if((HelperSpace::instanceof<NameToken>(row[0].get()))) {
                NameToken * name_token = dynamic_cast<NameToken *>(row[0].get());

                LiteralNode * literal_node = new LiteralNode();
                literal_node->set_value(literal_token->get_value());
                literal_node->set_size(TreeHelper::get_data_type_size(literal_token->get_type()));

                math_node->set_left_operand(literal_node);
            }
        }
    }

    int weakest_operator_index = -1;
    unsigned int weakest_operator_score = (unsigned int)-1;

    for(int i = 0; i < row.size(); i++) {
        if(HelperSpace::instanceof<OperatorToken>(row[i].get())) {
            OperatorToken * operator_token = dynamic_cast<OperatorToken *>(row[i].get());

            if(TreeHelper::get_operator_strength(operator_token) < weakest_operator_score) {
                weakest_operator_index = i;
                weakest_operator_score = TreeHelper::get_operator_strength(operator_token);
            }
        }
    }

    std::vector<std::unique_ptr<AbstractToken>> left_lob;
    std::vector<std::unique_ptr<AbstractToken>> right_lob;
    std::unique_ptr<AbstractToken> operator_token;

    for(int i = 0; i < row.size(); i++) {
        if(i < weakest_operator_index) {
            left_lob.push_back(std::move(row[i]));
        }

        if(i > weakest_operator_index) {
            right_lob.push_back(std::move(row[i]));
        }

        if(i == weakest_operator_index) {
            operator_token = std::move(row[i]);
        }
    }
}