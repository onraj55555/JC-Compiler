//
// Created by jarno on 03/09/2023.
//

#include "PreProcesserHelper.h"

bool PreProcesserHelper::is_include(std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>> &row) {
    // #include<lib> ?
    if(row->size() != 4) {
        return false;
    }

    // #include ?
    if(!HelperSpace::instanceof<KeywordToken>((*row)[0])) {
        return false;
    }

    // Misschien functie in Helper om te testen of een string de include string is
    if((*row)[0]->get_type() != "INCL") {
        return false;
    }

    // #include< ?
    if(!HelperSpace::instanceof<OperatorToken>((*row)[1])) {
        return false;
    }

    if(((OperatorToken &)*(*row)[1]).get_operator() != "LT") {
        return false;
    }

    // #include<lib ?
    if(!HelperSpace::instanceof<NameToken>((*row)[2])) {
        return false;
    }

    // #include<lib> ?
    if(!HelperSpace::instanceof<OperatorToken>((*row)[3])) {
        return false;
    }

    if(((OperatorToken &)*(*row)[3]).get_operator() != "GT") {
        return false;
    }

    return true;
}

AbstractState *IdleState::get_next_state(const std::unique_ptr<AbstractToken> & token) {
    // fn ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());

        if(keywordToken->get_type() == Helper::get_keyword("fn")) {
            return new FunctionStates::FNFunctionState();
        }

        if(keywordToken->get_type() == Helper::get_keyword("for")) {
            return new ForLoopState::FORState();
        }

        if(keywordToken->get_type() == Helper::get_keyword("while") || keywordToken->get_type() == Helper::get_keyword("if")) {
            return new ConditionState::CONDITIONLoopState();
        }

        if(keywordToken->get_type() == Helper::get_keyword("else")) {
            return new ElseState::ELSEState();
        }

        if(keywordToken->get_type() == "SEMI") {
            return new EndState();
        }

        /*if(keywordToken->get_type() == Helper::get_bracket_type('}')) {
            return new EndState();
        }*/
    }
    return new IdleState();
}

AbstractState *InvalidState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    return new IdleState();
}

AbstractState *EndState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    return new IdleState();
}

AbstractState *FunctionStates::FirstCurlyState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    return new IdleState();
}

AbstractState *ConditionState::FirstCurlyState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    return new IdleState();
}

AbstractState *ForLoopState::FirstCurlyState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    return new IdleState();
}

AbstractState *ElseState::FirstCurlyState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    return new IdleState();
}

//----------START FUNCTION STATES----------//

AbstractState *FunctionStates::FNFunctionState::get_next_state(const std::unique_ptr<AbstractToken> & token) {
    // fn( ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type('(')) {
            return new FirstBracketState();
        }
    }
    return new InvalidState();
}

AbstractState *FunctionStates::FirstBracketState::get_next_state(const std::unique_ptr<AbstractToken> & token) {
    // IDEA: misschien fn() = fn(void)

    // fn(type ?
    if(HelperSpace::instanceof<TypeToken>(token)) {
        return new ReturnTypeState();
    }
    return new InvalidState();
}

AbstractState *FunctionStates::ReturnTypeState::get_next_state(const std::unique_ptr<AbstractToken> & token) {
    // fn(type) ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new SecondBracketState();
        }
    }
    return new InvalidState();
}

AbstractState *FunctionStates::SecondBracketState::get_next_state(const std::unique_ptr<AbstractToken> & token) {
    // fn(type) function_name ?
    if(HelperSpace::instanceof<NameToken>(token)) {
        return new FunctionNameState();
    }
    return new InvalidState();
}

AbstractState *FunctionStates::FunctionNameState::get_next_state(const std::unique_ptr<AbstractToken> & token) {
    // fn(type) function_name( ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type('(')) {
            return new ThirdBracketState();
        }
    }
    return new InvalidState();
}

AbstractState *FunctionStates::ThirdBracketState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    // fn(type) function_name(type ?
    if(HelperSpace::instanceof<TypeToken>(token)) {
        return new VariableTypeState();
    }

    // fn(type) function_name() ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new FourthBracketState();
        }
    }

    return new InvalidState();
}

AbstractState *FunctionStates::VariableTypeState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    // fn(type) function_name(type varname ?
    if(HelperSpace::instanceof<NameToken>(token)) {
        return new VariableNameState();
    }

    return new InvalidState();
}

AbstractState *FunctionStates::VariableNameState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    // fn(type) function_name(type varname, ?
    if(HelperSpace::instanceof<OperatorToken>(token)) {
        OperatorToken * operatorToken = dynamic_cast<OperatorToken*>(token.get());
        if(operatorToken->get_operator() == Helper::get_operator_type(',')) {
            return new CommaState();
        }
    }


    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new FourthBracketState();
        }
    }

    return new InvalidState();
}

AbstractState *FunctionStates::FourthBracketState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    // fn(type) function_name(type varname) ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type('{')) {
            return new FunctionStates::FirstCurlyState();
        }
    }
}

AbstractState *FunctionStates::CommaState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    // fn(type) function_name(type varname, type ?
    if(HelperSpace::instanceof<TypeToken>(token)) {
        return new VariableTypeState();
    }

    // fn(type) function_name(type varname,) ?
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new FourthBracketState();
        }
    }
}

// ----------START FOR STATES----------//

AbstractState *ForLoopState::FORState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type('(')) {
            return new ForLoopState::FirstBracketState();
        }
    }

    return new InvalidState();
}

AbstractState *ForLoopState::FirstBracketState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<OperatorToken>(token) || HelperSpace::instanceof<NameToken>(token) || HelperSpace::instanceof<TypeToken>(token) || HelperSpace::instanceof<LiteralToken>(token)) {
        return new ForLoopState::FirstBracketState();
    }

    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == "SEMI") {
            return new ForLoopState::FirstSemiState();
        }
    }

    return new InvalidState();
}

AbstractState *ForLoopState::FirstSemiState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<OperatorToken>(token) || HelperSpace::instanceof<NameToken>(token) || HelperSpace::instanceof<TypeToken>(token) || HelperSpace::instanceof<LiteralToken>(token)) {
        return new ForLoopState::FirstSemiState();
    }

    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == "SEMI") {
            return new ForLoopState::SecondSemiState();
        }
    }

    return new InvalidState();
}

AbstractState *ForLoopState::SecondSemiState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<OperatorToken>(token) || HelperSpace::instanceof<NameToken>(token) || HelperSpace::instanceof<TypeToken>(token) || HelperSpace::instanceof<LiteralToken>(token)) {
        return new ForLoopState::SecondSemiState();
    }

    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new SecondBracketState();
        }
    }

    return new InvalidState();
}

AbstractState *ForLoopState::SecondBracketState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type('{')) {
            return new ForLoopState::FirstCurlyState();
        }
    }

    return new InvalidState();
}

//----------START WHILE STATE----------//

AbstractState *ConditionState::CONDITIONLoopState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken*>(token.get());
        if(keywordToken->get_type() == Helper::get_bracket_type('(')) {
            return new ConditionState::FirstBracketState();
        }
    }

    return new InvalidState();
}

AbstractState *ConditionState::FirstBracketState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken *keywordToken = dynamic_cast<KeywordToken *>(token.get());
        if (keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new InvalidState();
        }
    }

    return new ConditionState::NotEmptyState();
}

AbstractState *ConditionState::NotEmptyState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<OperatorToken>(token)) {
        return new ConditionState::NotEmptyState();
    }

    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken *>(token.get());

        if(keywordToken->get_type() == Helper::get_bracket_type(')')) {
            return new ConditionState::SecondBracketState();
        }

        return new InvalidState();
    }

    return new ConditionState::NotEmptyState();
}

AbstractState *ConditionState::SecondBracketState::get_next_state(const std::unique_ptr<AbstractToken> &token) {
    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken *>(token.get());

        if(keywordToken->get_type() == Helper::get_bracket_type('{')) {
            return new ConditionState::FirstCurlyState();
        }
    }

    return new InvalidState();
}

AbstractState *ElseState::ELSEState::get_next_state(const std::unique_ptr<AbstractToken> &token) {


    if(HelperSpace::instanceof<KeywordToken>(token)) {
        KeywordToken * keywordToken = dynamic_cast<KeywordToken *>(token.get());

        if(keywordToken->get_type() == Helper::get_keyword("if")) {
            return new ConditionState::CONDITIONLoopState();
        }

        if(keywordToken->get_type() == Helper::get_bracket_type('{')) {
            return new ElseState::FirstCurlyState();
        }
    }

    return new InvalidState();
}