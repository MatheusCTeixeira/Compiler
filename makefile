CC=g++

hhh: lib/compiler.o lib/expr.o lib/file_reader.o lib/file_writer.o lib/final_code.o lib/intermediate_code.o lib/lex_analyzer.o lib/sem_analyzer.o lib/syn_analyzer.o
	$(CC) -o $@  $?

lib/compiler.o: source/compiler.cpp
	$(CC) $< -c -o $@
lib/expr.o : source/expr.cpp
	$(CC) $< -c -o $@
lib/file_reader.o : source/file_reader.cpp
	$(CC) $< -c -o $@
lib/file_writer.o : source/file_writer.cpp
	$(CC) $< -c -o $@
lib/final_code.o : source/final_code.cpp
	$(CC) $< -c -o $@
lib/intermediate_code.o : source/intermediate_code.cpp
	$(CC) $< -c -o $@
lib/lex_analyzer.o : source/lex_analyzer.cpp
	$(CC) $< -c -o $@
lib/sem_analyzer.o : source/sem_analyzer.cpp
	$(CC) $< -c -o $@
lib/syn_analyzer.o : source/syn_analyzer.cpp
	$(CC) $< -c -o $@
