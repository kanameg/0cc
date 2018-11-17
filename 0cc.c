/**
 * C compiler 0cc
 * @author kaname.g@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "0cc.h"


/**
   Define global data
 */
int p = 0;
Token tokens[100];  // result of tokenized


/**
   Make new operator node
*/
Node *new_op_node(int op, Node *left, Node *right) {
  Node *node = malloc(sizeof(Node));
  
  node->type = op;
  node->left = left;
  node->right = right;
  //fprintf(stderr, "op: %c, left: %d, right: %d\n", op, left->value, right->value);
  
  return node;
}


/**
   Make new number node
*/
Node *new_num_node(int value) {
  Node *node = malloc(sizeof(Node));
  
  node->type = NODE_NUM;
  node->value = value;
  
  return node;
}


/**
   Delete node
*/
void delete_node(Node *node) {
  if (node != NULL)
    free(node);
}


/**
   Expression parser
   E : T E'
*/
Node *expr() {
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
Node *term() {
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
Node *factor() {
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


/**
   Tokenizer
*/
void tokenize(char *p) {
  int i = 0;  // token counter
  
  while (*p) {
    // skip space character
    if (isspace(*p)) {
      p++;
      continue;
    }
    
    // token char
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' ||
	*p == '(' || *p == ')' || *p == '=') {
      tokens[i].type = *p;
      tokens[i].input = p;
      tokens[i].value = *p;
      p++;
      i++;
      continue;
    }

    // token variable
    if (*p >= 'a' && *p <= 'z') {
      tokens[i].type = TOKEN_IDENT;
      tokens[i].input = p;
      tokens[i].value = *p;
      p++;
      i++;
      continue;
    }

    // token EOT
    if (*p == ';') {
      tokens[i].type = TOKEN_EOT;
      tokens[i].input = p;
      tokens[i].value = *p;
      p++;
      i++;
      continue;
    }
    
    // integer token
    if (isdigit(*p)) {
      tokens[i].type = TOKEN_NUM;
      tokens[i].input = p;
      tokens[i].value = strtol(p, &p, 10);
      i++;
      continue;
    }
    
    fprintf(stderr, "Cannot tokenize: %s \n", p);
    exit(1);
  }
}


/**
   Print all tokens
*/
void print_token(Token *tokens) {
  int i = 0;
  fprintf(stderr, "[");
  while (tokens[i].type != TOKEN_EOT) {
    if (tokens[i].type == TOKEN_NUM)
      fprintf(stderr, "%d, ", tokens[i].value);
    else
      fprintf(stderr, "%c, ", tokens[i].value);
    i++;
  }
  fprintf(stderr, "EOT]\n");
}


/**
   Trace all node tree and generate assembler code.
 */
void generate_code(Node *node) {
  if (node->type == NODE_NUM) {
    printf("  push %d\n", node->value);
    return;   // return because leaf node.
  }
  
  // internal node
  generate_code(node->left);   // trace left hand tree and genarate code
  generate_code(node->right);  // trace right hand tree and generate code

  printf("  pop rdi\n");
  printf("  pop rax\n");
  
  switch (node->type) {
  case '+':
    printf("  add rax, rdi\n");
    break;
  case '-':
    printf("  sub rax, rdi\n");
    break;
  case '*':
    printf("  mul rdi\n");
    break;
  case '/':
    printf("  mov rdx, 0\n");
    printf("  div rdi\n");
    break;
  }
  
  printf("  push rax\n");
  
  return;
}


/**
   main function
*/
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Not correct number of argument.\n");
    return 1;
  }

  // lexical analysis    make tokens
  tokenize(argv[1]);

  // syntactic analysis   make abstract syntax tree.
  Node *node = expr();
  
  printf(".intel_syntax noprefix\n");
  printf("\n");
  printf(".global _main\n"); // change main to _main for mac
  printf("\n");
  printf(".text\n");
  printf("_main:\n"); // change main to _main for mac

  generate_code(node);
  
  printf("  pop rax\n");
  printf("  ret\n");
  
  return 0;
}
