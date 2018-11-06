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
*/
/*
Node *expr() {
  Node *left = factor();
  
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
  
  ERROR("Unexpected expression token: %s\n", tokens[p].input);
}
*/


/**
   Term parser
*/
/*
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
*/


/**
   Factor parser
*/
Node *factor() {
  if (tokens[p].type == TOKEN_NUM) {
    Node *node = new_num_node(tokens[p].value);
    p++;
    return node;
  }
  
  if (tokens[p].type == '(') {
    p++;
    //Node *node = expr();
    Node *node = factor();
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
   Print all tokens
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
   Print all nodes
 */
void print_node(Node *node) {
  if (node->type == NODE_NUM) {
    fprintf(stderr, "value: %d\n", node->value);
    fprintf(stderr, "type: %d\n", node->type);
    return;
  }

  print_node(node->left);
  print_node(node->right);

  switch (node->type) {
  case '+':
    fprintf(stderr, "value: %d\n", node->value);
    fprintf(stderr, "type: %d\n", node->type);
    break;
  case '-':
    fprintf(stderr, "value: %d\n", node->value);
    fprintf(stderr, "type: %d\n", node->type);
    break;
  }

  return;
}


void trace_node_tree(Node *node) {
  if (node->type == NODE_NUM) {
    printf("  mov rax, %d\n", node->value);
    return;   // return because leaf node.
  }

  // internal node
  trace_node_tree(node->left);   // trace left hand tree
  trace_node_tree(node->right);  // trace right hand tree
  
  switch (node->type) {
  case '+':
    printf("  add rax, %d\n", node->value);
    break;
  case '-':
    printf("  sub rax, %d\n", node->value);
    break;
  }
  
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
  
  tokenize(argv[1]);
  print_token(tokens);

  Node *node = factor();
  print_node(node);

  printf(".intel_syntax noprefix\n");
  printf("\n");
  printf(".global _main\n"); // change main to _main for mac
  printf("\n");
  printf(".text\n");
  printf("_main:\n"); // change main to _main for mac

  trace_node_tree(node);
  
  printf("  ret\n");
  
  return 0;
}
