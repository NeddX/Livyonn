#include "include/Lexer.h"

//\ |

namespace arma
{
    TokenList Lexer::Lex(const string& src)
    {
        TokenList tokens;
        Token currentToken;

        currentToken.line = 1;

        int cur = 0;
        int i = 0;
        for (char c : src)
        {
            if (currentToken.type == STRING_ESC)
            {
                switch (c)
                {
                    case 'n':
                    {
                        currentToken.text.append(1, '\n');
                        break;
                    }
                    case 't':
                    {
                        currentToken.text.append(1, '\t');
                        break;
                    }
                    case 'r':
                    {
                        currentToken.text.append(1, '\r');
                        break;
                    }
                    case '\\':
                    {
                        currentToken.text.append(1, '\\');
                        break;
                    }
                    default:
                    {
                        throw runtime_error(string("Unknown escape sequence: \\") + string(1, c)
                         + " in string on line" + to_string(currentToken.line) + ".");
                        break;
                    }
                }

                currentToken.type = STRING_LITERAL;
                continue;
            }
            else if (currentToken.type == POTENTIAL_COMMENT && c != '/' && c != '*')
            {
                currentToken.type = OPERATOR;
                currentToken.cur = cur;
                EndToken(currentToken, tokens);
                continue;
            }

            switch (c)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    if (currentToken.type == NONE) currentToken.type = INTEGER_LITERAL;
                    if (currentToken.type == POTENTIAL_DOUBLE) currentToken.type = DOUBLE_LITERAL;
                    currentToken.text.append(1, c);
                    break;
                }
                case '.':
                {
                    if (currentToken.type == NONE)
                    {
                        currentToken.type = POTENTIAL_DOUBLE;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c); 
                    }
                    else if (currentToken.type == INTEGER_LITERAL)
                    {
                        currentToken.type = DOUBLE_LITERAL;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                    }
                    else if (currentToken.type == STRING_LITERAL) currentToken.text.append(1, c);
                    else if (currentToken.type == COMMENT) currentToken.text.append(1, c);
                    else
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    break;
                }
                case '$':
                case '{':
                case '}':
                case '(':
                case ')':
                case ',':   
                {
                    if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT)
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    else currentToken.text.append(1, c);
                    break;
                } 
                case '*':
                {
                    if (currentToken.type == POTENTIAL_COMMENT)
                    {
                        currentToken.type = COMMENT;
                        currentToken.subType = RANGE_COMMENT;
                        currentToken.cur = cur;
                    }
                    else if (currentToken.subType == RANGE_COMMENT)
                    {
                        currentToken.type = POTENTIAL_RANGE_END;
                        currentToken.subType = NONE;
                    }
                }
                case '!':
                {
                    if (currentToken.subType == POTENTIAL_DOUBLE_OPERATOR && currentToken.text == "=")
                    {
                        currentToken.type = OPERATOR;
                        currentToken.subType = DOUBLE_OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    else if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT && currentToken.subType != POTENTIAL_DOUBLE_OPERATOR)
                    {
                        currentToken.subType = NONE;
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.subType = POTENTIAL_DOUBLE_OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        //EndToken(currentToken, tokens);
                    }
                    else currentToken.text.append(1, c);
                    break;
                }
                case '=':
                {
                    if (currentToken.subType == POTENTIAL_DOUBLE_OPERATOR)
                    {
                        currentToken.type = OPERATOR;
                        currentToken.subType = DOUBLE_OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    else if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT && currentToken.subType != POTENTIAL_DOUBLE_OPERATOR)
                    {
                        currentToken.subType = NONE;
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.subType = POTENTIAL_DOUBLE_OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        //EndToken(currentToken, tokens);
                    }
                    else currentToken.text.append(1, c);
                    break;
                }
                case '-':
                case '+':
                case '<':
                case '>':
                {
                    if (currentToken.subType == POTENTIAL_DOUBLE_OPERATOR && currentToken.text[0] == c)
                    {
                        currentToken.type = OPERATOR;
                        currentToken.subType = DOUBLE_OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    else if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT && currentToken.subType != POTENTIAL_DOUBLE_OPERATOR)
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.subType = POTENTIAL_DOUBLE_OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        //EndToken(currentToken, tokens);
                    }
                    else currentToken.text.append(1, c);
                    break;
                }
                //case '<':
                //case '>':
                //{
                //    if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT)
                //    {
                //        EndToken(currentToken, tokens);
                //        currentToken.type = OPERATOR;
                //        currentToken.cur = cur;
                //        currentToken.text.append(1, c);
                //        //EndToken(currentToken, tokens);
                //    }
                //    else currentToken.text.append(1, c);
                //    break;
                //}                    
                case ' ':
                case '\t':
                {
                    if (currentToken.type == STRING_LITERAL || currentToken.type == COMMENT) currentToken.text.append(1, c);
                    else
                    { 
                        EndToken(currentToken, tokens);
                    }    
                    break;
                }
                case '\n':
                {
                    if (currentToken.type == STRING_LITERAL) currentToken.text.append(1, '\n');
                    if (currentToken.type == COMMENT && currentToken.subType != RANGE_COMMENT) currentToken.type = NONE;
                    currentToken.line++;
                    cur = 0;
                    break;
                } 
                case '\r':
                {
                    if (currentToken.type == STRING_LITERAL) currentToken.text.append(1, '\r');
                    if (currentToken.type == COMMENT && currentToken.subType != RANGE_COMMENT) currentToken.type = NONE;
                    currentToken.line++;
                    cur = 0;
                    break;
                }
                case '"':
                {
                    if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT)
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = STRING_LITERAL;
                        currentToken.cur = cur;
                    }
                    else if (currentToken.type == STRING_LITERAL) EndToken(currentToken, tokens);
                    else currentToken.text.append(1, c);
                    break;
                }
                case '\\':
                {
                    if (currentToken.type == STRING_LITERAL) currentToken.type = STRING_ESC;
                    else if (currentToken.type == COMMENT) currentToken.text.append(1, c);
                    else
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    break;
                }
                case '/':
                {
                    if (currentToken.type == STRING_LITERAL || currentToken.type == COMMENT)
                        currentToken.text.append(1, c);
                    else if (currentToken.type == POTENTIAL_RANGE_END)
                    {
                        EndToken(currentToken, tokens);
                    }
                    else if (currentToken.type == POTENTIAL_COMMENT)
                    { 
                        currentToken.type = COMMENT;
                        currentToken.cur = cur;
                        currentToken.text.erase();
                    }
                    else
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = POTENTIAL_COMMENT;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                    }
                    break;
                }
                case ';':
                {
                    if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT)
                    { 
                        EndToken(currentToken, tokens);
                        if (currentToken.type == NONE)
                        {
                            currentToken.type = SEMICOLON;
                            currentToken.cur = cur;
                        }
                        currentToken.text.append(1, c);
                        EndToken(currentToken, tokens);
                    }
                    else currentToken.text.append(1, c);
                    break;
                }
                case '\0': continue;
                default:
                {
                   if (currentToken.type == NONE  || 
                    currentToken.type == INTEGER_LITERAL || 
                    currentToken.type == DOUBLE_LITERAL)
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = IDENTIFIER;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                    }
                    else
                    {
                        currentToken.text.append(1, c);
                    }
                    break;
                }
            }
            cur++;
            i++;
        }

        EndToken(currentToken, tokens);
        return tokens;
    }

    void EndToken(Token& t, TokenList& tokens)
    {
        if (t.type == COMMENT || t.type == RANGE_COMMENT) return;
        else if (t.type == IDENTIFIER && t.text == "true" || t.text == "false")
        {
            t.type = BOOLEAN_LITERAL;
            tokens.push_back(t);
        }
        else if (t.type != NONE)
        {
            if (t.type == IDENTIFIER)
            {
                if (t.text == "include") t.type = INCLUDE_DIRECTIVE;
            }
            tokens.push_back(t);
        }
        else if (t.type == POTENTIAL_DOUBLE) 
        {
            if (t.text == ".") t.type = OPERATOR; 
            else t.type = DOUBLE_LITERAL;
        }
        t.type = NONE;
        t.subType = NONE;
        t.text.erase();
    }

    void Token::TDumpTruck() const
    {
        cout << "token {\"" << text << "\" type: " << tokenTypeString[type] << " [" << line << "," << cur << "]" << "}" << NL;
    }
}