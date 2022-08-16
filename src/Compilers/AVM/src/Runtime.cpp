#include "../include/Runtime.h"

namespace avm
{
    InstHandler handlers[] =
    {
        PushHandler,
        AddHandler,
        AddSHandler,
        AddRHandler,
        PrintIntHandler,
        PrintHandler,
        CompIntLTHandler,
        CompIntETHandler,
        SaveIntHandler,
        LoadIntHandler,
        MoveHandler,
        JumpHandler,
        JumpLTHandler,
        JumpETHandler,
        JumpRelativeHandler,
        
        EndHandler
    };

    void Runtime::Run(Instruction* bytecode)
    {
        Runtime r(bytecode);
        while (r.pc != nullptr) 
        {
            handlers[r.pc->opcode](r);
        }
    }

    // End Instruction handler. Simply ends the runtime.
    void EndHandler(Runtime& r)
    {
        r.pc = nullptr;
    }

    // Push instruction handler. Pushes a value to the stack.
    void PushHandler(Runtime& r)
    {
        if (r.pc->reg1 == NUL) r.stack.Write64(r.pc->p3);
        else r.stack.Write64(r.regs[r.pc->reg1]);
        r.pc++;
    }

    // Add instruction handler. Adds two numbers together.
    void AddHandler(Runtime& r)
    {
        uint64_t rhv, lhv;
        if (r.pc->reg1 == NUL && r.pc->reg2 == NUL) 
        {
            rhv = r.stack.Read64();
            lhv = r.stack.Read64();
            r.stack.Write64(lhv + rhv);
        }    
        else
        {
            if (r.pc->reg1 != NUL) rhv = r.regs[r.pc->reg1];
            else rhv = r.stack.Read64();
            if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
            else lhv = r.pc->p3;
            r.regs[r.pc->reg1] = rhv + lhv;
        }
        r.pc++;
    }

    void AddSHandler(Runtime& r)
    {
        uint64_t rhv, lhv;
        rhv = r.stack.Read64();
        lhv = r.stack.Read64();
        r.stack.Write64(lhv + rhv);
        r.pc++;
    }

    void AddRHandler(Runtime& r)
    {
        uint64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.pc->reg2;
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1] = rhv + lhv;
        r.pc++;
    }

    // Print Int handler. Prints the last integer in the stack.
    void PrintIntHandler(Runtime& r)
    {
        cout << r.stack.Read64() << endl;
        r.pc++;
    }

    void PrintHandler(Runtime& r)
    {
        if (r.pc->reg1 == NUL && r.pc->reg2 == NUL) 
        {
            cout << r.stack.Read64() << endl;
        }
        else
        {
            if (r.pc->reg1 != NUL) cout << r.regs[r.pc->reg1] << endl;
            else cout << r.stack.Read64() << endl;
        }
        r.pc++;
    }

    // Compare Int Less Than instruction handler. Compares if int a is less than int b (a < b).
    void CompIntLTHandler(Runtime& r)
    {
        uint64_t rhv, lhv;
        if (r.pc->reg1 == NUL && r.pc->reg2 == NUL)
        {
            rhv = r.stack.Read64();
            lhv = r.stack.Read64();
            r.stack.Write(lhv < rhv);
        }
        else
        {
            if (r.pc->reg1 != NUL) rhv = r.regs[r.pc->reg1];
            else rhv = r.stack.Read64();
            if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
            else lhv = r.stack.Read64();

            r.stack.Write(lhv < rhv);
        }
        r.pc++;
    }

    void CompIntETHandler(Runtime& r)
    {
        uint64_t rhv, lhv;
        if (r.pc->reg1 == NUL && r.pc->reg2 == NUL)
        {
            rhv = r.stack.Read64();
            lhv = r.stack.Read64();
        }
        else
        {
            if (r.pc->reg1 != NUL) rhv = r.regs[r.pc->reg1];
            else rhv = r.stack.Read64();
            if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
            else lhv = r.stack.Read64();

        }
        r.stack.Write(lhv == rhv);
        r.pc++;
    }

    void SaveIntHandler(Runtime& r)
    {
        r.stack[r.pc->p2] = r.stack.Last();
        r.stack.Pop();
        r.pc++;
    }

    void LoadIntHandler(Runtime& r)
    {
        r.stack.Push(r.stack[r.pc->p2]);
        r.pc++;
    }

    void MoveHandler(Runtime& r)
    {
        if (r.pc->reg2 != NUL) r.regs[r.pc->reg1] = r.regs[r.pc->reg2];
        else r.regs[r.pc->reg1] = r.pc->p3;
        r.pc++;
    }

    void JumpHandler(Runtime& r)
    {
        if (r.pc->reg1 != NUL) r.pc = &r.bytecode[r.pc->reg1];
        else r.pc = &r.bytecode[r.pc->p3];
    }

    void JumpLTHandler(Runtime& r)
    {
        if (r.stack.Read() == 1) JumpHandler(r);
        else r.pc++;
    }

    void JumpETHandler(Runtime& r)
    {
        if (r.stack.Read() == 1) JumpHandler(r);
        else r.pc++;    
    }

    void JumpRelativeHandler(Runtime& r)
    {
        r.pc += r.pc->p2;
    }
}