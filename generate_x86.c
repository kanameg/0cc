#include "0cc.h"
#include "reg.h"

/**
   This is a assembler code generator for Intel x86 CPU.
 */


/**
   Genetate prologue code
 */
void generate_prologue(void) {
  // function prologue
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");

  return;
}


/**
   Generate code to make function frame for stack variable.
 */
void generate_frame(int byte) {
  printf("  sub rsp, %d\n", byte);
}


/**
   Generate epilogue code
 */
void generate_eliplogue(void) {
  // function epilogue
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");

}


/**
   Generate pop register
 */
void generate_pop(char *reg) {
  printf("  pop %s\n", reg);
  
  return;
}


/**
   Generate push register
 */
void generate_push(char *reg) {
  printf("  push %s\n", reg);
  
  return;
}


/**
   Genarate assembler of main function start code
 */
void generate_start(void) {
  printf(".intel_syntax noprefix\n");
  printf("\n");
  printf(".global _main\n"); // change main to _main for mac
  printf("\n");
  printf(".text\n");
  printf("_main:\n"); // change main to _main for mac
  
  generate_prologue();
  generate_frame(40);   // 40 byte space
  
  return;
}


/**
   Generate assembler 'return' code
 */
void generate_return(void) {
#ifdef CC0_DEBUG
  fprintf(stderr, "\n");
#endif
  generate_pop(RAX);
  generate_eliplogue();
  printf("  ret\n");
    
  return;
}


/**
   Generate assembler of binary opcode
 */
void generate_op(Node *node) {
  printf("  pop rdi\n");  // postfix value
  printf("  pop rax\n");  // prefix value
  
  switch (node->type) {
  case '+':
#ifdef CC0_DEBUG
    fprintf(stderr, "%c -> ", node->type);
#endif
    printf("  add rax, rdi\n");
    break;
  case '-':
#ifdef CC0_DEBUG1
    fprintf(stderr, "%c -> ", node->type);
#endif
    printf("  sub rax, rdi\n");
    break;
  case '*':
#ifdef CC0_DEBUG
    fprintf(stderr, "%c -> ", node->type);
#endif
    printf("  mul rdi\n");
    break;
  case '/':
#ifdef CC0_DEBUG
    fprintf(stderr, "%c -> ", node->type);
#endif
    printf("  mov rdx, 0\n");
    printf("  div rdi\n");
    break;
  }
  generate_push(RAX);
  
  return;
}


/**
   Generate assembler of number code
 p*/
void generate_num(Node *node) {
#ifdef CC0_DEBUG
  fprintf(stderr, "%d -> ", node->value);
#endif
  printf("  push %d\n", node->value);

  return;
}


/**
   Generate assembler of identifier code
 */
void generate_ident(Node *node) {
  if (node->type == NODE_IDENT) {
#ifdef CC0_DEBUG
    fprintf(stderr, "%c -> ", node->value);
#endif
    ; // identifier operation

  }
  
  return;  
}


/**
   Generate assembler of program code
 */
void generate_code(Node *node) {
  if (node->type == NODE_NUM) {
    generate_num(node);
    return;   // return because leaf node.
  }

  if (node->type == NODE_IDENT) {
    ;
    return;
  }

  if (node->type == '=') {
    generate_ident(node->left);
    generate_code(node->right);
#ifdef CC0_DEBUG
    fprintf(stderr, "%c -> ", node->type);
#endif
    printf("  mov [rbp - 8], rax\n");  
    return;
  }
  
  // internal node
  generate_code(node->left);   // trace left hand tree and genarate code
  generate_code(node->right);  // trace right hand tree and generate code

  generate_op(node);

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
