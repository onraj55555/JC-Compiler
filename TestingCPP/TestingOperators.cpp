//
// Created by jarno on 1/09/23.
//

#include <iostream>

using namespace std;

int main() {
    int i = 5;
    cout << +--i << endl;
    cout << -++i << endl;
    cout << +--++i << endl;
    (++i)++;
}