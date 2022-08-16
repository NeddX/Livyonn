#pragma once

#include <iostream>
#include <vector>

namespace arma
{
    using namespace std;

    enum FundamentalTypes
    {
        INT8,
        UINT8,
        INT32,
        UINT32,
        DOUBLE,
        STRING,
        STRING_INT,
        STRUCT,
        DYNAMIC,
        VOID,
        BOOLEAN
    };

    struct Type
    {
        public:
            string name;
            FundamentalTypes fType;
            vector<Type> field;
        
            Type(string name = "", FundamentalTypes fType = VOID) : name(name), fType(fType) {};
    };
}