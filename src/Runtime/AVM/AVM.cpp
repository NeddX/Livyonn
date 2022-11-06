#include "include/AVM.h"

namespace avm
{
    void AVM::Start(ByteCode& bytecode, optional<vector<string>> args)
    {
        int32_t result = 0;
        //ByteBuffer bargs;
        //if (args) for (auto& e : *args) bargs.Write32(stoi(e));
        Runtime::Run(bytecode.data(), result);
        printf("\nAVM halted with exit code: %ld\n", result);
    }    
}
