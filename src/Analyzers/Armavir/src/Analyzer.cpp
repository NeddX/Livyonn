#include "../include/Analyzer.h"

namespace arma
{
    struct OpEntry
    {
        string name;
        size_t priority;
        
        public:
            OpEntry(string name, size_t priority)
            {
                this->name = name;
                this->priority = priority;
            }
    };

    map<string, OpEntry> opEntries = 
    {
        { "+", OpEntry("+", 10) },
        { "-", OpEntry("-", 10) },
        { "*", OpEntry("*", 20) },
        { "/", OpEntry("/", 20) },
        { "=", OpEntry("=", 1) }
    };

    bool Analyzer::ExpectFunction()
    {
        auto analyzeStart = currentToken;
        if (currentToken->text == "fn")
        {
            currentToken++;
            auto possibleType = ExpectType();

            FunctionDefinition func;

            if (!possibleType)
                currentToken--; // Dynamic function

            auto possibleName = ExpectIdentifier();
            auto possibleOperator = ExpectOperator("(");

            if (possibleOperator)
            {
                //cout << "funkchn: " << possibleName->text << endl;
                func.name = possibleName->text;

                while (!ExpectOperator(")"))
                {
                    if (ExpectOperator(","))
                        continue;

                    auto possibleType = ExpectType();
                    if (!possibleType)
                    {
                        cerr << "Unknown argument type '" << currentToken->text << "' @ line (" << currentToken->line << "," << currentToken->cur << ")." << endl;
                        exit(1);
                    }

                    auto possibleIdentifier = ExpectIdentifier();
                    ParamDefinition param;
                    param.type = *possibleType;
                    if (possibleIdentifier)
                        param.name = possibleIdentifier->text;

                    func.params.push_back(param);
                }

                optional<vector<Statement>> statements = AnalyzeFunctionBody();
                if (!statements.has_value())
                {
                    currentToken = analyzeStart;
                    return false;
                }

                func.statements = *statements;
                funcs[func.name] = func;

                return true;
            }
            else
                currentToken = analyzeStart;
        }

        return false;
    }
    void Analyzer::StartAnalysis(const string& src)
    {
        TokenList tokens = Lexer::Lex(src);
        for (auto& t : tokens) t.TDumpTruck(); // debug print
        Analyze(tokens);
    }
    void Analyzer::Analyze(TokenList &tokens)
    {
        endToken = tokens.end();
        currentToken = tokens.begin();

        while (currentToken != tokens.end())
        {
            if (ExpectFunction())
            {
            }
            else
            {
                throw runtime_error(string("Unknown Identifier '" + currentToken->text + "' @ line (" + to_string(currentToken->line) + "," + to_string(currentToken->cur) + ")."));
                currentToken++;
                // return;
            }
        }
    }

    optional<Token> Analyzer::ExpectIdentifier(const string& name)
    {
        if (currentToken == endToken)
            return nullopt;
        if (currentToken->type != IDENTIFIER)
            return nullopt;
        if (!name.empty() && currentToken->text != name)
            return nullopt;

        Token returnToken = *currentToken;
        currentToken++;
        return returnToken;
    }

    optional<Token> Analyzer::ExpectOperator(const string& name)
    {
        if (currentToken == endToken)
            return nullopt;
        if (currentToken->type != OPERATOR && currentToken->type != SEMICOLON)
            return nullopt;
        if (!name.empty() && currentToken->text != name)
            return nullopt;

        Token returnToken = *currentToken;
        currentToken++;
        return returnToken;
    }

    Analyzer::Analyzer()
    {
        types["int"] = Type("int", INT32);
        types["uint"] = Type("uint", UINT32);
        types["char"] = Type("char", INT8);
        types["uint8"] = Type("uint8", UINT8);
        types["var"] = Type("dynamic", DYNAMIC);
        types["void"] = Type("void", VOID);
        types["double"] = Type("double", DOUBLE);
        types["string"] = Type("string", STRING);
        //types["true"] = Type("boolean", BOOLEAN);
        //types["false"] = Type("boolean", BOOLEAN);
        types["bool"] = Type("boolean", BOOLEAN);
    }

    optional<Type> Analyzer::ExpectType()
    {
        auto possibleType = ExpectIdentifier();
        if (!possibleType)
            return nullopt;
        auto foundType = types.find(possibleType->text);
        if (foundType == types.end())
            return nullopt;

        return foundType->second;
    }

    optional<vector<Statement>> Analyzer::AnalyzeFunctionBody()
    {
        vector<Statement> statements;

        if (!ExpectOperator("{"))
            throw runtime_error(string("Expeccted '{' after function definition @ line (" +
                                       to_string(currentToken->line) + "," + to_string(currentToken->cur) + ")."));

        while (!ExpectOperator("}"))
        {
            optional<Statement> statement = ExpectStatement();
            if (statement)
                statements.push_back(*statement);
            else
                throw runtime_error("Invalid statement '" + currentToken->text + "' @ line (" + to_string(currentToken->line) + "," + to_string(currentToken->cur) + ").");        
            //currentToken++;
        }
        //cout << "reached end of the function" << endl;

        return statements;
    }

    optional<Statement> Analyzer::ExpectValue()
    {

        // if (currentToken != endToken && currentToken->type == DYNAMIC)
        // {
        //     Statement doubleLiteral;
        //     doubleLiteral.name = currentToken->text;
        //     switch (currentToken->type)
        // }
        auto savedToken = currentToken;
        if (currentToken != endToken)
        {
            switch (currentToken->type)
            {
                case INTEGER_LITERAL:
                {
                    Statement intLiteral;
                    intLiteral.kind = LITERAL;
                    intLiteral.name = currentToken->text;
                    intLiteral.type = Type("int", INT32);
                    currentToken++;
                    return intLiteral;
                }
                case DOUBLE_LITERAL:
                {
                    Statement doubleLiteral;
                    doubleLiteral.kind = LITERAL;
                    doubleLiteral.name = currentToken->text;
                    doubleLiteral.type = Type("double", DOUBLE);
                    currentToken++;
                    return doubleLiteral;
                }
                case STRING_LITERAL:
                {
                    Statement strLiteral;
                    strLiteral.kind = LITERAL;
                    strLiteral.name = currentToken->text;
                    strLiteral.type = Type("string", STRING);
                    currentToken++;
                    return strLiteral;
                }
                case BOOLEAN_LITERAL:
                {
                    Statement boolLiteral;
                    boolLiteral.kind = LITERAL;
                    boolLiteral.name = currentToken->text;
                    boolLiteral.type = Type("boolean", BOOLEAN);
                    currentToken++;
                    return boolLiteral;
                }
                case OPERATOR:
                {
                    if (currentToken->text == "$")
                    {
                        currentToken++;
                        optional<Statement> statement = ExpectExpression();
                        if (statement) 
                        {
                            statement->type = Type("string_int", STRING_INT);
                            return statement;
                        }
                    }
                    break;
                }
            }

            if (auto varName = ExpectIdentifier())
            {
                if (ExpectOperator("("))
                {
                    currentToken = savedToken;
                }
                else
                {
                    if (ExpectOperator(")")) // func_call(indetifier')';
                    {
                        Statement varNameStatement;
                        varNameStatement.kind = IDENTIFIER_EXPRESSION;
                        varNameStatement.name = varName->text;
                        currentToken--; // 
                        return varNameStatement;
                    }
                    else
                    {
                        ExpectOperator(";");
                        Statement varNameStatement;
                        varNameStatement.kind = IDENTIFIER_EXPRESSION;
                        varNameStatement.name = varName->text;
                        //currentToken++;
                        return varNameStatement;
                    }
                }
            }

            if (ExpectOperator("("))
            {
                return ExpectExpression();
                if (!ExpectOperator(")"))
                    throw runtime_error("Expected ')' @ line (" + to_string(currentToken->line) + "," + to_string(currentToken->cur) + ").");
            }
        }

        return nullopt;
    }

    optional<Statement> Analyzer::ExpectVariableDecleration()
    {
        auto startToken = currentToken;
        auto possibleType = ExpectType();
        if (!possibleType)
        {
            currentToken = startToken;
            return nullopt;
        }

        // currentToken++;

        auto possibleIdentifier = ExpectIdentifier();
        if (!possibleIdentifier)
        {
            throw runtime_error("Expected an indentifier @ line(" + to_string(currentToken->line) + "," + to_string(currentToken->cur) + ").");
        }

        Statement statement;
        statement.kind = VARIABLE_DECLERATION;
        statement.name = possibleIdentifier->text;
        statement.type = *possibleType;

        if (ExpectOperator("="))
        {
            optional<Statement> value = ExpectExpression();//ExpectValue();
            if (!value)
                throw runtime_error("Unknown type '" + currentToken->text + "' @ line (" + to_string(currentToken->line) + "," + to_string(currentToken->cur) + ")");

            if(!ExpectOperator(";"))//if (currentToken->text != ";" && currentToken->type != SEMICOLON)
                throw runtime_error("Expected ';' @ line (" + to_string((--currentToken)->line) + "," + to_string((--currentToken)->cur) + ").");
            
            statement.params.push_back(*value);
        }
        else if (ExpectOperator(";"))
        {
            cout << "undef" << NL;
        }
        else
        {
            throw runtime_error("Expected ';' after '" + statement.name +
                                "' @ line (" + to_string(--possibleIdentifier->line) + "," + to_string(possibleIdentifier->cur) + ").");
        }

        return statement;
    }

    optional<Statement> Analyzer::ExpectFunctionCall()
    {
        auto analyzeStart = currentToken;

        optional<Token> possibleIdentifier = ExpectIdentifier();
        if (!possibleIdentifier)
        {
            currentToken = analyzeStart;
            return nullopt;
        }

        if (!ExpectOperator("("))
        {
            currentToken = analyzeStart;
            return nullopt;
        }

        Statement fnCall;
        fnCall.kind = FUNCTION_CALL;
        fnCall.name = possibleIdentifier->text;

        while (!ExpectOperator(";"))
        {
            if (ExpectOperator(",") || ExpectOperator("(") || ExpectOperator(")")) continue;

            optional<Statement> param = ExpectExpression();
            if (!param)
                throw runtime_error("Unknown type '" + currentToken->text + "' as function argument @ line (" + to_string(currentToken->line) + "," + to_string(currentToken->cur) + ").");
            fnCall.params.push_back(*param);

            // if (!ExpectOperator(",")) //throw an error
        }
 
        //if (!ExpectOperator(";"))
        //    throw runtime_error("Expected ';' @ line (" + to_string((--currentToken)->line) + "," + to_string((--currentToken)->cur) + ").");

        return fnCall;
    }

    optional<Statement> Analyzer::ExpectStatement()
    {        
        auto result = ExpectVariableDecleration();
        if (result) return result;

        result = ExpectKeyword();
        if (result) return result;
        
        result = ExpectExpression();
        if (result)
        { 
            ExpectOperator(";");
            return result;
        }

        result = ExpectFunctionCall();
        if (result) return result;        


        return nullopt;
    }

    optional<Statement> Analyzer::ExpectExpression()
    {

        auto lhv = ExpectValue();
        if (!lhv)
            return nullopt;
        // if (!lhv) 
        // {
        //     if (ExpectIdentifier())
        //     {
        //         if (ExpectOperator("="))
        //         {
        //             lhv = ExpectExpression();
        //             cout << NL;
        //         }
        //     }
        // }

        while (true)
        {
            optional<Token> op = ExpectOperator();
            if (!op) break;            

            size_t rhvPriority = GetOpPriority(op->text);
            if (rhvPriority == 0)
            {
                currentToken--;
                return lhv;
            }

            optional<Statement> rhv = ExpectValue();
            if (!rhv)
            {
                currentToken--;
                return lhv;
            }

            Statement* rms = GetRightMostStatement(&*lhv, rhvPriority);
            if (rms)
            {
                Statement opCall;
                opCall.kind = OPERATOR_CALL;
                opCall.name = op->text;
                opCall.params.push_back(rms->params[1]);
                opCall.params.push_back(*rhv);
                rms->params[1] = opCall;
            }
            else
            {
                Statement opCall;
                opCall.kind = OPERATOR_CALL;
                opCall.name = op->text;
                opCall.params.push_back(*lhv);
                opCall.params.push_back(*rhv);
                lhv = opCall;
            }

        }

        return lhv;
    }

    size_t Analyzer::GetOpPriority(const string& name)
    {
        auto op = opEntries.find(name);
        if (op == opEntries.end()) return 0;

        return op->second.priority;
    }

    Statement* Analyzer::GetRightMostStatement(Statement* lhv, size_t rhvPriority)
    {
        if (lhv->kind != OPERATOR_CALL) return nullptr;
        if (GetOpPriority(lhv->name) >= rhvPriority) return nullptr;
        
        Statement* rhv = &lhv->params[1];
        rhv = GetRightMostStatement(rhv, rhvPriority);
        if (!rhv) return lhv;

        else return rhv;
    }

    optional<Statement> Analyzer::ExpectKeyword() // TO BE IMPLEMENTED...
    {
        auto startToken = currentToken;
        if (auto possibleKeyword = ExpectIdentifier())
        {
            Statement rs;
            if (possibleKeyword->text == "if")
            {
                currentToken--; 
                return nullopt;
            }
            else if (possibleKeyword->text == "while")
            { 
                currentToken--;
                return nullopt;
            }
            else if (possibleKeyword->text == "return")
            {
                rs.kind = RETURN_STATEMENT;
                rs.name = possibleKeyword->text;
                if (auto possibleStatement = ExpectStatement())
                {
                    rs.params.push_back(*possibleStatement);
                }
                //currentToken++;

                return rs;
            }
            else
            {
                currentToken = startToken;
                return nullopt;
            }
        }
        currentToken = startToken;
        return nullopt;
    }

    void Analyzer::DumpTruck() const
    {
        for (auto &[name, f] : funcs)
        {            
            f.FuncDumpTruck();
        }
    }
}