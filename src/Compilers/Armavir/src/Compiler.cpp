#include "../include/Compiler.h"

namespace arc
{
    using namespace std;
    using namespace arma;

    ByteCode Compiler::Compile(Analyzer& syntaxTree)
    {
        ByteCode bytecode;
        map<string, FunctionDefinition> funcs = syntaxTree.GetFunctions();
        for (auto& [name, func] : funcs)
        {
            FunctionCodeGen(func, bytecode);
        }
        return bytecode;
    }

    void Compiler::FunctionCodeGen(const FunctionDefinition& func, ByteCode& compiledCode)
    {
        int intVarCount = 0;
        vector<int16_t> returnInstructionAddresses;
        map<string, VarInfo> varOffsets; 

        for (auto& cs : func.statements)
        {
            switch (cs.kind)
            {
                case VARIABLE_DECLERATION:
                {
                    switch (cs.type.fType)
                    {
                        case INT8:
                        {
                            break;
                        }
                        case UINT8:
                        {
                            break;
                        }
                        case INT32:
                        {
                            VarInfo vi;
                            vi.type.fType = INT32;
                            vi.offset = intVarCount;
                            int32_t data;
                            if (!cs.params.empty())
                            {
                                for (auto& p : cs.params)
                                {
                                    StatementCodeGen(p, varOffsets, compiledCode);
                                }
                            }
                            varOffsets[cs.name] = vi;
                            intVarCount++;
                            break;
                        }
                        case UINT32:
                        {
                            break;
                        }
                        case DOUBLE:
                        {
                            break;
                        }
                        case STRING:
                        {
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
                            break;                            
                        }
                    }
                    break;
                }
            }            
        }

        for (auto& cs : func.statements)
        {
            StatementCodeGen(cs, varOffsets, compiledCode);
        }

        size_t cleanupOffset = compiledCode.size() * sizeof(Instruction);
        for (auto& r : returnInstructionAddresses)
        {
            compiledCode[r].p3 = cleanupOffset;
        }
        for (auto& [key, var] : varOffsets)
        {
            switch (var.type.fType)
            {
                case UINT32:
                case INT32:
                {
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = 32 });
                    break;
                }
                case UINT8:
                case INT8:
                {
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = 16 });
                    break;
                }
                case DOUBLE:
                default:
                {
                    compiledCode.push_back(Instruction { .opcode = POP });
                    break;
                }
            }
        }
        compiledCode.push_back(Instruction { .opcode = RETURN });
    }

    void Compiler::StatementCodeGen(const Statement& st, map<string, VarInfo> varOffsets, ByteCode& compiledCode)
    {
        switch (st.kind)
        {
            case RETURN_STATEMENT:
            {
                compiledCode.push_back(Instruction { .opcode = PEEK, .pl = compiledCode.back().pl, .reg1 = FNR });
                break;
            }
            case FUNCTION_CALL:
            {
                break;
            }
            case VARIABLE_DECLERATION:
            {
                switch (st.type.fType)
                {
                    case INT32:
                    {
                        int32_t data;
                        if (!st.params.empty())
                        {
                            if (st.params[0].kind == LITERAL)
                            {
                                // Generate assignment statement?
                            }
                        }
                        break;
                    }
                }
                break;
            }
            case OPERATOR_CALL:
            {
                if (st.name == "+")
                {
                    size_t count = 0;
                    if (st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (auto& p : st.params)
                    {
                        StatementCodeGen(p, varOffsets, compiledCode);
                        count++;
                    }
                    uint8_t bitSize = compiledCode[compiledCode.size() - count].pl;
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = ADD, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .pl = bitSize, .reg1 = R9 });
                }
                else if (st.name == "-")
                {
                    size_t count = 0;
                    if (st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (auto& p : st.params)
                    {
                        StatementCodeGen(p, varOffsets, compiledCode);
                        count++;
                    }
                    uint8_t bitSize = compiledCode[compiledCode.size() - count].pl;
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = SUB, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .pl = bitSize, .reg1 = R9 });
                }
                else if (st.name == "*")
                {
                    size_t count = 0;
                    if (st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (auto& p : st.params)
                    {
                        StatementCodeGen(p, varOffsets, compiledCode);
                        count++;
                    }
                    uint8_t bitSize = compiledCode[compiledCode.size() - count].pl;
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = MUL, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .pl = bitSize, .reg1 = R9 });
                }
                else if (st.name == "/")
                {
                    size_t count = 0;
                    if (st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    for (auto& p : st.params)
                    {
                        StatementCodeGen(p, varOffsets, compiledCode);
                        count++;
                    }
                    uint8_t bitSize = compiledCode[compiledCode.size() - count].pl;
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R8 });
                    compiledCode.push_back(Instruction { .opcode = POP, .pl = bitSize, .reg1 = R9 });
                    compiledCode.push_back(Instruction { .opcode = DIV, .reg1 = R9, .reg2 = R8 });
                    compiledCode.push_back(Instruction { .opcode = PUSH, .pl = bitSize, .reg1 = R9 });
                }
                else if (st.name == "=")
                {
                    if (st.params.size() != 2)
                    {
                        throw runtime_error("Bad Operation. Exception class needed.");
                    }
                    auto foundVar = varOffsets.find(st.params[0].name);
                    if (foundVar == varOffsets.end())
                    {
                        throw runtime_error("err"); 
                    }   
                    StatementCodeGen(st.params[1], varOffsets, compiledCode);
                    compiledCode.push_back(Instruction { .opcode = PIBR, .pl = 32, .p3 = (uint16_t) foundVar->second.offset });  
                }
                break;
            }
            case LITERAL:
            {
                switch (st.type.fType)
                {
                    case VOID:
                    {
                        break;
                    }
                    case UINT8:
                    {
                        uint8_t data = stoi(st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .pl = 16, .p3 = data });
                        break;
                    }
                    case INT8:
                    {
                        int8_t data = stoi(st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .pl = 16, .p3 = (uint8_t) data });
                        break;
                    }
                    case UINT32:
                    {
                        uint32_t data = stoi(st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .pl = 32, .p3 = data });
                        break;
                    }
                    case INT32:
                    {
                        int32_t data = stoi(st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .pl = 32, .p3 = (uint32_t) data });
                        break;
                    }
                    case DOUBLE:
                    {
                        double data = stod(st.name);
                        compiledCode.push_back(Instruction { .opcode = PUSH, .p3 = (uint64_t) data });
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
                auto foundVar = varOffsets.find(st.name);
                if (foundVar == varOffsets.end())
                {
                    throw runtime_error("Identifier expression not found. Did you declare it?"); 
                } 
                uint8_t bitSize = compiledCode[compiledCode.size() - 1].pl;
                compiledCode.push_back(Instruction { .opcode = LIBR, .pl = bitSize, .p3 = (uint16_t) foundVar->second.offset });  
                break;
            }
        }
    }
}