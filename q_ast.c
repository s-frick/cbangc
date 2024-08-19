#include "q_ast.h"
#include <stdlib.h>


ASTNode_t** AST_children_op(ASTNode_t *self) {
	return ( (AST_op*)self )->children;
}

AST_TYPE AST_type_op(ASTNode_t *self) {
	return AST_OP;
}
void AST_delete_op(ASTNode_t *self) {
	AST_op *this = ((AST_op*)self);
	for (int i = 0; i < this->len; i++) {
		ASTNode_t *node = self->get_children(self)[i];
		node->del(node);
	}
	free(this->children);
	free(this);
	return;
}

AST_op *AST_create_op(char op) {
	AST_op *self = malloc(sizeof(AST_op));
	self->type = AST_type_op;
	self->del = AST_delete_op;
	self->get_children = AST_children_op;
	self->value = op;
	self->children = malloc(sizeof(ASTNode_t*) * 2);
	self->len = 2;

	return self;
}

ASTNode_t** AST_children_int(ASTNode_t *self) {
	return NULL;
}

AST_TYPE AST_type_int(ASTNode_t *self) {
	return AST_INT;
}
void AST_delete_int(ASTNode_t *self) {
	AST_int *this = ((AST_int*)self);
	free(this);
	return;
}

AST_int *AST_create_int(int64_t value) {
	AST_int *self = malloc(sizeof(AST_int));
	self->type = AST_type_int;
	self->del = AST_delete_int;
	self->get_children = AST_children_int;
	self->value = value;

	return self;
}

ASTNode_t** AST_children_func(ASTNode_t *self) {
	AST_func *this = ((AST_func*)self);
	return this->children;
}

AST_TYPE AST_type_func(ASTNode_t *self) {
	return AST_FUNC;
}
void AST_delete_func(ASTNode_t *self) {
	AST_func *this = ((AST_func*)self);
	for (int i = 0; i < this->len; i++) {
		ASTNode_t *node = AST_children_func(self)[i];
		node->del(node);
	}
	free(this->children);
	free(this);
	return;
}

const char* QfuncLiteral = "function";
void  AST_emit_func(FILE* fd, ASTNode_t* self) {
	AST_func *this = (AST_func*)self;
	char* linkage;
	if (this->linkage == QExport) {
		linkage = "export";
	}

	char* ret_type;
	if (this->ret_type == AST_INT) {
		ret_type = "w";
	}
	char* op;
	Qword operand1;
	Qword operand2;
	for (int i = 0; i < this->len; i++) {
		AST_op *op_node = (AST_op*)this->get_children(self)[0];
		if (op_node->value == '+') {
			op = "add";
		}
		AST_int *operand1_node = (AST_int*)op_node->get_children((ASTNode_t*)op_node)[0];
		operand1 = operand1_node->value;
		AST_int *operand2_node = (AST_int*)op_node->get_children((ASTNode_t*)op_node)[1];
		operand2 = operand2_node->value;
	}
	fprintf(fd, 
	 "data $fmt = { b \"One and two make %%d!\\n\", b 0 }\n"
	 "\n"
	 "%s %s %s $%s() {\n"
	 "@start\n"
	 "    %%r =w %s %i, %i\n"
	 "    call $printf(l $fmt, ..., w %%r)\n"
	 "    ret 0\n"
	 "}",
	 linkage, QfuncLiteral, ret_type, this->name, op, operand1, operand2);
	
}

AST_func *AST_create_func(char* name, AST_TYPE ret_type, Linkage linkage) {
	AST_func *self = malloc(sizeof(AST_func));
	self->type = AST_type_func;
	self->del = AST_delete_func;
	self->get_children = AST_children_func;
	self->emit = AST_emit_func;

	self->len = 0;
	self->name = name;
	self->ret_type = ret_type;
	self->linkage = linkage;
	// TODO: dynamic Array
	self->children = malloc(sizeof(ASTNode_t*) * 10);

	return self;
}
