#include "../header/final_code.hpp"

comp::final_code::final_code()
{
	this->m_map_instruction.insert({ "+",  &comp::final_code::add_instruction });
	this->m_map_instruction.insert({ "-",  &comp::final_code::sub_instruction });
	this->m_map_instruction.insert({ "*",  &comp::final_code::mul_instruction });
	this->m_map_instruction.insert({ "/",  &comp::final_code::div_instruction });
	this->m_map_instruction.insert({ "%",  &comp::final_code::mod_instruction });
	this->m_map_instruction.insert({ "=",  &comp::final_code::mov_instruction });
	this->m_map_instruction.insert({ "==", &comp::final_code::equ_instruction });
	this->m_map_instruction.insert({ "!=", &comp::final_code::dif_instruction });
	this->m_map_instruction.insert({ "&&", &comp::final_code::and_instruction });
	this->m_map_instruction.insert({ "||", &comp::final_code::or_instruction });
}

void comp::final_code::process(
	std::vector<std::string> vars,
	std::vector<std::pair<std::string, std::string>> string_vars,
	std::vector<std::vector<std::string>> intermediate_code
)
{
	this->m_intermediate_code = intermediate_code;		

	this->declare_vars(vars, string_vars);

	std::vector<std::string> compile_cmd;
	compile_cmd.push_back("; nasm -f elf file.hhh");
	this->m_final_code.push_back(compile_cmd);

	std::vector<std::string> link_cmd;
	link_cmd.push_back("; gcc-6 -m32 file.asm -o file");
	this->m_final_code.push_back(link_cmd);

	this->make_header();

	this->process_intructions();

	this->ret_instruction();
	
}

void comp::final_code::print_final_code() const
{
	for (auto row : this->m_final_code)
	{
		for (auto column : row)
			std::cout << std::setw(25) << column;

		std::cout << std::endl;
	}
}

void comp::final_code::output_to_file(std::string filename) const
{
	file_writer fw{filename};

	for (auto row : this->m_final_code)
	{
		std::string str;
		
		for (auto column : row)
			str += column + '\t';

		fw.write_line(str);
	}
}

void comp::final_code::declare_vars(std::vector<std::string> vars, std::vector<std::pair<std::string, std::string>> string_vars)
{
	std::vector<std::string> data;
	data.push_back("SECTION");
	data.push_back(".data");
	this->m_final_code.push_back(data);

	for (auto var : vars)
	{
		std::vector<std::string> v;
		v.push_back(var);
		v.push_back("DD");
		v.push_back("0");

		this->m_final_code.push_back(v);
	}


	for (auto var : string_vars)
	{
		std::vector<std::string> v;
		v.push_back(var.first);
		v.push_back("DB");
		v.push_back(var.second + ", 0xA, 0");

		this->m_final_code.push_back(v);
	}


	std::vector<std::string> print_int;
	print_int.push_back("print_int");
	print_int.push_back("DB");
	print_int.push_back("\"%d\" , 0xA, 0");
	this->m_final_code.push_back(print_int);

	std::vector<std::string> print_string;
	print_string.push_back("print_string");
	print_string.push_back("DB");
	print_string.push_back("\"%s\", 0xA, 0");
	this->m_final_code.push_back(print_string);

	std::vector<std::string> scan_int;
	scan_int.push_back("scan_int");
	scan_int.push_back("DB");
	scan_int.push_back("\"%d\"");
	this->m_final_code.push_back(scan_int);

	this->m_final_code.push_back(std::vector<std::string>{});
	
}

void comp::final_code::make_header()
{
	std::vector<std::string> text;
	text.push_back("SECTION");
	text.push_back(".text");
	this->m_final_code.push_back(text);

	std::vector<std::string> global;
	global.push_back("global");
	global.push_back("main");
	this->m_final_code.push_back(global);

	std::vector<std::string> print;
	print.push_back("extern");
	print.push_back("printf");
	this->m_final_code.push_back(print);

	std::vector<std::string> scan;
	scan.push_back("extern");
	scan.push_back("scanf");
	this->m_final_code.push_back(scan);

	this->m_final_code.push_back(std::vector<std::string>{});
	this->m_final_code.push_back(std::vector<std::string>{});


	std::vector<std::string> main;
	main.push_back("main:");
	this->m_final_code.push_back(main);

}

void comp::final_code::process_intructions()
{

	for (int i = 0; i < static_cast<int>(this->m_intermediate_code.size()); ++i)
	{
		if (this->m_intermediate_code[i][0].compare("INSTRUCTION") == 0)
		{
			
			auto func_ptr = this->m_map_instruction[this->m_intermediate_code[i][1]];
			(this->*func_ptr)(i);

			

			this->m_final_code.push_back(std::vector<std::string > {});
		}
		else if (this->m_intermediate_code[i][0].compare("label") == 0)
		{
			std::vector<std::string> label;
			label.push_back(this->m_intermediate_code[i][1] + ":");
			this->m_final_code.push_back(label);
		}
		else if (this->m_intermediate_code[i][0].compare("EVAL") == 0)
		{
			this->je_instruction(i);
		}
		else
		{
			this->m_final_code.push_back(this->m_intermediate_code[i]);
		}

	}
}

void comp::final_code::add_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> add;
	add.push_back("ADD");
	add.push_back("EAX,");
	add.push_back("EBX");

	this->m_final_code.push_back(add);

	std::vector<std::string> push;
	push.push_back("PUSH");
	push.push_back("EAX");

	this->m_final_code.push_back(push);
}


void comp::final_code::sub_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> sub;
	sub.push_back("SUB");
	sub.push_back("EAX,");
	sub.push_back("EBX");

	this->m_final_code.push_back(sub);

	std::vector<std::string> push;
	push.push_back("PUSH");
	push.push_back("EAX");

	this->m_final_code.push_back(push);

}

void comp::final_code::mul_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> mul;
	mul.push_back("IMUL");
	mul.push_back("EAX,");
	mul.push_back("EBX");

	this->m_final_code.push_back(mul);

	std::vector<std::string> push;
	push.push_back("PUSH");
	push.push_back("EAX");

	this->m_final_code.push_back(push);

}

void comp::final_code::div_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> xor_;
	xor_.push_back("XOR");
	xor_.push_back("EAX,");
	xor_.push_back("EAX");
	this->m_final_code.push_back(xor_);

	std::vector<std::string> div;
	div.push_back("IDIV");
	div.push_back("EBX");	

	this->m_final_code.push_back(div);

	std::vector<std::string> push;
	push.push_back("PUSH");
	push.push_back("EAX");

	this->m_final_code.push_back(push);

}

void comp::final_code::mod_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> xor_;
	xor_.push_back("XOR");
	xor_.push_back("EDX,");
	xor_.push_back("EDX");
	this->m_final_code.push_back(xor_);

	std::vector<std::string> mod;
	mod.push_back("IDIV");
	mod.push_back("EBX");

	this->m_final_code.push_back(mod);


	std::vector<std::string> mov;
	mov.push_back("MOV");
	mov.push_back("EAX,");
	mov.push_back("EDX");

	this->m_final_code.push_back(mov);

	std::vector<std::string> push;
	push.push_back("PUSH");
	push.push_back("EAX");

	this->m_final_code.push_back(push);

}

void comp::final_code::mov_instruction(int at)
{
	std::vector<std::string> arg_2;
	if (this->m_intermediate_code[at][3].compare("STK") == 0)
	{
		arg_2.push_back("POP");
		arg_2.push_back("EBX");
	}
	else if (this->is_num(this->m_intermediate_code[at][3]))
	{
		arg_2.push_back("MOV");
		arg_2.push_back("dword EBX,");
		arg_2.push_back(this->m_intermediate_code[at][3]);
	}
	else
	{
		arg_2.push_back("MOV");
		arg_2.push_back("EBX,");
		arg_2.push_back("[" + this->m_intermediate_code[at][3] + "]");
	}
	
	this->m_final_code.push_back(arg_2);

	std::vector<std::string> mov;
	mov.push_back("MOV");
	mov.push_back("[" + this->m_intermediate_code[at][2] + "],");
	mov.push_back("EBX");

	this->m_final_code.push_back(mov);

}

void comp::final_code::equ_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> cmp;
	cmp.push_back("CMP");
	cmp.push_back("EAX,");
	cmp.push_back("EBX");
	this->m_final_code.push_back(cmp);

	std::vector<std::string> lahf;
	lahf.push_back("LAHF");	
	this->m_final_code.push_back(lahf);

	std::vector<std::string> push;
	push.push_back("PUSH");	
	push.push_back("EAX");
	this->m_final_code.push_back(push);

}

std::string comp::final_code::find_eval(int at)
{
	for (int i = at; i < this->m_intermediate_code.size(); ++i)
		if (this->m_intermediate_code[i][0].compare("EVAL") == 0)
		{
			return this->m_intermediate_code[i][1];
		}

	return std::string{};
}

void comp::final_code::dif_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> cmp;
	cmp.push_back("CMP");
	cmp.push_back("EAX,");
	cmp.push_back("EBX");
	this->m_final_code.push_back(cmp);

	std::vector<std::string> lahf;
	lahf.push_back("LAHF");	
	this->m_final_code.push_back(lahf);

	std::vector<std::string> not_r;
	not_r.push_back("NOT");	
	not_r.push_back("EAX");
	this->m_final_code.push_back(not_r);

	std::vector<std::string> push;
	push.push_back("PUSH");	
	push.push_back("EAX");
	this->m_final_code.push_back(push);
}

void comp::final_code::and_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> and_;
	and_.push_back("AND");
	and_.push_back("EAX,");
	and_.push_back("EBX");
	this->m_final_code.push_back(and_);

	std::vector<std::string> push;
	push.push_back("PUSH");	
	push.push_back("EAX");
	this->m_final_code.push_back(push);
}

void comp::final_code::or_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string > {});
	this->load_args_to_register(at);

	std::vector<std::string> or_;
	or_.push_back("OR");
	or_.push_back("EAX,");
	or_.push_back("EBX");
	this->m_final_code.push_back(or_);

	std::vector<std::string> push;
	push.push_back("PUSH");	
	push.push_back("EAX");
	this->m_final_code.push_back(push);

}

void comp::final_code::je_instruction(int at)
{
	this->m_final_code.push_back(std::vector<std::string>{});

	std::vector<std::string> pop;
	pop.push_back("POP");	
	pop.push_back("EAX");
	this->m_final_code.push_back(pop);

	std::vector<std::string> and_;
	and_.push_back("AND");
	and_.push_back("AH,");
	and_.push_back("64");
	this->m_final_code.push_back(and_);

	std::vector<std::string> cmp;
	cmp.push_back("CMP");
	cmp.push_back("AH,");
	cmp.push_back("0");
	this->m_final_code.push_back(cmp);

	std::vector<std::string> je;
	je.push_back("JE");	
	je.push_back(this->m_intermediate_code[at][1]);
	this->m_final_code.push_back(je);

}

void comp::final_code::ret_instruction()
{
	this->m_final_code.push_back(std::vector<std::string>{});

	std::vector<std::string> ret;
	ret.push_back("RET");		
	this->m_final_code.push_back(ret);
}

void comp::final_code::load_args_to_register(int at)
{	
	std::vector<std::string> arg_2;
	if (this->m_intermediate_code[at][3].compare("STK") == 0)
	{	
		arg_2.push_back("POP");
		arg_2.push_back("EBX");		
	}
	else if (this->is_num(this->m_intermediate_code[at][3]))
	{		
		arg_2.push_back("MOV");
		arg_2.push_back("dword EBX,");
		arg_2.push_back(this->m_intermediate_code[at][3]);		
	}
	else
	{
		arg_2.push_back("MOV");
		arg_2.push_back("EBX,");
		arg_2.push_back("[" + this->m_intermediate_code[at][3] + "]");
	}
	this->m_final_code.push_back(arg_2);

	if (this->m_intermediate_code[at][1].compare("=") == 0)
		return;

	std::vector<std::string> arg_1;
	if (this->m_intermediate_code[at][2].compare("STK") == 0)
	{
		arg_1.push_back("POP");
		arg_1.push_back("EAX");
	}
	else if (this->is_num(this->m_intermediate_code[at][2]))
	{
		arg_1.push_back("MOV");
		arg_1.push_back("dword EAX,");
		arg_1.push_back(this->m_intermediate_code[at][2]);
	}
	else
	{
		arg_1.push_back("MOV");
		arg_1.push_back("EAX,");
		arg_1.push_back("[" + this->m_intermediate_code[at][2] + "]");
	}
	this->m_final_code.push_back(arg_1);
}



bool comp::final_code::is_num(std::string str) const
{
	std::locale loc{};
	for (auto c : str)
		if (!std::isdigit(c, loc))
			return false;

	return true;
}
