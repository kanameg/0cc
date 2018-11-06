/**
 * C compiler 0cc
 * @author kaname.g@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "0cc.h"

/**
   Make new operator node
*/
Node *new_op_node(int op, Node *left, Node *right) {
  Node *node = malloc(sizeof(Node));
  
  node->type = op;
  node->left = left;
  node->right = right;
  
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
   Parse expression
*/
Node *expr() {
  Node *left = term();
  
  if (tokens[p].type == TOKEN_EOT)
    return left;
  
  if (tokens[p].type == '+') {
    p++;
    return new_op_node('+', left, expr());
  }

  if (tokens[p].type == '-') {
    p++;
    return new_op_node('-', left, expr());
  }

  error(p);
  return NULL;
}


/**
   Parse term
*/
Node *term() {
  Node *left = factor();

  if (tokens[p].type == TOKEN_EOT)
    return left;

  if (tokens[p].type == '*') {
    p++;
    return new_op_node('*', left, term());
  }

  if (tokens[p].type == '/') {
    p++;
    return new_op_node('/', left, term());
  }
  
  error(p);
  return NULL;
}


/**
   Parse factor
*/
Node *factor() {
  if (tokens[p].type == TOKEN_NUM)
    return new_num_node(tokens[p++].value);
  
  if (tokens[p].type == '(') {
    p++;
    Node *node = expr();
    if (tokens[p].type != ')') {
      error(p);
    }
    p++;
    return node;
  }
  
  error(p);
  return NULL;
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
	*p == '(' || *p == ')') {
      tokens[i].type = *p;
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

  tokens[i].type = TOKEN_EOT;
  tokens[i].input = p;
  tokens[i].value = *p;
}


/**
   Printing all tokens
*/
void print_token(Token *tokens) {
  int i = 0;
  fprintf(stderr, "----------------------------------------------------\n");
  while (tokens[i].type != TOKEN_EOT) {
    fprintf(stderr, "Token[%d]\n", i);
    if (tokens[i].type == TOKEN_NUM) {
      fprintf(stderr, "	value: %d\n", tokens[i].value);
      fprintf(stderr, "	type: %d\n", tokens[i].type);
      fprintf(stderr, "	input: %s\n", tokens[i].input);
    }
    if (tokens[i].type == '+' || tokens[i].type == '-' ||
	tokens[i].type == '*' || tokens[i].type == '/' ||
	tokens[i].type == '(' || tokens[i].type == ')') {
      fprintf(stderr, "	value: %c\n", tokens[i].value);
      fprintf(stderr, "	type: %d\n", tokens[i].type);
      fprintf(stderr, "	input: %s\n", tokens[i].input);
    }
    i++;
  }
  fprintf(stderr, "EOT\n");
  fprintf(stderr, "	value: %c\n", tokens[i].value);
  fprintf(stderr, "	type: %d\n", tokens[i].type);
  fprintf(stderr, "	input: %s\n", tokens[i].input);
  fprintf(stderr, "----------------------------------------------------\n");
}


/**
   Printing token error message
*/
void error(int i) {
  fprintf(stderr, "Unexpected token: %s \n", tokens[i].input);
  exit(1);
}


/**
   main function
*/
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Not correct number of argument.\n");
    return 1;
  }
  
  tokenize(argv[1]);
  print_token(tokens);

  expr();

  printf(".intel_syntax noprefix\n");
  printf("\n");
  printf(".global _main\n"); // change main to _main for mac
  printf("\n");
  printf(".text\n");
  printf("_main:\n"); // change main to _main for mac

  int i = 0;
  // check first token is number?
  if (tokens[i].type != TOKEN_NUM)
    error(i);
  printf("  mov rax, %d\n", tokens[i].value);
  i++;

  while (tokens[i].type != TOKEN_EOT) {
    if (tokens[i].type == '+') {
      i++;
      if (tokens[i].type != TOKEN_NUM)
	error(i);
      printf("  add rax, %d\n", tokens[i].value);
      i++;
      continue;
    }

    if (tokens[i].type == '-') {
      i++;
      if (tokens[i].type != TOKEN_NUM)
	error(i);
      printf("  sub rax, %d\n", tokens[i].value);
      i++;
      continue;
    }

    error(i);
  }
  
  printf("  ret\n");
  
  return 0;
}
