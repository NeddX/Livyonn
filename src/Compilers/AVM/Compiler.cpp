#include "include/Compiler.h"

namespace amcc
{
    ByteCode Compiler::Compile(const TokenList& tokens)
    {
        Instruction* inst = new Instruction(); // Is this stupid? Dunno. Maybe? But it works right?  
        vector<Instruction> bytecode;
        map<string, size_t> labelAddresses;
        int i = 0;
        int cInstOperandCount = 0;
        bool ptrExp = false;
        int cExprCount = 0;
        for (auto& l : tokens)
        {
            if (l.type == INST) i++;
            else if (l.type == LABEL_DEFINITION) labelAddresses[l.text] = i;
        }
        for (auto& t : tokens)
        {
            switch (t.type)
            {
                case INST:
                {
                    if (inst->opcode == NOP)
                    {
                        inst->opcode = (OpCode)(t.data);
                        break;
                    }
                    cInstOperandCount = 0;
                    bytecode.push_back(*inst);
                    delete inst;
                    inst = new Instruction();   
                    inst->opcode = (OpCode) (t.data);
                    break;
                }
                case NUM:
                {
                    if (ptrExp)
                    {
                        if ()
                    }
                    inst->p3 = t.data;
                    if (!ptrExp) cInstOperandCount++;
                    break;
                }
                case REG:
                {
                    if (inst->reg1.reg == NUL) inst->reg1.reg = (Regs) (t.data);
                    else inst->reg2.reg = (Regs) (t.data);
                    if (!ptrExp) cInstOperandCount++;
                    break;
                }
                case BITSIZE_SPECIFIER:
                {
                    if (t.text == "BYTE") inst->pl = 8;
                    else if (t.text == "WORD") inst->pl = 16;
                    else if (t.text == "DWORD") inst->pl = 32;
                    else if (t.text == "QWORD") inst->pl = 64;
                    else throw runtime_error("Bad bit size specifier @ line (" + t.line + string(",") + to_string(t.cur) + string(")"));
                    break;
                }
                case OPERATOR:
                {
                    if (t.text == "[")
                    {
                        ptrExp = true;
                        if (cInstOperandCount == 0) inst->reg1.ptr = true;
                        else if (cInstOperandCount == 1) inst->reg2.ptr = true;
                        cExprCount++;
                    }
                    else if (t.text == "]")
                    {
                        cExprCount = 0;
                        ptrExp = false;
                        cInstOperandCount++;
                    }
                    else if (t.text == "-")
                    {
                        if (cInstOperandCount == 1) inst->reg1.offset = -1;
                        else if (cInstOperandCount == 2) inst->reg2.offset = -1;
                    }
                    else if (t.text == "+")
                    {
                        if (cInstOperandCount == 1) inst->reg1.offset = 1;
                        else if (cInstOperandCount == 2) inst->reg2.offset = 1;
                    }
                    break;
                }
                case LABEL_CALL:
                {
                    inst->p3 = labelAddresses[t.text];
                    cInstOperandCount++;
                    break;
                }
            }
        }
        bytecode.push_back(*inst); 

        // for (auto& e : bytecode)
        // {
            // cout << "INST: " << opcodeType[e.opcode] << " reg1.reg: " << regType[e.reg1.reg] << " reg2: " << regType[e.reg2] << " p3: " << e.p3 << NL;
        // }

        delete inst;
        return bytecode;
    }
}