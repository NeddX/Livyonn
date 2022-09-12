#include "include/Lexer.h"

namespace amca
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
            switch (c)
            {
                case '#': // Number
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    EndToken(currentToken, tokens);
                    currentToken.type = NUM;
                    // We dont append the hashtag
                    break;
                }
                case ',':
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    EndToken(currentToken, tokens);
                    break;
                }
                case '%': // Register
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    EndToken(currentToken, tokens);
                    currentToken.type = REG;
                    break;
                }
                case '@': // Label
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    EndToken(currentToken, tokens);
                    currentToken.type = LABEL_CALL;
                    break;
                }
                case ':':
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    if (currentToken.type == LABEL_CALL)
                    {
                        currentToken.type = LABEL_DEFINITION;
                        EndToken(currentToken, tokens);
                    }
                    break;
                }
                case '\n':
                case '\r':
                {
                    EndToken(currentToken, tokens);
                    cur = 0;
                    currentToken.line++;
                    currentToken.type = WHITESPACE;
                    break;
                }
                case ' ':
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    EndToken(currentToken, tokens);
                    break;
                }
                case ';':
                {
                    if (currentToken.type == COMMENT)
                    {
                        currentToken.text.append(1, c);
                        break;
                    }
                    EndToken(currentToken, tokens);
                    currentToken.type = COMMENT;
                    break;
                }
                default:
                {
                    if (currentToken.type == WHITESPACE)
                    {
                        EndToken(currentToken, tokens);
                        currentToken.type = INST;
                        currentToken.cur = cur;
                        currentToken.text.append(1, c);
                    }
                    else
                    {
                        currentToken.text.append(1, c);
                        currentToken.cur = cur + 1;
                    }
                }
                cur++;
                i++;
            }
        }

        EndToken(currentToken, tokens);
        return tokens;
    }

    void EndToken(Token& t, TokenList& tokens)
    {
        if (t.type != WHITESPACE)
        {
            switch (t.type)
            {
                case INST:
                {
                    t.data = GetInst(t.text);
                    break;
                }     
                case REG:
                {
                    t.data = GetReg(t.text);
                    break;
                }   
                case NUM:
                {
                    t.data = stoi(t.text);
                    break;
                }
            }
            tokens.push_back(t);
        }
        t.type = WHITESPACE;
        t.text.erase();
    }

    OpCode GetInst(string& inst)
    {
        transform(inst.begin(), inst.end(), inst.begin(), ::toupper);
        if (inst == "END") return END;
        if (inst == "PUSH") return PUSH;
        if (inst == "POP") return POP;
        if (inst == "ADD") return ADD;
        if (inst == "SUB") return SUB;
        if (inst == "MUL") return MUL;
        if (inst == "DIV") return DIV;
        if (inst == "PINT") return PRINT_INT;
        if (inst == "PRINT_STR") return PRINT_STR;
        if (inst == "CILT" ) return COMP_INT_LT;
        if (inst == "CIET") return COMP_INT_ET;
        if (inst == "CIGT" ) return COMP_INT_GT;
        if (inst == "CIET") return COMP_INT_NE;
        if (inst == "MOV") return MOV;
        if (inst == "MOVS") return MOVS;
        if (inst == "JMP") return JMP;
        if (inst == "CJMP") return CJMP;
        if (inst == "JRP") return JRP;
        if (inst == "CJRP") return CJRP;
        if (inst == "JRN") return JRN;
        if (inst == "JNE") return JNE;
        if (inst == "CALL") return CALL;
        if (inst == "RETURN") return RETURN;
        return NOP;
    }   

    Regs GetReg(string& reg)
    {
        transform(reg.begin(), reg.end(), reg.begin(), ::toupper);
        if (reg == "RAX") return RAX;
        if (reg == "RBX") return RBX;
        if (reg == "RCX") return RCX;
        if (reg == "RDX") return RDX;
        if (reg == "RBP") return RBP;
        if (reg == "RSP") return RSP;
        if (reg == "RSI") return RSI;
        if (reg == "RDI") return RDI;
        if (reg == "EAX") return EAX;
        if (reg == "EBX") return EBX;
        if (reg == "ECX") return ECX;
        if (reg == "EDX") return EDX;
        if (reg == "EBP") return EBP;
        if (reg == "ESP") return ESP;
        if (reg == "ESI") return ESI;
        if (reg == "EDI") return EDI;
        if (reg == "R8") return R8;
        if (reg == "R9") return R9;
        if (reg == "R10") return R10;
        if (reg == "R11") return R11;
        if (reg == "R12") return R12;
        if (reg == "R13") return R13;
        if (reg == "R14") return R14;
        if (reg == "R15") return R15;
        if (reg == "E8") return E8;
        if (reg == "E9") return E9;
        if (reg == "E10") return E10;
        if (reg == "E11") return E11;
        if (reg == "E12") return E12;
        if (reg == "E13") return E13;
        if (reg == "E14") return E14;
        if (reg == "E15") return E15;
        if (reg == "ZF") return ZF;
        if (reg == "FNR") return FNR;
        if (reg == "RFR") return RFR;
        if (reg == "EFR") return EFR;
        if (reg == "NUL") return NUL;
        return NUL;
    } 

    void Token::TDumpTruck() const
    {
        cout << "token {\"" << text << "\" type: " << tokenTypeString[type] << " data: " << data << " [" << line << "," << cur << "]" << "}" << NL;
    }
}