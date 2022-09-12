#include "include/Compiler.h"

namespace arc
{
    using namespace std;
    using namespace arma;

    ByteCode Compiler::Compile(Analyzer& syntaxTree)
    {
        map<string, FunctionDefinition> fds = syntaxTree.GetFunctions();

        // Get all the functions before processing them and store them in nice little map designed for
        // function calls
        for (auto& [name, fd] : fds)
        { 
            FuncInfo fn;
            fn.offset = 0;
            fn.fd = fd;
            funcOffsets[name] = fn;
        }

        // Generate code for the Entry function first and then delete it from the map
        FunctionCodeGen(funcOffsets[fds.find("Entry")->first]);
        funcOffsets.erase("Entry");

        // Now generate code for the rest of the functions
        for (auto& [name, fn] : funcOffsets) FunctionCodeGen(fn);
        return compiledCode;
    }

    void Compiler::FunctionCodeGen(FuncInfo& fn)
    {
        FunctionDefinition fd = fn.fd;

        //size_t pi = 0;
        //for (auto& arg : fd.params) fn.args[arg.name] = (Argument{arg.name, pi++, arg.type});

        // Set our offset incase someone calls us in the future
        auto& self = funcOffsets[fd.name];
        self.offset += compiledCode.size();

        // Generate code for those who called us before
        for (auto& c : self.calls) compiledCode[c] = Instruction { .opcode = CALL, .p3 = self.offset };
        
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

        for (auto& cs : fd.statements)
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
        for (const auto& [key, var] : fn.varOffsets)
        {
            switch (var.type.fType)
            {
                case STRING:
                {
                    for (int i = 0; i < var.size; i++) compiledCode.push_back(Instruction{ .opcode = POP, .pl = 16 });
                    break;
                }
                default:
                {
                    compiledCode.push_back(Instruction{ .opcode = POP });
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
                if (!sti.st.params.empty())
                {
                    for (auto& p : sti.st.params)
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
                    if (foundFunc->second.offset == 0)
                    {
                        foundFunc->second.calls.push_back(compiledCode.size());
                        compiledCode.push_back(Instruction { .opcode = NOP, }); // Placeholder instruction
                    }
                    else
                    {
                        compiledCode.push_back(Instruction { .opcode = CALL, .p3 = foundFunc->second.offset });
                    }
                }
                else if (sti.st.name == "_PrintInt")
                {
                    for (auto& p : sti.st.params)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                    }
                    compiledCode.push_back(Instruction { .opcode = PRINT_INT });
                }
                else if (sti.st.name == "_Print")
                {
                    for (auto& p : sti.st.params)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                    }
                    compiledCode.push_back(Instruction{ .opcode = PRINT_STR });
                }
                else if (sti.st.name == "_Println")
                {
                    if (sti.st.params.empty())
                    {
                        compiledCode.push_back(Instruction{ .opcode = DB, .bytes = { 10, 0 } });
                        compiledCode.push_back(Instruction{ .opcode = PRINT_STR });
                        break;
                    }
                    for (auto& p : sti.st.params)
                    {
                        StatementInfo lst(p, sti.fn);
                        StatementCodeGen(lst);
                    }
                    compiledCode.push_back(Instruction{ .opcode = PRINT_STR, .pl = 1 });
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
                        vi.source = fn.varCount;
                        int32_t data;
                        if (!sti.st.params.empty())
                        {
                            for (auto& p : sti.st.params)
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
                        vi.source = fn.varCount;
                        int32_t data;
                        // To get the size of the string (which can be dynamic) we will need to iterate through
                        // the compiled code and get the sum of every DB (DefineByte) instruction's byte size
                        // to make sure we don't accidentally sum together other DB instructions that are
                        // not realated to the current variable decleration, we will have to get the compiledCode
                        // size before actually generating code for variable's statements (value)
                        size_t rsi = compiledCode.size();
                        if (!sti.st.params.empty())
                        {
                            for (auto& p : sti.st.params)
                            {
                                StatementInfo lst(p, sti.fn);
                                StatementCodeGen(lst);
                            }
                        }
                        for (int i = rsi; i < compiledCode.size(); i++)
                        {
                            if (compiledCode[i].opcode == DB || compiledCode[i].opcode == LSBR)
                                vi.size += compiledCode[i].bytes.size() - 1;
                        }
                        vi.size += 1 + 4; // 1 for the null term and 4 for the size holder?
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
                        fn.varCount++;
                        break;
                    }
                }
                break;
            }
            case ASSIGNMENT_EXPRESSION:
            {
                if (!sti.st.params.empty())
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
                            for (vector<Statement>::reverse_iterator p = sti.st.params.rbegin();
                                p != sti.st.params.rend(); p++)
                            {
                                StatementInfo lst(*p, sti.fn);
                                StatementCodeGen(lst);
                            }

                            compiledCode.push_back(Instruction{ .opcode = PIBR, .p3 = foundVar->second.source });
                            break;
                        }
                        case STRING:
                        {
                            // Set the new and updated size
                            size_t rsi = compiledCode.size();
                            size_t p_size = foundVar->second.size;


                            for (vector<Statement>::reverse_iterator p = sti.st.params.rbegin();
                                p != sti.st.params.rend(); p++)
                            {
                                StatementInfo lst(*p, sti.fn);
                                StatementCodeGen(lst);
                            }
                            foundVar->second.size = 0; // Reset the previous size
                            for (int i = rsi; i < compiledCode.size(); i++)
                            {
                                if (compiledCode[i].opcode == DB || compiledCode[i].opcode == LSBR)
                                    foundVar->second.size += compiledCode[i].bytes.size() - 1;
                            }
                            foundVar->second.size += 1 + 4; // 1 for the null term and 4 for the size holder?

                            // Update varCount with the new size
                            // if there's no change in size then nothings gonna happen
                            sti.fn.varCount += foundVar->second.size - p_size;

                            compiledCode.push_back(Instruction{ .opcode = PSBR, .p3 = foundVar->second.source });
                            break;
                        }
                    }
                }
                else throw runtime_error("Reassignment to what then brother? Exception class needed.");
                break;
            }
            case OPERATOR_CALL:
            {
                if (sti.st.name == "+")
                {
                    Type pType;
                    if (sti.st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (vector<Statement>::reverse_iterator p = sti.st.params.rbegin();
                        p != sti.st.params.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                        {
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
                        }
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
                            compiledCode.push_back(Instruction{ .opcode = POP, .reg1 = R9 });
                            compiledCode.push_back(Instruction{ .opcode = POP, .reg1 = R8 });
                            compiledCode.push_back(Instruction{ .opcode = ADD, .reg1 = R9, .reg2 = R8 });
                            compiledCode.push_back(Instruction{ .opcode = PUSH, .reg1 = R9 });
                            break;
                        }
                    }
                }
                else if (sti.st.name == "-")
                {
                    size_t count = 0;
                    if (sti.st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (vector<Statement>::reverse_iterator p = sti.st.params.rbegin();
                        p != sti.st.params.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                        {
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
                        }
                        count++;
                    }
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = SUB, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                }
                else if (sti.st.name == "*")
                {
                    size_t count = 0;
                    if (sti.st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (vector<Statement>::reverse_iterator p = sti.st.params.rbegin();
                        p != sti.st.params.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                        {
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR });
                        }
                        count++;
                    }
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = MUL, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                }
                else if (sti.st.name == "/")
                {
                    size_t count = 0;
                    if (sti.st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (vector<Statement>::reverse_iterator p = sti.st.params.rbegin();
                        p != sti.st.params.rend(); p++)
                    {
                        StatementInfo lst(*p, sti.fn);
                        StatementCodeGen(lst);
                        if (p->kind == FUNCTION_CALL)
                        {
                            compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = FNR }); 
                        }
                        count++;
                    }
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = POP, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = DIV, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .reg1 = R9 });
                }
                else if (sti.st.name == "=") // VARIABLE ASSIGNMENT
                {
                    // irrelevant
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
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = (uint8_t) data });
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
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = (uint32_t) data });
                        break;
                    }
                    case DOUBLE:
                    {
                        double data = stod(sti.st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = (uint64_t) data });
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
                    case STRUCT:
                    {
                        break;
                    }
                } 
                break;
            }
            case IDENTIFIER_EXPRESSION:
            {
                auto& varOffsets = sti.fn.varOffsets;
                auto foundVar = varOffsets.find(sti.st.name);
                if (foundVar == varOffsets.end())
                {
                    // might be a function parameter
                    auto foundArg = sti.fn.args.find(sti.st.name);
                    if (foundArg != sti.fn.args.end())
                    {
                        switch (foundArg->second.type.fType)
                        {
                            case INT32:
                            {
                                compiledCode.push_back(Instruction{ .opcode = LIBR, .p3 = -1 - sti.fn.args.size() + foundArg->second.index });
                                break;
                            }
                            case STRING:
                            {
                                break;
                            }
                        }
                    }
                    throw runtime_error("Identifier expression not found. Did you declare it?"); 
                } 
                switch (foundVar->second.type.fType)
                {
                    case INT32:
                    {
                        compiledCode.push_back(Instruction{ .opcode = LIBR, .p3 = foundVar->second.source });
                        break;
                    }
                    case STRING:
                    {
                        compiledCode.push_back(Instruction{ .opcode = LSBR, .p3 = foundVar->second.source, .bytes = vector<uint16_t>(foundVar->second.size - 4) });
                        break;
                    }
                }
                break;
            }
        }
        return true;
    }
}