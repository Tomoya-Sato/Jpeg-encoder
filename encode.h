#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>

typedef struct _Picture
{
  int x;
  int y;
  unsigned char *r;
  unsigned char *g;
  unsigned char *b;
  unsigned char *a;
}Picture;

void check_error(int n, const char* p);

void check_error(char* n, const char* p);

void check_error(FILE* n, const char* p);

class Jpegencoder {
 public:
  Jpegencoder(char input[64], char output[64]) {
    check_error(strcpy(input_path_, input), "input_path_ strcpy");
    check_error(strcpy(output_path_, output), "output_path_, strcpy");
    check_error((input_fp_ = fopen(input_path_, "rb")), "input_fp_ fopen");
    check_error((output_fp_ = fopen(output_path_, "wb")), "output_fp_ fopen");
    count_ = 0;
  }

  ~Jpegencoder() {
    fclose(input_fp_);
    fclose(output_fp_);
  }
  
  void inputPpm(); 
  void makeHeader();
  void convertYCbCr();
  void freeYCbCr();
  void DCTconvert();
  void freeDCT();
  void quantization();
  void huffman();
  void endHeader();

  /* debug section */
  void printQuant();
  void printPic();
  
 private:
  char input_path_[64];
  char output_path_[64];
  FILE* input_fp_;
  FILE* output_fp_;
  int x_size_, y_size_;
  Picture *pPic_;
  int *col_y_;
  int *col_cb_;
  int *col_cr_;
  int fixed_x_, fixed_y_;
  int *dct_y_;
  int *dct_cr_;
  int *dct_cb_;
  int count_;
  unsigned int str_[128];

  int digit(int x);
  void outstream(unsigned int size, unsigned int x);
  int bit_inv(int size, int x);
};
