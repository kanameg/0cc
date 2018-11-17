#include "0cc.h"

/**
   This is a recursive-descendent parser which make abstract syntax tree
   from input program tokens.
 */


/**
   Program parser (Syntax analyzer)
 */
Node *parser(void) {
  return expr();
}


/**
   Expression parser
   E : T E'
*/
Node *expr(void) {
  Node *left = term();
  return expr2(left);
}


/**
   Expression' parser
   E' : '+' T E'
      | '-' T E'
      | e
 */
Node *expr2(Node *left) {
  if (tokens[p].type == '+') {
    p++;
    return expr2(new_op_node('+', left, term()));
  }
  if (tokens[p].type == '-') {
    p++;
    return expr2(new_op_node('-', left, term()));
  }
  
  return left;
}    


/**
   Term parser
   T : F T'
*/
Node *term(void) {
  Node *left = factor();
  
  return term2(left);
}


/**
   Term' parser
   T' : '*' F T'
      | '/' F T'
      | e
*/
Node *term2(Node *left) {
  if (tokens[p].type == '*') {
    p++;
    return term2(new_op_node('*', left, factor()));
  }
  if (tokens[p].type == '/') {
    p++;
    return term2(new_op_node('/', left, factor()));
  }
  
  return left;
}


/**
   Factor parser
   F : number
     | '(' E ')'
*/
Node *factor(void) {
  if (tokens[p].type == TOKEN_NUM) {
    Node *node = new_num_node(tokens[p].value);
    p++;
    return node;
  }
  
  if (tokens[p].type == '(') {
    p++;
    Node *node = expr();
    if (tokens[p].type != ')') {
      ERROR("Expected ), but token is: %s\n", tokens[p].input);
    }
    p++;
    return node;
  }
  
  ERROR("Unexpected factor token: %s\n", tokens[p].input);
}
