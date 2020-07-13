#ifndef TOKENIZER_H_
#define TOKENIZER_H_

typedef enum TokenType TokenType;
enum TokenType {
  kTokenTypeEmpty,
  kTokenTypeIntLiteral,
  kTokenTypePlus,
  kTokenTypeMinus,
  kTokenTypeAsterisk,
  kTokenTypeSlash,
  kTokenTypeOpenParen,
  kTokenTypeCloseParen,
};

typedef struct Token Token;
struct Token {
  TokenType type;
  char* value;
};

typedef struct TokenList TokenList;
struct TokenList {
  Token* tokens;
  int length;
  int capacity;
};

char* TokenTypeToStr(TokenType type);

Token* EmptyToken();

TokenList* Tokenize(char* input);

void TokenListPrint(TokenList* list);

#endif
