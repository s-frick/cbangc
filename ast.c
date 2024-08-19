#include <stdio.h>
#include <string.h>

#include "ast.h"

AST *ast_new(AST ast) {
    AST *ptr = malloc(sizeof(AST));
    if (ptr) {
        *ptr = ast;
    }
    return ptr;
}

ARG *arg_new(ARG arg) {
    ARG *ptr = malloc(sizeof(ARG));
    if (ptr) {
        *ptr = arg;
    }
    return ptr;
}

PARAM *param_new(PARAM p) {
    PARAM *ptr = malloc(sizeof(PARAM));
    if (ptr) {
        *ptr = p;
    }
    return ptr;
}

void ast_print(AST *ast) {
    switch (ast->tag) {
        case (AST_NUMBER): {
            AST_NUMBER_t data = ast->data.AST_NUMBER;
            printf("%li", data.number);
            return;
        };
        case AST_BI_OP: {
            AST_BI_OP_t data = ast->data.AST_BI_OP;
            printf("(");
            ast_print(data.left);
            printf(" + ");
            ast_print(data.right);
            printf(")");
            return;
        }
        case AST_SEXPR:
            break;
        case AST_EXPR:{  
            AST_EXPR_t data = ast->data.AST_EXPR;
            printf("(");
            for (size_t i = 0; i < data.argc; i++) {
                ast_print(data.args[i])
            }

            break;
        }
        case AST_FUNC_DEF: {  
            AST_FUNC_DEF_t data = ast->data.AST_FUNC_DEF;
            size_t buf_size = 10;
            char *args = malloc(sizeof(char*) * buf_size);
            for (size_t i = 0; i < data.argc; i++) {
                // TODO: build dyn array abstraction - ensure_capacity macro
                size_t needed = strlen(QType_Str[data.args[i].tag]) + strlen(data.args[i].name) + 1;
                if (needed > buf_size ) {
                    args = realloc(args, needed + buf_size);
                }
                strcat(args, QType_Str[data.args[i].tag]);
                strcat(args, " ");
                strcat(args, data.args[i].name);
            }
            strcat(args, "\0");
            printf("fn %s(%s): %s {\n", data.name, args, QType_Str[data.ret_type]);

            // for(int i = 0; i < data.bodyc; i++){
            //     ast_print(data.body[i]);
            // }

            printf("}\n");

          break; 
        }
        case AST_ASSIGN:
          break;
        case AST_CALL:
          break;
        }
}

void ast_emit(FILE* fd, AST *ast) {
    switch (ast->tag) {
        case (AST_NUMBER): {
            AST_NUMBER_t data = ast->data.AST_NUMBER;
            fprintf(fd, "%li", data.number);
            printf("hrooo");
            return;
        };
        case AST_BI_OP: {
            printf("hrooo");
            AST_BI_OP_t data = ast->data.AST_BI_OP;
            fprintf(fd, "add ");
            ast_emit(fd, data.left);
            fprintf(fd, ", ");
            ast_emit(fd, data.right);
            return;
        }
        case AST_SEXPR:
        case AST_EXPR:
        case AST_FUNC_DEF: {  
            AST_FUNC_DEF_t data = ast->data.AST_FUNC_DEF;
            size_t buf_size = 10;
            char *args = malloc(sizeof(char*) * buf_size);
            for (size_t i = 0; i < data.argc; i++) {
                size_t needed = strlen(QType_Str[data.args[i].tag]) + strlen(data.args[i].name) + 1;
                if (needed > buf_size ) {
                    args = realloc(args, needed + buf_size);
                }
                strcat(args, QType_Str[data.args[i].tag]);
                strcat(args, " ");
                strcat(args, data.args[i].name);
            }
            strcat(args, "\0");
            fprintf(fd, "%s function %s $%s(%s) {\n", Linkage_Str[data.linkage], QType_Str[data.ret_type], data.name, args);


            fprintf(fd, "@start\n");
            // for(int i = 0; i < data.bodyc; i++){
            //     ast_print(data.body[i]);
            // }

            fprintf(fd, "    ret 0\n");
            fprintf(fd, "}\n");

          break; 
        }
          break;
        case AST_ASSIGN:
          break;
        case AST_CALL:
          break;
    }
}

void ast_emit_qbe(char* file, AST *ast) {
    FILE* fd = fopen(file, "w");
    if (fd == NULL) {
        printf("ERROR: Can't open file %s.\n", file);
        exit(1);
    }
    fprintf(fd, 
        "data $fmt = { b \"One and two make %%d!\\n\", b 0 }\n"
        "\n"
        );

    printf("HROOOO\n");
    ast_emit(fd, ast);

    // fprintf(fd,
    //     "    \ncall $printf(l $fmt, ..., w %%r)\n"
    //     "    ret 0\n"
    //     "}");

    fclose(fd);

}

