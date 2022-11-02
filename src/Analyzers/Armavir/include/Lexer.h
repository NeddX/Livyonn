#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <string>

#include "../../../Lib/include/Define.h"

namespace arma
{
    using namespace std;
    
    enum TokenType
    {
        NONE,
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
        RANGE_COMMENT,
        POTENTIAL_RANGE_END,
        SEMICOLON,
        INCLUDE_DIRECTIVE,
        ESCAPE_SEQUENCE,
        POTENTIAL_DOUBLE_OPERATOR,
        DOUBLE_OPERATOR
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
        "RANGE_COMMENT",
        "POTENTIAL_RANGE_END",
        "SEMICOLON",
        "INCLUDE_DIRECTIVE",
        "POTENTIAL_DOUBLE_OPERATOR",
        "DOUBLE_OPERATOR"
    };

    struct Token
    {
        public:
            TokenType type = NONE;
            TokenType subType = NONE;
            string text;
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
}