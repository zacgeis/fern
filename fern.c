#include <stdio.h>
#include <stdlib.h>

typedef enum {
  NUMBER_LITERAL,
  PLUS,
} Token;

void PrintError(const char* message) {
  printf("[ERROR] %s\n", message);
}

char* ReadFile(const char* filename) {
  FILE* file_handle = fopen(filename, "r");
  if (file_handle == NULL) {
    PrintError("Failed to open file.");
    return NULL;
  }

  fseek(file_handle, 0, SEEK_END);
  int file_size = ftell(file_handle);
  fseek(file_handle, 0, SEEK_SET);

  char* buffer = (char*) malloc(sizeof(char) * (file_size + 1));

  if (buffer == NULL) {
    PrintError("Failed to allocate buffer.");
    return NULL;
  }

  int read_size = fread(buffer, sizeof(char), file_size, file_handle);

  buffer[file_size] = '\0';

  if (read_size != file_size) {
    PrintError("Failed to read expected size.");
    free(buffer);
    return NULL;
  }

  fclose(file_handle);

  return buffer;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    PrintError("No filed provided.");
    return -1;
  }

  char* input = ReadFile(argv[1]);
  printf("Contents: %s\n", input);
  free(input);

  return 0;
}
