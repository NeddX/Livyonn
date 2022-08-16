#pragma once

#include <iostream>
#include <vector>
#include <exception>

#include "../../../Lib/include/Define.h"

namespace arma
{
    using namespace std;
    
    enum TokenType
    {
        WHITESPACE,
        IDENTIFIER,
        INTEGER_LITERAL,
        STRING_LITERAL,
        STRING_INT_LITERAL,
        BOOLEAN_LITERAL,
        DOUBLE_LITERAL,
        POTENTIAL_DOUBLE,
        OPERATOR,
        STRING_ESC,
        COMMENT,
        POTENTIAL_COMMENT,
        SEMICOLON
    };

    static const char* tokenTypeString[] =
    {
        "WHITESPACE",
        "IDENTIFIER",
        "INTEGER_LITERAL",
        "STRING_LITERAL",
        "STRING_INT_LITERAL",
        "BOOLEAN_LITERAL",  
        "DOUBLE_LITERAL",
        "POTENTIAL_DOUBLE",
        "OPERATOR",
        "STRING_ESC",
        "COMMENT",
        "POTENTIAL_COMMENT",
        "SEMICOLON"
    };

    struct Token
    {
        public:
            TokenType type {WHITESPACE};
            string text;
            size_t line = 0;
            size_t cur = 0;
            void TDumpTruck() const;
    };

    struct Lexer
    {
        static vector<Token> Lex(const string& src);
    };

    void EndToken(Token& t, vector<Token>& tokens);
}