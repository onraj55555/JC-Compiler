//
// Created by jarno on 11/09/23.
//

#ifndef COMPILER_CPP_TREENODESREVAMPED_H
#define COMPILER_CPP_TREENODESREVAMPED_H

#include <memory>
#include <string>
#include <vector>

/*
 * Abstract node for polymorphism
 */
class AbstractNode {

};

/*
 * RValueNode, can only be on the right side of an = operator and have a public method to give out their value
 */
class RValueNode : public AbstractNode {
public:
    //virtual std::string & get_value() = 0;
};

class LiteralNode : public RValueNode {
private:
    std::string m_value;
    int m_size;
public:
    LiteralNode() {}
    LiteralNode(const std::string & value) : m_value(value) {}
    LiteralNode(const char * value) : m_value(value) {}
    LiteralNode(const LiteralNode * literal_node) : m_value(literal_node->m_value), m_size(literal_node->m_size) {}

    /*std::string & get_value() override {
        return m_value;
    }*/

    void set_value(const char * value) {
        m_value = value;
    }

    void set_value(const std::string & value) {
        m_value = value;
    }

    void set_size(int size) {
        m_size = size;
    }
};

class VariableNode : public RValueNode {
private:
    std::string m_name;
    std::string m_type;
    RValueNode * m_value;
    int m_size;
public:
    VariableNode(const std::string & name, const std::string & type, LiteralNode * value, int size) : m_name(name), m_type(type), m_value(value), m_size(size) {}
    VariableNode(const char * name, const char * type, LiteralNode * value, int size) : m_name(name), m_type(type), m_value(value), m_size(size) {}
    VariableNode() {};
    VariableNode(const VariableNode * variable_node) : m_name(variable_node->m_name), m_type(variable_node->m_type), m_value(variable_node->m_value), m_size(variable_node->m_size) {}

    /*std::string & get_value() override {
        return m_value->get_value();
    }*/

    virtual ~VariableNode() {
        delete m_value;
    }

    void set_name(const std::string & name) {
        m_name = name;
    }

    void set_name(const char * name) {
        m_name = name;
    }

    void set_value(RValueNode * value) {
        m_value = value;
    }

    void set_type(const std::string & type) {
        m_type = type;
    }

    void set_type(const char * type) {
        m_type = type;
    }

    void set_size(const int size) {
        m_size = size;
    }

    std::string & get_name() {
        return m_name;
    }
};

class MathNode : public RValueNode {
private:
    RValueNode * m_left_operand;
    std::string m_operator;
    RValueNode * m_right_operand;

    std::string m_value;
public:
    MathNode() {}
    MathNode(RValueNode * left_operand, const std::string & math_operator, RValueNode * right_operand) : m_left_operand(left_operand), m_operator(math_operator), m_right_operand(right_operand) {}
    MathNode(RValueNode * left_operand, const char * math_operator, RValueNode * right_operand) : m_left_operand(left_operand), m_operator(math_operator), m_right_operand(right_operand) {}


    /*std::string & get_value() override {
        m_value = m_left_operand->get_value() + m_operator + m_right_operand->get_value();
        return m_value;
    }*/

    virtual ~MathNode() {
        delete m_left_operand;
        delete m_right_operand;
    }

    void set_left_operand(RValueNode * left_operand) {
        m_left_operand = left_operand;
    }

    void set_operator(const std::string & math_operator) {
        m_operator = math_operator;
    }

    void set_operator(const char * math_operator) {
        m_operator =  math_operator;
    }

    void set_right_operator(RValueNode * right_operator) {
        m_right_operand = right_operator;
    }
};

class FunctionCallNode : public RValueNode {
private:
    std::vector<RValueNode *> m_parameters;
    std::string m_name;
public:
    FunctionCallNode() {}
    FunctionCallNode(const std::string & name) : m_name(name) {}
    FunctionCallNode(const char * name) : m_name(name) {}

    void add_parameter(RValueNode * parameter) {
        m_parameters.push_back(parameter);
    }

    void set_name(const std::string & name) {
        m_name = name;
    }

    void set_name(const char * name) {
        m_name = name;
    }
};

class LValueNode : public AbstractNode {
protected:
    RValueNode * m_value;
public:
    virtual ~LValueNode() {
        delete m_value;
    }

    void set_value(RValueNode * value) {
        m_value = value;
    }
};

class InitializeNode : public LValueNode {
private:
    std::string m_type;
    std::string m_name;
    int m_size;
public:
    InitializeNode() {}
    InitializeNode(const std::string & type, const std::string & name, RValueNode * value, int size) : m_type(type), m_name(name), m_size(size) {
        m_value = value;
    }
    InitializeNode(const char * type, const char * name, RValueNode * value) : m_type(type), m_name(name) {
        m_value = value;
    }

    void set_type(const std::string & type) {
        m_type = type;
    }

    void set_type(const char * type) {
        m_type = type;
    }

    void set_name(const std::string & name) {
        m_name = name;
    }

    void set_name(const char * name) {
        m_name = name;
    }

    void set_size(const int size) {
        m_size = size;
    }
};

class AssignNode : public LValueNode {
private:
    std::string m_name;
public:
    AssignNode() {}
    AssignNode(const std::string & name, RValueNode * value) : m_name(name) {
        m_value = value;
    }
    AssignNode(const char * name, RValueNode * value) : m_name(name) {
        m_value = value;
    }

    void set_name(const std::string & name) {
        m_name = name;
    }

    void set_name(const char * name) {
        m_name = name;
    }
};

class FunctionNode : public LValueNode {
private:
    std::vector<InitializeNode *> m_parameters;
    std::string m_name;
    std::string m_return_type;
public:
    FunctionNode() {};
    FunctionNode(const std::string & name, const std::string & return_type) : m_name(name), m_return_type(return_type) {}
    FunctionNode(const char * name, const char * return_type) : m_name(name), m_return_type(return_type) {}

    void set_name(const std::string & name) {
        m_name = name;
    }

    void set_name(const char * name) {
        m_name = name;
    }

    void set_return_type(const std::string & return_type) {
        m_return_type = return_type;
    }

    void set_return_type(const char * return_type) {
        m_return_type = return_type;
    }
};

class ReturnNode : public LValueNode {
public:
    ReturnNode() {}
    ReturnNode(RValueNode * value) {
        m_value = value;
    }

    void set_value(RValueNode * value) {
        m_value = value;
    }
};

class IfNode : public LValueNode {
protected:
    RValueNode * m_condition;
    std::vector<IfNode *> m_next_conditions;
    IfNode * m_else_node;

    std::vector<LValueNode *> m_body;
public:
    IfNode() {}
    IfNode(RValueNode * condition) : m_condition(condition) {}

    void set_condition(RValueNode * condition) {
        m_condition = condition;
    }

    void add_next_condition(IfNode * next_condition) {
        m_next_conditions.push_back(next_condition);
    }

    void add_else_condition(IfNode * else_node) {
        m_else_node = else_node;
    }

    void add_body(LValueNode * body_part) {
        m_body.push_back(body_part);
    }

    virtual ~IfNode() {
        if(m_condition != nullptr) {
            delete m_condition;
        }

        for(IfNode * next_condition : m_next_conditions) {
            if(next_condition != nullptr) {
                delete next_condition;
            }
        }

        if(m_else_node != nullptr) {
            delete m_else_node;
        }

        for(LValueNode * body_part : m_body) {
            if(body_part != nullptr) {
                delete body_part;
            }
        }
    }
};

class ElseNode : public IfNode {
    ElseNode() {
        m_condition = new LiteralNode("1");
    }
};


#endif //COMPILER_CPP_TREENODESREVAMPED_H
