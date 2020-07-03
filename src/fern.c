#include "utils.h"
#include "tokenizer.h"
#include "parser.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("[ERROR] No filed provided.\n");
    return -1;
  }

  char* input = ReadFile(argv[1]);
  TokenList* token_list = Tokenize(input);
  TokenListPrint(token_list);

  Node* node = Parse(token_list);

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
