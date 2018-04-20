#include <iostream>
#include <stdlib.h>
#include "encode.h"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "argument error\n");
		exit(1);
	}
		
	Jpegencoder pic(argv[1], argv[2]);
	pic.inputPpm();
	std::cout << "inputPpm..." << std::endl;
	pic.makeHeader();
	std::cout << "makeHeader..." << std::endl;	
	pic.convertYCbCr();
	std::cout << "convertYCbCr..." << std::endl;		
	//pic.printPic();
	pic.DCTconvert();
	std::cout << "DCTconvert..." << std::endl;		
	pic.freeYCbCr();
	pic.quantization();
	std::cout << "quantization..." << std::endl;		
	//pic.printQuant();
	pic.huffman();
	std::cout << "huffman..." << std::endl;		
	pic.freeDCT();
	pic.endHeader();
	std::cout << "complete!" << std::endl;
	
	return 0;
}
