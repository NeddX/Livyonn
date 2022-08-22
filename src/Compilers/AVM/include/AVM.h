#pragma once

#include <iostream>
#include <vector>

#include "Instruction.h"
#include "Runtime.h"

namespace avm
{
    using namespace std;

    struct AVM
    {
        public:
            void Start(vector<Instruction>& bytecode);
    };
}