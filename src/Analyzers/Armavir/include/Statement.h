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
        FUNCTION_CALL,
        LITERAL,
        OPERATOR_CALL
    };

    enum OpKind
    {
        BLOCK,

    };

    static const string kindStr[] = 
    {
        "VARIABLE_DECLERATION",
        "ASSIGNMENT_EXPRESSION",
        "IDENTIFIER_EXPRESSION",
        "RETURN_STATEMENT",
        "FUNCTION_CALL",
        "LITERAL",
        "OPERATOR_CALL"
    };

    static const string opKindStr[] =
    {
        "enn"
    };

    struct Statement
    {
        string name;
        Type type = Type("void", VOID);
        bool isVar;
        vector<Statement> params;
        Kind kind = FUNCTION_CALL;

        void StDumpTruck() const;
    };
}