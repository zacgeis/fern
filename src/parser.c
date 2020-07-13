#include "parser.h"

#include "utils.h"

typedef struct {
  TokenList* token_list;
  int i;
} ParseState;

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

char* BinaryOpTypeToStr(BinaryOpType type) {
  switch (type) {
    case kBinaryOpTypeMul:
      return "kBinaryOpTypeMul";
    case kBinaryOpTypeDiv:
      return "kBinaryOpTypeDiv";
    case kBinaryOpTypeAdd:
      return "kBinaryOpTypeAdd";
    case kBinaryOpTypeSub:
      return "kBinaryOpTypeSub";
    default:
      return "UnknownNode";
  }
}

Node* EmptyNode() {
  static Node node = {
    .type = kNodeTypeEmpty,
    .typed_node = NULL,
  };
  return &node;
}

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

Node* IntLiteralNodeNew(int value) {
  Node* node = (Node*) malloc(sizeof(Node));
  node->type = kNodeTypeIntLiteral;
  node->typed_node = (void*) malloc(sizeof(IntLiteralNode));
  IntLiteralNode* node_int_literal = (IntLiteralNode*) node->typed_node;
  node_int_literal->value = value;
  return node;
}

Token* Peek(ParseState* state) {
  if (state->i == state->token_list->length) {
    return EmptyToken();
  }
  return &state->token_list->tokens[state->i];
}

Token* Next(ParseState* state) {
  if (state->i == state->token_list->length) {
    return EmptyToken();
  }
  return &state->token_list->tokens[state->i++];
}

Token* Match(ParseState* state, TokenType token_type) {
  Token* token = Peek(state);
  if (token->type == token_type) {
    return Next(state);
  }

  Fatal(StrFmt("Missing expected parse token: $", TokenTypeToStr(token_type)));

  return EmptyToken();
}

// Keep in order of precedence.
Node* ParseIntLiteral(ParseState* state);
Node* ParseExpressionMulOrDiv(ParseState* state);
Node* ParseExpressionAddOrSub(ParseState* state);

Node* ParseIntLiteral(ParseState* state) {
  Token* token = Next(state);
  int value = StrToInt(token->value);
  return IntLiteralNodeNew(value);
}

Node* ParseExpressionMulOrDiv(ParseState* state) {
  Node* left = EmptyNode();
  Node* right = EmptyNode();
  Token* op = EmptyToken();

  left = ParseIntLiteral(state);
  while (Peek(state)->type == kTokenTypeAsterisk
      || Peek(state)->type == kTokenTypeSlash) {
    op = Next(state);
    right = ParseIntLiteral(state);

    switch (op->type) {
      case kTokenTypeAsterisk:
        left = BinaryOpNodeNew(kBinaryOpTypeMul, left, right);
        break;
      case kTokenTypeSlash:
        left = BinaryOpNodeNew(kBinaryOpTypeDiv, left, right);
        break;
      default:
        Fatal("Unexpected parse condition.");
    }
  }

  return left;
}

Node* ParseExpressionAddOrSub(ParseState* state) {
  Node* left = EmptyNode();
  Node* right = EmptyNode();
  Token* op = EmptyToken();

  left = ParseExpressionMulOrDiv(state);
  while (Peek(state)->type == kTokenTypePlus
      || Peek(state)->type == kTokenTypeMinus) {
    op = Next(state);
    right = ParseExpressionMulOrDiv(state);

    switch (op->type) {
      case kTokenTypePlus:
        left = BinaryOpNodeNew(kBinaryOpTypeAdd, left, right);
        break;
      case kTokenTypeMinus:
        left = BinaryOpNodeNew(kBinaryOpTypeSub, left, right);
        break;
      default:
        Fatal("Unexpected parse condition.");
    }
  }

  return left;
}

Node* Parse(TokenList* token_list) {
  ParseState parse_state = {
    .i = 0,
    .token_list = token_list,
  };

  // Start from the lowest precedence.
  return ParseExpressionAddOrSub(&parse_state);
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
      Fatal("Attempting to print unknown node type.");
  }
}

void EmptyNodePrint(Node* node, int indent) {
  PrintIndent(indent);
  printf("[EmptyNode]\n");
}

void BinaryOpNodePrint(BinaryOpNode* node, int indent) {
  PrintIndent(indent);
  printf("[BinaryOpNode] binary_op: %s\n", BinaryOpTypeToStr(node->op_type));
  NodePrint(node->left, indent + 2);
  NodePrint(node->right, indent + 2);
}

void IntLiteralNodePrint(IntLiteralNode* node, int indent) {
  PrintIndent(indent);
  printf("[IntLiteralNode] value: %d\n", node->value);
}
