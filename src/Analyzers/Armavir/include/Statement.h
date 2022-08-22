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
        FUNCTION_CALL,
        LITERAL,
        OPERATOR_CALL
    };

    static const string KindStr[] = 
    {
        "VARIABLE_DECLERATION",
        "FUNCTION_CALL",
        "LITERAL",
        "OPERATOR_CALL"
    };

    struct Statement
    {
        public:
            string name;
            Type type = Type("void", VOID);
            bool isVar;
            vector<Statement> params;
            Kind kind = FUNCTION_CALL;

            void StDumpTruck() const;
    };
}