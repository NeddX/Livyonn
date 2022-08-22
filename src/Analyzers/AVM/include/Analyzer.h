#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "Lexer.h"
#include "../../../Compilers/AVM/include/AVM.h"

namespace arba
{
    using namespace std;
    using namespace avm;

    struct Analyzer
    {
        public:
            vector<Instruction> StartAnalysis(const string& src);

        private:
            vector<Instruction> Analyze(vector<Token>& tokens);    
    };
}