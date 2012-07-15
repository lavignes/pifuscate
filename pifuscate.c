#include "pifuscate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned short key = 0;

int rotr(int inbyte, int rotamt) {

  while (rotamt) {
  
    inbyte = ((inbyte & 0x01) << 7) ^ (inbyte >> 1);
    
    --rotamt;
  }
  
  return inbyte;
}

int rotl(int inbyte, int rotamt) {

  while (rotamt) {
  
    inbyte = ((inbyte & 0x80) >> 7) ^ ((inbyte << 1) & 0xFF);
       
    --rotamt;
  }
  
  return inbyte;
}

void usage() {

  fprintf(stderr, "pifuscate 1.0 - A file encryptor/decryptor based on digits of pi.\n"
         "usage:\n"
         "pifuscate {encrypt|decrypt|--help} [Options] infile\n"
         
         "\nOptions:\n"
         "-o <outfile>\n"
         "\tOutputs the pifuscate code to file <outfile>.\n\tBy default the "
         "output will be stdout.\n"
         "-k <key>\n"
         "\tStarts the pifuscation at index key.\n\tThe key should be a number "
         "between 0 and 65535.\n\tThe default is 314.\n"
         "-i <iterations>\n"
         "\tRuns the algorithm <iterations> number of times. (More is slower)\n"
         "\tThe default is 1.\n"
         
         "\nExamples:\n"
         "encrypting a file with the key 1337 25 times:\n"
         "\n\tpifuscate encrypt -k 1337 -i 25 file -o outfile\n\n"
         "decrypting the same file and printing it on stdout:\n"
         "\n\tpifuscate decrypt -k 1337 -i 25 outfile\n\n"
         );

  exit(1);
}

int main(int argc, char* argv[]) {

  char* ifilename = NULL;
  char* ofilename = NULL;
  FILE* input = NULL;
  FILE* output = NULL;
  int mode = 0; // 0 => encrypt --- 1=> decrypt
  int iter = 1;
  int i;
  
  // Need at least 2 arguments
  if (argc > 2) {
  
    if (!strcmp(argv[1], "encrypt")) {
    
      mode = 0;
    
    } else if (!strcmp(argv[1], "decrypt")) {
    
      mode = 1;
    
    } else {
    
      usage();
    }
  
    for (i = 2; i < argc; ++i) {
    
      if ((!strcmp(argv[i], "-o") || !strcmp(argv[i], "--out"))
          && ((i + 1) != argc)) {
        
        ofilename = argv[i + 1];
        ++i;
      
      } else if ((!strcmp(argv[i], "-k") || !strcmp(argv[i], "--key"))
          && ((i + 1) != argc)) {

        sscanf(argv[i + 1], "%hu", &key);
        ++i;
        
      }  else if ((!strcmp(argv[i], "-i") || !strcmp(argv[i], "--iterate"))
          && ((i + 1) != argc)) {
      
        sscanf(argv[i + 1], "%u", &iter);
        ++i;
      
      } else {

        ifilename = argv[i];      
      }
    }
  }
  
  if (ifilename == NULL) {
  
    usage();
    
  } else {
  
    input = fopen(ifilename, "r");
  }
  
  if (input == NULL) {
  
    fprintf(stderr, "There was an error opening \"%s\" for reading:\n", ifilename);
    perror(NULL);
    exit(1);
  }
  
  if (!strcmp(ifilename, ofilename)) {
  
    fprintf(stderr, "Input and output files cannot match!\n");
    exit(1);
  }
  
  if (ofilename == NULL) {
  
    output = stdout;
  
  } else {
  
    output = fopen(ofilename, "w");
  }
  
  if (output == NULL) {
  
    fprintf(stderr, "There was an error opening \"%s\" for writing:\n", ofilename);
    perror(NULL);
    exit(1);
  }
  
  int c;
  int o;
  
  if (mode == 0) { // encrypt
  
    do {
    
      c = fgetc(input);
      
      if (c == EOF) continue;
      
      o = c;
      for (i = 0; i < iter; ++i) {
        o = rotr(o, pi[key]);
        ++key; 
      }
      
      fputc(o, output);
    
    } while (c != EOF);
  }
  
  else if (mode == 1) { // decrypt
  
    do {
    
      c = fgetc(input);
      
      if (c == EOF) continue;
      
      o = c;
      for (i = 0; i < iter; ++i) {
        o = rotl(o, pi[key]);
        ++key; 
      }
      
      fputc(o, output);
    
    } while (c != EOF);
  }

  fclose(input);
  if (output != stdout) fclose(output);

  return 0;
}
