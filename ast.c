#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "ast.h"
#include "array.h"

AST *ast_new(AST ast) {
    AST *ptr = malloc(sizeof(AST));
    if (ptr) {
        *ptr = ast;
    }
    return ptr;
}


void ast_print(AST *ast, FILE* fd, unsigned int lvl) {
    switch (ast->tag) {
        case AST_NUMBER: {
            fprintf(fd, "%*s%s %li\n", 
                    lvl * PRINT_INDENT, " ", AST_TYPE_STR[AST_NUMBER], 
                    ast->data.AST_NUMBER.value);
            return;
        }

        case AST_SYMBOL: {
            fprintf(fd, "%*s%s %s\n", 
                    lvl * PRINT_INDENT, " ", AST_TYPE_STR[AST_SYMBOL], 
                    ast->data.AST_SYMBOL.sym);
            return;
        }

        case AST_SEXPR: {
            fprintf(fd, "%*s%s\n", 
                    lvl * PRINT_INDENT, " ", AST_TYPE_STR[AST_SEXPR]);
            lvl++;

            if (ast->data.AST_SEXPR.left != NULL) {
                ast_print(ast->data.AST_SEXPR.left, fd, lvl);
            }
            if (ast->data.AST_SEXPR.right != NULL) {
                ast_print(ast->data.AST_SEXPR.right, fd, lvl);
            }

            return;
        }

        case AST_FUNC: {
            fprintf(fd, "%*s%s %s -> \n",
                    lvl * PRINT_INDENT, " ", AST_TYPE_STR[AST_FUNC], ast->data.AST_FUNC.argName);
            lvl++;
            ast_print(ast->data.AST_FUNC.body, fd, lvl);

            return;
        }

        case AST_EXPR: {
            fprintf(fd, "%*s%s\n", 
                    lvl * PRINT_INDENT, " ", AST_TYPE_STR[AST_EXPR]);
            lvl++;
            ast_print(ast->data.AST_EXPR.expr, fd, lvl);
            return;
        }
    }
    fflush(stdout);
}

bool is_atom(AST *ast) {
    switch (ast->tag) {
    case AST_NUMBER:
    case AST_SYMBOL:
    case AST_BUILTIN:
        return true;
    case AST_FUNC:
    case AST_SEXPR:
    case AST_EXPR:
        return false;
      break;
    }
}

bool is_sexpr(AST *ast) {
    switch (ast->tag) {
    case AST_SEXPR:
        return true;
    default: 
        return false;
    }
}

void *ast_eval(ENV *env, AST *ast){ return NULL; }
AST *ast_eval_(ENV *env, AST *ast) {
    switch (ast->tag) {
        case AST_NUMBER: {
            // terminal
            AST_NUMBER_t data = ast->data.AST_NUMBER;
            long *res = malloc(sizeof(long));
            *res = data.value;
            printf("number %li\n", *res);
            return ast;
        }

        case AST_SYMBOL: {
            AST_SYMBOL_t data = ast->data.AST_SYMBOL;
            printf("%s\n", data.sym);
            AST *res = env_get(env, data.sym);
            if (res == NULL){
                printf("symbol '%s' not found", data.sym);
                exit(1);
            }
            printf("%s\n", AST_TYPE_STR[res->tag]);

            return res;
        }
        case AST_BUILTIN: {
            AST_SYMBOL_t data = ast->data.AST_SYMBOL;
            return ast;
        }

        case AST_SEXPR: {
            AST_SEXPR_t data = ast->data.AST_SEXPR;

            if(data.left != NULL) {
                data.left = ast_eval_(env, data.left);
                if (data.left == NULL) {
                    printf("evaluation returned NULL");
                }
                printf("%s\n", AST_TYPE_STR[data.left->tag]);
            }
            if(data.right != NULL) {
                data.right = ast_eval_(env, data.right);
            }
            if (data.left != NULL & data.left->tag == AST_BUILTIN) {

              // int a = data.left->data.AST_BUILTIN.call(5, 6);
              int a = ( (int (*)(int, int))data.left->data.AST_BUILTIN.call)(5, 6);
              printf("BUILTIN FUNC found - res is %i\n", a);
            }
            printf("HROOOO\n");

            return ast;
        }
        case AST_FUNC: {
            AST_FUNC_t data = ast->data.AST_FUNC;
            if (data.env == NULL) {
                data.env = env_create();
            }
            ast_eval_(env, data.body);
            if (is_sexpr(data.body) && is_atom(data.body->data.AST_SEXPR.left)) {
                // bind atom to env
                env_bind(data.env, data.body->data.AST_SEXPR.left, data.argName);
                return ast_eval_(data.env, data.body->data.AST_SEXPR.right);
            }
            break;
        }

        case AST_EXPR: {
            AST_EXPR_t data = ast->data.AST_EXPR;
            printf("expr\n");
            return ast_eval_(env, data.expr);
        }
    }
    return NULL;
}

ENV *env_create() {
    ENV *env = (ENV*)malloc(sizeof(ENV));
    env->sym = (char**)array_create(sizeof(char*));
    env->val = (AST**)array_create(sizeof(AST*));
    env->par = NULL;

    if (env == NULL || env->sym == NULL || env->val == NULL) {
        fprintf(stderr, "Unsufficient memory.");
        exit(1);
    }
    return env;
}

void env_bind_(ENV *e, void (*atom)(), char* sym) {
    if (e->sym == NULL) {
        e->sym = array_create(sizeof(char*));
    }
    if (e->val == NULL) {
        e->val = array_create(sizeof(void *));
    }
    array_push(e->sym, sym);
    array_push(e->val, atom);
}
void env_bind(ENV *e, AST *atom, char* sym) {
    if (is_atom(atom)) {
        if (e->sym == NULL) {
            e->sym = array_create(sizeof(char*));
        }
        if (e->val == NULL) {
            e->val = array_create(sizeof(AST*));
        }
        array_push(e->sym, sym);
        array_push(e->val, atom);
    }
}


AST *env_get(ENV *e, char* sym) {
    if (e == NULL) return NULL;
    if (e->sym == NULL) {
        printf("env symbols empty");
        exit(1);
    }
    if (e->val == NULL) {
        printf("env symbols empty");
        exit(1);
    }
    for (size_t i = 0; i < array_len(e->sym); i++) {
        if (strcmp(sym, e->sym[i]) == 0) {
            return e->val[i];
        }
    }
    if (e->par != NULL) {
        return env_get(e->par, sym);
    } else {
        return NULL;
    }
}
