#include "include/AVM.h"

namespace avm
{
    void AVM::Start(ByteCode& bytecode, optional<vector<string>> args)
    {
        int64_t result = 0;
        ByteBuffer bargs;
        if (args) for (auto& e : *args) bargs.Write64(stoi(e));
        Runtime::Run(bytecode.data(), result, bargs);
        printf("\nAVM halted with the code: %d\n", result);
    }    
}