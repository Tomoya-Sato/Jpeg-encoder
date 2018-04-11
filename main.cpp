#include <stdio.h>
#include <stdlib.h>
#include "encode.h"

int main(int argc, char* argv[])
{
  if (argc != 5) 
    fprintf(stderr, "argument error\n");
  
  Jpegencoder pic(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
  pic.inputPpm();
  pic.makeHeader();
  pic.convertYCbCr();
  //pic.printPic();
  pic.DCTconvert();
  pic.freeYCbCr();
  pic.quantization();
  //pic.printQuant();
  pic.huffman();
  pic.freeDCT();
  pic.endHeader();

  return 0;
}
