// Types QBE
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define GENERATE_ENUM(ENUM, STRING, STRUCT) ENUM,
#define GENERATE_STRING(ENUM, STRING, STRUCT) #STRING,
#define GENERATE_TYPEDEF(ENUM, STRING, STRUCT) typedef struct ENUM ENUM##_t;
#define GENERATE_STRUCT(ENUM, STRING, STRUCT) struct ENUM STRUCT ENUM;
#define GENERATE_CAST(ENUM, STRING, TYPE)                                      \
  TYPE *cbang_cast_##TYPE(void *value) { return (TYPE *)value; }
#define CAST_FUNC(FUNC, RET, ...) ((RET(*)(__VA_ARGS__))FUNC)
#define FROM_ARR(ARR, SIZE)                                                    \
  for (size_t i = 0; i < SIZE; i++) {                                          \
  }

#define FOREACH_CBANG_TYPE(CBANG_TYPE)                                         \
  CBANG_TYPE(INTEGER, <int>, int)                                              \
  CBANG_TYPE(LONG, <long>, long)

typedef enum { FOREACH_CBANG_TYPE(GENERATE_ENUM) } CBANG_TYPE;

typedef struct ENV ENV;
// DEFINE AST HERE!
#define FOREACH_AST_TYPE(AST_TYPE)                                             \
  AST_TYPE(AST_NUMBER, <number>, { long value; })                              \
  AST_TYPE(AST_SYMBOL, <symbol>, { char *sym; })                               \
  AST_TYPE(AST_BUILTIN, <builtin>, {                                           \
    void (*call)();                                                            \
    CBANG_TYPE ret;                                                            \
    CBANG_TYPE *args;                                                          \
  })                                                                           \
  AST_TYPE(AST_FUNC, <func>, {                                                 \
    ENV *env;                                                                  \
    char *argName;                                                             \
    AST *body;                                                                 \
  })                                                                           \
  AST_TYPE(AST_SEXPR, <sexpr>, {                                               \
    AST *left;                                                                 \
    AST *right;                                                                \
  })                                                                           \
  AST_TYPE(AST_EXPR, <expr>, { AST *expr; })
enum AST_TYPE { FOREACH_AST_TYPE(GENERATE_ENUM) };

static const char *AST_TYPE_STR[] = {FOREACH_AST_TYPE(GENERATE_STRING)};

// AST
typedef struct AST AST;
FOREACH_AST_TYPE(GENERATE_TYPEDEF)
struct AST {
  enum AST_TYPE tag;
  union {
    FOREACH_AST_TYPE(GENERATE_STRUCT)
  } data;
};
AST *ast_new(AST ast);
void ast_print(AST *ast, FILE *fd, unsigned int lvl);
void *ast_eval(ENV *env, AST *ast);
AST *ast_eval_(ENV *env, AST *ast);
#define ast_call(env, builtin, ...) builtin(##__VA_ARGS__)
#define AST_PRINT(ast, fd, ...) ast_print(ast, fd, (0, ##__VA_ARGS__))
#define PRINT_INDENT 4

#define AST_NEW(tag, ...)                                                      \
  ast_new((AST){tag, {.tag = (struct tag){__VA_ARGS__}}})

struct ENV {
  ENV *par;
  char **sym;
  AST **val;
};

ENV *env_create();
void env_bind(ENV *e, AST *atom, char *sym);
void env_bind_(ENV *e, void (*atom)(), char *sym);
AST *env_get(ENV *e, char *sym);

// AST end
