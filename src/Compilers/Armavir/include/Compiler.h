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
        size_t source = 0;
        Type type;
        size_t size = 0;
    };

    struct Argument
    {
        string name;
        size_t index;
        Type type;
    };

    struct FuncInfo
    {
        uint16_t offset;
        vector<uint32_t> calls;
        FunctionDefinition fd;
        size_t varCount = 0;
        map<string, VarInfo> varOffsets;
        map<string, Argument> args;
        vector<int16_t> returnInstructionAddresses;
    };
    
    struct StatementInfo
    {
        Statement st;
        FuncInfo& fn;

        public:
        StatementInfo(Statement& st, FuncInfo& fn)
        : st(st), fn(fn) {};
    };

    struct Compiler
    {
        private:
            map<string, FuncInfo> funcOffsets;
            ByteCode compiledCode;

        public:
            ByteCode Compile(Analyzer& syntaxTree);
            void FunctionCodeGen(FuncInfo& func);
            bool StatementCodeGen(StatementInfo& sti);
            void FunctionEpilogue(const FuncInfo& fn);
    };  
}