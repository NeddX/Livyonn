#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <exception>
#include <optional>
#include <cstdlib>
#include <iterator>

#include "Lib/include/Define.h"
#include "Lib/include/System.h"
#include "Analyzers/Armavir/include/Analyzer.h"
#include "Analyzers/AVM/include/Analyzer.h"
#include "Runtime/AVM/include/AVM.h"
#include "Compilers/AVM/include/Compiler.h"
#include "Compilers/Armavir/include/Compiler.h"

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
            //char buffer[8192];
            //setvbuf(stdout, buffer, 0, sizeof(buffer));

            /* Low Level */

            //std::optional<std::string> mc_src = System::File::ReadToString("../../../../Test/test1.amc");
            //if (!mc_src.has_value()) throw std::runtime_error(std::string("Failed to open source file."));

            //avm::AVM avm;  
            //amca::Analyzer an;
            //amcc::Compiler c;
            //avm::ByteCode bytecode = c.Compile(an.StartAnalysis(*mc_src));
            ////std::cout << NL;
            //avm.Start(bytecode);

            /* end */

            /* High Level */
            std::optional<std::string> src = System::File::ReadToString("../../../../Test/test1.arm");
            if (!src.has_value()) throw std::runtime_error(std::string("Failed to open source file."));
            // ARMA (Armavir Analyzer) and ARBA (Armavi Bytecode Analyzer) 
            arma::Analyzer an; 
            an.StartAnalysis(*src);
            //an.DumpTruck();
            
            arc::Compiler c;
            arc::CompilerOptions* options = new arc::CompilerOptions
            {
                .outputType = arc::EXE,
                .outputExecutable = true,
                .path = "../../../../Test/out.aex"
            };
            avm::ByteCode compiledCode = c.Compile(an, options);

            avm::AVM avm;

            avm.Start(compiledCode);

            /* end */

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
                    std::cout << "AVM Bytecode file detected" << NL;

                    std::optional<std::string> src = System::File::ReadToString(file);
                    if (!src.has_value()) throw std::runtime_error(std::string("Failed to open source file."));

                    size_t i = find(args.begin(), args.end(), file) - args.begin();

                    std::vector<std::string> fargs(args.begin() + ++i, args.end());
                    avm::AVM avm; // The Virtual Machine (aka the Runtime)
                    amca::Analyzer an; // The Analyzer/Parser 
                    amcc::Compiler c; // The Compiler
                    avm::ByteCode bytecode = c.Compile(an.StartAnalysis(*src));
                    std::cout << NL;
                    avm.Start(bytecode, fargs);
                }
                else if (file.ends_with(".arm"))
                {
                    std::cout << "Armavir source file detected" << NL;

                    std::optional<std::string> src = System::File::ReadToString(file);
                    if (!src.has_value()) throw std::runtime_error(std::string("Failed to open source file."));

                    // ARMA (Armavir Analyzer) and ARBA (Armavi Bytecode Analyzer) 
                    arma::Analyzer an; 
                    an.StartAnalysis(*src);
                    an.DumpTruck();

                    arc::Compiler c;
                    avm::ByteCode compiledCode = c.Compile(an);

                    avm::AVM avm;
                    avm.Start(compiledCode);
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
