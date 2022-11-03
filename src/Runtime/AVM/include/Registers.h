#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "ByteBuffer.h"

namespace avm
{
    using namespace std;

    enum Regs
    {
        // Generic x86_64 registers
        RAX,
        RBX,
        RCX,
        RDX,
        RBP,
        RSP,
        RSI,
        RDI,

        EAX,
        EBX,
        ECX,
        EDX,
        EBP,
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

        // Special Registers for this specific virutal machine
        CF, // Conditional Flag, either 0 (false) or 1 (true)
        FNR, // Function Return Register (returns the pointer pointing to the function return value)
        RFR, // 64bit something?
        EFR, // 32bit something?

        NUL
    };

    static const char* regType[] =
    {
        "RAX",
        "RBX",
        "RCX",
        "RDX",
        "RBP",
        "RSP",
        "RSI",
        "RDL",
        "EAX",
        "EBX",
        "ECX",
        "EDX",
        "EBP",
        "ESP",
        "ESI",
        "EDI",
        "R8",
        "R9",
        "R10",
        "R11",
        "R12",
        "R13",
        "R14",
        "R15",
        "E8",
        "E9",
        "E10",
        "E11",
        "E12",
        "E13",
        "E14",
        "E15",
        "CF",
        "FNR",
        "RFR",
        "EFR",
        "NUL"
    };

    struct Registers
    {
        private:
            uint64_t buffer[NUL] = { 0 };

        public:
            constexpr uint64_t &operator[](size_t index)
            {
                return buffer[index];
            }
    };
}