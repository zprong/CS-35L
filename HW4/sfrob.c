#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const* a, char const* b);
int compare(const void* a, const void* b);

int main(void) {
  char c;
  char cNext;
  char* tempWord;
  char** tempWords;
  char** words;
  int i;
  int j;
  int strNum = 0; 
  int offset = 0;
  int sizeStr = 0;
  int result = 0;
  int isEmpty = 0;
  int iter = 0;
  words = (char**)malloc(sizeof(char*));
  if(words == NULL) {
    fprintf(stderr, "Failed to allocate memory");
    exit(1);
  }
  c = getchar();
  if(c == EOF)  // stdin is empty
    isEmpty = 1;
  else
    cNext = getchar();
  while(c != EOF) { // stop loop when done reading the file
    if(offset == 0) {  // start of new word
      if (strNum >= 1) { 
	tempWords = (char**)realloc(words, sizeof(char*) * (strNum+1));
	if(tempWords == NULL) {
	  fprintf(stderr, "Failed to allocate memory");
	  free(words);
	  exit(1);
	}
	words = tempWords;
      }
      words[strNum] = (char*)malloc(1);
      if(words[strNum] == NULL) {
	fprintf(stderr, "Failed to allocate memory");
	free(words);
	exit(1);
      }
    }
    if(offset != 0) { // string isn't empty
      tempWord = (char*)realloc(words[strNum], sizeStr+1);
      if(tempWord == NULL) {
	fprintf(stderr, "Failed to allocate memory");
	free(words);
	exit(1);
      }
      words[strNum] = tempWord;
    }
    words[strNum][offset] = c;
    sizeStr++;
    offset++;
    if(c == ' ' && cNext != EOF) {  // If current char is a space, move onto next string 
      strNum++;
      offset = 0;
      sizeStr = 0;
    }
    c = cNext;
    if(cNext != EOF) {
      cNext = getchar();
    }
  }

  if(!isEmpty) {
    if(words[strNum][offset-1] != ' ') {
      tempWord = (char*)realloc(words[strNum], sizeStr+1);
      if(tempWord == NULL) {
	fprintf(stderr, "Failed to allocate memory");
	free(words);
	exit(1);
      }
      words[strNum] = tempWord;
      words[strNum][offset] = ' ';
    }
  
    qsort(words, strNum+1, sizeof(char*), compare);
    for(i = 0; i < strNum+1; i++) {
      for(j = 0;; j++) {
	putchar(words[i][j]);
	if(words[i][j] == ' ') {
	  break;
	}
      }
    }
    for(i = 0; i < strNum+1; i++) 
      free(words[i]);
  }
  else { // file is empty
    tempWord = malloc(1);
    if(tempWord == NULL) {
      fprintf(stderr, "Failed to allocate memory");
      free(words);
      exit(1);
    }
    words[0] = tempWord;
    words[0] = ' ';
  }
  free(words);
  exit(0);
}

int frobcmp(char const* a, char const* b) {
  while (*a == *b) {
    a++;
    b++;
    if (*a == ' ') // a is suffix of b
      return -1;
    if (*b == ' ') // b is suffix of a
      return 1;
  }
  int decryptA = *a ^ 42;
  int decryptB = *b ^ 42;
  return (decryptA > decryptB ? 1 : (decryptA == decryptB ? 0 : -1)); 
}

// This function is used to match the same type as qsort's function ptr
int compare(const void* a, const void* b) {
  const char* u = *(const char**)a;
  const char* v = *(const char**)b;
  return frobcmp(u, v);
}
