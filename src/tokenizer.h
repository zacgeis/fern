#ifndef TOKENIZER_H_
#define TOKENIZER_H_

typedef enum {
  kTokenTypeEOF,
  kTokenTypeIntLiteral,
  kTokenTypePlus,
  kTokenTypeMinus,
  kTokenTypeMultiply,
  kTokenTypeDivide,
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

Token* EOFToken();

TokenList* Tokenize(char* input);

void TokenListPrint(TokenList* list);

#endif
