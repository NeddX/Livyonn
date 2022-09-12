#pragma once

#include <iostream>
#include <cstdint>
#include <map>

#include "../../../Lib/include/Define.h"
#include "Registers.h"

namespace avm
{
    using namespace std;

    enum OpCode : uint16_t
    {
        END,
        PUSH,
        POP,
        PEEK,
        ADD,
        SUB,
        MUL,
        DIV,
        PRINT_INT,
        PRINT_STR,
        COMP_INT_LT, // If Less Than
        COMP_INT_ET, // If Equal To
        COMP_INT_GT, // If Greater than
        COMP_INT_NE, // If Not Equal
        MOV,
        MOVS,
        PIBR, //Push Int Basepointer Relative
        LIBR, // Liberal (ew) Load Int Basepointer Relative
        PSBR,
        LSBR,
        JMP, // Unconditional Address Jump
        CJMP, // Jump to Address If ZF Flag is 1
        JRP, // Unconditional Relative Jump
        CJRP, // Jump Relative if ZF Flag is 1
        JRN, // Jump Relative if ZF Flag is 0
        JNE, // Jump if ZF Flag is 0
        CALL,
        RETURN,
        DB,
        CMSTR,

        NOP
    };

    static const char* opcodeType[] =
    {
        "END",      
        "PUSH",      
        "POP",   
        "PEEK",   
        "ADD",      
        "SUB",      
        "MUL",      
        "DIV",      
        "PRINT_INT",
        "PRINT_STR",
        "PRINT",      
        "COMP_INT_LT,",         
        "COMP_INT_ET,",      
        "COMP_INT_GT,",           
        "COMP_INT_NE,",              
        "MOV",      
        "MOVS",      
        "PIBR",
        "LIBR",
        "PSBR",
        "LSBR",
        "JMP",  
        "CJMP", 
        "JRP",
        "CJRP",    
        "JRN",
        "JNE",
        "CALL",      
        "RETURN",  
        "NOP",
        "DB",
        "RB"
    };

    struct Instruction
    {
        OpCode opcode = NOP;
        uint8_t pl = 0;
        Regs reg1 = NUL;
        Regs reg2 = NUL;
        uint16_t p2 = 0;
        uint64_t p3 = 0;
        vector<uint16_t> bytes;
    };    

    typedef vector<Instruction> ByteCode;
}