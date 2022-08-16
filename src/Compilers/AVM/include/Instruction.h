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
        PUSH,
        ADD,
        ADDS,
        ADDR,
        PRINT_INT,
        PRINT,
        COMP_INT_LT,
        COMP_INT_ET,
        SAVE_INT,
        LOAD_INT,
        MOV,
        JMP, // Unconditional Jump
        JLT, // Jump if Less Than
        JET, // Jump if Equal To
        JRL, // Jump Relative

        END,
        NOP
    };

    struct Instruction
    {
        OpCode opcode = NOP;
        uint8_t pl = 0;
        int16_t p2 = 0;
        int64_t p3 = 0;
        Regs64 reg1 = NUL;
        Regs64 reg2 = NUL;
    };    
}