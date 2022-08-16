#include <iostream>
#include <vector>
#include <exception>
#include <optional>
#include <cstdlib>

#include "src/Lib/include/Define.h"
#include "src/Lib/include/Utils.h"
#include "src/Analyzer/Armavir/include/Analyzer.h"
#include "src/Compilers/AVM/include/AVM.h"

//\ |

using namespace arma;
using namespace avm;

int main(int argc, char** argv) 
{
    try
    {
        std::optional<std::string> src = ReadToString("/home/nedd/source/repos/Livyonn/Test/HL_Sample.am");
        if (!src.has_value()) throw runtime_error(string("Failed to open source file."));

        // Analyzer an;
        // an.StartAnalysis(*src);
        // an.DumpTruck(); // debug print
    
        AVM avm;
        avm.Start();    
    }
    catch (exception& ex)
    {
        std::cerr << RED << " Error: " << ex.what() << RESET << std::endl;

        return 2;
    }
    catch (...)
    {
        std::cerr << RED << "Unknown exception occured." << RESET << std::endl;

        return 1;
    }
}
