//
// Created by jarno on 02/09/2023.
//

#ifndef COMPILER_CPP_ERRORS_H
#define COMPILER_CPP_ERRORS_H
#include <string>
#include <iostream>
#include <memory>

class CompilerError {
private:
    std::string m_message;
public:
    CompilerError() = default;
    virtual ~CompilerError() = default;
    explicit CompilerError(const char * message) : m_message(message) {}
    explicit CompilerError(const std::string& message) : m_message(message) {}
    CompilerError & operator+=(const std::string & string);
    CompilerError & operator+=(const char * string);
    CompilerError & operator+=(const char c);
    CompilerError & operator+=(const int i);
    CompilerError & operator+=(const double d);
    friend std::ostream & operator<<(std::ostream& os, const CompilerError & compilerError);
    virtual void print(std::ostream& os) const = 0;
};

class SyntaxError : public CompilerError {
public:
    SyntaxError() : CompilerError() {}
    explicit SyntaxError(const char * message) : CompilerError(message) {}
    explicit SyntaxError(const std::string & message) : CompilerError(message) {}
    void print(std::ostream &os) const override;
};

class RedefinitionError : public CompilerError {
public:
    RedefinitionError() : CompilerError() {}
    RedefinitionError(const char * message) : CompilerError(message) {}
    RedefinitionError(const std::string & message) : CompilerError(message) {}
    void print(std::ostream &os) const override;
};

class InformationError : public CompilerError {
public:
    InformationError() : CompilerError() {}
    InformationError(const char * message) : CompilerError(message) {}
    InformationError(const std::string & message) : CompilerError(message) {}
    void print(std::ostream &os) const override;
};

class ErrorDecorator : public CompilerError {
protected:
    const CompilerError * m_compiler_error;
public:
    explicit ErrorDecorator(const CompilerError * compilerError) : m_compiler_error(compilerError) {}
    void print(std::ostream& os) const override = 0;
    ~ErrorDecorator() {
        delete m_compiler_error;
    }
};

class LineDecoratorError : public ErrorDecorator {
private:
    const int m_line;
public:
    LineDecoratorError(const CompilerError * prev_error, const int line) : ErrorDecorator(prev_error), m_line(line) {
    }
    void print(std::ostream &os) const override;
};


#endif //COMPILER_CPP_ERRORS_H
