#include "0cc.h"

/**
   This is a assembler code generator for Intel x86 CPU.
 */


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
