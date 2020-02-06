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

void PrintIndent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
}

/*
  Tokenizer
*/

typedef enum {
  kTokenEOF,
  kTokenIntLiteral,
  kTokenPlus,
  kTokenMinus,
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

Token* TokenEOF() {
  static Token token;
  token.type = kTokenEOF;
  token.value = "";
  return &token;
}

char* TokenTypeStr(TokenType type) {
  switch (type) {
    case kTokenEOF:
      return "kTokenEOF";
    case kTokenIntLiteral:
      return "kTokenIntLiteral";
    case kTokenPlus:
      return "kTokenPlus";
    case kTokenMinus:
      return "kTokenMinus";
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
      token->type = kTokenPlus;
      token->value = SubStr(input, i, 1);
    } else if (c == '-') {
      token = TokenListAppend(&list);
      token->type = kTokenMinus;
      token->value = SubStr(input, i, 1);
    } else if (c == '1') {
      token = TokenListAppend(&list);
      token->type = kTokenIntLiteral;
      token->value = SubStr(input, i, 1);
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
    char* token_type = TokenTypeStr(token->type);
    printf("  [%s] value: '%s'\n", token_type, token->value);
  }
  printf("\n");
}

/*
   Parser
*/

typedef enum {
  kNodeEmpty,
  kNodeIntLiteral,
  kNodeBinaryOp,
} NodeType;

char* NodeTypeStr(NodeType type) {
  switch (type) {
    case kNodeEmpty:
      return "kNodeEmpty";
    case kNodeIntLiteral:
      return "kNodeIntLiteral";
    case kNodeBinaryOp:
      return "kNodeBinaryOp";
    default:
      return "UNKNOWN";
  }
}

typedef enum {
  kBinaryOpPlus,
  kBinaryOpMinus,
} BinaryOpType;

char* BinaryOpTypeStr(BinaryOpType type) {
  switch (type) {
    case kBinaryOpPlus:
      return "kBinaryOpPlus";
    case kBinaryOpMinus:
      return "kBinaryOpMinus";
    default:
      return "UNKNOWN";
  }
}

typedef struct {
  NodeType type;
  void* typed_node;
} Node;

Node* NodeEmpty() {
  static Node node;
  node.type = kNodeEmpty;
  node.typed_node = NULL;

  return &node;
}

typedef struct {
  BinaryOpType op_type;
  Node* left;
  Node* right;
} NodeBinaryOp;

Node* NodeBinaryOpNew(BinaryOpType type, Node* left, Node* right) {
  Node* node = (Node*) malloc(sizeof(Node));
  node->type = kNodeBinaryOp;
  node->typed_node = (void*) malloc(sizeof(NodeBinaryOp));
  NodeBinaryOp* node_binary_op = (NodeBinaryOp*) node->typed_node;
  node_binary_op->op_type = type;
  node_binary_op->left = left;
  node_binary_op->right = right;
  return node;
}

typedef struct {
  int value;
} NodeIntLiteral;

Node* NodeIntLiteralNew(int value) {
  Node* node = (Node*) malloc(sizeof(Node));
  node->type = kNodeIntLiteral;
  node->typed_node = (void*) malloc(sizeof(NodeIntLiteral));
  NodeIntLiteral* node_int_literal = (NodeIntLiteral*) node->typed_node;
  node_int_literal->value = value;
  return node;
}

typedef struct {
  TokenList* token_list;
  int i;
} ParseState;

Token* Peek(ParseState* state) {
  if (state->i == state->token_list->length) {
    return TokenEOF();
  }
  return &state->token_list->tokens[state->i];
}

Token* Next(ParseState* state) {
  if (state->i == state->token_list->length) {
    return TokenEOF();
  }
  return &state->token_list->tokens[state->i++];
}

Token* Match(ParseState* state, TokenType token_type) {
  Token* token = Peek(state);
  if (token->type == token_type) {
    return Next(state);
  }

  // TODO: Handle match error here.
  return TokenEOF();
}

Node* ParseIntLiteral(ParseState* state);
Node* ParseExpressionL0(ParseState* state);
Node* ParseExpressionL1(ParseState* state);

Node* ParseExpressionL0(ParseState* state) {
  Node* left = NodeEmpty();
  Node* right = NodeEmpty();

  if (Peek(state)->type == kTokenEOF) {
    return NodeEmpty();
  }

  left = ParseExpressionL1(state);

  if (Peek(state)->type == kTokenMinus) {
    Next(state);
  } else {
    return left;
  }

  right = ParseExpressionL0(state);

  return NodeBinaryOpNew(kBinaryOpMinus, left, right);
}

Node* ParseExpressionL1(ParseState* state) {
  Node* left = NodeEmpty();
  Node* right = NodeEmpty();

  if (Peek(state)->type == kTokenEOF) {
    return NodeEmpty();
  }

  if (Peek(state)->type == kTokenIntLiteral) {
    left = ParseIntLiteral(state);
  }

  if (Peek(state)->type == kTokenPlus) {
    Next(state);
  } else {
    return left;
  }

  right = ParseExpressionL0(state);

  return NodeBinaryOpNew(kBinaryOpPlus, left, right);
}

Node* ParseIntLiteral(ParseState* state) {
  Token* token = Next(state);
  return NodeIntLiteralNew(1);
}

Node* Parse(TokenList* token_list) {
  ParseState parse_state;
  parse_state.i = 0;
  parse_state.token_list = token_list;

  return ParseExpressionL0(&parse_state);
}

void NodeEmptyPrint(Node* node, int indent);
void NodeBinaryOpPrint(NodeBinaryOp* node, int indent);
void NodeIntLiteralPrint(NodeIntLiteral* node, int indent);

void NodePrint(Node* node, int indent) {
  switch (node->type) {
    case kNodeEmpty:
      NodeEmptyPrint(node, indent);
      break;
    case kNodeIntLiteral:
      NodeIntLiteralPrint((NodeIntLiteral*) node->typed_node, indent);
      break;
    case kNodeBinaryOp:
      NodeBinaryOpPrint((NodeBinaryOp*) node->typed_node, indent);
      break;
    default:
      printf("[ERROR] Attempting to print unknown node type: %d\n", node->type);
  }
}

void NodeEmptyPrint(Node* node, int indent) {
  PrintIndent(indent);
  printf("[NodeEmpty]\n");
}

void NodeBinaryOpPrint(NodeBinaryOp* node, int indent) {
  PrintIndent(indent);
  printf("[NodeBinaryOp] binary_op: %s\n", BinaryOpTypeStr(node->op_type));
  NodePrint(node->left, indent + kIndent);
  NodePrint(node->right, indent + kIndent);
}

void NodeIntLiteralPrint(NodeIntLiteral* node, int indent) {
  PrintIndent(indent);
  printf("[NodeIntLiteral] value: %d\n", node->value);
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
  Node* n1 = NodeIntLiteralNew(1);
  Node* n2 = NodeIntLiteralNew(1);
  Node* n3 = NodeBinaryOpNew(kBinaryOpPlus, n1, n2);
  */

  NodePrint(node, 0);

  return 0;
}
