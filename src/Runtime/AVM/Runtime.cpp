#include "include/Runtime.h"

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
        PrintStrHandler,
        CompIntLTHandler,
        CompIntETHandler,
        ComptIntGTHandler,
        CompIntNEHandler,
        MoveHandler,
        MoveSHandler,
        PushIntBasepointerRelative,
        LoadIntBasepointerRelative,
        PushStrBasepointerRelative,
        LoadStrBasepointerRelative,
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

        NopHandler
    };

    void Runtime::Run(Instruction* bytecode, int64_t& result, optional<ByteBuffer> args)
    {
        Runtime r(bytecode);

        r.stack.Reserve(1024);
        r.stack.Write64(r.baseIndex);
        if (result) r.stack.Write64(0);
        r.stack.Write64(NULL);
        r.baseIndex = r.stack.Size();

        if (args) r.stack.InsertBack(*args);

        while (r.pc != nullptr) handlers[r.pc->opcode](r);

        //size_t argc = args->Size();
        //r.stack.RemoveRange(0, argc);

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
        switch (r.pc->pl)
        {
        case 16:
        {
            cout << r.stack.Read();
            break;
        }
        case 32:
        {
            cout << r.stack.Read32();
            break;
        }
        default:
        {
            cout << r.stack.Read64();
            break;
        }
        }
        //cout << "\n";
        r.pc++;
    }

    void PrintStrHandler(Runtime& r)
    {
        string str;
        uint16_t byte = r.stack.Read();
        while (byte != 0)
        {
            str += byte;
            byte = r.stack.Read();
        }
        r.stack.Pop64(); // Get rid of the size at the beggining cause we dont really need it
        if (r.pc->pl == 0) cout << str;
        else cout << str << endl;
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
        r.stack.WriteToAddress64(r.stack.Read64(), (r.pc->p3 == 0) ? r.baseIndex : r.pc->p3 + r.baseIndex);
        r.pc++;
    }

    void LoadIntBasepointerRelative(Runtime& r)
    {
        r.stack.PushFromAddress64((r.pc->p3 == 0) ? r.baseIndex : r.pc->p3 + r.baseIndex);
        r.pc++;
    }

    void PushStrBasepointerRelative(Runtime& r)
    {
#define s_buffer r.stack.buffer // Makes things easier

        size_t oldIndex = (r.pc->p3 == 0) ? r.baseIndex : r.pc->p3 + r.baseIndex;

        vector<uint8_t> buffer;
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
        r.stack.WriteToAddress64(n_size - 4, oldIndex);

        // Erase the old string (beggining + offset + size holder, beggining + offset + previous size)
        s_buffer.erase(s_buffer.begin() + oldIndex + 4, s_buffer.begin() + oldIndex + p_size);

        // Then insert the string itself (beggining of the stack + it's offset in the stack
        // + 4 for the size cause we already updated it)
        s_buffer.insert(r.stack.buffer.begin() + oldIndex + 4,
            buffer.rbegin(), buffer.rend());

        r.pc++;
    }

    void LoadStrBasepointerRelative(Runtime& r)
    {
        size_t index = (r.pc->p3 == 0) ? r.baseIndex : r.pc->p3 + r.baseIndex;
        r.stack.PushFromAddress64(index); // Pushing the byte array size to the stack
        size_t size = r.stack.Read64() + 4; // Loading ^ (+ 4 for the size)
        for (int i = 0; i < size; i++)
        {
            r.stack.Write(r.stack[index + i]);
        }
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

    void DefineBytesHandler(Runtime& r)
    {
        size_t size = r.pc->bytes.size();
        r.stack.Write64(size);
        for (int i = size - 1; i >= 0; i--)
        {
            r.stack.Write(r.pc->bytes[i]);
        }
        r.pc++;
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

        r.stack.Write64(str.Size()); // Write the size

        for (auto& e : str.buffer) r.stack.Write(e); // Push it
        r.pc++;
    }
}
