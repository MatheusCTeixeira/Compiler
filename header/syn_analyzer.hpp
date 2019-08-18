#include <iostream>
#include <iomanip>
#include <queue>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "lex_analyzer.hpp"
#include "file_reader.hpp"
#include "file_writer.hpp"

#ifndef _SYN_ANALYZER_
#define _SYN_ANALYZER_

namespace comp{
    class syn_analyzer{
    public:
        syn_analyzer();

        void process(std::vector<lex_analyzer::token_type> tokens);
        void print_log();
		bool has_error() const;
        ~syn_analyzer();

    protected:
        void read_production();
        void read_map();

        bool is_not_terminal(std::string expr) const;

    private:

        std::queue<std::pair<std::string, std::string>> m_log;
        std::stack<std::string> m_stack;
        std::vector<std::vector<std::string>> m_production;// non-terminal, terminal or non-terminal ... --> <non-terminal> ::= <terminal or non-terminal>... 
        std::unordered_map<std::string, std::unordered_map<std::string, int>> m_map;//nonterminal, terminal

		bool m_has_error = false;
    };
}

#endif