#pragma once

#include <string>
#include <vector>
#include <map>
#include <cassert>

#include "../../../Runtime/AVM/include/AVM.h"
#include "../../../Analyzers/Armavir/include/Analyzer.h"

namespace arc
{
    using namespace avm;
    using namespace arma;

    struct VarInfo
    {
        int16_t offset;
        Type type;
    };

    struct Compiler
    {
        ByteCode Compile(Analyzer& syntaxTree, ByteCode& bytecode);
        void FunctionCodeGen(const FunctionDefinition& func, ByteCode& compiledCode);
        void StatementCodeGen(const Statement& st, map<string, VarInfo> varOffsets, ByteCode& compiledCode);
    };  
}