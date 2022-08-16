#pragma once

#include <iostream>

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
            Registers regs;

            Runtime(Instruction* pc) : pc(pc), bytecode(pc) {};
            static void Run(Instruction* bytecode);
    };

    // Instruction Handlers    
    void EndHandler(Runtime& r);
    void PushHandler(Runtime& r);
    void AddHandler(Runtime& r);
    void AddSHandler(Runtime& r);
    void AddRHandler(Runtime& r);
    void PrintIntHandler(Runtime& r);
    void PrintHandler(Runtime& r);
    void CompIntLTHandler(Runtime& r);
    void CompIntETHandler(Runtime& r);
    void SaveIntHandler(Runtime& r);
    void LoadIntHandler(Runtime& r);
    void MoveHandler(Runtime& r);
    void JumpLTHandler(Runtime& r);
    void JumpHandler(Runtime& r);
    void JumpETHandler(Runtime& r);
    void JumpRelativeHandler(Runtime& r);

    // Instruction Handlers Map
    typedef void(*InstHandler)(Runtime&);
    extern InstHandler instHandlers[];
}