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
    case kBinaryOpTypePlus:
      return "kBinaryOpTypePlus";
    case kBinaryOpTypeMinus:
      return "kBinaryOpTypeMinus";
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
Node* ParseExpressionAddOrMinus(ParseState* state);
// Node* ParseExpressionMultiplyOrDivide(ParseState* state);
// Node* ParseExpression(ParseState* state);

Node* ParseExpressionAddOrMinus(ParseState* state) {
  Node* left = EmptyNode();
  Node* right = EmptyNode();

  if (Peek(state)->type == kTokenTypeEOF) {
    return EmptyNode();
  }

  /* if (Peek(state)->type == kTokenTypeOpenParen) { */
  /*   Match(state, kTokenTypeOpenParen); */
  /*   left = ParseExpressionL0(state); */
  /*   Match(state, kTokenTypeCloseParen); */
  /* } else { */
    // Attempt to parse higher precedence first.
  // }

  left = ParseExpressionAddOrMinus(state);

  if (Peek(state)->type == kTokenTypeMinus) {
    Next(state);
  } else {
    return left;
  }

  right = ParseExpressionAddOrMinus(state);

  return BinaryOpNodeNew(kBinaryOpTypeMinus, left, right);
}

// Node* ParseExpressionMultiplyOrDivide(ParseState* state) {
//   Node* left = EmptyNode();
//   Node* right = EmptyNode();
//
//   if (Peek(state)->type == kTokenTypeEOF) {
//     return EmptyNode();
//   }
//
//   if (Peek(state)->type == kTokenTypeIntLiteral) {
//     left = ParseIntLiteral(state);
//   }
//
//   if (Peek(state)->type == kTokenTypePlus) {
//     Next(state);
//   } else {
//     return left;
//   }
//
//   // We're on addition, only parse addition or higher precedence to the right.
//   right = ParseExpressionL1(state);
//
//   return BinaryOpNodeNew(kBinaryOpTypePlus, left, right);
// }

Node* ParseIntLiteral(ParseState* state) {
  Token* token = Next(state);
  int value = StrToInt(token->value);
  return IntLiteralNodeNew(value);
}

Node* Parse(TokenList* token_list) {
  ParseState parse_state;
  parse_state.i = 0;
  parse_state.token_list = token_list;

  return ParseExpressionAddOrMinus(&parse_state);
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
  NodePrint(node->left, indent + 2);
  NodePrint(node->right, indent + 2);
}

void IntLiteralNodePrint(IntLiteralNode* node, int indent) {
  PrintIndent(indent);
  printf("[IntLiteralNode] value: %d\n", node->value);
}
