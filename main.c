// #include "greatest.h"

#include <stdint.h>
#include <stdio.h>

#include "ast.h"

int add(int x, int y) { return x + y; }

int main(void) {
  printf("Build AST.\n");

  /*
  (+ (5 6))
   * */
  AST *call = AST_NEW(
      AST_SEXPR, AST_NEW(AST_SYMBOL, "+"),
      AST_NEW(AST_SEXPR, AST_NEW(AST_NUMBER, 5), AST_NEW(AST_NUMBER, 6)));

  ENV *env = env_create();
  CBANG_TYPE args[2] = {INTEGER, INTEGER};
  env_bind(env, AST_NEW(AST_BUILTIN, (void (*)())add, INTEGER, args), "+");

  AST *res = ast_eval_(env, call);

  printf("res: %li", res->data.AST_NUMBER.value);
  printf("END PRINT\n");

  return 0;
}
