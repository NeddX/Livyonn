#include "../include/Runtime.h"

namespace avm
{
    InstHandler handlers[] =
    {
        EndHandler,
        PushHandler,
        PopHandler,
        PeekHandler,
        AddHandler,
        SubHandler,
        MulHandler,
        DivHandler,
        PrintIntHandler,
        PrintHandler,
        CompIntLTHandler,
        CompIntETHandler,
        ComptIntGTHandler,
        CompIntNEHandler,
        MoveHandler,
        MoveSHandler,
        PushIntBasepointerRelative,
        LoadIntBasepointerRelative,
        JumpHandler,
        ConditionalJumpHandler,
        RelativeJumpHandler,
        ConditionalRelativeJumpHandler,
        RelativeJumpNotEqualHandler,
        JumpNotEqualHandler,
        CallHandler,
        ReturnHandler,
        
        NopHandler
    };

    void Runtime::Run(Instruction* bytecode, int64_t& result, optional<ByteBuffer> args)
    {
        Runtime r(bytecode);
        
        r.stack.Reserve(1024);
        r.stack.Write64(r.baseIndex);
        if (result) r.stack.Write64(0); 
        r.stack.Write64(0); // nullptr
        r.baseIndex = r.stack.Size();

        if (args) r.stack.InsertBack(*args);

        while (r.pc != nullptr) 
            handlers[r.pc->opcode](r);

        size_t argc = args->Size();
        r.stack.RemoveRange(0, argc);

        result = r.regs[FNR];
    }

    void NopHandler(Runtime& r)
    {
        r.pc++;
    }

    // End Instruction handler. Simply ends the runtime.
    void EndHandler(Runtime& r)
    {
        r.pc = nullptr;
    }

    // Push instruction handler. Pushes a value to the stack.
    void PushHandler(Runtime& r)
    {
        //if (r.pc->reg1 != NUL) r.stack.Write64(r.regs[r.pc->reg1]);
        //else r.stack.Write64(r.pc->p3);
        //r.pc++;
        switch (r.pc->pl)
        {
            case 16:
            {
                if (r.pc->reg1 != NUL) r.stack.Write(r.regs[r.pc->reg1]);
                else r.stack.Write(r.pc->p3);
                break;
            }
            case 32:
            {
                if (r.pc->reg1 != NUL) r.stack.Write32(r.regs[r.pc->reg1]);
                else r.stack.Write32(r.pc->p3);
                break;
            }
            case 64:
            default:
            {
                if (r.pc->reg1 != NUL) r.stack.Write64(r.regs[r.pc->reg1]);
                else r.stack.Write64(r.pc->p3);
                break;
            } 
        }
        r.pc++;
    }

    void PopHandler(Runtime& r)
    {
        switch (r.pc->pl)
        {
            case 16:
            {
                if (r.pc->reg1 != NUL) r.regs[r.pc->reg1] = r.stack.Read();
                else r.stack.Pop();
                break;
            }
            case 32:
            {
                if (r.pc->reg1 != NUL) r.regs[r.pc->reg1] = r.stack.Read32();
                else r.stack.Pop32();
                break;
            }
            case 64:
            default:
            {
                if (r.pc->reg1 != NUL) r.regs[r.pc->reg1] = r.stack.Read64();
                else r.stack.Pop64();
                break;
            } 
        }
        r.pc++;
    }

    void PeekHandler(Runtime& r)
    {
        switch (r.pc->pl)
        {
            case 16:
            {
                r.regs[r.pc->reg1] = r.stack.Peek();
                break;
            }
            case 32:
            {
                r.regs[r.pc->reg1] = r.stack.Peek32();
                break;
            }
            case 64:
            default:
            {
                r.regs[r.pc->reg1] = r.stack.Peek64();
                break;
            } 
        }
        r.pc++;
    }

    // Add instruction handler. Adds two numbers together.
    void AddHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1] = rhv + lhv;
        r.pc++;
    }

    void SubHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1] = rhv - lhv;
        r.pc++;
    }

    void MulHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1] = rhv * lhv;
        r.pc++;
    }

    void DivHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1] = rhv / lhv;
        r.pc++;
    }  

    // Print Int handler. Prints the last integer in the stack.
    void PrintIntHandler(Runtime& r)
    {
        //uint64_t ra = r.stack.Read64();
        cout << r.stack.Read64() << NL;
        r.pc++;
    }

    void PrintHandler(Runtime& r)
    {
        if (r.pc->reg1 != NUL) cout << r.regs[r.pc->reg1] << endl;
        else cout << r.stack.Read64() << endl;
        r.pc++;
    }

    // Compare Int Less Than instruction handler. Compares if int a is less than int b (a < b).
    void CompIntLTHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[ZF] = rhv < lhv;
        r.pc++;
    }

    void CompIntETHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[ZF] = lhv == rhv;//r.stack.Write(lhv == rhv);
        r.pc++;
    }

    void CompIntNEHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.stack.Read64();
        r.regs[ZF] = lhv != rhv;//r.stack.Write(lhv == rhv);
        r.pc++;
    }

    void ComptIntGTHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1];
        if (r.pc->reg2 != NUL) lhv = r.regs[r.pc->reg2];
        else lhv = r.pc->p3;
        r.regs[ZF] = rhv > lhv;
        r.pc++;
    }

    void MoveHandler(Runtime& r)
    {
        if (r.pc->reg2 != NUL) r.regs[r.pc->reg1] = r.regs[r.pc->reg2];
        else r.regs[r.pc->reg1] = r.pc->p3;
        r.pc++;
    }

    void MoveSHandler(Runtime& r)
    {
        r.regs[r.pc->reg1] = r.stack.Read64();
        r.pc++;
    }

    void PushIntBasepointerRelative(Runtime& r)
    {
        switch (r.pc->pl)
        {
            case 16:
            {
                r.stack.WriteToAddress(r.stack.Read(), r.pc->p3 + r.baseIndex);
                break;
            }
            case 32:
            {
                r.stack.WriteToAddress32(r.stack.Read32(), r.pc->p3 + r.baseIndex);
                break;
            }
            default:
            case 64:
            {
                r.stack.WriteToAddress64(r.stack.Read64(), r.pc->p3 + r.baseIndex);
                break;
            }
        }
        r.pc++;
    }

    void LoadIntBasepointerRelative(Runtime& r)
    {
        switch (r.pc->pl)
        {
            case 16:
            {
                r.stack.PushFromAddress(r.pc->p3 + r.baseIndex);
                break;
            }
            case 32:
            {
                r.stack.PushFromAddress32(r.pc->p3 + r.baseIndex);
                break;
            }
            default:
            case 64:
            {
                r.stack.PushFromAddress64(r.pc->p3 + r.baseIndex);
                break;
            }
        }
        //r.stack.PushFromAddress64(r.pc->p3 + r.baseIndex);
        r.pc++;
    }

    void RelativeJumpHandler(Runtime& r)
    {
        if (r.pc->reg1 != NUL) r.pc += r.regs[r.pc->reg1];
        else r.pc += r.pc->p3;
    }

    void ConditionalRelativeJumpHandler(Runtime& r)
    {
        if (r.regs[ZF] == 1) RelativeJumpHandler(r);
        else r.pc++;    
    }

    void JumpHandler(Runtime& r)
    {
        if (r.pc->reg1 != NUL) r.pc = &r.bytecode[r.regs[r.pc->reg1]];
        else r.pc = &r.bytecode[r.pc->p3];
    }

    void ConditionalJumpHandler(Runtime& r)
    {
        if (r.regs[ZF] == 1) JumpHandler(r);
        else r.pc++;    
    }

    void RelativeJumpNotEqualHandler(Runtime& r)
    {
        if (r.regs[ZF] == 0) RelativeJumpHandler(r);
        else r.pc++;
    }

    void JumpNotEqualHandler(Runtime& r)
    {
        if (r.regs[ZF] == 0) JumpHandler(r);
        else r.pc++;
    }

    void CallHandler(Runtime& r)
    {
        r.stack.Write(r.baseIndex);
        r.stack.Write64(reinterpret_cast<int64_t>(r.pc + 1));
        r.baseIndex = r.stack.Size();
        JumpHandler(r);
    }

    void ReturnHandler(Runtime& r)
    {
        Instruction* addr = reinterpret_cast<Instruction*>(r.stack.Read64());
        r.baseIndex = r.stack.Read();
        r.pc = addr;
    }    
}