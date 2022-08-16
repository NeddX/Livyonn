#pragma once

#include <iostream>
#include <vector>

#include "Type.h"
#include "Statement.h"
#include "../../../Lib/include/Define.h"

namespace arma
{
    using namespace std;

    struct ParamDefinition
    {
        public:
            string name;
            Type type;

    };

    struct FunctionDefinition
    {
        public:
            string name;
            vector<ParamDefinition> params;
            vector<Statement> statements;
            
            void FuncDumpTruck() const;
    };
}