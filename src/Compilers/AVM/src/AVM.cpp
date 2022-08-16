#include "../include/AVM.h"

namespace avm
{
    void AVM::Start()
    {
        Instruction bytecode[]
        {
            Instruction { .opcode = MOV, .p3 = 0, .reg1 = RAX  },
            Instruction { .opcode = MOV, .p3 = 99999, .reg1 = RBX },
            Instruction { .opcode = PRINT, .reg1 = RAX },
            Instruction { .opcode = ADDR, .p3 = 1, .reg1 = RAX },
            Instruction { .opcode = COMP_INT_LT, .reg1 = RBX, .reg2 = RAX },
            Instruction { .opcode = JET, .p3 = 2 },

            Instruction { .opcode = END }
        };

        Runtime::Run(bytecode);
    }    
}