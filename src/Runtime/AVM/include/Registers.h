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

        ZF,
        FNR,
        RFR,
        EFR,

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
        "ZF",
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