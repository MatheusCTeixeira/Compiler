#include <string>
#include <iostream>

#include "../header/lex_analyzer.hpp"
#include "../header/file_reader.hpp"
#include "../header/args.hpp"
#include "../header/syn_analyzer.hpp"


int main(int argc, char ** argv){

    // Analizador léxico
    comp::info arg = comp::process_params(argv, argc);
    comp::file_reader file{arg.m_filename};

    std::string source;
    while (!file.eof())
        source += file.next_line() + "\n";

    comp::lex_analyzer lex{source};

    std::cout << std::endl;

    lex.process();
    lex.analyzer();
    lex.optimize();
    
    if (arg.m_list_tokens)
        lex.list_token(); 
  
    // Analizador sintático
    comp::syn_analyzer syn{};        
    
    syn.process(lex.tokens());

    if (arg.m_log)
        syn.print_log();

    return 0;
}