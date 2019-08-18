#include <string>
#include <iostream>

#include "../header/args.hpp"
#include "../header/file_reader.hpp"
#include "../header/final_code.hpp"
#include "../header/intermediate_code.hpp"
#include "../header/lex_analyzer.hpp"
#include "../header/syn_analyzer.hpp"
#include "../header/sem_analyzer.hpp"

int main(int argc, char **argv)
{

    // Analisador léxico
    comp::info arg = comp::process_params(argv, argc);
    comp::file_reader file{arg.m_filename};
    bool has_error = false;

    std::string source;

    while (!file.eof())
        source += file.next_line() + "\n";

    comp::lex_analyzer lex{source};

    std::cout << std::endl;
    std::cout << "-----------------------lexical analyzer [begin]-----------------------" << std::endl;
    lex.process();
    lex.analyzer();
    lex.optimize();
    std::cout << "-----------------------lexical analyzer [end]-----------------------" << std::endl;
    has_error = has_error || lex.has_error();

    if (arg.m_list_tokens)
        lex.list_token();

    // Analisador sintático
    comp::syn_analyzer syn{};

    std::cout << "-----------------------syntatical analyzer [begin]-----------------------" << std::endl;
    syn.process(lex.tokens());
    std::cout << "-----------------------syntatical analyzer [end]-----------------------" << std::endl;

    has_error = has_error || syn.has_error();

    if (arg.m_syn_log)
        syn.print_log();

    //  Analisador semântico
    comp::sem_analyzer sem{};

    std::cout << "-----------------------semantic analyzer [begin]-----------------------" << std::endl;
    sem.process(lex.tokens());
    std::cout << "-----------------------semantic analyzer [end]-----------------------" << std::endl;

    has_error = has_error || sem.has_error();

    if (arg.m_sem_log)
        sem.print_log();

    if (has_error)
    {
        std::cout << "-----------------------build stoped-----------------------" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "-----------------------build initialized-----------------------" << std::endl;
    }

    comp::intermediate_code inc{};

    inc.process(lex.tokens());

    if (arg.m_cod_log)
    {
        std::cout << "Intermediate code:" << std::endl
                  << std::endl;
        inc.print_code();
    }

    std::cout << "-------------------------------------------------------------------" << std::endl;   

    comp::final_code fnc{};
    fnc.process(inc.list_vars(), inc.list_string_vars(), inc.code());

    if (arg.m_cod_log)
    {
        std::cout << "Final code:" << std::endl
                  << std::endl;
        fnc.print_final_code();
    }

    
    fnc.output_to_file(arg.m_outputfilename);

     std::cout << "-----------------------build finished-----------------------" << std::endl;

    
    return 0;
}