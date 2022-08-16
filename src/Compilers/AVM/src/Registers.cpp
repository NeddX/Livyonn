#include "../include/Registers.h"

namespace avm
{
    uint64_t& Registers::operator[](size_t index)
    {
        return buffer[index];
    }
}