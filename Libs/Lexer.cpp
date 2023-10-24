//
// Created by jarno on 22/08/2023.
//

#include "Tokens.h"
#include "Lexer.h"
#include "Helper.h"

void Lexer::parse_operators(std::string &operator_string, std::vector<std::unique_ptr<AbstractToken>>& row) {
    if(Helper::is_double_operator(operator_string)) {
        //std::cout << operator_string << std::endl;
        row.push_back(std::make_unique<OperatorToken>(Helper::get_operator_type(operator_string)));
        operator_string.clear();
        return;
    }

    row.push_back(std::make_unique<OperatorToken>(Helper::get_operator_type(operator_string[0])));
    //std::cout << operator_string[0] << std::endl;
    if(operator_string.size() == 2) {
        char temp = operator_string[1];
        operator_string.clear();
        operator_string += temp;
        return;
    }
    operator_string.clear();
}

void Lexer::parse_string(std::string &string, std::vector<std::unique_ptr<AbstractToken>>& row) {
    if(string.empty()) {
        return;
    }

    // Check for trailing whitespaces, if parsed could result in error
    for (const char c: string) {
        if (Helper::is_whitespace(c)) {
            return;
        }
    }

    if(Helper::is_single_operator(string)) {
        parse_operators(string, row);
        return;
    }

    //Check keywords
    // Return: return
    /*if(string == "return") {
        row.push_back(std::make_unique<KeywordToken>("RET"));
        string.clear();
        return;
    }

    // Include: #include
    if(string == "#include") {
        std::cout << "FOUND INCLUDE" << std::endl;
        row.push_back(std::make_unique<KeywordToken>("INCL"));
        string.clear();
        return;
    }*/
    if(Helper::is_keyword(string)) {
        row.push_back(std::make_unique<KeywordToken>(Helper::get_keyword(string)));
        string.clear();
        return;
    }

    //Check literals
    // Ascii Char?
    if(Helper::is_ascii_char(string)) {
        std::string value;
        value += string[1];
        row.push_back(std::make_unique<LiteralToken>(Helper::get_type("char"), value));
        string.clear();
        return;
    }

    // TODO: eigen functie maken voor to int en to float zodat in bits omgezet voor nauwkeurugheid

    // Int32?
    if(Helper::is_int32(string)) {
        row.push_back(std::make_unique<LiteralToken>(Helper::get_type("int"), string));
        string.clear();
        return;
    }

    // Float32?
    if(Helper::is_float32(string)) {
        double val = std::stod(string);
        row.push_back(std::make_unique<LiteralToken>(Helper::get_type("float"), string)); // int i.p.v. float in token en val -> foreach bit in val zelfde bit in val_int
        string.clear();
        return;
    }

    if(Helper::is_type(string)) {
        row.push_back(std::make_unique<TypeToken>(Helper::get_type(string)));
        string.clear();
        return;
    }

    // Variable name?
    if(Helper::is_valid_variable_name(string)) {
        row.push_back(std::make_unique<NameToken>(string));
        string.clear();
        return;
    }

    SyntaxError * syntaxError = new SyntaxError(string);
    throw syntaxError;
}

void Lexer::parse_line(const std::string &line) {
    std::string string;
    std::vector<std::unique_ptr<AbstractToken>> row;
    if(line.empty()) {
        //std::cout << "Empty line" << std::endl;
        return;
    }

    try {
        for (const char c: line) {
            // BUG
            // TODO: oplossen
            if(string == "/" && c == '/') {
                m_tokens.push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(row)));
                return;
            }

            // Break on whitespace
            if (Helper::is_whitespace(c)) {
                parse_string(string, row);
                continue;
            }

            if (Helper::is_line_end(c)) {
                parse_string(string, row);
                row.push_back(std::make_unique<KeywordToken>("SEMI"));
                continue;
            }

            if (Helper::is_bracket(c)) {
                parse_string(string, row);
                row.push_back(std::make_unique<KeywordToken>(Helper::get_bracket_type(c)));
                continue;
            }

            /*if(Helper::only_operators(string) && !Helper::is_single_operator(c)) {
                Helper::parse_operators(string, row);
                string.clear();
                continue;
            }*/

            if(Helper::is_single_operator(string) && Helper::is_single_operator(c)) {
                string += c;
                parse_operators(string, row);
                continue;
            }

            if(Helper::is_single_operator(string) && !Helper::is_single_operator(c)) {
                //std::cout << string << std::endl;
                parse_operators(string, row);
                string += c;
                continue;
            }

            if(Helper::is_single_operator(c)) {
                //std::cout << string << std::endl;
                parse_string(string, row);
                string += c;
                continue;
            }

            string += c;
        }

        if(Helper::is_single_operator(string)) {

            Lexer::parse_operators(string, row);
            m_tokens.push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(row)));
            return;
        }

        parse_string(string, row);
        m_tokens.push_back(std::make_unique<std::vector<std::unique_ptr<AbstractToken>>>(std::move(row)));
    }
    catch (const CompilerError * compilerError) {
        throw compilerError;
    }
}

void Lexer::lex() {
    std::fstream file(m_filename);

    if(!file.is_open()) {
        CompilerError * error = new InformationError("Could not open file: " + m_filename);
        throw error;
    }

    std::string line;
    int line_number = 1;
    while(std::getline(file, line)) {
        //std::cout << "Parsing " << line << std::endl;
        try {
            parse_line(line);
            line_number++;
        }
        catch (const CompilerError * compilerError) {
            file.close();
            CompilerError * lineDecoratorError = new LineDecoratorError(compilerError, line_number);
            throw lineDecoratorError;
        }
    }

    file.close();
}

void Lexer::print(std::ostream &os) {
    int rows = m_tokens.size();
    //std::cout << "Number of lines: " << rows << std::endl;
    for(const auto& row : m_tokens) {
        for(const auto& element : *row) {
            os << *element;
        }
        os << std::endl;
    }
}

Lexer::Lexer(char *filename) : m_filename(filename) {
    /*std::fstream keywords("D:\\Programming\\C++\\Compiler_CPP\\keywords.txt");

    if(!keywords.is_open()) {
        std::string error("Could not open keywords.txt, maybe it's not present?");
        throw error;
    }

    std::string line;
    while(std::getline(keywords, line)) {
        if(line.empty()) {
            continue;
        }
        std::string key, val;
        int index = line.find(" ");
        if(index == std::string::npos) {
            std::string error("Error while parsing keywords: ");
            error += line;
            throw error;
        }
        key = line.substr(0, index);
        val = line.substr(index + 1);
        Lexer::keywords[key] = val;
    }*/
}

std::vector<std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>> &Lexer::get_output() {
    return m_tokens;
}