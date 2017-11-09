#include <string>
#include <regex>
#include <vector>
#include <utility>  
#include <iostream>
#include <iomanip>

#ifndef _LEX_ANALYZER_
#define _LEX_ANALYZER_

namespace comp{
    class lex_analyzer{
        struct quaternion_result{
            std::string m_token;
            std::string m_lexeme;
            int m_line;
            int m_column;
        };

        struct quaternion_error{
            std::string m_lexeme;
            std::vector<std::string> m_sugestion;
            int m_line;
            int m_column;
        };

    public:
        lex_analyzer(const std::string& source );

        void process();
        void analyzer();
        void optimize();
        void list_token();
        std::vector<std::string>&& tokens();

    private:
        /*
         * result >  0 -> indica o número de matchs
         * result =  0 -> não houve nem um caractere q deu match
         * result = -1 -> perfect match
        */
        //int match(std::string input, std::string set_point) const;
        //int is_end_token(const std::string& token, int pos) const;  
        
        int match_operator(int pos) const;
        int match_terminal(int pos) const;
        int match(int pos, const std::vector<std::string>& tokens) const;
        int match_non_terminal(int pos) const;
        bool match_regex(std::string input, std::string reg);
        

        //std::vector<std::string> m_end_token;
        std::vector<std::string> m_operator;
        std::vector<std::string> m_terminal;
        std::vector<std::pair<std::string, std::string>> m_non_terminal;

        std::vector<quaternion_result> m_result;
        std::vector<quaternion_error> m_error;
        std::vector<std::pair<std::string, std::string>> m_sugestion;

        std::string m_source;
    };

    
}

#endif