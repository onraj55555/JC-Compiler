//
// Created by jarno on 1/09/23.
//

#include "Tokens.h"

/*
 * ostream& << operator for AbstractToken class
 */
template <typename T>
void debug(const T &t) {
    std::cout << "DEBUG: " << t << std::endl;
}

std::ostream& operator<<(std::ostream& os, const AbstractToken& absTkn) {
    os << "[";
    absTkn.draw(os);
    os << "]";
    return os;
}

/*
 * Implementation of the draw lidfunction used in the << operator foreach instance of the AbstractToken class
 */
void AbstractToken::draw(std::ostream &os) const {
    os << "ATKN";
}

void KeywordToken::draw(std::ostream &os) const {
    os << m_type;
}

void NameToken::draw(std::ostream &os) const {
    os << m_type << ";" << m_name;
}

void OperatorToken::draw(std::ostream &os) const {
    os << m_type << ";" << m_operator;
}

void TypeToken::draw(std::ostream &os) const {
    os << m_type;
}

const std::string& AbstractToken::get_type() const {
    return m_type;
}

const std::string &NameToken::get_name() {
    return m_name;
}

const std::string &OperatorToken::get_operator() {
    return m_operator;
}

void LiteralToken::draw(std::ostream &os) const {
    os << m_type << ";" << m_value;
}

const std::string &LiteralToken::get_value() const {
    return m_value;
}
