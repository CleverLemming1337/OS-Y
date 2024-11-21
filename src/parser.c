#include <efi.h>
#include <efilib.h>
#include "parser.h"

#ifndef PARSER
#define PARSER

int splitArgs(CHAR16* input, int n) {
  /*
  Splits the command and the args.

  Args:
  * input: CHAR16*: Start address of input
  * n: int:         Length of input
  
  Returns:
  * cmdLengths: int:  Length of command

  Example:
  * Input: Pointer to "echo Hello, world!"
  * Searching for ' ', when found, return i
  * Returns: 4 (offset from input to end of command)
  */

  for(int i = 0; i<n; i++, input++) {
    if (*input == ' ') {
      return i;
    }
  }
}

void sliceString(CHAR16 *source, CHAR16 *dest, int start, int end) {
    int j = 0;
    for (int i = start; i < end && source[i] != '\0'; i++) {
        dest[j++] = source[i];
    }
    dest[j] = '\0';
}

#endif