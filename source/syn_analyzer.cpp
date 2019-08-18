#include "../header/syn_analyzer.hpp"

namespace comp {
	syn_analyzer::syn_analyzer() {
		this->read_map();
		this->read_production();
	}

	void syn_analyzer::process(std::vector<lex_analyzer::token_type> tok) {

		this->m_stack.push("program");

		// analizar condição de parada em caso de erro!!!!
		while (!this->m_stack.empty() && !tok.empty()) {

			if (this->is_not_terminal(this->m_stack.top())) {
				// std::cout << this->m_stack.top() << " " << tok[0].m_token << std::endl;

				this->m_log.push(std::pair<std::string, std::string>{this->m_stack.top(), tok.front().m_token});

				std::string top = this->m_stack.top();
				this->m_stack.pop();

				for (int i = this->m_production[this->m_map[top][tok.front().m_token]].size() - 1; i > 0; --i)
					this->m_stack.push(this->m_production[this->m_map[top][tok.front().m_token]][i]);


			}
			else if (this->m_stack.top() == tok.front().m_token) {
				// std::cout << this->m_stack.top() << " " << tok[0].m_token << std::endl;
				this->m_log.push(std::pair<std::string, std::string>{this->m_stack.top(), tok.front().m_token});
				this->m_stack.pop();
				tok.erase(tok.begin(), tok.begin() + 1);


			}
			else if (this->m_stack.top() == "ì") {
				// std::cout << this->m_stack.top() << " " << tok[0].m_token << std::endl;
				this->m_log.push(std::pair<std::string, std::string>{this->m_stack.top(), tok.front().m_token});
				this->m_stack.pop();
			}
			else {
				// std::cout << this->m_stack.top() << " " << tok[0].m_token << std::endl;
				this->m_has_error = true;
				std::cout << "error: unexpected syntax" << std::endl;
				std::cout << "\t" << "at line:" << tok.front().m_line << " column: " << tok.front().m_column << std::endl;


				return;
			}

		}
	}

	void syn_analyzer::print_log() {
		std::cout << "Syntax analyzer log: " << std::endl << std::endl;
		std::cout << std::setw(15) << "stack" << "\t\t" << "vector" << std::endl;
		while (!this->m_log.empty()) {
			std::cout << std::setw(15) << this->m_log.front().first << "\t\t" << this->m_log.front().second << std::endl;

			this->m_log.pop();
		}
	}

	bool syn_analyzer::has_error() const
	{
		return this->m_has_error;
	}

	syn_analyzer::~syn_analyzer() {

	}

	void syn_analyzer::read_production() {
		file_reader file{ "others//production.xml" };
		//others//production.xml
		if (!file.is_opened())
			return;

		while (!file.eof()) {
			std::string line = file.next_line();
			int b_cursor = 0, e_cursor;
			std::vector<std::string> prod;

			while (b_cursor < (int)line.size()) {
				if (line[b_cursor] == '<') {
					e_cursor = ++b_cursor;
					while (line[e_cursor] != '>')
						++e_cursor;

					prod.push_back(line.substr(b_cursor, e_cursor - b_cursor));
					b_cursor = e_cursor;
				}
				b_cursor++;
			}

			this->m_production.push_back(prod);
		}
	}

	void syn_analyzer::read_map() {
		file_reader file{ "others//data.map" };
	//c:\Users\mathe\Downloads\new_compiler\others\data.map
		if (!file.is_opened())
			return;

		while (!file.eof()) {
			std::unordered_map<std::string, int> map{};
			std::string line = file.next_line();

			// captura o nome da produção
			std::string prod = line.substr(0, line.find_first_of(":"));

			line = file.next_line();
			while (!line.empty() && !file.eof()) {
				int b_cursor = 0, e_cursor = 0;

				// captura o não terminal                
				while (line[e_cursor] != ',') { ++e_cursor; continue; }
				std::string non_terminal = line.substr(b_cursor, e_cursor - b_cursor);
				b_cursor = ++e_cursor;

				while (line[e_cursor] != ' ' && line[e_cursor] != '\0') ++e_cursor;
				int value = std::stoi(line.substr(b_cursor, e_cursor - b_cursor));

				map.insert({ non_terminal, value });

				line = file.next_line();
			}
			this->m_map.insert({ prod, map });
		}
	}

	bool syn_analyzer::is_not_terminal(std::string expr) const {
		for (auto t : this->m_production)
			if (expr == t[0])
				return true;
		return false;
	}
}