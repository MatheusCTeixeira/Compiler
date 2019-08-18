#pragma once

#include <vector>
#include <stack>
#include <string>

#ifndef _EXPR_
#define _EXPR_

namespace func
{
	bool is_operator(std::string ev);

	bool is_operand(std::string ev);

	int get_priority(std::string op);

	std::vector<std::string> infix_to_prefix(std::vector<std::string> expr);	
	std::vector<std::vector<std::string>> parse_to_intermediate_code(std::vector<std::string> expr);
	
}

#endif