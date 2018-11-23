#include "0cc.h"

/**
   This is a recursive-descent parser which make abstract syntax tree
   from program tokens array.
 */


/**
   Program parser (Syntax analyzer)
 */
Node *parser(void) {
  return program();
}


/**
   Program parser
   P : ident '=' E
     | E
 */
Node *program(void) {
  if (tokens[p].type == TOKEN_IDENT) {
    Node *left = new_ident_node(tokens[p].value);
    p++;
    if (tokens[p].type == '=') {
      return new_op_node(tokens[p++].type, left, expr());
    }
    ERROR("Unexpected program token: %s\n", tokens[p].input);  
  }
  
  return expr();
}

/**
   Expression parser
   E : T {'+' T}
     | T {'-' T}
*/
Node *expr(void) {
  Node *left = term();

  while (tokens[p].type == '+' || tokens[p].type == '-') {
    left = new_op_node(tokens[p++].type, left, term());
  }

  return left;
}


/**
   Term parser
   T : F {'*' F}
     | F {'/' F}
*/
Node *term(void) {
  Node *left = factor();
  
  while (tokens[p].type == '*' || tokens[p].type == '/') {
    left = new_op_node(tokens[p++].type, left, factor());
  }

  return left;
}


/**
   Factor parser
   F : '(' E ')'
     | number
*/
Node *factor(void) {
  if (tokens[p].type == '(') {
    p++;
    Node *node = expr();
    if (tokens[p].type != ')') {
      ERROR("Expected ), but token is: %s\n", tokens[p].input);
    }
    p++;
    return node;
  }
  
  if (tokens[p].type == TOKEN_NUM) {
    Node *node = new_num_node(tokens[p].value);
    p++;
    return node;
  }
  
  ERROR("Unexpected factor token: %s\n", tokens[p].input);
}
