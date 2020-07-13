#include "utils.h"
#include "tokenizer.h"
#include "parser.h"
#include "compiler.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    Fatal("Usage fern [input] [output].\n");
  }

  char* file_in = argv[1];
  char* file_out = argv[2];

  char* input = FileRead(file_in);
  TokenList* token_list = Tokenize(input);
  TokenListPrint(token_list);

  Node* node = Parse(token_list);
  NodePrint(node, 0);
  char* output = Compile(node);

  FileWrite(file_out, output);

  /*
  Node* n1 = IntLiteralNodeNew(1);
  Node* n2 = IntLiteralNodeNew(1);
  Node* n3 = BinaryOpNodeNew(kBinaryOpTypePlus, n1, n2);
  */

  return 0;
}
