//
// Created by jarno on 22/08/2023.
//

#include <iostream>
#include <vector>
#include <memory> // unique_ptr

class Animal {
public:
    virtual void make_sound() = 0;
};

class Pig : public Animal {
public:
    void make_sound() override {
        std::cout << "Oink" << std::endl;
    }
};

class Cow : public Animal {
public:
    void make_sound() override {
        std::cout << "Moo" << std::endl;
    }
};

int main() {
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Pig>());
    animals.push_back(std::make_unique<Cow>());
    for(std::unique_ptr<Animal> const & a : animals) {
        a->make_sound();
    }
    return 0;
}