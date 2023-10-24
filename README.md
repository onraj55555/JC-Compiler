# Compiler_CPP

Compiler written in C++ for the new language: .jc (future .jcx?)

TODO:
- Lexer
- Parser
- Syscalls voor nasm
- Linking

Idee
- Voor externe standaard lib, vb print, een andere file met de naam van de functie er in en de syntax, dan er achter de asm om te printen
- Bij iedere functie oproep, push iedere register naar de stack, voer de functie uit en kijk welke niet gebruikt zijn, undo de push van die registers
- Stack als vector van std::string -> push int i == i, i, i, i in stack om gemakkelijk terug te vinden