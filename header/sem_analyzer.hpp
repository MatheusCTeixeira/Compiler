#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <stack>
#include <unordered_map>

#include "lex_analyzer.hpp"
#include "infix_to_prefix.hpp"

namespace comp
{
    struct var_table
    {
        std::string m_name;
        int m_value;
        int m_scope;
    };

    enum class DECLARATION_TYPE{ NOT_DECLARED = 0, DECLARED_OUTER_SCOPE, DECLARED_IN_SCOPE};

    class sem_analyzer
    {
    public:
        sem_analyzer();
        ~sem_analyzer();

		bool has_error() const;

        void process(std::vector<lex_analyzer::token_type> tokens);
        void print_log() const;


    protected:
    private:
        DECLARATION_TYPE already_declared(std::string var_name) const;
        void begin_scope(lex_analyzer::token_type token);
        void end_scope(lex_analyzer::token_type token);

        void process_declaration(lex_analyzer::token_type token); /* Switch beetween process_(re)declaration_(*) */
        void process_declaration_in_scope(lex_analyzer::token_type token);
        void process_declaration_outer_scope(lex_analyzer::token_type token);
        void process_no_declared(lex_analyzer::token_type token);

        void process_reference(lex_analyzer::token_type token);
        void process_reference_valid(lex_analyzer::token_type token);
        void process_reference_invalid(lex_analyzer::token_type token);

        void process_div_by_zero(lex_analyzer::token_type token);

        std::vector<std::string> m_log;
        std::unordered_map<std::string, std::stack<std::string>> m_var_table;

		bool m_has_error = false;
        
    };
}