//
// Created by jarno on 02/09/2023.
//

#include "Errors.h"

CompilerError &CompilerError::operator+=(const std::string &string) {
    m_message += string;
    return *this;
}

CompilerError &CompilerError::operator+=(const char c) {
    m_message += c;
}

CompilerError &CompilerError::operator+=(const int i) {
    m_message += i;
}

CompilerError &CompilerError::operator+=(const double d) {
    m_message += d;
}

std::ostream &operator<<(std::ostream &os, const CompilerError &compilerError) {
    compilerError.print(os);
    return os;
}

void CompilerError::print(std::ostream &os) const {
    os << m_message;
}

CompilerError &CompilerError::operator+=(const char *string) {
    m_message += string;
    return *this;
}

void SyntaxError::print(std::ostream &os) const {
    os << "Syntax error: \'";
    CompilerError::print(os);
    os << "\' isn't valid syntax!";
}

void LineDecoratorError::print(std::ostream &os) const {
    os << "Error at line " << m_line << ": ";
    m_compiler_error->print(os);
}

void RedefinitionError::print(std::ostream &os) const {
    os << "Redefinition of \'";
    CompilerError::print(os);
    os << "\'";
}

void InformationError::print(std::ostream &os) const {
    CompilerError::print(os);
}
