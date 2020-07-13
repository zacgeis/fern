#include "compiler.h"

#include "utils.h"

char* Compile(Node* node) {
  StrBuf* buf = StrBufNew();

  StrBufAppend(buf, "global _main\n");
  StrBufAppend(buf, "\n");

  StrBufAppend(buf, "section .data\n");
  StrBufAppend(buf, "m_test: db \"test\", 10\n");
  StrBufAppend(buf, "\n");

  StrBufAppend(buf, "section .text\n");
  StrBufAppend(buf, "_main:\n");

  StrBufAppend(buf, "mov rax, 0x2000004 ; write\n");
  StrBufAppend(buf, "mov rdi, 1 ; 1 stdout\n");
  StrBufAppend(buf, "mov rsi, m_test\n");
  StrBufAppend(buf, "mov rdx, 5 ; message length\n");
  StrBufAppend(buf, "syscall\n");
  StrBufAppend(buf, "\n");

  StrBufAppend(buf, "mov rax, 0x2000001 ; exit\n");
  StrBufAppend(buf, "mov rdi, 0 ; 0 exit code\n");
  StrBufAppend(buf, "syscall\n");

  return StrBufBuild(buf);
}
