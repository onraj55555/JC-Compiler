//
// Created by jarno on 9/09/23.
//

#ifndef COMPILER_CPP_TREENODES_H
#define COMPILER_CPP_TREENODES_H

#include <iostream>
#include <string>
#include <vector>

/*
 * Abstract node for value holders
 */
class ValueNode {
public:
    virtual std::string get_value() const = 0;
};

/*
 * Literal node, holds literal value
 */
class LiteralNode : public ValueNode {
private:
    std::string m_value;
    std::string m_data_type;
public:
    std::string get_value() const override;
    LiteralNode(std::string & data_type, std::string & value) : m_data_type(data_type), m_value(value) {}
    LiteralNode() {}

    void set_value(std::string & value) {
        m_value = value;
    }

    void set_data_type(std::string & data_type) {
        m_data_type = data_type;
    }
};

/*
 * Math node, holds expression, eg: 4 + 5
 */
class MathNode : public ValueNode {
private:
    ValueNode * m_left_operand;
    std::string m_operator;
    ValueNode * m_right_operand;
public:
    std::string get_value() const override;
    MathNode() {}
    MathNode(ValueNode * left_operand, std::string & expression_operator, ValueNode * right_operand) : m_left_operand(left_operand), m_operator(expression_operator), m_right_operand(right_operand) {}
    virtual ~MathNode() {
        delete m_left_operand;
        delete m_right_operand;
    }

    void set_left_operand(ValueNode * left_operand) {
        m_left_operand = left_operand;
    }

    void set_right_operand(ValueNode * right_operand) {
        m_right_operand = right_operand;
    }

    void set_operator(std::string & expression_operator) {
        m_operator = expression_operator;
    }
};

class AbstractNode {
public:
    virtual void print(std::ostream& os) const = 0;
    virtual void to_asm(std::ostream& os) const = 0;
};

class VariableNode : public AbstractNode, public ValueNode {
private:
    std::string m_data_type;
    std::string m_variable_name;
    ValueNode * m_value;
public:
    void print(std::ostream &os) const override;
    void to_asm(std::ostream &os) const override;

    std::string get_value() const override {
        return m_value->get_value();
    }

    void set_data_type(const std::string & data_type) {
        m_data_type = data_type;
    }

    void set_variable_name(const std::string & variable_name) {
        m_variable_name = variable_name;
    }

    void set_value(ValueNode * value) {
        m_value = value;
    }
};

class FunctionNode : public AbstractNode, public ValueNode {
private:
    std::string m_function_name;
    std::vector<VariableNode *> m_parameters;
    std::vector<AbstractNode *> m_body;
    VariableNode * m_return_type;
public:
    void to_asm(std::ostream &os) const override;
    void print(std::ostream &os) const override;

    FunctionNode() {}
    FunctionNode(std::string & function_name, VariableNode * return_type) {
        m_function_name = function_name;
        m_return_type = return_type;
    }

    void set_function_name(std::string & function_name) {
        m_function_name = function_name;
    }

    void set_return_type(VariableNode * return_type) {
        m_return_type = return_type;
    }

    void add_parameter(VariableNode * parameter) {
        m_parameters.push_back(parameter);
    }

    void add_body_part(AbstractNode * body_part) {
        m_body.push_back(body_part);
    }
};


class LexerToASM {

};
#endif //COMPILER_CPP_TREENODES_H
