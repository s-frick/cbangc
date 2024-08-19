

.PHONY: build test-build test-run
build: clean
	@ mkdir build && gcc -std=c11 -Wall -Wextra -Wswitch -Werror=switch -pedantic-errors main.c q_ast.c ast.c -o build/cbangc && echo 'Compilation successful.\nC! Compiler v0.0.1\nRun it with ./build/cbangc\n-----------------\n'
run: clean
	@mkdir build && gcc -std=c11 -Wall -Wextra -Wswitch -Werror=switch -pedantic-errors main.c q_ast.c ast.c -o build/cbangc && ./build/cbangc

clean:
	@rm -rf build

test-build: build
	@./build/cbangc && qbe -o build/test.s build/test.ssa && cc build/test.s -o build/test

test-run: test-build
	@echo "\nRun Test\n-----------------\n" && ./build/test

example: 
	@qbe -o out.s file.ssa && cc out.s
