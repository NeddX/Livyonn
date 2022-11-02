#pragma once

#include <iostream>
#include <vector>

#include "Type.h"
#include "../../../Lib/include/Define.h"

namespace arma
{
    using namespace std;

    enum Kind
    {
        VARIABLE_DECLERATION,
        ASSIGNMENT_EXPRESSION,
        IDENTIFIER_EXPRESSION,
        RETURN_STATEMENT,
        WHILE_STATEMENT,
        IF_STATEMENT,
        ELSE_IF_STATEMENT,
        ELSE_STATEMENT,
        BREAK_STATEMENT,
        FUNCTION_CALL,
        POST_INCREMENT_EXPRESSION,
        PRE_INCREMENT_EXPRESSION,
        POST_DECREMENT_EXPRESSION,
        PRE_DECREMENT_EXPRESSION,
        LITERAL,
        OPERATOR_CALL
    };

    static const string kindStr[] = 
    {
        "VARIABLE_DECLERATION",
        "ASSIGNMENT_EXPRESSION",
        "IDENTIFIER_EXPRESSION",
        "RETURN_STATEMENT",
        "WHILE_STATEMENT",
        "IF_STATEMENT",
        "ELSE_IF_STATEMENT",
        "ELSE_STATEMENT",
        "BREAK_STATEMENT",
        "FUNCTION_CALL",
        "LITERAL",
        "OPERATOR_CALL"
    };

    enum OperationType
    {
        LOCAL_REFERENCE,
        ARGUMENT_REFERENCE
    };

    struct Operation
    {
        OperationType type;
    };

    struct Statement
    {
        string name;
        Type type = Type("void", VOID);
        bool isVar;
        vector<Statement> args;
        vector<Statement> body;
        Kind kind = FUNCTION_CALL;
        Operation op;

        void StDumpTruck() const;
    };
}