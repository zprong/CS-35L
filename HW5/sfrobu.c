#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

int frobcmp(char const* a, char const* b);
int frobcmp2(char const* a, char const* b);
int compare(const void* a, const void* b);
int compare2(const void* a, const void* b);
 
int main(int argc, char* argv[]) {
  char* c;
  char* cNext;
  char* tempWord;
  char** tempWords;
  char** fWords;
  char** words;
  int i;
  int j;
  int strNum = 0; 
  int offset = 0;
  int sizeStr = 0;
  int result = 0;
  int isEmpty = 0;
  int iter = 0;
  ssize_t res;
  ssize_t resNext;
  ssize_t fres;
  char* file;
  char* tempFile;
  int option = 0;
  unsigned int fSize;
  if (argc > 2) {
    write(2, "Invalid number of arguments.", 100);
    exit(1);
  }
  else if (argc == 2){ // argcount had flag
    if (strlen(argv[1]) == 2 && argv[1][1] == 'f' && argv[1][0] == '-')
      option = 1;
    else {
      write(2, "Invalid flag.", 100);
      exit(1);
    }
  }
  struct stat st;
  if (fstat(0, &st)) { 
      fprintf(stderr, "fstat error");
      exit(1);
  }
  fSize = st.st_size;
  // taking words from a regular file 
  if (S_ISREG(st.st_mode)) { // file is a regular file
    file = (char*)malloc(fSize);
    if (file == NULL) {
      write(2, "Failed to allocate memory", 100); 
      exit(1);
    }
    fres = read(0, file, fSize);
    if (fres == -1) {
      write(2, "Failed to read file", 100);       
      exit(1);
    }
    if(file[fSize-2] != ' ') { // Find out how many words there are and append space to end
      fSize += 1;
      tempFile = realloc(file, fSize);
      if (tempFile == NULL) {
	write(2, "Failed to allocate memory", 100);
	exit(1);
      }
      file = tempFile;
      file[fSize-1] = EOF;
      file[fSize-2] = ' ';
    }
    
    int numChars = 0;
    int numWords = 0;
    int isSpace = 1;
    for (i = 0; i < fSize-1; i++) {
      if (file[i] == ' ' && isSpace)
	continue;
      else if (file[i] == ' ' && !isSpace) {
	isSpace = 1;
	numWords++;
      }
    }
    fWords = (char**)malloc(numWords*sizeof(char*));
    if (fWords == NULL) {
      write(2, "Failed to allocate memory", 100);
      exit(1);
    }
    isSpace = 1;
    sizeStr = 0;
    for (i = 0; i < fSize-1; i++) {  // split file into words
      if (file[i] == ' ' && isSpace) // handle repeated spaces
	continue;
      else if (offset == 0) { // start of a string
	fWords[strNum] = (char*)malloc(1);
	fWords[strNum][offset] = file[i];
	isSpace = 0;
	offset = 1;
      }
      else if (file[i] == ' ' && !isSpace) { // end of a string
	tempWord = (char*)realloc(fWords[strNum], offset+1);
	fWords[strNum] = tempWord;
	fWords[strNum][offset] = file[i];
	isSpace = 1;
	offset = 0;
	strNum++;
      }
      else { // anywhere inbetween
	tempWord = (char*)realloc(fWords[strNum], offset+1);
	fWords[strNum] = tempWord;
	fWords[strNum][offset] = file[i];
	offset++;
      }
    }
    if (option == 0)
      qsort(fWords, strNum, sizeof(char*), compare);
    else // -f flag is on
      qsort(fWords, strNum, sizeof(char*), compare2);
    for (i = 0; i < strNum; i++) {
      for (j = 0;; j++) {
	write(1, &fWords[i][j], 1);
	if (fWords[i][j] == ' ') 
	  break;
      }
    }
  }

  // Taking words from stdin
  words = (char**)malloc(sizeof(char*));
  if (words == NULL) {
    write(2, "Failed to allocate memory", 100);
    exit(1);
  }
  c = (char*)malloc(1);
  res = read(0, c, 1);
  if (res == 0)  // stdin is empty
    isEmpty = 1;
  else if (res == -1) {
    write(2, "Failed to allocate memory", 100);
    exit(1);
  }   
  else {
    cNext = (char*)malloc(1);
    resNext = read(0, cNext, 1);
  }
  
  strNum = 0;
  offset = 0;
  while (res != 0) { // stop loop when done reading the file
    if (offset == 0) {  // start of new word
      if (strNum >= 1) { 
	tempWords = (char**)realloc(words, sizeof(char*) * (strNum+1));
	if (tempWords == NULL) {
	  write(2, "Failed to allocate memory", 100);
	  free(words);
	  exit(1);
	}
	words = tempWords;
      }
      words[strNum] = (char*)malloc(1);
      if (words[strNum] == NULL) {
	write(2, "Failed to allocate memory", 100);
	free(words);
	exit(1);
      }
    }
    if (offset != 0) { // current string isn't empty
      tempWord = (char*)realloc(words[strNum], sizeStr+1);
      if (tempWord == NULL) {
	write(2, "Failed to allocate memory", 100);
	free(words);
	exit(1);
      }
      words[strNum] = tempWord;
    }
    words[strNum][offset] = *c;
    sizeStr++;
    offset++;
    if (*c == ' ' && *cNext != EOF) {  // If current char is a space, move onto next string 
      strNum++;
      offset = 0;
      sizeStr = 0;
    }
    *c = *cNext;
    res = resNext;
    if (*cNext != EOF) {
      resNext = read(0, cNext, 1);
      if (resNext == -1) {
	write(2, "Failed to allocate memory", 100);
	free(words);
	exit(1);
      }
    }
  }

  if (!isEmpty) {
    if (words[strNum][offset-1] != ' ') { 
      tempWord = (char*)realloc(words[strNum], sizeStr+1);
      if (tempWord == NULL) {
	write(2, "Failed to allocate memory", 100);
	free(words);
	exit(1);
      }
      words[strNum] = tempWord;
      words[strNum][offset] = ' '; // Append space to last string
    }
    if (option == 0)
      qsort(words, strNum+1, sizeof(char*), compare);
    else // -f flag is on
      qsort(words, strNum+1, sizeof(char*), compare2);
    for (i = 0; i < strNum+1; i++) {
      for (j = 0;; j++) {
	write(1, &words[i][j], 1);
	if (words[i][j] == ' ') {
	  break;
	}
      }
    }
    for (i = 0; i < strNum+1; i++) 
      free(words[i]);
  }
  else { // file is empty
    tempWord = malloc(1);
    if (tempWord == NULL) {
      write(2, "Failed to allocate memory", 100);
      free(words);
      exit(1);
    }
    words[0] = tempWord;
    words[0][0] = ' ';
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

int frobcmp2(char const* a, char const* b) {
  while (*a == *b) {
    a++;
    b++;
    if (*a == ' ') // a is suffix of b
      return -1;
    if (*b == ' ') // b is suffix of a
      return 1;
  }
  if ((int)(*a ^ 42) < EOF || (int)(*a ^ 42) > UCHAR_MAX
    ||(int)(*b ^ 42) < EOF || (int)(*b ^ 42) > UCHAR_MAX) {
    write(2, "toupper argument is not in range", 100);
    exit(1);
  }
  int decryptA = toupper(*a ^ 42);
  int decryptB = toupper(*b ^ 42);
  return (decryptA > decryptB ? 1 : (decryptA == decryptB ? 0 : -1));
}

// This function is used to match the same type as qsort's function ptr
int compare(const void* a, const void* b) {
  const char* u = *(const char**)a;
  const char* v = *(const char**)b;
  return frobcmp(u, v);
}
int compare2(const void* a, const void* b) {
  const char* u = *(const char**)a;
  const char* v = *(const char**)b;
  return frobcmp2(u, v);
}

