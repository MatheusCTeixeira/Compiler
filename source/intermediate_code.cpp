#include "../header/intermediate_code.hpp"

void comp::intermediate_code::process(std::vector<comp::lex_analyzer::token_type> token_table)
{
	this->m_processed_token_table = token_table;

	this->standardize_statements();
	this->link_and_declare_vars();
	this->remove_declarations();
	this->remove_curly_brackets();
	this->generate_intermediate_code();

}

void comp::intermediate_code::print_tokens() const
{
	for (auto row : this->m_processed_token_table)
	{
		//if (row.m_token.compare("if") == 0 || row.m_token.compare("else") == 0 || row.m_token.compare("endif") == 0 ||
		//	row.m_token.compare("while") == 0 || row.m_token.compare("endwhile") == 0 || row.m_token.compare("id") == 0)
			std::cout << row.m_token << std::setw(20) << row.m_lexeme << std::endl;
	}
}

std::vector<std::vector<std::string>> comp::intermediate_code::code() const
{
	return this->m_code;
}

std::vector<std::string> comp::intermediate_code::list_vars() const
{
	return this->m_list_vars;
}

std::vector<std::pair<std::string, std::string>> comp::intermediate_code::list_string_vars() const
{
	return this->m_list_string_vars;
}

void comp::intermediate_code::print_code() const
{
	for (auto row : this->m_code)
	{
		for (auto column : row)
			std::cout << std::setw(15) << column;
		std::cout << std::endl;
	}
}

void comp::intermediate_code::standardize_statements()
{
	this->standardize_if_stmt();
	this->standardize_while_stmt();

}

void comp::intermediate_code::standardize_if_stmt()
{
	std::stack<int> stack_token_if{};
	std::stack<bool> stack_has_else{};
	std::stack<std::pair<int, comp::lex_analyzer::token_type>> stack_post_to_update{};

	int sz = this->m_processed_token_table.size();
	int id_if = 0;

	for (int i = 0; i < sz; ++i)
	{

		if (this->m_processed_token_table[i].m_token.compare("if") == 0)
		{
			stack_token_if.push(id_if);
			this->m_processed_token_table[i].m_lexeme += (std::string{ "_" } +std::to_string(stack_token_if.top()));
			stack_has_else.push(false);
			++id_if;
		}
		else if (this->m_processed_token_table[i].m_token.compare("else") == 0)
		{
			this->m_processed_token_table[i].m_lexeme += (std::string{ "_" } +std::to_string(stack_token_if.top()));
			stack_has_else.pop();
			stack_has_else.push(true);
		}
		else if (this->m_processed_token_table[i].m_token.compare("endif") == 0)
		{
			if (!stack_has_else.top())
			{
				comp::lex_analyzer::token_type else_stmt{ "else", "else" };
				else_stmt.m_lexeme += (std::string{ "_" } +std::to_string(stack_token_if.top()));

				stack_post_to_update.push({ i , else_stmt });

			}

			this->m_processed_token_table[i].m_lexeme += (std::string{ "_" } +std::to_string(stack_token_if.top()));
			stack_has_else.pop();
			stack_token_if.pop();//remove if
		}


	}

	while (!stack_post_to_update.empty())
	{
		auto par = stack_post_to_update.top();
		this->m_processed_token_table.insert(this->m_processed_token_table.begin() + par.first, par.second);
		stack_post_to_update.pop();
	}

}

void comp::intermediate_code::standardize_while_stmt()
{
	std::stack<int> stack_token_while{};
	int id_while = 0;

	for (int i = 0; i < static_cast<int>(this->m_processed_token_table.size()); ++i)
	{
		if (this->m_processed_token_table[i].m_token.compare("while") == 0)
		{
			stack_token_while.push(id_while);
			this->m_processed_token_table[i].m_lexeme += (std::string{ "_" } +std::to_string(stack_token_while.top()));
			++id_while;
		}
		else if (this->m_processed_token_table[i].m_token.compare("endwhile") == 0)
		{
			this->m_processed_token_table[i].m_lexeme += (std::string{ "_" } +std::to_string(stack_token_while.top()));
			stack_token_while.pop();//remove while
		}
	}
}

void comp::intermediate_code::link_and_declare_vars()
{
	std::unordered_map<std::string, std::vector<std::string>> list_vars{};
	int var_id = 0;

	for (int i = 0; i < static_cast<int>(this->m_processed_token_table.size()); ++i)
	{
		if (this->m_processed_token_table[i].m_token.compare("{") == 0)
		{
			for (auto& vec : list_vars)
			{
				vec.second.push_back("{");
			}
		}
		else if (this->m_processed_token_table[i].m_token.compare("}") == 0)
		{
			for (auto& vec : list_vars)
			{
				while (!vec.second.empty() && vec.second.back().compare("{") != 0)
				{
					vec.second.pop_back();
				}

				if (!vec.second.empty())
					vec.second.pop_back();
			}
		}
		else if (this->m_processed_token_table[i].m_token.compare("id") == 0)
		{
			//declara��o
			if (this->m_processed_token_table[i - 1].m_token.compare("int") == 0)
			{
				std::string var_name = std::string{ "_" + this->m_processed_token_table[i].m_lexeme + "_" + std::to_string(var_id) };

				if (list_vars.find(this->m_processed_token_table[i].m_lexeme) == list_vars.end())
				{
					list_vars.insert({ this->m_processed_token_table[i].m_lexeme ,
						std::vector<std::string>{} }
					);
				}

				list_vars.at(this->m_processed_token_table[i].m_lexeme).push_back(var_name);

				this->m_processed_token_table[i].m_lexeme = var_name;
				this->m_list_vars.push_back(var_name);

				++var_id;
			}
			else
			{
				auto vec = list_vars.at(this->m_processed_token_table[i].m_lexeme);
				for (int k = vec.size() - 1; k >= 0; --k)
				{
					if (vec[k].compare("{") != 0)
					{
						this->m_processed_token_table[i].m_lexeme = vec[k];

						break;
					}
				}
			}
		}
		else if (this->m_processed_token_table[i].m_token.compare("string") == 0)
		{
			std::string name = "str_" + std::to_string(this->m_list_string_vars.size());
			this->m_list_string_vars.push_back({ name, this->m_processed_token_table[i].m_lexeme });
			this->m_processed_token_table[i].m_lexeme = name;
		}
	}
}

void comp::intermediate_code::remove_declarations()
{
	std::stack<std::pair<int, int>> stack_post_to_remove{};

	for (int i = 0; i < static_cast<int>(this->m_processed_token_table.size()); ++i)
	{
		if (this->m_processed_token_table[i].m_token.compare("id") == 0 &&
			this->m_processed_token_table[i - 1].m_token.compare("int") == 0)
		{
			int begin = i - 1, end = i;

			if (this->m_processed_token_table[i + 1].m_token.compare(";") == 0)
				end = i + 1;

			stack_post_to_remove.push({ begin, end });
		}
	}

	this->remove_tokens(stack_post_to_remove);
	
}

void comp::intermediate_code::remove_curly_brackets()
{
	std::stack<std::pair<int, int>> stack_post_to_remove{};

	for (int i = 0; i < static_cast<int>(this->m_processed_token_table.size()); ++i)
	{
		if (this->m_processed_token_table[i].m_token.compare("{") == 0 ||
			this->m_processed_token_table[i].m_token.compare("}") == 0)
		{
			stack_post_to_remove.push({ i, i + 1 });
		}
	}

	this->remove_tokens(stack_post_to_remove);
}

void comp::intermediate_code::remove_tokens(std::stack<std::pair<int, int>> stack_post_to_remove)
{
	while (!stack_post_to_remove.empty())
	{
		int begin = stack_post_to_remove.top().first;
		int end = stack_post_to_remove.top().second;

		this->m_processed_token_table.erase(
			this->m_processed_token_table.begin() + begin,
			this->m_processed_token_table.begin() + end
		);

		stack_post_to_remove.pop();
	}
}

void comp::intermediate_code::generate_intermediate_code()
{
	/*
	std::vector<std::string> declarations;
	declarations.push_back("VARIABLES");
	this->m_code.push_back(declarations);
	declare_variables();

	std::vector<std::string> empty;	
	this->m_code.push_back(empty);
	
	std::vector<std::string> code;
	code.push_back("CODE");
	this->m_code.push_back(code);
	*/
	process_statement();
}

void comp::intermediate_code::process_statement()
{
	int i = 0;
	while (i < static_cast<int>(this->m_processed_token_table.size()))
	{
		if (this->m_processed_token_table[i].m_token.compare("if") == 0)
		{
			i += this->process_if(i);
		}
		else if (this->m_processed_token_table[i].m_token.compare("else") == 0)
		{
			i += this->process_else(i);
		}
		else if (this->m_processed_token_table[i].m_token.compare("endif") == 0)
		{
			i += this->process_endif(i);
		}
		else if (this->m_processed_token_table[i].m_token.compare("while") == 0)
		{
			i += this->process_while(i);
		}
		else if (this->m_processed_token_table[i].m_token.compare("endwhile") == 0)
		{
			i += this->process_endwhile(i);
		}
		else if (this->m_processed_token_table[i].m_token.compare("scan") == 0)
		{
			i += this->process_scan(i);
		}
		else if (this->m_processed_token_table[i].m_token.compare("print") == 0)
		{
			i += this->process_print(i);
		}
		else
		{
			i += this->process_expression(i);
		}
	}
}

void comp::intermediate_code::declare_variables()
{
	for (auto var : this->m_list_vars)
	{
		std::vector<std::string> variable;
		variable.push_back(var);
		variable.push_back("DD");
		variable.push_back("0");

		this->m_code.push_back(variable);
	}

	for (auto var : this->m_list_string_vars)
	{
		std::vector<std::string> variable;
		variable.push_back(var.first);
		variable.push_back("DD");
		variable.push_back("	" + var.second + ", 0xA, 0x0" );

		this->m_code.push_back(variable);
	}
}

int comp::intermediate_code::process_if(int at)
{
	//apenas para identifica��o do 'if'
	std::vector<std::string> label_if;
	label_if.push_back("label");
	label_if.push_back(this->m_processed_token_table[at].m_lexeme);
	this->m_code.push_back(label_if);

	//condi��o
	auto condition = this->extract_condition(at + 1);
	auto infix_expression = func::infix_to_prefix(condition);
	auto parse_expression = func::parse_to_intermediate_code(infix_expression);

	for (auto instruction : parse_expression)
		this->m_code.push_back(instruction);

	//salto para o 'else' caso a condi��o seja falsa
	std::vector<std::string> label_else;
	label_else.push_back("EVAL");
	std::string else_str = this->m_processed_token_table[at].m_lexeme;
	else_str.replace(0, else_str.find("_"), "else");
	label_else.push_back(else_str);
	this->m_code.push_back(label_else);
		
	return 1 + condition.size() ;
	//     if  ...  -> +1 +1 + size +1
}

int comp::intermediate_code::process_else(int at)
{
	std::vector<std::string> endif_label;
	endif_label.push_back("JMP");
	std::string endif_str = this->m_processed_token_table[at].m_lexeme;
	endif_str.replace(0, endif_str.find("_"), "endif");
	endif_label.push_back(endif_str);
	this->m_code.push_back(endif_label);

	std::vector<std::string> else_label;
	else_label.push_back("label");
	else_label.push_back(this->m_processed_token_table[at].m_lexeme);
	this->m_code.push_back(else_label);
	return 1;
}

int comp::intermediate_code::process_endif(int at)
{
	std::vector<std::string> endif_label;
	endif_label.push_back("label");
	endif_label.push_back(this->m_processed_token_table[at].m_lexeme);
	this->m_code.push_back(endif_label);

	return 1;
}

int comp::intermediate_code::process_while(int at)
{
	std::vector<std::string> while_label;
	while_label.push_back("label");
	while_label.push_back(this->m_processed_token_table[at].m_lexeme);
	this->m_code.push_back(while_label);

	//condition
	auto condition = this->extract_condition(at + 1);
	auto infix_expression = func::infix_to_prefix(condition);
	auto parse_expression = func::parse_to_intermediate_code(infix_expression);

	for (auto instruction : parse_expression)
		this->m_code.push_back(instruction);

	std::vector<std::string> endwhile_label;
	endwhile_label.push_back("EVAL");
	std::string endwhile_str = this->m_processed_token_table[at].m_lexeme;
	endwhile_str.replace(0, endwhile_str.find("_"), "endwhile");
	endwhile_label.push_back(endwhile_str);
	this->m_code.push_back(endwhile_label);
	return 1 + condition.size();
}

int comp::intermediate_code::process_endwhile(int at)
{	
	std::vector<std::string> while_label;
	while_label.push_back("JMP");
	std::string while_str = this->m_processed_token_table[at].m_lexeme;
	while_str.replace(0, while_str.find("_"), "while");
	while_label.push_back(while_str);
	this->m_code.push_back(while_label);

	std::vector<std::string> endwhile_label;
	endwhile_label.push_back("label");
	endwhile_label.push_back(this->m_processed_token_table[at].m_lexeme);
	this->m_code.push_back(endwhile_label);

	return 1;
}

int comp::intermediate_code::process_print(int at)
{		
	if (this->m_processed_token_table[at + 2].m_token.compare("string") == 0)	
	{
		std::vector<std::string> push_argf;
		push_argf.push_back("PUSH"); push_argf.push_back("print_string");
		this->m_code.push_back(push_argf);

		std::vector<std::string> push_argv;
		push_argv.push_back("PUSH");
		push_argv.push_back(this->m_processed_token_table[at + 2].m_lexeme);
		this->m_code.push_back(push_argv);
	}
	else
	{		
		std::vector<std::string> push_argv;
		push_argv.push_back("PUSH");
		push_argv.push_back("dword [" + this->m_processed_token_table[at + 2].m_lexeme + "]");
		// push_argv.push_back("[");push_argv.push_back(this->m_processed_token_table[at + 2].m_lexeme);push_argv.push_back("]");
		this->m_code.push_back(push_argv);

		std::vector<std::string> push_argf;
		push_argf.push_back("PUSH"); push_argf.push_back("print_int");
		this->m_code.push_back(push_argf);
	}
	
	std::vector<std::string> call_print;
	call_print.push_back("CALL");
	call_print.push_back("printf");
	this->m_code.push_back(call_print);

	std::vector<std::string> pop_args;
	pop_args.push_back("POP"); pop_args.push_back("EAX");
	this->m_code.push_back(pop_args);
	this->m_code.push_back(pop_args);

	return 5;	
}

int comp::intermediate_code::process_scan(int at)
{	
	std::vector<std::string> push_argv;
	push_argv.push_back("PUSH");
	push_argv.push_back(this->m_processed_token_table[at + 2].m_lexeme);
	this->m_code.push_back(push_argv);
	
	std::vector<std::string> push_argf;
	push_argf.push_back("PUSH"); push_argf.push_back("scan_int");
	this->m_code.push_back(push_argf);

	std::vector<std::string> call_scan;
	call_scan.push_back("CALL");
	call_scan.push_back("scanf");
	this->m_code.push_back(call_scan);

	std::vector<std::string> pop_args;
	pop_args.push_back("POP"); pop_args.push_back("EAX");
	this->m_code.push_back(pop_args);
	this->m_code.push_back(pop_args);

	return 5;
	
}

int comp::intermediate_code::process_expression(int at)
{
	auto expression = this->extract_expr(at);
	auto infix_expression = func::infix_to_prefix(expression);
	auto parse_expression = func::parse_to_intermediate_code(infix_expression);

	for (auto instruction : parse_expression)
		this->m_code.push_back(instruction);

	return expression.size() + 1;
}


//arrumar para extrair express�o
std::vector<std::string> comp::intermediate_code::extract_condition(int begin)
{
	std::vector<std::string> result;
	std::stack<int> expr{};	

	for (int i = begin; i < static_cast<int>(this->m_processed_token_table.size()); ++i)
	{
		result.push_back(this->m_processed_token_table[i].m_lexeme);
		
		if (this->m_processed_token_table[i].m_token.compare("(") == 0) 
			expr.push(i);
		else if (this->m_processed_token_table[i].m_token.compare(")") == 0) 
			expr.pop();

		if (expr.empty())
			break;

	}

	return result;
}

std::vector<std::string> comp::intermediate_code::extract_expr(int begin)
{
	std::vector<std::string> result{};

	for (int i = begin; i < static_cast<int>(this->m_processed_token_table.size()) && 
		this->m_processed_token_table[i].m_token.compare(";") != 0; ++i)
	{		
		result.push_back(this->m_processed_token_table[i].m_lexeme);
	}

	return result;
}
