#ifndef __0CC_H__
#define __0CC_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
   Display error message
 */
#define ERROR(fmt, ...)					\
  fprintf(stderr, "%s(%d): ", __FILE__, __LINE__);	\
  fprintf(stderr, fmt, ##__VA_ARGS__);			\
  exit(1);


/**
 * Declare data type
 */

/**
   type of node
*/
enum {
      NODE_NUM = 256, // integer node
      NODE_IDENT,     // identifier node
};


/**
   node data type
*/
typedef struct Node {
  int type;            // type of node
  struct Node *left;   // left hand side node
  struct Node *right;  // right hand side node
  int value;
} Node;


/**
   type of token
*/
enum {
      TOKEN_NUM = 256, // integer token
      TOKEN_IDENT,     // identifier token
      TOKEN_EOT,       // end of token
};


/**
   token data type
*/
typedef struct {
  int type;   // type of token
  int value;  // token value
  char *input; // token string (for error message)
} Token;



/**
 * Declare function
 */
Node *parser(void);

Node *new_ident_node(int ident);
Node *new_op_node(int op, Node *left, Node *right);
Node *new_num_node(int value);
void delete_node(Node *node);

Node *factor(void);
Node *term(void);
Node *expr(void);
Node *program(void);

void tokenize(char *p);
void print_token(Token *tokens);

void generate_start(void);
void generate_return(void);

void generate_num(Node *node);
void generate_op(Node *node);
void generate_ident(Node *node);
void generate_code(Node *node);
void generator(Node *node);

void error(int i);

/**
 *  Declare global data
 */
extern int p;
extern Token tokens[100];  // result of tokenized


#endif // __0CC_H__
