#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef enum bool bool;
enum bool {false, true};

void Fatal(char* message);
char* FileRead(char* filename);
void FileWrite(char* filename, char* data);
char* StrFmt(char* base, ...);
char* SubStr(char* base, int i, int len);
bool IsDigit(char c);
int StrToInt(char* str);
void PrintIndent(int indent);

typedef struct StrBuf StrBuf;
struct StrBuf {
  char** items;
  int length;
  int capacity;
};
StrBuf* StrBufNew();
void StrBufAppend(StrBuf* buf, char* str);
char* StrBufBuild(StrBuf* buf);

#endif
