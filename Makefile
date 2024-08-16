

.PHONY: build test-build test-run
build: clean
	@ mkdir build && gcc -std=c99 -Wall -Wextra -pedantic-errors main.c -o build/cbangc && echo 'Compilation successful.\nC! Compiler v0.0.1\nRun it with ./build/cbangc\n-----------------\n'

clean:
	@rm -rf build

test-build: build
	@./build/cbangc && qbe -o build/test.s build/test.ssa && cc build/test.s -o build/test

test-run: test-build
	@echo "\nRun Test\n-----------------\n" && ./build/test

example: 
	@qbe -o out.s file.ssa && cc out.s
