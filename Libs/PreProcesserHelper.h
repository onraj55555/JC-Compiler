//
// Created by jarno on 03/09/2023.
//

#ifndef COMPILER_CPP_PREPROCESSERHELPER_H
#define COMPILER_CPP_PREPROCESSERHELPER_H

#include "Tokens.h"
#include "Helper.h"
#include <string>
#include <vector>
#include <memory>

class PreProcesserHelper {
public:
    static bool is_include(std::unique_ptr<std::vector<std::unique_ptr<AbstractToken>>>& row);
};

class AbstractState {
public:
    //virtual bool check(const std::unique_ptr<AbstractToken>& token) { return false; }
    virtual AbstractState * get_next_state(const std::unique_ptr<AbstractToken> & token) = 0;
    virtual bool is_end() {
        return false;
    }
    //AbstractState& operator=(AbstractState&& other);
};

class IdleState : public AbstractState {
public:
    //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }
    AbstractState * get_next_state(const std::unique_ptr<AbstractToken> & token) override;
};

class InvalidState : public AbstractState {
    AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    bool is_end() override {
        return true;
    }
};

class EndState : public AbstractState {
    AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    bool is_end() override {
        return true;
    }
};

namespace FunctionStates {
    class FNFunctionState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstBracketState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class ReturnTypeState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class SecondBracketState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class ThirdBracketState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FunctionNameState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FourthBracketState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class VariableTypeState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class VariableNameState : public AbstractState {
    public:
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class CommaState : public AbstractState {
        //bool check(const std::unique_ptr<AbstractToken> &token) override { return false; }

        AbstractState *get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstCurlyState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
        bool is_end() override {
            return true;
        }
    };

}

namespace ForLoopState {
    class FORState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstBracketState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstSemiState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class SecondSemiState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class SecondBracketState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstCurlyState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
        bool is_end() override {
            return true;
        }
    };
}

namespace ConditionState {
    class CONDITIONLoopState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstBracketState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class NotEmptyState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class SecondBracketState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstCurlyState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
        bool is_end() override {
            return true;
        }
    };
}

namespace ElseState {
    class ELSEState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
    };

    class FirstCurlyState : public AbstractState {
        AbstractState * get_next_state(const std::unique_ptr<AbstractToken> &token) override;
        bool is_end() override {
            return true;
        }
    };
}
#endif //COMPILER_CPP_PREPROCESSERHELPER_H
