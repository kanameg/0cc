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
  
  return;
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

  return;
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

  return;
}


/**
   Genarate assembler start code
 */
void generate_start(void) {
  printf(".intel_syntax noprefix\n");
  printf("\n");
  printf(".global _main\n"); // change main to _main for mac
  printf("\n");
  printf(".text\n");
  printf("_main:\n"); // change main to _main for mac

  return;
}


/**
   Generate assembler of 'return' code
 */
void generate_return(void) {
  printf("  pop rax\n");
  printf("  ret\n");

  return;
}


/**
   Generate assembler of opcode
 */
void generate_op(int op) {
  printf("  pop rdi\n");
  printf("  pop rax\n");
  
  switch (op) {
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
   Generate assembler of number code
 */
void generate_num(int num) {
  printf("  push %d\n", num);

  return;
}


/**
   Generate assembler of program code
 */
void generate_code(Node *node) {
  if (node->type == NODE_NUM) {
    generate_num(node->value);
    return;   // return because leaf node.
  }
  
  // internal node
  generate_code(node->left);   // trace left hand tree and genarate code
  generate_code(node->right);  // trace right hand tree and generate code

  generate_op(node->type);
  
  return;
}


/**
   Assember code generator
 */
void generator(Node *node) {
  generate_start();
  
  generate_code(node);

  generate_return();
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

  // syntactic analysis  make abstract syntax tree.
  Node *node = parser();

  // code generator
  generator(node);
  
  return 0;
}
