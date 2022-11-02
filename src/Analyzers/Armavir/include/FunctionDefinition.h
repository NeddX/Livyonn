#pragma once

#include <iostream>
#include <vector>

#include "Type.h"
#include "Statement.h"
#include "../../../Lib/include/Define.h"

namespace arma
{
    using namespace std;

    struct ArgDefinition
    {
        public:
            string name;
            Type type;
    };

    struct FunctionDefinition
    {
        public:
            string name;
            Type type;
            vector<ArgDefinition> args;
            vector<Statement> body;
            
            void FuncDumpTruck() const;
    };
}