#include "include/Runtime.h"

namespace avm
{
    void (*handlers[])(Runtime&) =
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
        PrintStrHandler,
        CompIntLessThanHandler,
        CompIntEqualToHandler,
        ComptIntGreaterThanHandler,
        CompIntNotEqualHandler,
        MoveHandler,
        MoveSHandler,
        PushIntBasepointerRelative,
        LoadIntBasepointerRelative,
        PushStrBasepointerRelative,
        LoadStrBasepointerRelative,
        PushByteBasepointerRelative,
        LoadByteBasepointerRelative,
        JumpHandler,
        ConditionalJumpHandler,
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
        PopStrHandler,

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
        //r.regs[RBP] = r.baseIndex;
		r.regs[RBP] = r.baseIndex; 

        if (args) r.stack.InsertBack(*args);
        r.stack.Resize(1024);
        r.regs[RSP] = r.stack.Size();

        while (r.pc != nullptr) handlers[r.pc->opcode](r);

        size_t argc = args->Size();
        r.stack.RemoveRange(0, argc);

        result = r.regs[FNR];
    }

    void NopHandler(Runtime& r)
    {
        UpdateAndProceed(r);
    }

    // End Instruction handler. Simply ends the runtime.
    void EndHandler(Runtime& r)
    {
        r.pc = nullptr;
    }

    // Push instruction handler. Pushes a value to the stack.
    void PushHandler(Runtime& r)
    {
        //if (r.pc->reg1.reg != NUL) r.stack.Write64(r.regs[r.pc->reg1.reg]);
        //else r.stack.Write64(r.pc->p3);
        //UpdateAndProceed(r);
        switch (r.pc->pl)
        {
            case 16:
            {
                if (r.pc->reg1.reg != NUL) r.stack.Write(r.regs[r.pc->reg1.reg]);
                else r.stack.Write(r.pc->p3);
                break;
            }
            case 32:
            {
                if (r.pc->reg1.reg != NUL) r.stack.Write32(r.regs[r.pc->reg1.reg]);
                else r.stack.Write32(r.pc->p3);
                break;
            }
            case 64:
            default:
            {
                if (r.pc->reg1.reg != NUL) r.stack.Write64(r.regs[r.pc->reg1.reg]);
                else r.stack.Write64(r.pc->p3);
                break;
            }
        }
        UpdateAndProceed(r);
    }

    void PopHandler(Runtime& r)
    {
        switch (r.pc->pl)
        {
            case 16:
            {
                if (r.pc->reg1.reg != NUL) r.regs[r.pc->reg1.reg] = r.stack.Read();
                else r.stack.Pop();
                break;
            }
            case 32:
            {
                if (r.pc->reg1.reg != NUL) r.regs[r.pc->reg1.reg] = r.stack.Read32();
                else r.stack.Pop32();
                break;
            }
            case 64:
            default:
            {
                if (r.pc->reg1.reg != NUL) r.regs[r.pc->reg1.reg] = r.stack.Read64();
                else r.stack.Pop64();
                break;
            }
        }
        UpdateAndProceed(r);
    }

    void PeekHandler(Runtime& r)
    {
        switch (r.pc->pl)
        {
            case 16:
            {
                r.regs[r.pc->reg1.reg] = r.stack.Peek();
                break;
            }
            case 32:
            {
                r.regs[r.pc->reg1.reg] = r.stack.Peek32();
                break;
            }
            case 64:
            default:
            {
                r.regs[r.pc->reg1.reg] = r.stack.Peek64();
                break;
            }
        }
        UpdateAndProceed(r);
    }

    // Add instruction handler. Adds two numbers together.
    void AddHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1.reg] = rhv + lhv;
        UpdateAndProceed(r);
    }

    void SubHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1.reg] = rhv - lhv;
        UpdateAndProceed(r);
    }

    void MulHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1.reg] = rhv * lhv;
        UpdateAndProceed(r);
    }

    void DivHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[r.pc->reg1.reg] = rhv / lhv;
        UpdateAndProceed(r);
    }

    // Print Int handler. Prints the last integer in the stack.
    void PrintIntHandler(Runtime& r)
    {
        //cout << r.regs[r.pc->reg1.reg] << NL;
        switch (r.pc->pl)
        {
            case 16:
            {
                printf("%d", r.stack.Read());
                break;
            }
            case 32:
            {
                printf("%d", r.stack.Read32());
                break;
            }
            default:
            {
                printf("%lld", r.stack.Read64());
                break;
            }
        }
        printf("\n");
        UpdateAndProceed(r);
    }

    void PrintStrHandler(Runtime& r)
    {
        string str;
        uint16_t byte = r.stack.Read();
        size_t i = 0;
        while (byte != 0)
        {
            str += byte;
            byte = r.stack.Read();
        }
        r.stack.Pop64(); // Get rid of the size at the begining cause we dont really need it
        if (r.pc->pl == 0) printf("%s", str.c_str());
        else printf("%s\n", str.c_str());
        UpdateAndProceed(r);
    }

    // Compare Int Less Than instruction handler. Compares if int a is less than int b (a < b).
    void CompIntLessThanHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[CF] = rhv < lhv;
        UpdateAndProceed(r);
    }

    void CompIntEqualToHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[CF] = lhv == rhv;//r.stack.Write(lhv == rhv);
        UpdateAndProceed(r);
    }

    void CompIntNotEqualHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.stack.Read64();
        r.regs[CF] = lhv != rhv;//r.stack.Write(lhv == rhv);
        UpdateAndProceed(r);
    }

    void ComptIntGreaterThanHandler(Runtime& r)
    {
        int64_t rhv, lhv;
        rhv = r.regs[r.pc->reg1.reg];
        if (r.pc->reg2.reg != NUL) lhv = r.regs[r.pc->reg2.reg];
        else lhv = r.pc->p3;
        r.regs[CF] = rhv > lhv;
        UpdateAndProceed(r);
    }

    void MoveHandler(Runtime& r)
    {
        int64_t rhv;
		if (r.pc->reg1.ptr)
		{
			if (r.pc->reg2.reg != NUL) rhv = r.regs[r.pc->reg2.reg];
			else rhv = r.pc->p3;

			switch (r.pc->pl)
			{
                case 8:
                case 16:
                case 32:
                case 64:
				default:
				{
					r.stack.WriteAt64(r.regs[r.pc->reg1.reg], rhv);
                    break;
				}
			}
		}
		else
		{
            if (r.pc->reg2.reg != NUL) r.regs[r.pc->reg1.reg] = r.regs[r.pc->reg2.reg];
            else r.regs[r.pc->reg2.reg] = r.pc->p3;
		}
		/*
        if (r.pc->reg2.reg != NUL) 
		{
			if (r.pc
		}r.regs[r.pc->reg1.reg] = r.regs[r.pc->reg2.reg];
        else r.regs[r.pc->reg1.reg] = r.pc->p3;
        */
		UpdateAndProceed(r);
    }

    void MoveSHandler(Runtime& r)
    {
        r.regs[r.pc->reg1.reg] = r.stack.Read64();
        UpdateAndProceed(r);
    }

    void PushIntBasepointerRelative(Runtime& r)
    {
        r.stack.WriteAt64(r.stack.Read64(), (r.pc->reg1.reg != NUL) ? r.regs[r.pc->reg1.reg] + r.pc->p3 : r.pc->p3 + r.baseIndex);
        UpdateAndProceed(r);
    }

    void LoadIntBasepointerRelative(Runtime& r)
    {
        r.stack.PushFromAddress64((r.pc->reg1.reg != NUL) ? r.regs[r.pc->reg1.reg] + r.pc->p3 : r.pc->p3 + r.baseIndex);
        UpdateAndProceed(r);
    }

    void PushStrBasepointerRelative(Runtime& r)
    {
#define s_buffer r.stack.buffer // Makes things easier

        size_t oldIndex = (r.pc->reg1.reg != NUL) ? r.regs[r.pc->reg1.reg] + r.pc->p3 : r.pc->p3 + r.baseIndex;

        vector<uint16_t> buffer;
        uint16_t byte = r.stack.Read();
        while (true)
        {
            buffer.push_back(byte);
            byte = r.stack.Read();
            if (byte == 0)
            {
                buffer.push_back(0);
                break;
            }
        }
        size_t n_size = r.stack.Read64() + 4; // + 4 for the size holder itself

        r.stack.PushFromAddress64(oldIndex);
        size_t p_size = r.stack.Read64() + 4; // + 4 for the size holder itself

        //r.shift += n_size - p_size;

        // Insert the new size first ( - 4 because the size indicates the size of the string not the array)
        r.stack.WriteAt64(n_size - 4, oldIndex);

        // Erase the old string (begining + offset + size holder, begining + offset + previous size)
        s_buffer.erase(s_buffer.begin() + oldIndex + 4, s_buffer.begin() + oldIndex + p_size);

        // Then insert the string itself (begining of the stack + it's offset in the stack
        // + 4 for the size cause we already updated it)
        s_buffer.insert(r.stack.buffer.begin() + oldIndex + 4,
            buffer.rbegin(), buffer.rend());

        UpdateAndProceed(r);
    }

    void LoadStrBasepointerRelative(Runtime& r)
    {
        size_t index = (r.pc->reg1.reg != NUL) ? r.regs[r.pc->reg1.reg] + r.pc->p3 : r.pc->p3 + r.baseIndex;
        r.stack.PushFromAddress64(index); // Pushing the byte array size to the stack
        size_t size = r.stack.Read64() + 4; // Loading ^ (+ 4 for the size)
        for (int i = 0; i < size; i++)
        {
            r.stack.Write(r.stack[index + i]);
        }
        UpdateAndProceed(r);
    }

    void PushByteBasepointerRelative(Runtime& r)
    {
        // This is same as PushIntBasepointerRelative but for bytes.
        r.stack.WriteAt(r.stack.Read(), (r.pc->reg1.reg != NUL) ? r.regs[r.pc->reg1.reg] + r.pc->p3 : r.pc->p3 + r.baseIndex);
        UpdateAndProceed(r);
    }

    void LoadByteBasepointerRelative(Runtime& r)
    {
        // This is same as LoadIntBasepointerRelative but for bytes.
        r.stack.PushFromAddress((r.pc->reg1.reg != NUL) ? r.regs[r.pc->reg1.reg] + r.pc->p3 : r.pc->p3 + r.baseIndex);
        UpdateAndProceed(r);
    }


    void RelativeJumpHandler(Runtime& r)
    {
        if (r.pc->reg1.reg != NUL) r.pc += r.regs[r.pc->reg1.reg];
        else r.pc += r.pc->p3;
    }

    void ConditionalRelativeJumpHandler(Runtime& r)
    {
        if (r.regs[CF] == 1) RelativeJumpHandler(r);
        else UpdateAndProceed(r);
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

    void RelativeJumpNotEqualHandler(Runtime& r)
    {
        if (r.regs[CF] == 0) RelativeJumpHandler(r);
        else UpdateAndProceed(r);
    }

    void JumpNotEqualHandler(Runtime& r)
    {
        if (r.regs[CF] == 0) JumpHandler(r);
        else UpdateAndProceed(r);
    }

    void CallHandler(Runtime& r)
    {
        r.regs[RBP] = r.baseIndex;
        r.stack.Write64(r.baseIndex);
        r.stack.Write64(reinterpret_cast<int64_t>(r.pc + 1));
        r.baseIndex = r.stack.Size();
        JumpHandler(r); // Absolute jump, not relative.
    }

    void ReturnHandler(Runtime& r)
    {
        Instruction* addr = reinterpret_cast<Instruction*>(r.stack.Read64());
        r.baseIndex = r.stack.Read64();
        r.pc = addr;
    }

    void DefineBytesHandler(Runtime& r)
    {
        int64_t size = r.pc->bytes.size();
        r.stack.Write64(size);
        for (int64_t i = size - 1; i >= 0; i--)
        {
            r.stack.Write(r.pc->bytes[i]);
        }
        UpdateAndProceed(r);
    }

    void CombineStringHandler(Runtime& r)
    {
        // AUTISM
        ByteBuffer str;
        uint8_t nte = 0;
        while (true)
        {
            uint16_t byte = r.stack.Read();
            if (byte == 0)
            {   
                r.stack.Pop64();
                if (++nte > 1)
                {
                    str.buffer.insert(str.buffer.begin(), 0); // Insert the null terminator
                    break;
                }
            }
            else str.buffer.insert(str.buffer.begin(), byte);
        }

        r.stack.Write64(str.Size()); // Write the size at the begining

        for (auto& e : str.buffer) r.stack.Write(e); // Push it
        UpdateAndProceed(r);
    }

    void IncrementHandler(Runtime& r)
    {
        if (r.pc->reg1.reg == NUL)
        {
            int64_t data = r.stack.Read64();
            r.stack.Write64(++data);
        }
        else r.regs[r.pc->reg1.reg] += 1;
        UpdateAndProceed(r);
    }

    void DecrementHandler(Runtime& r)
    {
        if (r.pc->reg1.reg == NUL)
        {
            int64_t data = r.stack.Read64();
            r.stack.Write64(--data);
        }
        else r.regs[r.pc->reg1.reg] -= 1;
        UpdateAndProceed(r);
    }

    void PopStrHandler(Runtime& r)
    {
        uint16_t byte = r.stack.Read();
        while (byte != 0) byte = r.stack.Read();
        r.stack.Pop64();
        UpdateAndProceed(r);
    }

    void UpdateAndProceed(Runtime& r)
    {
        r.regs[RSP] = r.stack.Size() - 1;
        r.pc++;
    }
}
