#include <malloc_dbg.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "sds.h"
#include "print.h"

void sigintHandler(int sig) {
  if (sig == SIGINT) {
    // ctrl+c退出时执行的代码
    printf("ctrl+c pressed!\n");
    // 打印内存泄漏信息
    printLeaks();
  }
}

typedef struct {
  char* buffer;
  size_t bufferLength;
  ssize_t inputLength;
} InputBuffer;

InputBuffer* newInputBuffer() {
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->bufferLength = 0;
  input_buffer->inputLength = 0;

  return input_buffer;
}

void printPrompt() { printf("db > "); }

void readInput(sds *inputBuffer) {
  int len;
  ssize_t bytesRead = getline(inputBuffer, &len, stdin);

  if (bytesRead <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }
  DBG_PRINTF(*inputBuffer);

  // Ignore trailing newline
  // inputBuffer->inputLength = bytesRead - 1;
  // inputBuffer->buffer[bytesRead - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sigintHandler);
  sds inputBuffer = NULL;
  
  while (true) {
    printPrompt();
    readInput(&inputBuffer);
    DBG_PRINTF(inputBuffer);
    if (strcmp(inputBuffer, ".exit") == 0) {
      close_input_buffer(inputBuffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", inputBuffer);
    }
  }
}