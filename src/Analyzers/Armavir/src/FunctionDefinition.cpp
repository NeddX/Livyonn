#include "../include/FunctionDefinition.h"

namespace arma
{
    void FunctionDefinition::FuncDumpTruck() const
    {
        cout << name << "(";

        auto it = params.begin();
        while (true)
        {
            if (it == params.end()) break;
            else if (it + 1 == params.end()) cout << it->type.name << " " << it->name;
            else cout << it->type.name << " " << it->name << ", ";
            it++;
        }

        cout << ")" << NL << "{" << NL;
    
        for (auto& s : statements)
        {
            cout << TAB;
            s.StDumpTruck();            
        }

        cout << NL << "}" << endl;
    }
}