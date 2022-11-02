#pragma once

#include <vector>
#include <optional>
#include <iostream>
#include <map>
#include <string>

#include "Lexer.h"
#include "Type.h"
#include "FunctionDefinition.h"
#include "Statement.h"
#include "../../../Lib/include/Define.h"

namespace arma
{
    using namespace std; 

    struct Analyzer
    {   
        private:
            void Analyze(TokenList& tokens);
            void LookForDirectives(string& src);
            optional<Type> ExpectType(); 
            optional<Token> ExpectIdentifier(const string& name = "");
            optional<Token> ExpectOperator(const string& name = "");
            bool ExpectFunction();
            void ExpectIncludeDirective(TokenList& tokens);
            optional<vector<Statement>> AnalyzeFunctionBody();
            optional<Statement> ExpectVariableDecleration();
            optional<Statement> ExpectFunctionCall();
            optional<Statement> ExpectValue();
            optional<Statement> ExpectStatement();
            optional<Statement> ExpectExpression();
            optional<Statement> ExpectKeyword();
            optional<Type> GetOperationType(optional<Statement> st);
            size_t GetOpPriority(const string& name);
            Statement* GetRightMostStatement(Statement* lhs, size_t rhvPriority);

            TokenList::iterator currentToken;
            TokenList::iterator endToken;
            map<string, Type> types;
            map<string, FunctionDefinition> funcs;

        public:
            Analyzer();
            void StartAnalysis(string& src);
            void DumpTruck() const;
            map<string, FunctionDefinition> GetFunctions() { return funcs; }
            map<string, Statement> variableMap;
            map<string, ArgDefinition> currentFuncArgs;
    };
}