#ifndef PARSER_H_
#define PARSER_H_

#include "tokenizer.h"

typedef enum {
  kNodeTypeEmpty,
  kNodeTypeIntLiteral,
  kNodeTypeBinaryOp,
} NodeType;

typedef enum {
  kBinaryOpTypePlus,
  kBinaryOpTypeMinus,
} BinaryOpType;

typedef struct {
  NodeType type;
  void* typed_node;
} Node;

typedef struct {
  BinaryOpType op_type;
  Node* left;
  Node* right;
} BinaryOpNode;

typedef struct {
  int value;
} IntLiteralNode;


char* NodeTypeToStr(NodeType type);
char* BinaryOpTypeToStr(BinaryOpType type);

Node* EmptyNode();

Node* Parse(TokenList* token_list);

void NodePrint(Node* node, int indent);

#endif
