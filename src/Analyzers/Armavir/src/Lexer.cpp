#include "../include/Lexer.h"

//\ |

namespace arma
{
    vector<Token> Lexer::Lex(const string& src)
    {
        vector<Token> tokens;
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
            else if (currentToken.type == POTENTIAL_COMMENT && c != '/')
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
                    if (currentToken.type == WHITESPACE) currentToken.type = INTEGER_LITERAL;
                    if (currentToken.type == POTENTIAL_DOUBLE) currentToken.type = DOUBLE_LITERAL;
                    currentToken.text.append(1, c);
                    break;
                }
                case '.':
                {
                    if (currentToken.type == WHITESPACE)
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
                    else if (currentToken.type == COMMENT) continue;
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
                case '=':
                case '-':
                case '+':
                case '*':
                {
                    if (currentToken.type != STRING_LITERAL && currentToken.type != COMMENT)
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = OPERATOR;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                        //EndToken(currentToken, tokens);
                    }
                    else currentToken.text.append(1, c);
                    break;
                }                    
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
                    if (currentToken.type == COMMENT) currentToken.type = WHITESPACE;
                    currentToken.line++;
                    cur = 0;
                    break;
                } 
                case '\r':
                {
                    if (currentToken.type == STRING_LITERAL) currentToken.text.append(1, '\r');
                    if (currentToken.type == COMMENT) currentToken.type = WHITESPACE;
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
                    if (currentToken.type == STRING_LITERAL) currentToken.text.append(1, c);
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
                        if (currentToken.type == WHITESPACE)
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
                default:
                {
                   if (currentToken.type == WHITESPACE  || 
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

    void EndToken(Token& t, vector<Token>& tokens)
    {
        if (t.type == COMMENT) cout << "ignoring nn: " << t.text << endl;
        if (t.type == IDENTIFIER && t.text == "true" || t.text == "false")
        {
            t.type = BOOLEAN_LITERAL;
            tokens.push_back(t);
        }
        else if (t.type != WHITESPACE) tokens.push_back(t);
        else if (t.type == POTENTIAL_DOUBLE) 
        {
            if (t.text == ".") t.type = OPERATOR; 
            else t.type = DOUBLE_LITERAL;
        }   
        t.type = WHITESPACE;
        t.text.erase();
    }

    void Token::TDumpTruck() const
    {
        cout << "token {\"" << text << "\" type: " << tokenTypeString[type] << " [" << line << "," << cur << "]" << "}" << NL;
    }
}