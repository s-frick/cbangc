#include <math.h>
#include <stdint.h>
#include <stdio.h>

// Types QBE
typedef char Qsigil; // : $ % @
typedef int32_t Qword; // int32 => w
typedef int64_t Qlong; // int64 => l
typedef float_t Qsingle; // float32 => s
typedef double_t Qdouble; // float64 => d

// Extended types QBE
typedef int8_t Qbyte; // => b
typedef int16_t Qhalf; // => h

typedef Qlong Qptr; // pointer 

typedef enum {
	QExport,
	QThread,
	QSection
} Linkage;

typedef enum {
	AST_INT,
	AST_OP,
	AST_LONG,
	AST_STR,
	AST_FUNC,
} AST_TYPE;


typedef struct ASTNode ASTNode_t;
typedef AST_TYPE (*TypeFunc) (ASTNode_t*);
typedef void (*DeleteFunc) (ASTNode_t*);
typedef ASTNode_t** (*ChildrenFunc) (ASTNode_t*);
typedef void (*EmitFunc) (FILE*, ASTNode_t*);
struct ASTNode {
	TypeFunc type;
	DeleteFunc del;
	ChildrenFunc get_children;
	EmitFunc emit;
};


typedef struct AST_op AST_op;
struct AST_op {
	TypeFunc type;
	DeleteFunc del;
	ChildrenFunc get_children;
	EmitFunc emit;

	char value;

	int len;
	ASTNode_t** children;
};
AST_op *AST_create_op(char op);

typedef struct AST_int AST_int;
struct AST_int {
	TypeFunc type;
	DeleteFunc del;
	ChildrenFunc get_children;
	EmitFunc emit;

	int64_t value;
};
AST_int *AST_create_int(int64_t value);

typedef struct AST_func AST_func;
struct AST_func {
	TypeFunc type;
	DeleteFunc del;
	ChildrenFunc get_children;
	EmitFunc emit;

	char* name;
	AST_TYPE ret_type;
	Linkage linkage;
	// TODO: args

	int len;
	ASTNode_t** children;
};
AST_func *AST_create_func(char* name, AST_TYPE ret_type, Linkage linkage);
