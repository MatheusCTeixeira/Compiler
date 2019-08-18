#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../header/intermediate_code.hpp"
#include "../header/file_writer.hpp"

#ifndef _FINAL_CODE_
#define _FINAL_CODE_

namespace comp {
	class final_code
	{
	public:
		final_code();
		
		void process(
						std::vector<std::string> vars,
						std::vector<std::pair<std::string, std::string>> string_vars,
						std::vector<std::vector<std::string>> intermediate_code
					);

		void print_final_code() const;
		void output_to_file(std::string filename) const;

	protected:

	private:
		void declare_vars(std::vector<std::string> vars, std::vector<std::pair<std::string, std::string>> string_vars);

		void make_header();
		void process_intructions();

		void add_instruction(int at);
		void sub_instruction(int at);
		void mul_instruction(int at);
		void div_instruction(int at);
		void mod_instruction(int at);
		void mov_instruction(int at);
		void equ_instruction(int at);
		void dif_instruction(int at);
		void and_instruction(int at);
		void  or_instruction(int at);
		void  je_instruction(int at);
		void ret_instruction();

		void load_args_to_register(int at);
		std::string find_eval(int at);

		bool is_num(std::string str) const;		


		std::vector<std::vector<std::string>> m_final_code;
		std::vector<std::vector<std::string>> m_intermediate_code;
		std::unordered_map < std::string, void(final_code:: *)(int at)> m_map_instruction;
	};
}
#endif