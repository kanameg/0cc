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
   main function
*/
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Not correct number of argument.\n");
    return 1;
  }
  
  // lexical analysis    make tokens
  tokenize(argv[1]);
  print_token(tokens);

  // syntactic analysis  make abstract syntax tree.
  Node *node = parser();

  // code generator
  generator(node);
  
  return 0;
}
