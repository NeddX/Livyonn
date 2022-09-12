#include "include/Compiler.h"

namespace amcc
{
    ByteCode Compiler::Compile(const TokenList& tokens)
    {
        Instruction* inst = new Instruction(); // Is this stupid? Dunno. Maybe? But it works right?  
        vector<Instruction> bytecode;
        map<string, size_t> labelAddresses;
        int i = 0;
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
                        inst->opcode = static_cast<OpCode>(t.data);
                        break;
                    }   
                    bytecode.push_back(*inst);    
                    inst = new Instruction();            
                    inst->opcode = static_cast<OpCode>(t.data);
                    break;
                }
                case NUM:
                {
                    inst->p3 = t.data;
                    break;
                }
                case REG:
                {
                    if (inst->reg1 == NUL) inst->reg1 = static_cast<Regs>(t.data);
                    else inst->reg2 = static_cast<Regs>(t.data);
                    break;
                }
                case LABEL_CALL:
                {
                    inst->p3 = labelAddresses[t.text];
                    break;
                }
            }
        }
        bytecode.push_back(*inst); 

        // for (auto& e : bytecode)
        // {
            // cout << "INST: " << opcodeType[e.opcode] << " reg1: " << regType[e.reg1] << " reg2: " << regType[e.reg2] << " p3: " << e.p3 << NL;
        // }

        delete inst;
        return bytecode;
    }
}