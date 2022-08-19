#pragma once

#include <iostream>
#include <cstdint>
#include <map>

#include "../../../Lib/include/Define.h"
#include "Registers.h"

namespace avm
{
    using namespace std;

    enum OpCode : uint16_t
    {
        END,
        PUSH,
        POP,
        ADD,
        SUB,
        MUL,
        DIV,
        PRINT_INT,
        PRINT,
        COMP_INT_LT, // If Less Than
        COMP_INT_ET, // If Equal To
        COMP_INT_GT, // If Greater than
        MOV,
        MOVS,
        JMP, // Unconditional Jump
        CJMP, // Jump if Equal To
        CALL,
        RETURN,

        NOP
    };

    struct Instruction
    {
        OpCode opcode = NOP;
        Regs64 reg1 = NUL;
        Regs64 reg2 = NUL;
        uint8_t pl = 0;
        int16_t p2 = 0;
        int64_t p3 = 0;
    };    
}