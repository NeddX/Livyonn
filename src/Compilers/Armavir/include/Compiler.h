#pragma once

#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <fstream>
#include <filesystem>

#include "../../../Runtime/AVM/include/AVM.h"
#include "../../../Analyzers/Armavir/include/Analyzer.h"
#include "../../../Lib/include/System.h"

namespace arc
{
    using namespace avm;
    using namespace arma;

    enum OutputType
    {
        EXE,
        DYN_LIB,
        STATIC_LIB
    };

    struct CompilerOptions
    {
        OutputType outputType = EXE;
        bool outputExecutable = false;
        string path;
    };

    struct VarInfo
    {
        size_t index = 0;
        size_t size = 0;
        Type type;
    };

    struct Argument
    {
        string name;
        size_t index;
        size_t size;
        Type type;
    };

    struct FuncInfo
    {
        FunctionDefinition fd;

        size_t addr = 0;

        size_t varCount = 0;
        map<string, VarInfo> varOffsets;
        vector<VarInfo> varAddr;

        vector<size_t> calls;
        map<string, Argument> args;

        vector<int16_t> returnInstructionAddresses;

        vector<size_t> ifInsts;

        Type returnType;
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
            ByteCode Compile(Analyzer& syntaxTree, CompilerOptions* options = nullptr);

        private:
            void FunctionCodeGen(FuncInfo& func);
            bool StatementCodeGen(StatementInfo& sti);
            void FunctionEpilogue(const FuncInfo& fn);
            void WriteToBinary(CompilerOptions* options = nullptr);
    };  
}