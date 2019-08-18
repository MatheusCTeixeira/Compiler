#include "../header/expr.hpp"

bool func::is_operator(std::string ev)
{
	for (auto op : { "+", "-", "/", "*", "%", "==", "!=", "&&", "||", "=" })
		if (ev.compare(op) == 0)
			return true;

	return false;
}

bool func::is_operand(std::string ev)
{
	return (!is_operator(ev) && (ev.compare("(") != 0 && ev.compare(")") != 0));
}

int func::get_priority(std::string op)
{
	if (op.compare("(") == 0)
		return 0;

	if (op.compare("=") == 0)
		return 1;

	if (op.compare("&&") == 0 || op.compare("||") == 0)
		return 2;

	if (op.compare("==") == 0 || op.compare("!=") == 0)
		return 3;

	if (op.compare("+") == 0 || op.compare("-") == 0)
		return 4;

	if (op.compare("*") == 0 || op.compare("/") == 0 || op.compare("%") == 0)
		return 5;

	return (-1);
}

std::vector<std::string> func::infix_to_prefix(std::vector<std::string> expr)
{
	std::vector<std::string> output{};
	std::stack<std::string> stack_op{};

	for (auto elem : expr)
	{
		if (is_operand(elem))
			output.push_back(elem);
		else if (elem.compare("(") == 0)
			stack_op.push(elem);
		else if (elem.compare(")") == 0)
		{
			while (stack_op.top().compare("(") != 0)
			{
				output.push_back(stack_op.top());
				stack_op.pop();
			}

			stack_op.pop();
		}

		else if (is_operator(elem))
		{
			while (!stack_op.empty() && (get_priority(elem) < get_priority(stack_op.top())))
			{
				output.push_back(stack_op.top());
				stack_op.pop();
			}

			stack_op.push(elem);
		}
	}

	while (!stack_op.empty())
	{
		output.push_back(stack_op.top());
		stack_op.pop();
	}

	return output;
}

std::vector<std::vector<std::string>> func::parse_to_intermediate_code(std::vector<std::string> expr)
{
	std::vector<std::vector<std::string>> output{};
	std::stack<std::string> stack_temp{};

	static int count = 0;

	for (int i = 0; i < static_cast<int>(expr.size()); ++i)
	{
		stack_temp.push(expr[i]);

		if (is_operator(stack_temp.top()) && stack_temp.size() >= 3)
		{
			std::string operator_ = stack_temp.top(); stack_temp.pop();
			std::string operand_2 = stack_temp.top(); stack_temp.pop();
			std::string operand_1 = stack_temp.top(); stack_temp.pop();

			std::vector<std::string> code{};	
			code.push_back("INSTRUCTION");
			code.push_back(operator_);
			code.push_back(operand_1);
			code.push_back(operand_2);

			stack_temp.push("STK");
			output.push_back(code);

			++count;
		}
	}

	return output;
}
