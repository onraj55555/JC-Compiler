//
// Created by jarno on 22/08/2023.
//

#include <memory>
#include <iostream>

int main() {
    int i = 5;
    auto ip = std::make_unique<int>(i);
    std::cout << *ip.get() << std::endl;
    return 0;
}