// Types QBE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

typedef char Qsigil; // : $ % @

// Extended types QBE
typedef int8_t Qbyte; // => b
typedef int16_t Qhalf; // => h

#define GENERATE_ENUM(ENUM, STRING) ENUM,
#define GENERATE_STRING(ENUM, STRING) #STRING,

#define FOREACH_Q_TYPE(Q_TYPE) \
    Q_TYPE(Qword, w) \
    Q_TYPE(Qlong, l) \
    Q_TYPE(Qsingle, s) \
    Q_TYPE(Qdouble, d) \

#define FOREACH_LINKAGE(LINKAGE) \
    LINKAGE(QExport, export) \
    LINKAGE(QThread, thread) \
    LINKAGE(QSection, section) 
    
typedef enum Linkage {
    FOREACH_LINKAGE(GENERATE_ENUM)
} Linkage;

static const char *Linkage_Str[] = {
    "export",
    "thread",
    "section",
};

typedef enum QType {
    FOREACH_Q_TYPE(GENERATE_ENUM)
} QType;

static const char *QType_Str[] = {
    FOREACH_Q_TYPE(GENERATE_STRING)
};


enum BIOP { PLUS, MINUS, MUL, DIV };

typedef struct ARG ARG;
struct ARG {
    enum QType tag;
    union {
        int32_t Qword;
        int64_t Qlong;
        float_t Qsingle;
        double_t Qdouble;
    } val;
    char* name;
};

typedef struct PARAM PARAM;
struct PARAM {
    enum QType tag;
    union {
        int32_t Qword;
        int64_t Qlong;
        float_t Qsingle;
        double_t Qdouble;
    } val;
};

// AST
typedef struct AST AST;
typedef struct AST_EXPR AST_EXPR_t;
typedef struct AST_SEXPR AST_SEXPR_t;
typedef struct AST_NUMBER AST_NUMBER_t;
typedef struct AST_BI_OP AST_BI_OP_t;
typedef struct AST_FUNC_DEF AST_FUNC_DEF_t;
struct AST {
    enum {
        AST_NUMBER,
        AST_BI_OP,
        AST_SEXPR,
        AST_EXPR,
        AST_CALL,
        AST_FUNC_DEF,
        AST_ASSIGN
    } tag;
    union {
        struct AST_NUMBER { int64_t number; } AST_NUMBER;
        struct AST_BI_OP { AST *left; AST *right; enum BIOP op; } AST_BI_OP;
        struct AST_SEXPR { AST_EXPR_t **children; } AST_SEXPR;
        struct AST_EXPR { AST *func; AST *left; AST *right; } AST_EXPR;
        struct AST_CALL { char* name; ARG *args; } AST_CALL;
        struct AST_FUNC_DEF { Linkage linkage; ARG *args; size_t argc; char* name; enum QType ret_type; AST **body; size_t bodyc; } AST_FUNC_DEF;
        struct AST_ASSIGN { AST_EXPR_t *node; } AST_ASSIGN;
    } data;
};
AST *ast_new(AST ast);
ARG *arg_new(ARG ast);
PARAM *param_new(PARAM p);
void ast_print(AST *ast);
void ast_emit_qbe(char* file, AST *ast);
void AST_NUMBER_print(AST *ast);

#define AST_NEW(tag, ...) \
    ast_new((AST){tag, {.tag=(struct tag){__VA_ARGS__}}})

#define ARG_NEW(tag, name, ...) \
    arg_new((ARG){tag, {.name = name, .tag=(tag){__VA_ARGS__}}})

#define PARAM_NEW(tag, ...) \
    param_new((PARAM){tag, {.tag=(struct tag){__VA_ARGS__}}})



// AST end
