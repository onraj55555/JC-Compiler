//
// Created by jarno on 02/09/2023.
//

#include "../../Errors.h"

using namespace std;

int main() {
    CompilerError * syntaxError = new SyntaxError("bla bla");
    LineDecoratorError lineDecoratorError(syntaxError, 5);
    cout << lineDecoratorError << endl;
    return 0;
}