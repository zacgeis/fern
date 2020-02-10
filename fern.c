#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   Utility functions
*/

#define kIndent 2

typedef enum {false, true} bool;

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

bool IsDigit(char c) {
  return c >= '0' && c <= '9';
}

int StrToInt(char* str) {
  int value = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    value *= 10; // Multiply first, avoid overflow.
    value += str[i] - '0';
  }
  return value;
}

void PrintIndent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
}

/*
  Tokenizer
*/

typedef enum {
  kTokenTypeEOF,
  kTokenTypeIntLiteral,
  kTokenTypePlus,
  kTokenTypeMinus,
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

Token* EOFToken() {
  static Token token;
  static bool init = true;
  if (init) {
    token.type = kTokenTypeEOF;
    token.value = "";
    init = false;
  }
  return &token;
}

char* TokenTypeToStr(TokenType type) {
  switch (type) {
    case kTokenTypeEOF:
      return "kTokenTypeEOF";
    case kTokenTypeIntLiteral:
      return "kTokenTypeIntLiteral";
    case kTokenTypePlus:
      return "kTokenTypePlus";
    case kTokenTypeMinus:
      return "kTokenTypeMinus";
    case kTokenTypeOpenParen:
      return "kTokenTypeOpenParen";
    case kTokenTypeCloseParen:
      return "kTokenTypeCloseParen";
    default:
      return "UNKNOWN";
  }
}

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
      token->type = kTokenTypePlus;
      token->value = SubStr(input, i, 1);
    } else if (c == '-') {
      token = TokenListAppend(&list);
      token->type = kTokenTypeMinus;
      token->value = SubStr(input, i, 1);
    } else if (c == '(') {
      token = TokenListAppend(&list);
      token->type = kTokenTypeOpenParen;
      token->value = SubStr(input, i, 1);
    } else if (c == ')') {
      token = TokenListAppend(&list);
      token->type = kTokenTypeCloseParen;
      token->value = SubStr(input, i, 1);
    } else if (IsDigit(c)) {
      token = TokenListAppend(&list);
      token->type = kTokenTypeIntLiteral;
      int j = i;
      while (input[j] != '\0' && IsDigit(input[j])) {
        j++;
      }
      token->value = SubStr(input, i, j - i);
      i = j - 1;
    } else {
      printf("[ERROR] Bad token.\n");
      // TODO: Add pretty printing of the token in the string here.
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

/*
   Parser
*/

typedef enum {
  kNodeTypeEmpty,
  kNodeTypeIntLiteral,
  kNodeTypeBinaryOp,
} NodeType;

char* NodeTypeToStr(NodeType type) {
  switch (type) {
    case kNodeTypeEmpty:
      return "kNodeTypeEmpty";
    case kNodeTypeIntLiteral:
      return "kNodeTypeIntLiteral";
    case kNodeTypeBinaryOp:
      return "kNodeTypeBinaryOp";
    default:
      return "UNKNOWN";
  }
}

typedef enum {
  kBinaryOpTypePlus,
  kBinaryOpTypeMinus,
} BinaryOpType;

char* BinaryOpTypeToStr(BinaryOpType type) {
  switch (type) {
    case kBinaryOpTypePlus:
      return "kBinaryOpTypePlus";
    case kBinaryOpTypeMinus:
      return "kBinaryOpTypeMinus";
    default:
      return "UNKNOWN";
  }
}

typedef struct {
  NodeType type;
  void* typed_node;
} Node;

Node* EmptyNode() {
  static Node node;
  static bool init = true;
  if (init) {
    node.type = kNodeTypeEmpty;
    node.typed_node = NULL;
    init = false;
  }
  return &node;
}

typedef struct {
  BinaryOpType op_type;
  Node* left;
  Node* right;
} BinaryOpNode;

Node* BinaryOpNodeNew(BinaryOpType type, Node* left, Node* right) {
  Node* node = (Node*) malloc(sizeof(Node));
  node->type = kNodeTypeBinaryOp;
  node->typed_node = (void*) malloc(sizeof(BinaryOpNode));
  BinaryOpNode* node_binary_op = (BinaryOpNode*) node->typed_node;
  node_binary_op->op_type = type;
  node_binary_op->left = left;
  node_binary_op->right = right;
  return node;
}

typedef struct {
  int value;
} IntLiteralNode;

Node* IntLiteralNodeNew(int value) {
  Node* node = (Node*) malloc(sizeof(Node));
  node->type = kNodeTypeIntLiteral;
  node->typed_node = (void*) malloc(sizeof(IntLiteralNode));
  IntLiteralNode* node_int_literal = (IntLiteralNode*) node->typed_node;
  node_int_literal->value = value;
  return node;
}

typedef struct {
  TokenList* token_list;
  int i;
} ParseState;

Token* Peek(ParseState* state) {
  if (state->i == state->token_list->length) {
    return EOFToken();
  }
  return &state->token_list->tokens[state->i];
}

Token* Next(ParseState* state) {
  if (state->i == state->token_list->length) {
    return EOFToken();
  }
  return &state->token_list->tokens[state->i++];
}

Token* Match(ParseState* state, TokenType token_type) {
  Token* token = Peek(state);
  if (token->type == token_type) {
    return Next(state);
  }

  // TODO: Add pretty print error message here with file contents.
  printf("[ERROR] Missing expected parse token: %s\n", TokenTypeToStr(token_type));

  // TODO: Handle match error here. Stop parsing...
  return EOFToken();
}

Node* ParseIntLiteral(ParseState* state);
Node* ParseExpressionL0(ParseState* state);
Node* ParseExpressionL1(ParseState* state);

// TODO: Instead or parsing int literals within L0 and L1, we should be
//       parsing another level that includes parens and literals.
//       Something like L2?

Node* ParseExpressionL0(ParseState* state) {
  Node* left = EmptyNode();
  Node* right = EmptyNode();

  if (Peek(state)->type == kTokenTypeEOF) {
    return EmptyNode();
  }

  if (Peek(state)->type == kTokenTypeOpenParen) {
    Match(state, kTokenTypeOpenParen);
    left = ParseExpressionL0(state);
    Match(state, kTokenTypeCloseParen);
  } else {
    // Attempt to parse higher precedence first.
    left = ParseExpressionL1(state);
  }

  if (Peek(state)->type == kTokenTypeMinus) {
    Next(state);
  } else {
    return left;
  }

  right = ParseExpressionL0(state);

  return BinaryOpNodeNew(kBinaryOpTypeMinus, left, right);
}

Node* ParseExpressionL1(ParseState* state) {
  Node* left = EmptyNode();
  Node* right = EmptyNode();

  if (Peek(state)->type == kTokenTypeEOF) {
    return EmptyNode();
  }

  if (Peek(state)->type == kTokenTypeIntLiteral) {
    left = ParseIntLiteral(state);
  }

  if (Peek(state)->type == kTokenTypePlus) {
    Next(state);
  } else {
    return left;
  }

  // We're on addition, only parse addition or higher precedence to the right.
  right = ParseExpressionL1(state);

  return BinaryOpNodeNew(kBinaryOpTypePlus, left, right);
}

Node* ParseIntLiteral(ParseState* state) {
  Token* token = Next(state);
  int value = StrToInt(token->value);
  return IntLiteralNodeNew(value);
}

Node* Parse(TokenList* token_list) {
  ParseState parse_state;
  parse_state.i = 0;
  parse_state.token_list = token_list;

  return ParseExpressionL0(&parse_state);
}

void EmptyNodePrint(Node* node, int indent);
void BinaryOpNodePrint(BinaryOpNode* node, int indent);
void IntLiteralNodePrint(IntLiteralNode* node, int indent);

void NodePrint(Node* node, int indent) {
  switch (node->type) {
    case kNodeTypeEmpty:
      EmptyNodePrint(node, indent);
      break;
    case kNodeTypeIntLiteral:
      IntLiteralNodePrint((IntLiteralNode*) node->typed_node, indent);
      break;
    case kNodeTypeBinaryOp:
      BinaryOpNodePrint((BinaryOpNode*) node->typed_node, indent);
      break;
    default:
      printf("[ERROR] Attempting to print unknown node type: %d\n", node->type);
  }
}

void EmptyNodePrint(Node* node, int indent) {
  PrintIndent(indent);
  printf("[EmptyNode]\n");
}

void BinaryOpNodePrint(BinaryOpNode* node, int indent) {
  PrintIndent(indent);
  printf("[BinaryOpNode] binary_op: %s\n", BinaryOpTypeToStr(node->op_type));
  NodePrint(node->left, indent + kIndent);
  NodePrint(node->right, indent + kIndent);
}

void IntLiteralNodePrint(IntLiteralNode* node, int indent) {
  PrintIndent(indent);
  printf("[IntLiteralNode] value: %d\n", node->value);
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
  TokenListPrint(&token_list);

  Node* node = Parse(&token_list);

  // TODO: test parser with new parse state.
  // free(input);

  /*
  Node* n1 = IntLiteralNodeNew(1);
  Node* n2 = IntLiteralNodeNew(1);
  Node* n3 = BinaryOpNodeNew(kBinaryOpTypePlus, n1, n2);
  */

  NodePrint(node, 0);

  return 0;
}
