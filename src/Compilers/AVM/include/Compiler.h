#pragma once

#include "../../../Runtime/AVM/include/AVM.h"
#include "../../../Analyzers/AVM/include/Analyzer.h"

namespace amcc
{
    using namespace std; // STD Is already included in AVM/ARBA so using AVM/ARBA would automatically include STD which might cause confusion.
    using namespace avm;
    using namespace amca;

    struct Compiler
    {
        ByteCode Compile(const TokenList& tokens);
    };
}