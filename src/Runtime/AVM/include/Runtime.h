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
        public:
            Instruction* bytecode;
            Instruction* pc;
            ByteBuffer stack;
            size_t baseIndex;
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
    void AddSHandler(Runtime& r);
    void AddRHandler(Runtime& r);
    void SubHandler(Runtime& r);
    void SubSHandler(Runtime& r);
    void SubRHandler(Runtime& r);
    void MulHandler(Runtime& r);
    void MulSHandler(Runtime& r);
    void MulRHandler(Runtime& r);
    void DivHandler(Runtime& r);
    void DivSHandler(Runtime& r);
    void DivRHandler(Runtime& r);
    void PrintIntHandler(Runtime& r);
    void PrintStrHandler(Runtime& r);
    void CompIntLTHandler(Runtime& r);
    void CompIntETHandler(Runtime& r);
    void CompIntNEHandler(Runtime& r);
    void ComptIntGTHandler(Runtime& r);
    void MoveHandler(Runtime& r);
    void MoveSHandler(Runtime& r);
    void PushIntBasepointerRelative(Runtime& r);
    void LoadIntBasepointerRelative(Runtime& r);
    void PushStrBasepointerRelative(Runtime& r);
    void LoadStrBasepointerRelative(Runtime& r);
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

    // Instruction Handlers Map
    typedef void(*InstHandler)(Runtime&);
    extern InstHandler instHandlers[];
}