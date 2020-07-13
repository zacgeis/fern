#include "utils.h"

void Fatal(char* message) {
  printf("[fatal error]: %s\n", message);
  exit(1);
}

char* FileRead(char* filename) {
  FILE* file_handle = fopen(filename, "r");
  if (file_handle == NULL) {
    Fatal("Failed to open file.");
  }

  fseek(file_handle, 0, SEEK_END);
  int file_size = ftell(file_handle);
  fseek(file_handle, 0, SEEK_SET);

  char* buffer = (char*) malloc(sizeof(char) * (file_size + 1));

  if (buffer == NULL) {
    Fatal("Failed to allocate buffer.");
  }

  int read_size = fread(buffer, sizeof(char), file_size, file_handle);

  buffer[file_size] = '\0';

  if (read_size != file_size) {
    Fatal("Failed to read expected size.");
  }

  fclose(file_handle);

  return buffer;
}

void FileWrite(char* filename, char* data) {
  FILE* file_handle = fopen(filename, "w");
  if (file_handle == NULL) {
    Fatal("Failed to open file.");
  }

  int data_length = strlen(data);

  int write_size = fwrite(data, sizeof(char), data_length, file_handle);

  if (write_size != data_length) {
    Fatal("Write length unexpected.");
  }

  fclose(file_handle);
}

char* StrFmt(char* message, ...) {
  va_list args;

  int message_length = strlen(message);
  int result_length = 0;
  // + 1 for null terminator.
  result_length += message_length + 1;

  // Determine final length of result string to simplify allocation.
  va_start(args, message);
  for (int i = 0; i < message_length; i++) {
    if (message[i] == '$') {
      char* arg = va_arg(args, char*);
      result_length += strlen(arg);
    }
  }
  va_end(args);

  // Copy base and arg strings into results.
  char* result = (char*) malloc(result_length * sizeof(char));
  int result_i = 0;
  int message_copied = 0;
  va_start(args, message);
  for (int message_i = 0; message_i < message_length; message_i++) {
    if (message[message_i] == '$') {
      char* arg = va_arg(args, char*);
      int arg_length = strlen(arg);
      memcpy(result + result_i, arg, arg_length * sizeof(char));
      result_i += arg_length;
      // Skip the '$'.
      message_copied++;
    } else {
      result[result_i] = message[message_i];
      result_i++;
    }
  }
  va_end(args);

  result[result_length] = '\0';

  return result;
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

StrBuf* StrBufNew() {
  StrBuf* buf = (StrBuf*) malloc(sizeof(StrBuf));
  buf->length = 0;
  buf->capacity = 10;
  buf->items = (char**) malloc(sizeof(char*) * buf->capacity);
  return buf;
}

void StrBufAppend(StrBuf* buf, char* str) {
  if (buf->length == buf->capacity) {
    buf->capacity *= 2;
    buf->items = (char**) realloc(buf->items, sizeof(char*) * buf->capacity);
  }
  buf->items[buf->length++] = str;
}

char* StrBufBuild(StrBuf* buf) {
  int result_length = 0;
  for (int i = 0; i < buf->length; i++) {
    result_length += strlen(buf->items[i]);
  }
  // Terminating character.
  result_length += 1;
  char* result = (char*) malloc(result_length * sizeof(char));
  int result_i = 0;
  for (int i = 0; i < buf->length; i++) {
    int item_length = strlen(buf->items[i]);
    memcpy(result + result_i, buf->items[i], item_length);
    result_i += item_length;
  }
  result[result_length] = '\0';
  return result;
}
