#include "../include/AVM.h"

namespace avm
{
    void AVM::Start(vector<Instruction>& bytecode)
    {
        // using Inst = Instruction;
        // Instruction bytecode[]
        // {
        //     Inst { .opcode = POP, .reg1 = RAX },
        //     Inst { .opcode = POP, .reg1 = RDX },

        //     Inst { .opcode = COMP_INT_ET, .reg1 = RDX, .p3 = 0 },
        //     Inst { .opcode = PUSH, .reg1 = RAX },
        //     Inst { .opcode = PRINT_INT },
        //     Inst { .opcode = CJMP, .p3 = 8 },
        //     Inst { .opcode = CALL, .p3 = 9 },
        //     Inst { .opcode = ADD, .reg1 = RAX, .reg2 = FNR },
        //     Inst { .opcode = MOV, .reg1 = FNR, .p3 = -1 },
        //     Inst { .opcode = RETURN },


        //     Inst { .opcode = COMP_INT_ET, .reg1 = RAX, .p3 = 0 },
        //     Inst { .opcode = PUSH, .reg1 = RAX },
        //     Inst { .opcode = POP },
        //     Inst { .opcode = CJMP,  .p3 = 7 },
        //     Inst { .opcode = ADD, .reg1 = RAX, .p3 = -1 },
        //     Inst { .opcode = CALL, .p3 = 1 },
        //     Inst { .opcode = MOV, .reg1 = FNR, .p3 = -1 },
        //     Inst { .opcode = RETURN }
        // };

        int64_t result = 0;
        //ByteBuffer args;
        //args.Write64(30);
        //args.Write64(9999);
        Runtime::Run(bytecode.data(), result);

        cout << "Return result: " << result << endl;
    }    
}