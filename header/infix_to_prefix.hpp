#include <stack>
#include <string>
#include <vector>
#include <iostream>

#ifndef _POLONESE_NOTATION_
#define _POLONESE_NOTATION_

namespace comp {

	struct util {

		int get_priority(std::string op)
		{
			if (op.compare("=") == 0)
				return 0;
			
			if (op.compare("!=") == 0 || op.compare("==") == 0)
				return 1;

			if (op.compare("&&") == 0 || op.compare("||") == 0)
				return 2;

			if (op.compare("(") == 0)
				return 3;

			if (op.compare("+") == 0 || op.compare("-") == 0)
				return 4;

			if (op.compare("*") == 0 || op.compare("/") == 0)
				return 5;

			return -1;
		}

		bool has_greater_or_equal_priority(std::string op_l, std::string op_r)
		{
			return (get_priority(op_l) >= get_priority(op_r));
		}

		bool is_operator(std::string op)
		{
			return ((op.compare("+") == 0) ||
					(op.compare("-") == 0) ||
					(op.compare("*") == 0) ||
					(op.compare("/") == 0));
		}

		bool is_num(std::string op)
		{
			if ((op[0] == '-' || op[0] == '+') && (op.size() > 1))
				op.erase(op.begin(), op.begin() + 1);

			return (!is_operator(op) && (op.compare("(") != 0 && op.compare(")") != 0));

			/*
			std::locale loc;
			for (auto element : op)
				if (!std::isdigit(element, loc))
					return false;

			return true;
			*/
		}

		std::vector<std::string> infix_to_prefix(std::vector<std::string> infix)
		{
			std::vector<std::string> result{};
			std::stack<std::string> parse{};

			for (auto element : infix)
			{
				if (is_num(element))
				{
					result.insert(result.begin(), element);
				}
				else if (element.compare("(") == 0)
				{
					parse.push(element);
				}
				else if (element.compare(")") == 0)
				{
					while (parse.top().compare("(") != 0)
					{
						result.insert(result.begin(), parse.top());
						parse.pop();
					}

					parse.pop();
				}
				else if (is_operator(element))
				{
					while (!parse.empty() && has_greater_or_equal_priority(parse.top(), element))
					{
						result.insert(result.begin(), parse.top());
						parse.pop();
					}
					parse.push(element);
				}
			}

			while (!parse.empty())
			{
				result.insert(result.begin(), parse.top());
				parse.pop();
			}

			return result;
		}


		bool has_div_by_zero(std::vector<std::string> result)
		{
			while (is_operator(result[0]))
			{
				for (int i = 0; i < (int)(result.size() - 2); ++i)
				{
					int j = result.size() - 2;
					if (i >= j)
						break;

					if (is_operator(result.at(i)) &&
						is_num(result.at(i + 1)) &&
						is_num(result.at(i + 2)))
					{
						std::string op = result.at(i);
						int r = std::stoi(result.at(i + 1));
						int l = std::stoi(result.at(i + 2));

						result.erase(result.begin() + i, result.begin() + i + 3);

						if (op.compare("+") == 0)
						{
							result.insert(result.begin() + i, std::to_string(l + r));
						}
						else if (op.compare("-") == 0)
						{
							result.insert(result.begin() + i, std::to_string(l - r));
						}
						else if (op.compare("/") == 0)
						{
							if (r == 0)
								return true;
							result.insert(result.begin() + i, std::to_string(static_cast<int>(l / r)));
						}
						else if (op.compare("*") == 0)
						{
							result.insert(result.begin() + i, std::to_string(l * r));
						}
					}
				}
			}

			return false;
		}

		std::string eval(std::vector<std::string> result)
		{
			while (is_operator(result[0]))
			{
				for (int i = 0; i < (int)(result.size() - 2); ++i)
				{
					int j = result.size() - 2;
					if (i >= j)
						break;

					if (is_operator(result.at(i)) &&
						is_num(result.at(i + 1)) &&
						is_num(result.at(i + 2)))
					{
						std::string op = result.at(i);
						int r = std::stoi(result.at(i + 1));
						int l = std::stoi(result.at(i + 2));

						result.erase(result.begin() + i, result.begin() + i + 3);

						if (op.compare("+") == 0)
						{
							result.insert(result.begin() + i, std::to_string(l + r));
						}
						else if (op.compare("-") == 0)
						{
							result.insert(result.begin() + i, std::to_string(l - r));
						}
						else if (op.compare("/") == 0)
						{
							result.insert(result.begin() + i, std::to_string(static_cast<int>(l / r)));
						}
						else if (op.compare("*") == 0)
						{
							result.insert(result.begin() + i, std::to_string(l * r));
						}
					}
				}
			}

			return result[0];
		}
	};
}

#endif