#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int input[1];
  char* from = argv[1]; 
  char* to = argv[2];
  size_t length;
  size_t i; // offset
  size_t j; // refer to unique char check
  size_t k; // refer to unique char check
  int isTo = 0; // is in old string
  ssize_t readVal;
  // ===== CASE CHECKING ==============
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
  // ==================================
  

  while (0 != (readVal = read(0, input, 1))) {
    if (readVal == -1) {
      fprintf(stderr, "Invalid use of read() function.");
      exit(1);
    }	
    for (i = 0; i < length; i++) {
      if(from[i] == *input) {
	write(1, &(to[i]), 1); 
	isTo = 1;
	break;
      }
    }
    if (isTo == 1) {
      isTo = 0;
      continue;
    }
    else
      write(1, input, 1);
  }
}
