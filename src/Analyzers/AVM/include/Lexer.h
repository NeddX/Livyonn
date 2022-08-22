#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "../../../Lib/include/Define.h"
#include "../../../Compilers/AVM/include/AVM.h"

namespace arba
{
    using namespace std;
    using namespace avm;
    
    enum TokenType
    {
        WHITESPACE,
        COMMENT,
        INST,
        NUM,
        REG,
        LABEL_DEFINITION,
        LABEL_CALL
    };

    static const char* tokenTypeString[] =
    {
        "WHITESPACE",
        "COMMENT",
        "INST",
        "NUM",
        "REG",
        "LABEL_DEFINITION",
        "LABEL_CALL"
    };

    struct Token
    {
        public:
            TokenType type {WHITESPACE};
            string text;
            int data;
            //string text;
            size_t line = 0;
            size_t cur = 0;
            void TDumpTruck() const;
    };

    struct Lexer
    {
        static vector<Token> Lex(const string& src);
    };

    void EndToken(Token& t, vector<Token>& tokens);
    OpCode GetInst(string& inst);
    Regs64 GetReg(string& reg);
}