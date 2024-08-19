// #include "greatest.h"

#include <stdint.h>
#include <stdio.h>

#include "ast.h"

/* 

function w $add(w %a, w %b) {
@start
	%c =w add %a, %b
	ret %c
}

export function w $main() {
@start
	%r =w call $add(w 1, w 1)
	call $printf(1 $fmt, ..., w %r)
	ret 0
}
data $fmt = { b "One and one make %d!\n", b 0 }

 * */

int main(void) {
	printf("Build AST.\n");

	AST body[1];
	body[0] = *AST_NEW(AST_BI_OP, AST_NEW(AST_NUMBER, 4), AST_NEW(AST_NUMBER, 6), PLUS);
	AST* root = AST_NEW(AST_FUNC_DEF, QExport, NULL, 0, "main", Qword, (AST**)body, 1);
	ast_print(root);

	AST* expr;
	PARAM *params = malloc(sizeof(PARAM) * 2);
	params[0] = ( PARAM ){ .tag = Qword, .val = 5 };
	params[1] = ( PARAM ){ .tag = Qword, .val = 10 };
	expr = AST_NEW(AST_EXPR, "add", 2, params);
	ast_print(root);
	printf("PRINT EXPR\n");
	ast_print(expr);


	char* filename = "build/test.ssa";
	printf("Emit QBE file %s.\n",filename);

	ast_emit_qbe(filename, root);

	printf("-----------------\n");
	return 0;
}
