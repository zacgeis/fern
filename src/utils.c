#include "utils.h"

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

bool IsDigit(char c) {
  return c >= '0' && c <= '9';
}

int StrToInt(char* str) {
  int value = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    value *= 10; // Multiply first, avoid overflow.
    value += str[i] - '0';
  }
  return value;
}

void PrintIndent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
}
