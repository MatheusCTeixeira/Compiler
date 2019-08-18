#include "../header/lex_analyzer.hpp"

namespace comp{
    lex_analyzer::lex_analyzer(const std::string& source)
        :m_source{source}
    {       
        /* 
        this->m_end_token.push_back("&&");        this->m_end_token.push_back("||");        this->m_end_token.push_back("==");
        this->m_end_token.push_back("!=");        this->m_end_token.push_back(" ");         this->m_end_token.push_back("\n");
        this->m_end_token.push_back("=");         this->m_end_token.push_back("+");         this->m_end_token.push_back("-");
        this->m_end_token.push_back("*");         this->m_end_token.push_back("%");         this->m_end_token.push_back("/");
        this->m_end_token.push_back(";");         this->m_end_token.push_back("(");         this->m_end_token.push_back(")");
        this->m_end_token.push_back("{");         this->m_end_token.push_back("}");
        */

        this->m_operator.push_back("&&");  //0
        this->m_operator.push_back("||");
        this->m_operator.push_back("==");
        this->m_operator.push_back("!=");
        this->m_operator.push_back(";");
        this->m_operator.push_back("+");
        this->m_operator.push_back("-");
        this->m_operator.push_back("*");
        this->m_operator.push_back("/");
        this->m_operator.push_back("%");
        this->m_operator.push_back("{");
        this->m_operator.push_back("}");
        this->m_operator.push_back("(");
        this->m_operator.push_back(")");
        this->m_operator.push_back("=");    //13

        this->m_terminal.push_back("if");
        this->m_terminal.push_back("int");
        this->m_terminal.push_back("else");
        this->m_terminal.push_back("endif");
        this->m_terminal.push_back("while");
		this->m_terminal.push_back("endwhile");
        this->m_terminal.push_back("print");
        this->m_terminal.push_back("scan");
        
        this->m_non_terminal.push_back(std::pair<std::string, std::string>{"string", "^(\"[[:blank:][:print:]]*\")"});
        this->m_non_terminal.push_back(std::pair<std::string, std::string>{"literal", "^([[:digit:]]+)"});
        this->m_non_terminal.push_back(std::pair<std::string, std::string>{"comment", "^(#[[:print:]]*)"});
        this->m_non_terminal.push_back(std::pair<std::string, std::string>{"id", "^([_[:alpha:]]([[:alnum:]_])*)"});

        this->m_sugestion.push_back(std::pair<std::string, std::string>{"invalid id: begin with numbers", "([[:digit:]]+[_[:alpha:][:alnum:]]+)"});
        this->m_sugestion.push_back(std::pair<std::string, std::string>{"invalid caracter: you want &&", "(&)"});
        this->m_sugestion.push_back(std::pair<std::string, std::string>{"invalid caracter: you want !=", "(!)"});
        this->m_sugestion.push_back(std::pair<std::string, std::string>{"invalid caracter: you want ||", "(|)"});
        this->m_sugestion.push_back(std::pair<std::string, std::string>{"invalid : unrecognized ", "(.+)"});
    }

    void lex_analyzer::process(){
        int line = 1, col = 0;
        int cursor = 0;

        while (cursor < (int)m_source.size()){
            if (this->m_source[cursor] == ' ' || this->m_source[cursor] == '\t') {++col; ++cursor; continue;};
            if (this->m_source[cursor] == '\n'){ col = 0; ++line; ++cursor; continue;  } 

            if (int i = this->match_operator(cursor)){
                this->m_result.push_back(quaternion_result{this->m_source.substr(cursor, i), this->m_source.substr(cursor, i), line, col});
                cursor += i;
                col += i;  
                continue;              
            }

            if (int i = this->match_terminal(cursor)){
                this->m_result.push_back(quaternion_result{this->m_source.substr(cursor, i), this->m_source.substr(cursor, i), line, col});
                cursor += i;
                col += i;  
                continue;  
            }

            if (int i = this->match_non_terminal(cursor)){
                this->m_result.push_back(quaternion_result{std::string{}, this->m_source.substr(cursor, i), line, col});
                cursor += i;
                col += i;  
                continue; 
            }

            this->m_error.push_back(quaternion_error{this->m_source.substr(cursor, 1),std::vector<std::string>{}, line, col});
            ++cursor;   ++col;  
        }
    }

    void lex_analyzer::analyzer(){        
        for (int i =0; i < (int)this->m_result.size(); ++i)
            if (this->m_result[i].m_token.empty())
                for (auto reg_ex: this->m_non_terminal)
                    if (this->match_regex(this->m_result[i].m_lexeme, std::get<1>(reg_ex)))
                        this->m_result[i].m_token = std::get<0>(reg_ex);
                        
        for (int i =0; i < (int)this->m_result.size(); ++i)
            if (this->m_result[i].m_token.empty() || this->m_result[i].m_token.compare("error") == 0){
                this->m_error.push_back(quaternion_error{this->m_result[i].m_lexeme, std::vector<std::string>{}, this->m_result[i].m_line, this->m_result[i].m_column});                 
                this->m_result.erase(this->m_result.begin() + i, this->m_result.begin() + i + 1);
            }
    }

    /* 
     *      Otimiza a mensagem de erro:
     *          Identifica id invalida, operadores &=! incompletos
     */
    void lex_analyzer::optimize(){
        for (auto& error: this->m_error)
            for (auto sug: this->m_sugestion)
                if (std::regex_match(error.m_lexeme, std::regex{std::get<1>(sug)}))
                    error.m_sugestion.push_back(std::get<0>(sug));

        for (auto error: this->m_error)
        {
            std::cout << error.m_lexeme << " at: " << "["<< error.m_line << ", " << error.m_column << "]" << std::endl;
            for (int i = 0; i< (int)error.m_sugestion.size(); ++i)
                std::cout << "\t" << "[" << i << "]: " << error.m_sugestion[i] << std::endl;
            std::cout << std::endl;
        }
    }

    void lex_analyzer::list_token(){
        std::cout << std::setw(15) << "token" << " | " << std::setw(15) << "lexeme" << " | " << std::setw(7) << "#line" << " | " << std::setw(7) << "#column" << std::endl << std::endl; 
        for (auto token: this->m_result)
            std::cout << std::setw(15) << token.m_token << " | " << std::setw(15) << token.m_lexeme << " | " << std::setw(7) << token.m_line << " | " << std::setw(7) << token.m_column << std::endl;
    }

	bool lex_analyzer::has_error() const
	{
		return ((int)this->m_error.size() > 0);
	}

    std::vector<lex_analyzer::token_type> lex_analyzer::tokens(){
        std::vector<lex_analyzer::token_type> result;

        for (auto r:this->m_result)
            result.push_back(r);

        return result;
    }

    int lex_analyzer::match_operator(int pos) const{
        return this->match(pos, this->m_operator);  
    }

    int lex_analyzer::match_terminal(int pos) const {
        int ret = this->match(pos, this->m_terminal);
        
        // se o tamanho é zero não deu match
        if (!ret)
            return 0;
        
        // testa se não é um não-terminal que inicia como um terminal
        if (((int)this->m_source.size() > (ret + pos)) &&
            (std::string{this->m_source[ret + pos]}.find_first_of("\n {}!*()-+;=&|/%\0") == 0))
                return ret;

        return 0;
    }

    /* compara a string na posição 'pos' e verifica se ela esta no vetor de tokens 
     * se estiver retonar o tamanho do match
     * caso contrário, retorna zero
     */
    int lex_analyzer::match(int pos, const std::vector<std::string>& tokens) const {
        for (auto op: tokens)
            if (this->m_source.substr(pos, op.size()).compare(op) == 0)
                return op.size();

     return 0;  
    }

    int lex_analyzer::match_non_terminal(int pos) const {
        return this->m_source.find_first_of("\n {}!*()-+;=&|/%\0", pos) - pos;
    }

    bool lex_analyzer::match_regex(std::string input, std::string reg){
        return(std::regex_match(input, std::regex{reg}));
    }

    /*
    int lex_analyzer::match(std::string input, std::string set_point)const {
        if (set_point.substr(0, 2).compare("r:")){
            int count = 0;            
            for (count = 0; count < input.size() && count < set_point.size(); ++count)
                if (input[count] == set_point[count])
                    ++count;
            return((count == input.size() && count == set_point.size()) ? -1: count);
        } else {
            return(std::regex_match(input, std::regex{set_point.substr(2, set_point.size())})?-1:0);
        }
    }
    bool lex_analyzer::is_end_token(const std::string& token, int pos) const{
        int i = token.size() - pos;
        int count = 0;

        if (i <= 0)  return false;        
        if (i == 1)  count  = 4;

        for (;count < m_end_token.size(); ++count)
            if (token.compare(m_end_token) == 0)
                return true;

        return false;
    } 
    */   
}