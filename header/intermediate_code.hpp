#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include <unordered_map>
#include "../header/lex_analyzer.hpp"

#include "../header/expr.hpp"

#ifndef _INTERMEDIATE_CODE_
#define _INTERMEDIATE_CODE_
namespace comp
{
	class intermediate_code
	{
	public:
		void process(std::vector<comp::lex_analyzer::token_type> token_table);
		void print_tokens() const;
		std::vector<std::vector<std::string>> code() const;
		std::vector<std::string> list_vars() const;
		std::vector<std::pair<std::string, std::string>> list_string_vars() const;
		void print_code() const;
	protected:

	private:
		/*
		 * Está função padroniza todos os statements da linguagem para que o tratamento
		 * de cada um deles siga um template e também para facilitar nested statements 
		 * como o caso do if's e while's.		 		 
		 */
		void standardize_statements();
		void standardize_if_stmt();
		void standardize_while_stmt();

		/*
		* Após passar pela análise semântica todos as variáveis são únicas em seus respectivos
		* escopos. Portanto, sempre que achar o nome de uma variável é possível gerar um novo nome.
		* O que simplifica o processo de declaração de variaveis
		* 		
		*/
		void link_and_declare_vars();		
		void remove_declarations();
		void remove_curly_brackets();
		void remove_tokens(std::stack<std::pair<int, int>> stack_post_to_remove);

		/*
		 *
		 *
		 */
		void generate_intermediate_code();

		void process_statement();

		void declare_variables();

		/*
			Começa a processar a partir de at e retornam até onde avançaram 
		*/
		int process_if(int at);
		int process_else(int at);
		int process_endif(int at);
		int process_while(int at);
		int process_endwhile(int at);
		int process_print(int at);
		int process_scan(int at);
		int process_expression(int at);



		std::vector<std::string> extract_condition(int begin);
		std::vector<std::string> extract_expr(int begin);

		std::vector<comp::lex_analyzer::token_type> m_processed_token_table;
		std::vector<std::string> m_list_vars;
		std::vector<std::pair<std::string, std::string>> m_list_string_vars;
		std::vector<std::vector<std::string>> m_code;
	};
}
#endif
