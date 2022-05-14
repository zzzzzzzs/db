#include <malloc_dbg.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "print.h"
#include "sds.h"

void sigintHandler(int sig) {
  if (sig == SIGINT) {
    // ctrl+c退出时执行的代码
    printf("ctrl+c pressed!\n");
    // 打印内存泄漏信息
    printLeaks();
  }
}

// meta command
typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

// prepared statement
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type
} Statement;

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

void readInput(sds* inputBuffer) {
  int len;
  ssize_t bytesRead = getline(inputBuffer, &len, stdin);

  if (bytesRead <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }
  // Remove the last line
  strtok(*inputBuffer, "\n");
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

// check meta-commands
MetaCommandResult doMetaCommand(sds* inputBuffer) {
  if (strcmp(*inputBuffer, ".exit") == 0) {
    printf("program is success exit.\n");
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

// SQL Compiler
PrepareResult prepareStatement(sds* inputBuffer, Statement* statement) {
  if (strncmp(*inputBuffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(*inputBuffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

// execute sql
void executeStatement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case (STATEMENT_SELECT):
      printf("This is where we would do a select.\n");
      break;
  }
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sigintHandler);
  sds inputBuffer = NULL;

  // loop
  while (true) {
    printPrompt();
    readInput(&inputBuffer);

    // meta command
    if (inputBuffer[0] == '.') {
      switch (doMetaCommand(&inputBuffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s' \n", inputBuffer);
          continue;
      }
    }
    Statement statement;
    switch (prepareStatement(&inputBuffer, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n", inputBuffer);
        continue;
    }
    executeStatement(&statement);
    printf("Executed.\n");
  }
}