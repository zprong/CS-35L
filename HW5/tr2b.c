
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int input;
  char* from = argv[1];
  char* to = argv[2];
  size_t length;
  size_t i; // string offset
  size_t j; // refer to unique char check
  size_t k; // refer to unique char check 
  size_t isTo = 0; // is in old string
  // ======= CASE CHECKING =============
  if (argc != 3) {
    fprintf(stderr, "Invalid number of argments. SEE YA.");
    exit(1);
  }
  if(strlen(from) != strlen(to)) {
     fprintf(stderr, "Mismatch in string lengths. SEE YA.");
     exit(1);
  }
  length = strlen(from);
  for(j = 0; j < length; j++) { // unique char check
    for(k = 0; k < length; k++)
      if (j != k)
        if (from[j] == from[k])
          fprintf(stderr, "ERROR: First string must have only unique characters. SEE YA.");
  }
  // ===================================
  
  while (EOF != (input = getchar())) {
    for (i = 0; i < length; i++) {
      if(from[i] == input) {
	putchar(to[i]);
	isTo = 1;
	break;
      }
    }
    if (isTo == 1)
      isTo = 0;
    else
      putchar(input);
  }
}
