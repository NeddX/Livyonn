#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "ByteBuffer.h"

namespace avm
{
    using namespace std;

    enum Regs64
    {
        RAX,
        RBX,
        RCX,
        RDX,
        RBP,
        RSP,
        RSI,
        RDL,

        EAX,
        EBX,
        ECX,
        EDX,
        EBp,
        ESP,
        ESI,
        EDI,

        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,

        E8,
        E9,
        E10,
        E11,
        E12,
        E13,
        E14,
        E15,

        NUL
    };

    struct Registers
    {

        private:
            uint64_t buffer[32];

        public:
            uint64_t &operator[](size_t index);
    };
}