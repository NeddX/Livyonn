#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "../../../Lib/include/Define.h"
#include "../../../Runtime/AVM/include/AVM.h"

namespace amca
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
            size_t line = 0;
            size_t cur = 0;
            void TDumpTruck() const;
    };
    typedef vector<Token> TokenList;

    struct Lexer
    {
        static TokenList Lex(const string& src);
    };

    void EndToken(Token& t, TokenList& tokens);
    OpCode GetInst(string& inst);
    Regs GetReg(string& reg);
}