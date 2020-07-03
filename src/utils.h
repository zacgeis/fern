#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {false, true} bool;

char* ReadFile(const char* filename);
char* SubStr(char* base, int i, int len);
bool IsDigit(char c);
int StrToInt(char* str);
void PrintIndent(int indent);

#endif
