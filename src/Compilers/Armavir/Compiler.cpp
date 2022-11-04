#include "include/Compiler.h"

namespace arc
{
    using namespace std;
    using namespace arma;

    ByteCode Compiler::Compile(Analyzer& syntaxTree, CompilerOptions* options)
    {
        map<string, FunctionDefinition> fds = syntaxTree.GetFunctions();

        // Get all the functions before processing them and store them in nice little map designed for
        // function calls
        for (auto& [name, fd] : fds)
        { 
            FuncInfo fn;
            fn.addr = 0;
            fn.fd = fd;
            fn.returnType = fd.type;
            size_t i = 0;
            for (auto& argDef : fd.args)
            {
                Argument arg { .name = argDef.name, .index = i++, .type = argDef.type };
                fn.args[argDef.name] = arg;
            }
            funcOffsets[name] = fn;
        }

        // Generate code for the Entry function first and then delete it from the map
        auto entryFunc = fds.find("Entry");
        if (entryFunc == fds.end()) throw runtime_error("Compiler Error: Entry function was not defined.");
        FunctionCodeGen(funcOffsets[entryFunc->first]);
        funcOffsets.erase("Entry");

        // Now generate code for the rest of the functions
        for (auto& [name, fn] : funcOffsets) FunctionCodeGen(fn);

        if (options)
        {
            if (options->outputExecutable) WriteToBinary(options);
        }

        return compiledCode;
    }

    void Compiler::FunctionCodeGen(FuncInfo& fn)
    {
        FunctionDefinition fd = fn.fd;

        //size_t pi = 0;
        //for (auto& arg : fd.params) fn.args[arg.name] = (Argument{arg.name, pi++, arg.type});

        // Set our offset incase someone calls us in the future
        auto& self = funcOffsets[fd.name];
        self.addr += compiledCode.size();

        // Generate code for those who called us before
        for (auto& c : self.calls) compiledCode[c] = Instruction { .opcode = CALL, .p3 = (int64_t) self.addr };
        
        // Look for variable declerations first
        //for (auto& cs : fd.statements)
        //{
        //    switch (cs.kind)
        //    {
        //        //case VARIABLE_DECLERATION:
        //        //{
        //        //    if (varOffsets.find(cs.name) != varOffsets.end())
        //        //    {
        //        //        throw runtime_error("Re-declaration of an already exiting variable in the same context.");
        //        //    }

        //        //    switch (cs.type.fType)
        //        //    {
        //        //        case INT8:
        //        //        {
        //        //            intVarCount++;
        //        //            break;
        //        //        }
        //        //        case UINT8:
        //        //        {
        //        //            intVarCount++;;
        //        //            break;
        //        //        }
        //        //        case INT32:
        //        //        {
        //        //            VarInfo vi;
        //        //            vi.type.fType = INT32;
        //        //            vi.source = intVarCount;
        //        //            int32_t data;
        //        //            if (!cs.params.empty())
        //        //            {
        //        //                for (auto& p : cs.params)
        //        //                {
        //        //                    StatementInfo lst(p, varOffsets, args);
        //        //                    if (p.kind == FUNCTION_CALL)
        //        //                    {
        //        //                        //compiledCode.push_back(Instruction { .opcode = NOP });
        //        //                        StatementCodeGen(lst);
        //        //                        compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
        //        //                    }
        //        //                    else StatementCodeGen(lst);
        //        //                }
        //        //            }
        //        //            varOffsets[cs.name] = vi;
        //        //            intVarCount += 4;
        //        //            break;
        //        //        }
        //        //        case UINT32:
        //        //        {
        //        //            intVarCount += 2;
        //        //            break;
        //        //        }
        //        //        case DOUBLE:
        //        //        {
        //        //            intVarCount += 4;
        //        //            break;
        //        //        }
        //        //        case STRING:
        //        //        {
        //        //            VarInfo vi;
        //        //            vi.type.fType = STRING;
        //        //            vi.source = intVarCount;
        //        //            int32_t data;
        //        //            // To get the size of the string (which can be dynamic) we will need to iterate through
        //        //            // the compiled code and get the sum of every DB (DefineByte) instruction's byte size
        //        //            // to make sure we don't accidentally sum together other DB instructions that are
        //        //            // not realated to the current variable decleration, we will have to get the compiledCode
        //        //            // size before actually generating code for variable's statements (value)
        //        //            size_t rsi = compiledCode.size();
        //        //            if (!cs.params.empty())
        //        //            {
        //        //                for (auto& p : cs.params)
        //        //                {
        //        //                    StatementInfo lst(p, varOffsets, args);
        //        //                    StatementCodeGen(lst);
        //        //                }
        //        //            }
        //        //            for (int i = rsi; i < compiledCode.size(); i++)
        //        //            {
        //        //                if (compiledCode[i].opcode == DB || compiledCode[i].opcode == LSBR)
        //        //                    vi.size += compiledCode[i].bytes.size() - 1;
        //        //            }
        //        //            vi.size += 1 + 4; // 1 for the null term and 4 for the size holder?
        //        //            varOffsets[cs.name] = vi;
        //        //            intVarCount += vi.size;
        //        //            break;
        //        //        }
        //        //        case STRING_INT:
        //        //        {
        //        //            break;
        //        //        }
        //        //        case STRUCT:
        //        //        {
        //        //            break;
        //        //        }
        //        //        case DYNAMIC:
        //        //        {
        //        //            break;
        //        //        }
        //        //        case VOID:
        //        //        {
        //        //            break;
        //        //        }
        //        //        case BOOLEAN:
        //        //        {
        //        //            intVarCount++;
        //        //            break;                            
        //        //        }
        //        //    }
        //        //    break;
        //        //}
        //    }            
        //}

        for (auto& cs : fd.body)
        {
            StatementInfo lst(cs, fn);
            bool a = StatementCodeGen(lst);
            if (a == false) break;
        }

        // Not needed for now?
        size_t cleanupOffset = compiledCode.size() * sizeof(Instruction);
        for (auto& r : fn.returnInstructionAddresses)
        {
            compiledCode[r].p3 = cleanupOffset;
        }

        // If it's the entry boii
        if (fd.name == "Entry")
        {
            compiledCode.push_back(Instruction { .opcode = MOV, .reg1 = FNR, .p3 = 0 });
        }
        FunctionEpilogue(fn);
    }

    // Clean after yourself
    void Compiler::FunctionEpilogue(const FuncInfo& fn)
    {
        // Clean the local variables
        for (const auto& [key, var] : fn.varOffsets)
        {
            switch (var.type.fType)
            {
                case STRING:
                {
                    compiledCode.push_back(Instruction { .opcode = POP_STR });
                    //for (int i = 0; i < var.size; i++) compiledCode.push_back(Instruction { .opcode = POP, .pl = 16 });
                    break;
                }
                case BOOLEAN:
                {
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = 16 });
                    break;
                }
                default:
                {
                    compiledCode.push_back(Instruction { .opcode = POP });
                    break;
                }
            }
        }
        compiledCode.push_back(Instruction { .opcode = RETURN });
    }

    bool Compiler::StatementCodeGen(StatementInfo& sti)
    {
        switch (sti.st.kind)
        {
            case RETURN_STATEMENT:
            {
                if (!sti.st.body.empty())
                {
                    for (auto& p : sti.st.body)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                        compiledCode.push_back(Instruction { .opcode = POP, .reg1 = FNR });
                    }
                }
                FunctionEpilogue(sti.fn);
                break;
            }
            case FUNCTION_CALL:
            {
                auto foundFunc = funcOffsets.find(sti.st.name);
                if (foundFunc != funcOffsets.end())
                {
                    size_t i = 0;
                    size_t addrCount = 0;

                    auto arit = funcOffsets[sti.st.name].args.begin();
                    for (auto arg = sti.st.args.begin(); arg != sti.st.args.end(); arg++)
                    {
                        size_t rbp = compiledCode.size();

                        // Generate code for the funciton argument
                        StatementInfo lst(*arg, sti.fn);
                        StatementCodeGen(lst);

                        // If the types don't match
                        if (arit->second.type.fType != arg->type.fType)
                            throw runtime_error("Compiler Error: Expected an argument type " + arit->second.type.name + " but was passed " + arg->type.name + ".");


                        // This should work
                        switch (arg->type.fType)
                        {
                            case INT32:
                            {
                                arit->second.index = addrCount;
                                addrCount += 4;
                                break;
                            }
                            case STRING:
                            {
                                arit->second.index = addrCount;
                                for (size_t i = rbp; i < compiledCode.size(); i++)
                                {
                                    if (compiledCode[i].opcode == DB || compiledCode[i].opcode == LSBR)
                                        arit->second.size += compiledCode[i].bytes.size() - 1;
                                }
                                arit->second.size += 1 + 4;
                                addrCount += arit->second.size;
                                break;
                            }
                        }
                        i++;
                        arit++;
                    }

                    if (i != foundFunc->second.args.size())
                        throw runtime_error("Compiler Error: Too few arguments were passed for function '" + foundFunc->first + "'.");

                    if (foundFunc->second.addr == 0)
                    {
                        foundFunc->second.calls.push_back(compiledCode.size());
                        compiledCode.push_back(Instruction{ .opcode = NOP, }); // Placeholder instruction
                    }
                    else compiledCode.push_back(Instruction{ .opcode = CALL, .p3 = (int64_t)foundFunc->second.addr });

                    // Let's clean after us
                    for (auto arg : funcOffsets[sti.st.name].args)
                    {
                        switch (arg.second.type.fType)
                        {
                            case STRING:
                            {
                                compiledCode.push_back(Instruction { .opcode = POP_STR });
                                //for (int i = 0; i < arg.second.size; i++) compiledCode.push_back(Instruction{ .opcode = POP, .pl = 16 });
                                break;
                            }
                            case BOOLEAN:
                            {
                                compiledCode.push_back(Instruction{ .opcode = POP, .pl = 16 });
                                break;
                            }
                            default:
                            {
                                compiledCode.push_back(Instruction{ .opcode = POP });
                                break;
                            }
                        }
                    }
                }
                else if (sti.st.name == "_PrintInt")
                {
                    for (auto& p : sti.st.args)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                    }
                    compiledCode.push_back(Instruction { .opcode = PRINT_INT });
                }
                else if (sti.st.name == "_Print")
                {
                    for (auto& p : sti.st.args)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                    }
                    compiledCode.push_back(Instruction{ .opcode = PRINT_STR });
                }
                else if (sti.st.name == "_Println")
                {
                    if (sti.st.args.empty())
                    {
                        compiledCode.push_back(Instruction{ .opcode = DB, .bytes = { 10, 0 } });
                        compiledCode.push_back(Instruction{ .opcode = PRINT_STR });
                        break;
                    }
                    for (auto& p : sti.st.args)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                    }
                    compiledCode.push_back(Instruction{ .opcode = PRINT_STR, .pl = 1 });
                }
                else if (sti.st.name == "_TypeCheck")
                {
                    
                }
                else if (sti.st.name == "Exit")
                {
                    compiledCode.push_back(Instruction{ .opcode = END });
                }
                else
                {
                    throw runtime_error(string("Function '" + sti.st.name + "' does not exist in the current context."));
                }
                break;
            }
            case VARIABLE_DECLERATION:
            {
                FuncInfo& fn = sti.fn;
                if (fn.varOffsets.find(sti.st.name) != fn.varOffsets.end())
                {
                    throw runtime_error("Re-declaration of an already exiting variable in the same context.");
                }

                switch (sti.st.type.fType)
                {
                    case INT8:
                    {
                        fn.varCount++;
                        break;
                    }
                    case UINT8:
                    {
                        fn.varCount++;;
                        break;
                    }
                    case INT32:
                    {
                        VarInfo vi;
                        vi.type.fType = INT32;
                        vi.index = fn.varCount;
                        if (!sti.st.body.empty())
                        {
                            for (auto& p : sti.st.body)
                            {
                                StatementInfo lst(p, sti.fn);
                                if (p.kind == FUNCTION_CALL)
                                {
                                    //compiledCode.push_back(Instruction { .opcode = NOP });
                                    StatementCodeGen(lst);
                                    compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = FNR });
                                }
                                else StatementCodeGen(lst);
                            }
                        }
                        fn.varOffsets[sti.st.name] = vi;
                        fn.varCount += 4;
                        break;
                    }
                    case UINT32:
                    {
                        fn.varCount += 2;
                        break;
                    }
                    case DOUBLE:
                    {
                        fn.varCount += 4;
                        break;
                    }
                    case STRING:
                    {
                        VarInfo vi;
                        vi.type.fType = STRING;
                        vi.index = fn.varCount;
                        // To get the size of the string we will need to iterate through
                        // the compiled code and get the sum of every DB (DefineByte) instruction's byte size
                        // to make sure we don't accidentally sum together other DB instructions that are
                        // not realated to the current variable decleration, we will have to get the compiledCode
                        // size before actually generating code for variable's statements.
                        size_t rbp = compiledCode.size();

                        compiledCode.push_back(Instruction { .opcode = MOV, .reg1 = RAX, .reg2 = RSP });

                        if (!sti.st.body.empty())
                        {
                            for (auto& p : sti.st.body)
                            {
                                StatementInfo lst(p, sti.fn);
                                StatementCodeGen(lst);
                            }
                        }
                        /*for (size_t i = rbp; i < compiledCode.size(); i++)
                        {
                            if (compiledCode[i].opcode == DB || compiledCode[i].opcode == LSBR)
                                vi.size += compiledCode[i].bytes.size() - 1;
                        }*/
                        compiledCode.push_back(Instruction{ .opcode = MOV, .reg1 = RBX, .reg2 = RSP });
                        compiledCode.push_back(Instruction{ .opcode = SUB, .reg1 = RBX, .reg2 = RAX });
                        
                        //vi.size += 1 + 4; // 1 for the null term and 4 for the size holder?
                        vi.size++;
                        fn.varOffsets[sti.st.name] = vi;
                        fn.varCount += vi.size;
                        break;
                    }
                    case STRING_INT:
                    {
                        break;
                    }
                    case STRUCT:
                    {
                        break;
                    }
                    case DYNAMIC:
                    {
                        break;
                    }
                    case VOID:
                    {
                        break;
                    }
                    case BOOLEAN:
                    {
                        VarInfo vi;
                        vi.type.fType = BOOLEAN;
                        vi.index = fn.varCount;
                        if (!sti.st.body.empty())
                        {
                            for (auto& p : sti.st.body)
                            {
                                StatementInfo lst(p, sti.fn);
                                if (p.kind == FUNCTION_CALL)
                                {
                                    //compiledCode.push_back(Instruction { .opcode = NOP });
                                    StatementCodeGen(lst);
                                    compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = FNR });
                                }
                                else StatementCodeGen(lst);
                            }
                        }
                        fn.varOffsets[sti.st.name] = vi;
                        fn.varCount++;
                        break;
                    }
                }
                break;
            }
            case ASSIGNMENT_EXPRESSION:
            {
                if (!sti.st.body.empty())
                {
                    // Generate assignment statement?
                    // int a = 10;
                    // a = 100;

                    auto foundVar = sti.fn.varOffsets.find(sti.st.name);
                    if (foundVar == sti.fn.varOffsets.end())
                        throw runtime_error("Compiler Error: The name '" + sti.st.name + "' does not exist in the current context.");

                    switch (foundVar->second.type.fType)
                    {
                        case INT32:
                        {
                            for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                            {
                                StatementInfo lst(*p, sti.fn);
                                StatementCodeGen(lst);
                            }

                            compiledCode.push_back(Instruction { .opcode = PIBR, .p3 = (int64_t) foundVar->second.index });
                            break;
                        }
                        case STRING:
                        {
                            // Set the new and updated size
                            size_t rsi = compiledCode.size();
                            size_t p_size = foundVar->second.size;


                            for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                            {
                                StatementInfo lst(*p, sti.fn);
                                StatementCodeGen(lst);
                            }
                            foundVar->second.size = 0; // Reset the previous size
                            for (size_t i = rsi; i < compiledCode.size(); i++)
                            {
                                if (compiledCode[i].opcode == DB || compiledCode[i].opcode == LSBR)
                                    foundVar->second.size += compiledCode[i].bytes.size() - 1;
                            }
                            foundVar->second.size += 1 + 4; // 1 for the null term and 4 for the size holder?

                            // Update varCount with the new size
                            // if there's no change in size then nothings gonna happen
                            sti.fn.varCount += foundVar->second.size - p_size;

                            compiledCode.push_back(Instruction{  .opcode = PSBR, .p3 = (int64_t) foundVar->second.index });
                            break;
                        }
                        case BOOLEAN:
                        {
                            for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                            {
                                StatementInfo lst(*p, sti.fn);
                                StatementCodeGen(lst);
                            }

                            compiledCode.push_back(Instruction{ .opcode = PBBR, .p3 = (int64_t) foundVar->second.index });
                            break;
                        }
                    }
                }
                else throw runtime_error("Reassignment to what then? Exception class needed.");
                break;
            }
            case POST_INCREMENT_EXPRESSION:
            {
                auto foundVar = sti.fn.varOffsets.find(sti.st.name);
                if (foundVar == sti.fn.varOffsets.end())
                    throw runtime_error("Compiler Error: The name '" + sti.st.name + "' does not exist in the current context.");

                for (auto& st : sti.st.body)
                {
                    StatementInfo lst(st, sti.fn);
                    StatementCodeGen(lst);
                }

                compiledCode.push_back(Instruction { .opcode = INC });
                compiledCode.push_back(Instruction { .opcode = PIBR, .p3 = (int64_t) foundVar->second.index });
                break;
            }
            case POST_DECREMENT_EXPRESSION:
            {
                auto foundVar = sti.fn.varOffsets.find(sti.st.name);
                if (foundVar == sti.fn.varOffsets.end())
                    throw runtime_error("Compiler Error: The name '" + sti.st.name + "' does not exist in the current context.");

                for (auto& st : sti.st.body)
                {
                    StatementInfo lst(st, sti.fn);
                    StatementCodeGen(lst);
                }

                compiledCode.push_back(Instruction { .opcode = DEC });
                compiledCode.push_back(Instruction { .opcode = PIBR, .p3 = (int64_t) foundVar->second.index });
                break;
            }
            case OPERATOR_CALL:
            {
                if (sti.st.body.size() != 2)
                    throw runtime_error("Bad Operation. Exception class needed.");

                if (sti.st.name == "+")
                {
                    Type pType;
                    for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
                    }
                    switch (sti.st.type.fType)
                    {
                        case STRING:
                        {
                            compiledCode.push_back(Instruction { .opcode = CMSTR });
                            break;
                        }
                        case INT8:
                        case UINT8:
                        case INT32:
                        case UINT32:
                        default:
                        {
                            compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                            compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                            compiledCode.push_back(Instruction { .opcode = ADD, .reg1 = R9, .reg2 = R8 });
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                            break;
                        }
                    }
                }
                else if (sti.st.name == "-")
                {
                    for (auto p = sti.st.body.rbegin();
                        p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
                    }

                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = SUB, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                }
                else if (sti.st.name == "*")
                {
                    for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
                    }

                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = MUL, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                }
                else if (sti.st.name == "/")
                {
                    for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR }); 
                    }

                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = DIV, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                }
                else if (sti.st.name == "<")
                {
                    for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = FNR });
                    }

                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = CILT, .reg1 = R9, .reg2 = R8 });
                }
                else if (sti.st.name == ">")
                {
                    for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = FNR });
                    }

                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = CIGT, .reg1 = R9, .reg2 = R8 });
                }
                else if (sti.st.name == "!=")
                {
                    for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                            compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = FNR });
                    }

                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = CINE, .reg1 = R9, .reg2 = R8 });
                }
                else if (sti.st.name == "==")
                {
                for (auto p = sti.st.body.rbegin(); p != sti.st.body.rend(); p++)
                {
                    StatementInfo lst(*p, sti.fn);
                    StatementCodeGen(lst);
                    if (p->kind == FUNCTION_CALL)
                        compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = FNR });
                }

                compiledCode.push_back(Instruction{ .opcode = POP, .reg1 = R9 });
                compiledCode.push_back(Instruction{ .opcode = POP, .reg1 = R8 });
                compiledCode.push_back(Instruction{ .opcode = CIET, .reg1 = R9, .reg2 = R8 });
                }
                break;
            }
            case LITERAL:
            {
                switch (sti.st.type.fType)
                {
                    case VOID:
                    {
                        break;
                    }
                    case UINT8:
                    {
                        uint8_t data = stoi(sti.st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = data });
                        break;
                    }
                    case INT8:
                    {
                        int8_t data = stoi(sti.st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = data });
                        break;
                    }
                    case UINT32:
                    {
                        uint32_t data = stoi(sti.st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = data });
                        break;
                    }
                    case INT32:
                    {
                        int32_t data = stoi(sti.st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = data });
                        break;
                    }
                    case DOUBLE:
                    {
                        break;
                    }
                    case STRING:
                    {
                        vector<uint16_t> bytes(sti.st.name.size());
                        for (int i = 0; i < sti.st.name.size(); i++) bytes[i] = sti.st.name[i];
                        bytes.push_back(0);
                        compiledCode.push_back(Instruction{ .opcode = DB, .bytes = bytes });
                        break;
                    }
                    case BOOLEAN:
                    {
                        if (sti.st.name == "true") compiledCode.push_back(Instruction { .opcode = PUSH, .pl = 16, .p3 = 1 });
                        else compiledCode.push_back(Instruction { .opcode = PUSH, .pl = 16, .p3 = 0 });
                        break;
                    }
                    case STRUCT:
                    {
                        break;
                    }
                } 
                break;
            }
            case IDENTIFIER_EXPRESSION:
            {
                // Check if it's a function argument
                auto foundArg = sti.fn.args.find(sti.st.name);
                if (foundArg != sti.fn.args.end())
                {
                    // Function argument reference
                    switch (foundArg->second.type.fType)
                    {
                        case INT32:
                        {
                            compiledCode.push_back(Instruction { .opcode = LIBR, .reg1 = RBP, .p3 = (int64_t) foundArg->second.index });
                            break;
                        }
                        case STRING:
                        {
                            compiledCode.push_back(Instruction { .opcode = LSBR, .reg1 = RBP, .p3 = (int64_t) foundArg->second.index, });
                            break;
                        }
                        case BOOLEAN:
                        {
                            compiledCode.push_back(Instruction { .opcode = LBBR, .reg1 = RBP, .p3 = (int64_t) foundArg->second.index });
                            break;
                        }
                    }
                    break;
                }

                // Then check in the variable map
                auto& varOffsets = sti.fn.varOffsets;
                auto foundVar = varOffsets.find(sti.st.name);
                if (foundVar == varOffsets.end())
                    throw runtime_error("Identifier expression not found. Did you declare it?"); 

                switch (foundVar->second.type.fType)
                {
                    case INT32:
                    {
                        compiledCode.push_back(Instruction { .opcode = LIBR, .p3 = (int64_t) foundVar->second.index });
                        break;
                    }
                    case STRING:
                    {
                        compiledCode.push_back(Instruction { .opcode = LSBR, .p3 = (int64_t) foundVar->second.index });
                        break;
                    }
                    case BOOLEAN:
                    {
                        compiledCode.push_back(Instruction { .opcode = LBBR, .p3 = (int64_t) foundVar->second.index });
                        break;
                    }
                }
                break;
            }
            case WHILE_STATEMENT:
            {
                // Compilers are so fucking smart, except mine cause it was written by me.

                // Store the address of the loop body
                size_t bodyAddr = compiledCode.size();
                bool infi = false;

                // Generate code for the body first
                for (auto& st : sti.st.body)
                {
                    StatementInfo lsti(st, sti.fn);
                    StatementCodeGen(lsti);
                }
                //Store the end address of the loop body
                size_t bodyEndAddr = compiledCode.size();

                // And then generate code for the condition
                for (auto& arg : sti.st.args)
                {
                    if (arg.kind == LITERAL && arg.type.fType == BOOLEAN)
                    {
                        if (arg.name == "false") goto whileout; // Not going to execute.
                        else infi = true; // It's an infinity loop, no need for a condition.
                        continue;
                    }

                    StatementInfo lsti(arg, sti.fn);
                    StatementCodeGen(lsti);
                    // Implement function calls? dunno

                    if (arg.kind == IDENTIFIER_EXPRESSION && arg.type.fType == BOOLEAN)
                    {
                        // So many unnecessary insutrctions for so little. This shit definetly needs
                        // some aggressive optimization and I literally mean it, like holy hell.
                        compiledCode.push_back(Instruction { .opcode = POP, .pl = 16, .reg1 = R8 });
                        compiledCode.push_back(Instruction { .opcode = CIET, .reg1 = R8, .p3 = 1 });
                    }
                }

                // Search for any dummy keywords and replace them with actual instructions.
                for (size_t i = bodyAddr; i < bodyEndAddr; i++)
                {
                    // I am using a switch cause later on I am gona add more keywords...
                    switch (compiledCode[i].opcode)
                    {
                        case BREAK:
                        {
                            // Replace the dummy instruction with an unconditional jump and set the index
                            // to the end of the loop.
                            compiledCode[i] = Instruction { .opcode = JMP, .p3 = (int64_t) compiledCode.size() + 1 };
                            break;
                        }
                    }
                }

                // If the condition is met, then jump back to the body again
                if (!infi)
                    compiledCode.push_back(Instruction { .opcode = CJMP, .p3 = (int64_t) bodyAddr });
                else
                    compiledCode.push_back(Instruction { .opcode = JMP, .p3 = (int64_t) bodyAddr });
whileout:
                break;
            }
            case IF_STATEMENT:
            {
                bool infi = false;
                bool cont = true;

                // Generate code for the condition first
                for (auto& arg : sti.st.args)
                {
                    if (arg.kind == LITERAL && arg.type.fType == BOOLEAN)
                    {
                        if (arg.name == "false")
                        {
                            cont = false;
                            break;
                        } // Not going to execute.
                        else infi = true; // It's an infinity loop, no need for a condition.
                        continue;
                    }

                    StatementInfo lsti(arg, sti.fn);
                    StatementCodeGen(lsti);
                    // Implement function calls? dunno

                    if (arg.kind == IDENTIFIER_EXPRESSION && arg.type.fType == BOOLEAN)
                    {
                        // So many unnecessary insutrctions for so little. This shit definetly needs
                        // some aggressive optimization and I literally mean it, like holy hell.
                        compiledCode.push_back(Instruction { .opcode = POP, .pl = 16, .reg1 = R8 });
                        compiledCode.push_back(Instruction { .opcode = CIET, .reg1 = R8, .p3 = 1 });
                    }
                }

                if (!cont) break;

                // If the condition is not met, then jump to the following instruction after the body.
                // Right now this is just a temporary placeholder. It will get replaced once the compiler
                // finishes code generation for the body.
                compiledCode.push_back(Instruction { .opcode = JNE });
                size_t offset = compiledCode.size() - 1;

                // Generate code for the body second
                for (auto& st : sti.st.body)
                {
                    StatementInfo lsti(st, sti.fn);
                    StatementCodeGen(lsti);
                }


                // This will move to the next instruction but if there are any else/else if statements
                // then they will replace this with their end.
                // Basically if we get to this point in, then we sure know that the condition
                // was met and we don't have to check and/or execute following else/else if statements if they are
                // present of course. Obviously I will have to optimize this cause we are adding unnecessary
                // instruction at the end if there are no else/else if statements following this.
                // We also need to keep track of every If/If else instances cause if we have multiple else if
                // statements and for example the condition of the first else if statement was met, then we'll
                // have to skip rest of them but the problem is we are only going to skip the following 
                // else if statement and anything after that is going to be executed anyways.
                // We can prevent this by simply by storing the indexes of every If statement's JMP instruciton
                // and then coming back to it and updating the index.
                if (!sti.fn.ifInsts.empty()) sti.fn.ifInsts.clear();
                sti.fn.ifInsts.push_back(compiledCode.size());
                compiledCode.push_back(Instruction { .opcode = JMP, .p3 = (int64_t) compiledCode.size() + 1 });


                // Replace the placeholder with the actual instruction now that we have the address
                // of the instruction following the if statement.
                if (!infi)
                    compiledCode[offset] = Instruction { .opcode = JNE, .p3 = (int64_t) compiledCode.size() };
                else
                    // This If statement is going to execute anyways so let's remove the dummy JMP holder.
                    compiledCode.erase(compiledCode.begin() + offset);
                break;
            }
            case ELSE_STATEMENT:
            {
                // Get the index of the JMP insturction that IF generates at the very end.
                size_t jmpAddr = compiledCode.size() - 1;

                // Generate code for the body
                for (auto& st : sti.st.body)
                {
                    StatementInfo lsti(st, sti.fn);
                    StatementCodeGen(lsti);
                }

                // If there were no If/If else instance before then most likley there were no
                // If/If else statements to begin with.
                if (sti.fn.ifInsts.empty())
                    throw runtime_error("Compiler Error: Expected an IF statement before ELSE.\nException class needed.");

                // Now Update every If instance's JMP instruction.
                for (auto& addr : sti.fn.ifInsts) compiledCode[addr] = Instruction{ .opcode = JMP, .p3 = (int64_t) compiledCode.size() };

                // Clear all the If instances
                sti.fn.ifInsts.clear();
                break;
            }
            case ELSE_IF_STATEMENT:
            {
                // Basically the combination of the two above.

                // Get the index of the JMP insturction that IF generates at the very end.
                size_t jmpAddr = compiledCode.size() - 1;

                bool infi = false;
                bool cont = true;

                // Generate code for the condition first
                for (auto& arg : sti.st.args)
                {
                    if (arg.kind == LITERAL && arg.type.fType == BOOLEAN)
                    {
                        if (arg.name == "false")
                        {
                            cont = false;
                            break;
                        } // Not going to execute.
                        else infi = true; // It's an infinity loop, no need for a condition.
                        continue;
                    }

                    StatementInfo lsti(arg, sti.fn);
                    StatementCodeGen(lsti);
                    // Implement function calls? dunno

                    if (arg.kind == IDENTIFIER_EXPRESSION && arg.type.fType == BOOLEAN)
                    {
                        // So many unnecessary insutrctions for so little. This shit definetly needs
                        // optimization and I literally mean it, like what the fuck.
                        compiledCode.push_back(Instruction { .opcode = POP, .pl = 16, .reg1 = R8 });
                        compiledCode.push_back(Instruction { .opcode = CIET, .reg1 = R8, .p3 = 1 });
                    }
                }

                if (!cont) break;

                // If the condition is not met, then jump to the following instruction after the body.
                // Right now this is just a temporary placeholder. It will get replaced once the compiler
                // finishes code generation for the body.
                compiledCode.push_back(Instruction { .opcode = JNE });
                size_t offset = compiledCode.size() - 1;

                // Generate code for the body second
                for (auto& st : sti.st.body)
                {
                    StatementInfo lsti(st, sti.fn);
                    StatementCodeGen(lsti);
                }


                // This will move to the next instruction but if there are any else/else if statements
                // then they will replace this with their end.
                // Basically if we get to this point in, then we sure know that the condition
                // was met and we don't have to check and/or execute following else/else if statements if they are
                // present of course. Obviously I will have to optimize this cause we are adding unnecessary
                // instruction at the end if there are no else/else if statements following this.
                // We also need to keep track of every If/If else instances cause if we have multiple else if
                // statements and for example the condition of the first else if statement was met, then we'll
                // have to skip rest of them but the problem is we are only going to skip the following 
                // else if statement and anything after that is going to be executed anyways.
                // We can prevent this by simply by storing the indexes of every If statement's JMP instruciton
                // and then coming back to it and updating the index.
                sti.fn.ifInsts.push_back(compiledCode.size());
                compiledCode.push_back(Instruction { .opcode = JMP, .p3 = (int64_t) compiledCode.size() + 1 });


                // Replace the placeholder with the actual instruction now that we have the address
                // of the instruction following the if statement.
                if (!infi) compiledCode[offset] = Instruction { .opcode = JNE, .p3 = (int64_t) compiledCode.size() };
                else
                    // This If statement is going to execute anyways so let's remove the dummy JMP holder.
                    compiledCode.erase(compiledCode.begin() + offset);


                // If there were no If/If else instance before then most likley there were no
                // If/If else statements to begin with.
                if (sti.fn.ifInsts.empty())
                    throw runtime_error("Compiler Error: Expected an IF statement before ELSE.\nException class needed.");
                
                // Now Update every If instance's JMP instruction.
                for (auto& addr : sti.fn.ifInsts) compiledCode[addr] = Instruction{ .opcode = JMP, .p3 = (int64_t) compiledCode.size() };

                // Change the jump address to the end of ELSE.
                //compiledCode[jmpAddr] = Instruction { .opcode = JMP, .p3 = compiledCode.size() };
                break;
            }
            case BREAK_STATEMENT:
            {
                compiledCode.push_back(Instruction { .opcode = BREAK });
                break;
            }
        }
        return true;
    }

    void ReadBinary();

    void Compiler::WriteToBinary(CompilerOptions* options)
    {
        ofstream fs;
        fs.open(options->path, std::ios::out);

        ByteBuffer w_buffer;
        if (fs.is_open())
        {
            for (auto& bc : compiledCode)
            {
                for (auto& b : bc.bytes) w_buffer.Write(b);
                w_buffer.Write64(bc.bytes.size());
                w_buffer.Write(bc.opcode);
                w_buffer.Write(bc.pl);
                w_buffer.Write(bc.reg1.reg);
                w_buffer.Write(bc.reg2.reg);
                w_buffer.Write(bc.p2);
                w_buffer.Write64(bc.p3);
            }

            // This is a very bad implementation but for now it does the job
            // I might convert my stack from 16bit to 8bit
            System::File::WriteToBytes(options->path, System::Stack::Convert_16bitTo8bit(w_buffer.buffer));
        }
        else cerr << "Compiler Error: Could not create file to write to." << endl;
    }

    void ReadBinary()
    {
        ByteCode bin;
        ByteBuffer n_buffer;
        n_buffer.buffer = System::Stack::Convert_8bitTo16bit(*System::File::ReadAllBytes("../../../../Test/out.aex"));

        while (n_buffer.Size() != 0)
        {
            Instruction inst;
            inst.p3 = n_buffer.Read64();
            inst.p2 = n_buffer.Read();
            inst.reg2 = (Regs)n_buffer.Read();
            inst.reg1 = (Regs)n_buffer.Read();
            inst.pl = n_buffer.Read();
            inst.opcode = (OpCode)n_buffer.Read();

            size_t bs = n_buffer.Read64();
            if (bs != 0) for (size_t i = 0; i < bs; i++) inst.bytes.insert(inst.bytes.begin(), n_buffer.Read());

            bin.insert(bin.begin(), inst);
        }
    }
}