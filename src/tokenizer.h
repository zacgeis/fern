#ifndef TOKENIZER_H_
#define TOKENIZER_H_

typedef enum {
  kTokenTypeEmpty,
  kTokenTypeIntLiteral,
  kTokenTypePlus,
  kTokenTypeMinus,
  kTokenTypeAsterisk,
  kTokenTypeSlash,
  kTokenTypeOpenParen,
  kTokenTypeCloseParen,
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

char* TokenTypeToStr(TokenType type);

Token* EmptyToken();

TokenList* Tokenize(char* input);

void TokenListPrint(TokenList* list);

#endif
