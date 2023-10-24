//
// Created by jarno on 1/09/23.
//

#ifndef COMPILER_CPP_TOKENS_H
#define COMPILER_CPP_TOKENS_H

#include <string>
#include <iostream>
#include <memory>

/*
* Abstract token
*/
class AbstractToken {
public:
    //static const char* type;
    friend std::ostream& operator<<(std::ostream& os, const AbstractToken& absTkn);
    virtual void draw(std::ostream& os) const;
    const std::string& get_type() const;
protected:
    explicit AbstractToken(std::unique_ptr<AbstractToken> type) : m_type(type->m_type) {}
    explicit AbstractToken(const std::string & type) : m_type(std::string(type)) {}
    const std::string m_type;
};

/*
* Literal token, inherrited from abstract token
*/
class LiteralToken : public AbstractToken {
public:
    LiteralToken(const char * type, std::string & value) : AbstractToken(type), m_value(value) {};
    LiteralToken(const std::string & type, std::string & value) : AbstractToken(type), m_value(value) {}
    //static const char* type;
    //friend std::ostream& operator<<(std::ostream& os, const LiteralToken<T>& absTkn);
    void draw(std::ostream& os) const override;
    const std::string & get_value() const;
protected:
    const std::string m_value;
};

class KeywordToken : public AbstractToken {
public:
    explicit KeywordToken(const char * type) : AbstractToken(type) {}
    explicit KeywordToken(const std::string & type) : AbstractToken(type) {}
    virtual void draw(std::ostream& os) const override;
};

/*
 * Variable names
 */
class NameToken : public KeywordToken {
public:
    explicit NameToken(const char * name) : KeywordToken("NAME"), m_name(name) {}
    explicit NameToken(const std::string & name) : KeywordToken("NAME"), m_name(std::string(name)) {}
    virtual void draw(std::ostream& os) const override;
    const std::string& get_name();
protected:
    std::string m_name;
};

class OperatorToken : public KeywordToken {
public:
    explicit OperatorToken(const char * op) : KeywordToken("OP"), m_operator(op) {}
    explicit OperatorToken(const std::string & op) : KeywordToken("OP"), m_operator(op) {}
    virtual void draw(std::ostream& os) const override;
    const std::string& get_operator();
protected:
    std::string m_operator;
};

class TypeToken : public KeywordToken {
public:
    explicit TypeToken(const char * type) : KeywordToken(type) {}
    explicit TypeToken(const std::string & type) : KeywordToken(type) {}
    virtual void draw(std::ostream& os) const override;
};

#endif //COMPILER_CPP_TOKENS_H
