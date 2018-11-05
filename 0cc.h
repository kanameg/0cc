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
 * Define data type
 */

/**
   type of node
*/
enum {
      NODE_NUM = 256, // integer node
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
 * Define function
 */
Node *new_op_node(int op, Node *left, Node *right);
Node *new_num_node(int value);
void delete_node(Node *node);

Node *factor();
Node *term();
Node *expr();

void tokenize(char *p);
void print_token(Token *tokens);

void error(int i);

/**
 *  Global data
 */
int p = 0;
Token tokens[100];  // result of tokenized


#endif // __0CC_H__
