#include "include/Runtime.h"

namespace avm
{
    void (*handlers[])(Runtime&) =
    {
        EndHandler,
        PushHandler,
        PopHandler,
        AddHandler,
        SubHandler,
        MulHandler,
        DivHandler,
        PrintIntHandler,
        MoveHandler,
        JumpHandler,
        ConditionalJumpHandler,
        JumpNotEqualHandler,
        CallHandler,
        ReturnHandler,
        CompIntEqualToHandler,
        CompIntNotEqualHandler,
        ComptIntGreaterThanHandler,
        CompIntLessThanHandler,
        IncrementHandler,
        DecrementHandler,

        /*PrintStrHandler,
        PushIntBasepointerRelative,
        LoadIntBasepointerRelative,
        PushStrBasepointerRelative,
        LoadStrBasepointerRelative,
        PushByteBasepointerRelative,
        LoadByteBasepointerRelative,
        RelativeJumpHandler,
        ConditionalRelativeJumpHandler,
        RelativeJumpNotEqualHandler,
        JumpNotEqualHandler,
        CallHandler,
        ReturnHandler,
        DefineBytesHandler,
        CombineStringHandler,
        IncrementHandler,
        DecrementHandler,
        PopStrHandler,*/

        NopHandler
    };

    void Runtime::Run(Instruction* bytecode, int32_t& result, optional<ByteBuffer> args)
    {
        Runtime r(bytecode);

        r.stack.Reserve(1024);

        r.stack.Write32(0); // nullptr (return address)

		r.regs[EBP] = r.baseIndex; 
        r.stack.Resize(32);
        r.regs[ESP] = r.stack.Size();

        while (r.pc != nullptr) handlers[r.pc->opcode](r);
        result = 0;
    }

    void EndHandler(Runtime& r)
    {
        r.pc = nullptr;
    }

    void PushHandler(Runtime& r)
    {
        if (r.pc->reg1.reg == NUL) r.stack.Write32(r.pc->p3);
        else r.stack.Write32(r.regs[r.pc->reg1.reg]);
        r.regs[ESP] = r.stack.Size();
        UpdateAndProceed(r);
    }

    void PopHandler(Runtime& r)
    {
        r.stack.Pop32();
        r.regs[ESP] = r.stack.Size();
        UpdateAndProceed(r);
    }

    void AddHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg == NUL) ? r.pc->p3 : r.regs[r.pc->reg2.reg];
        r.regs[r.pc->reg1.reg] += rhv;
        UpdateAndProceed(r);
    }

    void SubHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg == NUL) ? r.pc->p3 : r.regs[r.pc->reg2.reg];
        r.regs[r.pc->reg1.reg] -= rhv;
        UpdateAndProceed(r);
    }

    void MulHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg == NUL) ? r.pc->p3 : r.regs[r.pc->reg2.reg];
        r.regs[r.pc->reg1.reg] *= rhv;
        UpdateAndProceed(r);
    }

    void DivHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg == NUL) ? r.pc->p3 : r.regs[r.pc->reg2.reg];
        r.regs[r.pc->reg1.reg] /= rhv;
        UpdateAndProceed(r);
    }

    void PrintIntHandler(Runtime& r)
    {
        printf("%ld\n", r.stack.Read32());
        UpdateAndProceed(r);
    }

    void MoveHandler(Runtime& r)
    {
        if (r.pc->reg1.ptr)
        {
            if (r.pc->reg2.reg != NUL) r.stack.WriteAt32(r.regs[r.pc->reg1.reg], r.regs[r.pc->reg2.reg]);
            else r.stack.WriteAt32(r.regs[r.pc->reg1.reg], r.pc->p3);
        }
        else
        {
            if (r.pc->reg2.ptr)
            {
                r.regs[r.pc->reg1.reg] = r.stack.ReadFrom32(r.regs[r.pc->reg2.reg]);
            }
            else
            {
                if (r.pc->reg2.reg != NUL) r.regs[r.pc->reg1.reg] = r.regs[r.pc->reg2.reg];
                else r.regs[r.pc->reg1.reg] = r.pc->p3;
            }
        }
        UpdateAndProceed(r);
    }

    void JumpHandler(Runtime& r)
    {
        if (r.pc->reg1.reg != NUL) r.pc = &r.bytecode[r.regs[r.pc->reg1.reg]];
        else r.pc = &r.bytecode[r.pc->p3];
    }

    void ConditionalJumpHandler(Runtime& r)
    {
        if (r.regs[CF] == 1) JumpHandler(r);
        else UpdateAndProceed(r);
    }

    void JumpNotEqualHandler(Runtime& r)
    {
        if (r.regs[CF] == 0) JumpHandler(r);
        else UpdateAndProceed(r);
    }

    void CompIntEqualToHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg != NUL) ? r.regs[r.pc->reg2.reg] : r.pc->p3;
        r.regs[CF] = r.regs[r.pc->reg1.reg] == rhv;
        UpdateAndProceed(r);
    }

    void CompIntNotEqualHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg != NUL) ? r.regs[r.pc->reg2.reg] : r.pc->p3;
        r.regs[CF] = r.regs[r.pc->reg1.reg] != rhv;
        UpdateAndProceed(r);
    }

    void ComptIntGreaterThanHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg != NUL) ? r.regs[r.pc->reg2.reg] : r.pc->p3;
        r.regs[CF] = r.regs[r.pc->reg1.reg] > rhv;
        UpdateAndProceed(r);
    }

    void CompIntLessThanHandler(Runtime& r)
    {
        int32_t rhv = (r.pc->reg2.reg != NUL) ? r.regs[r.pc->reg2.reg] : r.pc->p3;
        r.regs[CF] = r.regs[r.pc->reg1.reg] < rhv;
        UpdateAndProceed(r);
    }

    void CallHandler(Runtime& r)
    {
        r.baseIndex = r.regs[EBP];
        r.stack.WriteAt32(r.regs[EBP] + 4, r.baseIndex);
        r.stack.WriteAt32(r.regs[EBP] + 8, (uint64_t) (r.pc + 1));
        r.regs[EBP] += 16;
        JumpHandler(r);
    }

    void ReturnHandler(Runtime& r)
    {
        Instruction* addr = (Instruction*) (r.stack.ReadFrom32(r.regs[EBP]));
        r.baseIndex = r.stack.ReadFrom32(r.regs[EBP] + 4);
        r.pc = addr;
    }

    void IncrementHandler(Runtime& r)
    {
        if (r.pc->reg1.reg == NUL)
        {
            int64_t data = r.stack.Read32();
            r.stack.Write32(++data);
        }
        else r.regs[r.pc->reg1.reg] += 1;
        UpdateAndProceed(r);
    }

    void DecrementHandler(Runtime& r)
    {
        if (r.pc->reg1.reg == NUL)
        {
            int64_t data = r.stack.Read32();
            r.stack.Write32(--data);
        }
        else r.regs[r.pc->reg1.reg] -= 1;
        UpdateAndProceed(r);
    }

    /* NOT HERE! */

    void NopHandler(Runtime& r)
    {
        UpdateAndProceed(r);
    }

    inline void UpdateAndProceed(Runtime& r)
    {
        r.pc++;
    }
}
