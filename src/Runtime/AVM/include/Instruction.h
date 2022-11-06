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
        ADD,
        SUB,
        MUL,
        DIV,
        PRINT_INT,
        MOV,
        JMP, // Unconditional Address Jump
        CJMP, // Jump to Address If ZF Flag is 1
        JNE, // Jump if ZF Flag is 0
        CALL,
        RETURN,
        CIET, // If Equal To
        CINE, // If Not Equal
        CIGT, // If Greater than
        CILT, // If Less Than
        INC,
        DEC,

        PRINT_STR,
        MOVS,
        PIBR, //Push Int Basepointer Relative
        LIBR, // Liberal (ew) Load Int Basepointer Relative
        PSBR,
        LSBR,
        PBBR,
        LBBR,
        DB,
        CMSTR,
        POP_STR,

        BREAK, // Dummy instruction, gets replaced by JMP in the Compiler and the Runtime will crash if used.

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
        "PBBR",
        "LBBR",
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
        "RB",
        "INC",
        "DEC",

        "NUL"
    };

    struct Instruction
    {
        OpCode opcode = NOP;
        uint8_t pl = 0;
        Reg reg1;
        Reg reg2;
        //Regs reg1 = NUL;
        //Regs reg2 = NUL;
        uint32_t p3 = 0;
        vector<uint8_t> bytes;
    };    

    typedef vector<Instruction> ByteCode;
}