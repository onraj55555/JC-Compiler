//
// Created by jarno on 12/09/23.
//

#ifndef COMPILER_CPP_TREEHELPER_H
#define COMPILER_CPP_TREEHELPER_H

#include <string>
#include "Helper.h"
#include "Errors.h"

class TreeHelper {
private:
    static enum operator_scores {
        OR = 0,
        AND,
        EQ_NOTEQ,
        LT_GT_LTEQ_GTEQ,
        PLUS_MIN,
        MUL_DIV
    };
public:
    static int get_data_type_size(const std::string & data_type) {
        if(!Helper::is_type(data_type)) {
            InformationError * error = new InformationError(data_type);
            error->operator+=(" is not a valid data type!");
            throw error;
        }

        if(data_type == "INT") {
            return 4;
        }

        if(data_type == "CHA") {
            return 1;
        }

        if(data_type == "FLO") {
            return 4;
        }

        if(data_type == "VOID") {
            return 0;
        }
    }

    static unsigned int get_operator_strength(OperatorToken * operator_token) {
        if(operator_token->get_operator() == Helper::get_operator_type("||")) {
            return operator_scores::OR;
        }

        if(operator_token->get_operator() == Helper::get_operator_type("&&")) {
            return operator_scores::AND;
        }

        if(operator_token->get_operator() == Helper::get_operator_type("==") || operator_token->get_operator() == Helper::get_operator_type("!=")) {
            return operator_scores::EQ_NOTEQ;
        }

        if(operator_token->get_operator() == Helper::get_operator_type('<') || operator_token->get_operator() == Helper::get_operator_type('>') || operator_token->get_operator() == Helper::get_operator_type("<=") || operator_token->get_operator() == Helper::get_operator_type(">=")) {
            return operator_scores::LT_GT_LTEQ_GTEQ;
        }

        if(operator_token->get_operator() == Helper::get_operator_type('+') || operator_token->get_operator() == Helper::get_operator_type('-')) {
            return operator_scores::PLUS_MIN;
        }

        if(operator_token->get_operator() == Helper::get_operator_type('*') || operator_token->get_operator() == Helper::get_operator_type('/') || operator_token->get_operator() == Helper::get_operator_type('%')) {
            return operator_scores::MUL_DIV;
        }

        return (unsigned int)-1;
    }
};


#endif //COMPILER_CPP_TREEHELPER_H
