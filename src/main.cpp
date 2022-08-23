#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <exception>
#include <optional>
#include <cstdlib>
#include <iterator>

#include "Lib/include/Define.h"
#include "Lib/include/Utils.h"
#include "Analyzers/Armavir/include/Analyzer.h"
#include "Analyzers/AVM/include/Analyzer.h"
#include "Compilers/AVM/include/AVM.h"

//\ |

enum Action
{
    HELP,
    VERSION,
    COMPILE,
    EXECUTE,
    DECOMPILE,
    DEBUG
};

int main(int argc, char** argv) 
{
    std::vector<std::string> args(argv, argv + argc);
    std::string file;
    Action ack = DEBUG;
    try
    {

        // if (argc < 2)
        // {
        //     std::cout << "Too few arguments." << std::endl;
        //     return 1;
        // }

        for (int i = 0; i < argc; i++)
        {
            if (args[i] == "compile")
            {
                file = args[i + 1];
                ack = COMPILE;
            }
            else if (args[i] == "decompile")
            {
                file = args[i + 1];
                ack = DECOMPILE;
            }
            else if (args[i] == "run")
            {
                file = args[i + 1];
                ack = EXECUTE;
            }
        }


        if (ack == DEBUG)
        {

            // std::optional<std::string> hl_src = ReadToString("/home/nedd/source/repos/Livyonn/Test/HL_Sample.arm");
            // if (!hl_src.has_value()) throw std::runtime_error(string("Failed to open source file."));

            std::optional<std::string> mc_src = ReadToString("/home/nedd/source/repos/Livyonn/Test/MC_Sample.amc");
            if (!mc_src.has_value()) throw std::runtime_error(std::string("Failed to open source file."));

            // Analyzer an;
            // an.StartAnalysis(*hl_src);
            // an.DumpTruck(); // debug print

            avm::AVM avm;  

            arba::Analyzer bca;
            arba::ByteCode v = bca.StartAnalysis(*mc_src);
            std::cout << NL;
            std::vector<std::string> fa = { "99999", "50" };
            avm.Start(v);

            return 0;
        }

        switch (ack)
        {
            case COMPILE:
            {
                break;
            }
            case EXECUTE:
            {
                std::cout << "Runtime started" << NL;
                if (file.ends_with(".aex"))
                {
                    std::cout << "AVM Exectuable detected" << NL;
                }
                else if (file.ends_with(".amc"))
                {
                    std::cout << "AVM Bytecode detected" << NL;

                    std::optional<std::string> src = ReadToString(file);
                    if (!src.has_value()) throw std::runtime_error(std::string("Failed to open source file."));

                    int i = find(args.begin(), args.end(), file) - args.begin();

                    std::vector<std::string> fargs(args.begin() + ++i, args.end());
                    avm::AVM avm;
                    arba::Analyzer an;
                    avm::ByteCode v = an.StartAnalysis(*src);
                    std::cout << NL;
                    avm.Start(v, fargs);
                }
                break;
            }
        }
    }
    catch (std::exception& ex)
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
