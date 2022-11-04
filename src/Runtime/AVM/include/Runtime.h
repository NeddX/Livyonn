#pragma once

#include <iostream>
#include <optional>

#include "Instruction.h"
#include "ByteBuffer.h"
#include "Registers.h"

namespace avm
{
    using namespace std;

    struct Runtime
    {
        // In my VM, bytes are stored as shorts (uint16),
        // so cope with the fact that im gonan refeer uint16s as bytes.
        public:
            Instruction* bytecode;
            Instruction* pc;
            ByteBuffer stack;
            size_t baseIndex = 0;
            Registers regs;

            Runtime(Instruction* pc) : pc(pc), bytecode(pc) {};
            static void Run(Instruction* bytecode, int64_t& result, optional<ByteBuffer> args = nullopt);
    };

    // Instruction Handlers    
    void NopHandler(Runtime& r);
    void EndHandler(Runtime& r);
    void PushHandler(Runtime& r);
    void PopHandler(Runtime& r);
    void PeekHandler(Runtime& r);
    void AddHandler(Runtime& r);
    void SubHandler(Runtime& r);
    void MulHandler(Runtime& r);
    void DivHandler(Runtime& r);
    void PrintIntHandler(Runtime& r);
    void PrintStrHandler(Runtime& r);
    void CompIntLessThanHandler(Runtime& r);
    void CompIntEqualToHandler(Runtime& r);
    void CompIntNotEqualHandler(Runtime& r);
    void ComptIntGreaterThanHandler(Runtime& r);
    void MoveHandler(Runtime& r);
    void MoveSHandler(Runtime& r);
    void PushIntBasepointerRelative(Runtime& r);
    void LoadIntBasepointerRelative(Runtime& r);
    void PushStrBasepointerRelative(Runtime& r);
    void LoadStrBasepointerRelative(Runtime& r);
    void PushByteBasepointerRelative(Runtime& r);
    void LoadByteBasepointerRelative(Runtime& r);
    void JumpHandler(Runtime& r);
    void ConditionalJumpHandler(Runtime& r);
    void RelativeJumpHandler(Runtime& r);
    void ConditionalRelativeJumpHandler(Runtime& r);
    void RelativeJumpNotEqualHandler(Runtime& r);
    void JumpNotEqualHandler(Runtime& r);
    void CallHandler(Runtime& r);
    void ReturnHandler(Runtime& r);
    void DefineBytesHandler(Runtime& r);
    void CombineStringHandler(Runtime& r);
    void IncrementHandler(Runtime& r);
    void DecrementHandler(Runtime& r);
    void PopStrHandler(Runtime& r);

    // Handler array
    extern void (*handlers[])(Runtime&);

    inline void UpdateAndProceed(Runtime& r);
}
