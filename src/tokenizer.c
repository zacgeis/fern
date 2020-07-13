#include "tokenizer.h"
#include "utils.h"

char* TokenTypeToStr(TokenType type) {
  switch (type) {
    case kTokenTypeEmpty:
      return "kTokenTypeEmpty";
    case kTokenTypeIntLiteral:
      return "kTokenTypeIntLiteral";
    case kTokenTypePlus:
      return "kTokenTypePlus";
    case kTokenTypeMinus:
      return "kTokenTypeMinus";
    case kTokenTypeAsterisk:
      return "kTokenTypeAsterisk";
    case kTokenTypeSlash:
      return "kTokenTypeSlash";
    case kTokenTypeOpenParen:
      return "kTokenTypeOpenParen";
    case kTokenTypeCloseParen:
      return "kTokenTypeCloseParen";
    default:
      return "UnknownToken";
  }
}

Token* EmptyToken() {
  static Token token = {
    .type = kTokenTypeEmpty,
    .value = "",
  };
  return &token;
}

TokenList* TokenListNew() {
  TokenList* list = (TokenList*) malloc(sizeof(TokenList));
  list->length = 0;
  list->capacity = 100;
  list->tokens = (Token*) malloc(list->capacity * sizeof(Token));
  return list;
}

Token* TokenListAppend(TokenList* list) {
  if (list->length == list->capacity) {
    list->capacity *= 2;
    list->tokens = (Token*) realloc(list->tokens, list->capacity * sizeof(Token));
  }
  return &list->tokens[list->length++];
}

TokenList* Tokenize(char* input) {
  TokenList* list = TokenListNew();

  Token* token = EmptyToken();
  for (int i = 0; input[i] != '\0'; i++) {
    char c = input[i];
    if (c == ' ' || c == '\n' || c == '\t') {
      continue;
    } else if (c == '+') {
      token = TokenListAppend(list);
      token->type = kTokenTypePlus;
      token->value = SubStr(input, i, 1);
    } else if (c == '-') {
      token = TokenListAppend(list);
      token->type = kTokenTypeMinus;
      token->value = SubStr(input, i, 1);
    } else if (c == '*') {
      token = TokenListAppend(list);
      token->type = kTokenTypeAsterisk;
      token->value = SubStr(input, i, 1);
    } else if (c == '/') {
      token = TokenListAppend(list);
      token->type = kTokenTypeSlash;
      token->value = SubStr(input, i, 1);
    } else if (c == '(') {
      token = TokenListAppend(list);
      token->type = kTokenTypeOpenParen;
      token->value = SubStr(input, i, 1);
    } else if (c == ')') {
      token = TokenListAppend(list);
      token->type = kTokenTypeCloseParen;
      token->value = SubStr(input, i, 1);
    } else if (IsDigit(c)) {
      token = TokenListAppend(list);
      token->type = kTokenTypeIntLiteral;
      int j = i;
      while (input[j] != '\0' && IsDigit(input[j])) {
        j++;
      }
      token->value = SubStr(input, i, j - i);
      i = j - 1;
    } else {
      Fatal("Tokenizer found unexpected token.");
    }
  }

  return list;
}

void TokenListPrint(TokenList* list) {
  printf("[TokenList] size: %d\n", list->length);
  for (int i = 0; i < list->length; i++) {
    Token* token = &list->tokens[i];
    char* token_type = TokenTypeToStr(token->type);
    printf("  [%s] value: '%s'\n", token_type, token->value);
  }
  printf("\n");
}
