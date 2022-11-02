#include "include/FunctionDefinition.h"

namespace arma
{
    void FunctionDefinition::FuncDumpTruck() const
    {
        cout << name << "(";

        auto it = args.begin();
        while (true)
        {
            if (it == args.end()) break;
            else if (it + 1 == args.end()) cout << it->type.name << " " << it->name;
            else cout << it->type.name << " " << it->name << ", ";
            it++;
        }

        cout << ")" << NL << "{" << NL;
    
        for (auto& s : body)
        {
            cout << TAB;
            s.StDumpTruck();            
        }

        cout << NL << "}" << endl;
    }
}