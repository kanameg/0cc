/**
 * C compiler 9cc
 * kaname.g@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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


Token tokens[100];  // result of tokenized


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
    if (*p == '+' || *p == '-') {
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
  while (tokens[i].type != TOKEN_EOT) {
    fprintf(stderr, "Token[%d]\n", i);
    if (tokens[i].type == TOKEN_NUM)
      fprintf(stderr, "	value: %d\n", tokens[i].value);
    if (tokens[i].type == '+' || tokens[i].type == '-')
      fprintf(stderr, "	value: %c\n", tokens[i].value);
    i++;
  }
  fprintf(stderr, "EOT\n\n");
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
  //print_token(tokens);
  
  printf(".intel_syntax noprefix\n");
  printf("\n");
  printf(".global _main\n");
  printf("\n");
  printf(".text\n");
  printf("_main:\n");

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

