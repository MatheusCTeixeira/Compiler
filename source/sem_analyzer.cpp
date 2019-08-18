#include "../header/sem_analyzer.hpp"

namespace comp
{
	sem_analyzer::sem_analyzer()
		: m_log{}, m_var_table{}
	{
	}

	sem_analyzer::~sem_analyzer()
	{
	}

	bool sem_analyzer::has_error() const
	{
		return this->m_has_error;
	}

	void sem_analyzer::process(std::vector<lex_analyzer::token_type> ttokens)
	{
		std::vector<lex_analyzer::token_type> tokens{ ttokens };

		for (int i = 0; i < (int)tokens.size(); ++i)
		{
			// Verify if is a declaration or a reference
			if (tokens[i].m_token.compare("id") == 0)
			{
				//Verify for declaration
				if (tokens[i - 1].m_token.compare("int") == 0)
				{
					this->process_declaration(tokens[i]);
				}
				// Is a reference
				else
				{
					this->process_reference(tokens[i]);
				}

				if (tokens[i + 1].m_token.compare("=") == 0)
				{
					std::vector<std::string> expr;

					for (int j = i + 2; tokens[j].m_token.compare(";") != 0; ++j)
					{
						if (tokens[j].m_token.compare("id") != 0)
							expr.push_back(tokens[j].m_lexeme);
						else
							goto end;
					}
					{

						comp::util notation{};

						if (notation.has_div_by_zero(notation.infix_to_prefix(expr)))
						{
							this->process_div_by_zero(tokens[i]);
						}
					}
				end:;
				}
			}
			else if (tokens[i].m_token.compare("{") == 0)
			{
				this->begin_scope(tokens[i]);
			}
			else if (tokens[i].m_token.compare("}") == 0)
			{
				this->end_scope(tokens[i]);
			}
		}
	}

	void sem_analyzer::process_declaration(lex_analyzer::token_type token)
	{
		switch (this->already_declared(token.m_lexeme))
		{
		case DECLARATION_TYPE::DECLARED_IN_SCOPE:
			this->process_declaration_in_scope(token);
			break;

		case DECLARATION_TYPE::DECLARED_OUTER_SCOPE:
			this->process_declaration_outer_scope(token);
			break;

		case DECLARATION_TYPE::NOT_DECLARED:
			this->process_no_declared(token);
			break;

		default:

			;
		}
	}

	DECLARATION_TYPE sem_analyzer::already_declared(std::string var_name) const
	{
		for (auto var : this->m_var_table)
		{
			if (var.first.compare(var_name) == 0)
			{
				if (var.second.top().compare("<::>") == 0)
					return DECLARATION_TYPE::DECLARED_OUTER_SCOPE;
				else
					return DECLARATION_TYPE::DECLARED_IN_SCOPE;
			}
		}

		return DECLARATION_TYPE::NOT_DECLARED;
	}

	void sem_analyzer::process_declaration_in_scope(lex_analyzer::token_type token)
	{
		std::string msg_log{ "attempt to re-declare: \"" + token.m_lexeme + "\"" +
							" [" + std::to_string(token.m_line) +
							", " + std::to_string(token.m_column) + "]" };

		std::cout << "error:" << std::endl
			<< "\t" << msg_log << std::endl;

		this->m_has_error = true;
		this->m_log.push_back(msg_log);
	}

	void sem_analyzer::process_declaration_outer_scope(lex_analyzer::token_type token)
	{
		std::string msg_log{ "var: \"" + token.m_lexeme + "\" [" + std::to_string(token.m_line) +
							", " + std::to_string(token.m_column) + "]" };

		auto &t_stack = this->m_var_table[token.m_lexeme];
		t_stack.push(token.m_lexeme + std::to_string(t_stack.size()));

		this->m_log.push_back(msg_log);
	}

	void sem_analyzer::process_no_declared(lex_analyzer::token_type token)
	{
		std::string msg_log{ "var: \"" + token.m_lexeme + "\" [" + std::to_string(token.m_line) +
							", " + std::to_string(token.m_column) + "]" };

		std::stack<std::string> t_stack{};
		t_stack.push({ token.m_lexeme + std::to_string(t_stack.size()) });

		this->m_var_table.insert(std::pair<std::string, std::stack<std::string>>{token.m_lexeme, t_stack});

		this->m_log.push_back(msg_log);
	}

	void sem_analyzer::begin_scope(lex_analyzer::token_type token)
	{
		std::string msg_log{ "new scope [" + std::to_string(token.m_line) + ", " +
							std::to_string(token.m_column) + "]" };

		/* Eu sou um animal! placeholder é uma cópia */
		for (auto &var : this->m_var_table)
		{
			var.second.push("<::>");
		}

		this->m_log.push_back(msg_log);
	}

	void sem_analyzer::end_scope(lex_analyzer::token_type token)
	{
		std::string msg_log{ "end scope [" + std::to_string(token.m_line) + ", " +
							std::to_string(token.m_column) + "]" };

		for (auto &var : this->m_var_table)
		{
			if (!var.second.empty() && var.second.top().compare("<::>")) //Vai remover se houver uma variavel nesse escopo
				var.second.pop();

			if (!var.second.empty())
				var.second.pop(); // remove o escopo
		}
		this->m_log.push_back(msg_log);
	}

	void sem_analyzer::print_log() const
	{
		std::cout << "Semantic log:" << std::endl << std::endl;
		for (auto log : this->m_log)
		{
			std::cout << log << std::endl;
		}
	}

	void sem_analyzer::process_reference(lex_analyzer::token_type token)
	{
		std::stack<std::string> stk{ this->m_var_table[token.m_lexeme] };

		while (!stk.empty() && stk.top().compare("<::>") == 0)
			stk.pop();

		if (this->m_var_table.end() == this->m_var_table.find(token.m_lexeme) || stk.empty())
		{
			this->process_reference_invalid(token);
		}
		else
		{
			this->process_reference_valid(token);
		}
	}

	void sem_analyzer::process_reference_valid(lex_analyzer::token_type token)
	{
		std::stack<std::string> stk{ this->m_var_table[token.m_lexeme] };

		while (!stk.empty() && stk.top().compare("<::>") == 0)
			stk.pop();

		std::string message{ "var: \"" + stk.top() + "\" referenced at[" +
							std::to_string(token.m_line) + ", " +
							std::to_string(token.m_column) + "]" };

		this->m_log.push_back(message);
	}

	void sem_analyzer::process_reference_invalid(lex_analyzer::token_type token)
	{
		std::string message{ "var: \"" + token.m_lexeme + "\" not declared at[" +
							std::to_string(token.m_line) + ", " +
							std::to_string(token.m_column) + "]" };

		std::cout << "error:\n\t" << message << std::endl;

		this->m_has_error = true;
		this->m_log.push_back(message);
	}

	void sem_analyzer::process_div_by_zero(lex_analyzer::token_type token)
	{
		std::string msg_log{ "attempt to div by zero in initialization/assign: \"" + token.m_lexeme + "\"" +
							" [" + std::to_string(token.m_line) +
							", " + std::to_string(token.m_column) + "]" };

		std::cout << "error:" << std::endl
			<< "\t" << msg_log << std::endl;

		this->m_has_error = true;
		this->m_log.push_back(msg_log);
	}
}