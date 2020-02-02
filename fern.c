#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   Utility functions
*/

char* ReadFile(const char* filename) {
  FILE* file_handle = fopen(filename, "r");
  if (file_handle == NULL) {
    printf("[ERROR] Failed to open file.\n");
    return NULL;
  }

  fseek(file_handle, 0, SEEK_END);
  int file_size = ftell(file_handle);
  fseek(file_handle, 0, SEEK_SET);

  char* buffer = (char*) malloc(sizeof(char) * (file_size + 1));

  if (buffer == NULL) {
    printf("[ERROR] Failed to allocate buffer.\n");
    return NULL;
  }

  int read_size = fread(buffer, sizeof(char), file_size, file_handle);

  buffer[file_size] = '\0';

  if (read_size != file_size) {
    printf("[ERROR] Failed to read expected size.\n");
    free(buffer);
    return NULL;
  }

  fclose(file_handle);

  return buffer;
}

char* SubStr(char* base, int i, int len) {
  // Create a new str with one additional char for the terminating '\0'.
  char* str = (char*) malloc((len + 1) * sizeof(char));
  memcpy(str, base + i, len * sizeof(char));
  str[len] = '\0';
  return str;
}

/*
  Tokenizer
*/

typedef enum {
  kIntLiteral,
  kPlus,
  kMinus,
} TokenType;

typedef struct {
  TokenType type;
  char* value;
} Token;

typedef struct {
  Token* tokens;
  int length;
  int capacity;
} TokenList;

Token* TokenListAppend(TokenList* list) {
  if (list->length == list->capacity) {
    list->capacity *= 2;
    list->tokens = (Token*) realloc(list->tokens, list->capacity * sizeof(Token));
  }
  return &list->tokens[list->length++];
}

TokenList Tokenize(char* input) {
  TokenList list;
  list.length = 0;
  list.capacity = 100;
  list.tokens = (Token*) malloc(list.capacity * sizeof(Token));

  Token* token = NULL;
  for (int i = 0; input[i] != '\0'; i++) {
    char c = input[i];
    if (c == ' ' || c == '\n' || c == '\t') {
      continue;
    } else if (c == '+') {
      token = TokenListAppend(&list);
      token->type = kPlus;
      token->value = SubStr(input, i, 1);
    } else if (c == '1') {
      token = TokenListAppend(&list);
      token->type = kIntLiteral;
      token->value = SubStr(input, i, 1);
    } else {
      printf("[ERROR] Bad token.\n");
      // TODO: Add pretty printing of the token in the string here.
    }
  }

  return list;
}

/*
   Main
*/

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("[ERROR] No filed provided.\n");
    return -1;
  }

  char* input = ReadFile(argv[1]);
  TokenList token_list = Tokenize(input);
  printf("Token list length: %d\n", token_list.length);
  printf("Plus token: %s\n", token_list.tokens[1].value);
  free(input);

  return 0;
}
