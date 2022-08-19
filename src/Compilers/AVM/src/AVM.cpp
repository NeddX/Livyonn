#include "../include/AVM.h"

namespace avm
{
    void AVM::Start()
    {
        using Inst = Instruction;
        Instruction bytecode[]
        {
            Inst { .opcode = MOV, .reg1 = RAX, .p3 = 0 },            
            Inst { .opcode = MOV, .reg1 = RBX, .p3 = 3 },
            Inst { .opcode = PUSH, .reg1 = RAX },
            Inst { .opcode = PRINT_INT }, 
            Inst { .opcode = ADD, .reg1 = RAX, .p3 = 1},
            Inst { .opcode = COMP_INT_LT, .reg1 = RAX, .reg2 = RBX },
            Inst { .opcode = CJMP, .p3 = 2 },
            Inst { .opcode = MOV, .reg1 = FNR, .reg2 = RAX },
            Inst { .opcode = RETURN },

            Inst { .opcode = END }
        };

        int64_t result = 0;
        optional<ByteBuffer> args;
        args->buffer = { 10 };
        Runtime::Run(bytecode, result, args);

        cout << "Return result: " << result << endl;
    }    
}