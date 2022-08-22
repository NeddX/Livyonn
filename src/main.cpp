#include <iostream>
#include <vector>
#include <exception>
#include <optional>
#include <cstdlib>

#include "Lib/include/Define.h"
#include "Lib/include/Utils.h"
#include "Analyzers/Armavir/include/Analyzer.h"
#include "Analyzers/AVM/include/Analyzer.h"
#include "Compilers/AVM/include/AVM.h"

//\ |

using namespace arma; // Armavir Analyzer
using namespace arba; // Armavir Bytecode Analyzer
using namespace avm; // Armavir Virtual Machine

int main(int argc, char** argv) 
{
    try
    {
        std::optional<std::string> hl_src = ReadToString("/home/nedd/source/repos/Livyonn/Test/HL_Sample.arm");
        if (!hl_src.has_value()) throw runtime_error(string("Failed to open source file."));

        std::optional<std::string> mc_src = ReadToString("/home/nedd/source/repos/Livyonn/Test/MC_Sample.amc");
        if (!mc_src.has_value()) throw runtime_error(string("Failed to open source file."));

        // Analyzer an;
        // an.StartAnalysis(*hl_src);
        // an.DumpTruck(); // debug print
    
        AVM avm;  

        arba::Analyzer bca;
        auto v = bca.StartAnalysis(*mc_src);
        //cout << NL;
        avm.Start(v);
    }
    catch (exception& ex)
    {
        std::cerr << RED << "Error: " << ex.what() << RESET << std::endl;

        return 2;
    }
    catch (...)
    {
        std::cerr << RED << "Unknown exception occured." << RESET << std::endl;

        return 1;
    }
}
