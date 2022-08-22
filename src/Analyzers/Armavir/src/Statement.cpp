#include "../include/Statement.h"

namespace arma
{
    void Statement::StDumpTruck() const
    {
        switch (kind)
        {
            case VARIABLE_DECLERATION:
            {
                cout << "<" << KindStr[kind] << "> " << type.name << " " << name << " = ";
                if (params.size() <= 0)
                {
                    cout << "<undefined>" << NL;
                    break;
                }
                for (auto& ps : params) 
                {
                    ps.StDumpTruck();
                }
          
                cout << ";" << NL;
                break;
            }
            case FUNCTION_CALL:
            {
                cout << "<" << KindStr[kind] << "> " << name << "(";

                int i = 0;
                for (auto& fp : params)
                {
                    fp.StDumpTruck();
                    if (i + 1 != params.size()) cout << ", ";
                    i++;
                }

                cout << ");";
                break;
            }
            case LITERAL:
            {
                if (type.fType == STRING) cout << '"' << name << '"';
                else cout << name;
                break;
            }
            case OPERATOR_CALL:
            {
                int i = 0;
                for (auto& op : params)
                {
                    if (i + 1 != params.size()) 
                    {
                        op.StDumpTruck(); 
                        cout << " " << name;
                    }
                    else
                    { 
                        cout << " ";
                        op.StDumpTruck();
                    }
                    i++;
                }
                break;
            }
        }
    }
}