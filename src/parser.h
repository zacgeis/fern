#ifndef PARSER_H_
#define PARSER_H_

#include "tokenizer.h"

typedef enum NodeType NodeType;
enum NodeType {
  kNodeTypeEmpty,
  kNodeTypeIntLiteral,
  kNodeTypeBinaryOp,
};

typedef enum BinaryOpType BinaryOpType;
enum BinaryOpType {
  kBinaryOpTypeMul,
  kBinaryOpTypeDiv,
  kBinaryOpTypeAdd,
  kBinaryOpTypeSub,
};

typedef struct Node Node;
struct Node {
  NodeType type;
  void* typed_node;
};

typedef struct BinaryOpNode BinaryOpNode;
struct BinaryOpNode {
  BinaryOpType op_type;
  Node* left;
  Node* right;
};

typedef struct IntLiteralNode IntLiteralNode;
struct IntLiteralNode {
  int value;
};

char* NodeTypeToStr(NodeType type);
char* BinaryOpTypeToStr(BinaryOpType type);

Node* EmptyNode();

Node* Parse(TokenList* token_list);

void NodePrint(Node* node, int indent);

#endif
