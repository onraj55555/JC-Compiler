//
// Created by jarno on 02/09/2023.
//
#include <iostream>
#include <memory>

using namespace std;

class Base {
public:
    virtual void Operation() = 0;
};

class A : public Base {
public:
    void Operation() override {
        cout << "Class A called" << endl;
    }
};

class Decorator : public Base {
protected:
    Base * m_base;
public:
    Decorator(Base * base) : m_base(base) {}
    void Operation() override {
        m_base->Operation();
    }
};

class DecoratorA : public Decorator {
public:
    DecoratorA(Base * base) : Decorator(base) {}
    void Operation() override {
        cout << "Decorator A called on: ";
        m_base->Operation();
    }
    ~DecoratorA() {
        delete m_base;
    }
};

int main() {
    Base * objA = new A();
    DecoratorA decoratorA(objA);
    decoratorA.Operation();
    return 0;
}