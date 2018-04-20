#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "encode.h"

#define N 403


void check_error(int n, const char* p) {
  if (n == -1) {
    perror(p);
    exit(1);
  }
}

void check_error(char* n, const char* p) {
  if (n == NULL) {
    perror(p);
    exit(1);
  }
}

void check_error(FILE* n, const char* p) {
  if (n == NULL) {
    perror(p);
    exit(1);
  }
}

void Jpegencoder::inputPpm() {
  int i, j, k;
  char ss[256];
  int type, max, tmp;
  double ratio;
  unsigned char *r, *g, *b;


  fgets(ss, 255, input_fp_);
  if(ss[0] != 'P') {
    fprintf(stderr, "Wrong File Type\n");
    exit(1);
  }

  sscanf(ss, "P%d", &type);
  if (type < 1 || type > 6) {
    fprintf(stderr, "Unknown Type\n");
    exit(1);
  }

  do fgets(ss, 255, input_fp_); while (ss[0] == '#');
    
  sscanf(ss, "%d%d", &x_size_, &y_size_);
  if (x_size_ < 1 || y_size_ < 1) {
    fprintf(stderr, "Size Error\n");
    exit(1);
  }

  if (type == 2 || type == 3 || type == 5 || type == 6) {
    do fgets(ss, 255, input_fp_); while(ss[0] == '#');
    sscanf(ss, "%d", &max);
      
    if (max < 1 || max > 255) {
      fprintf(stderr, "Max Error\n");
      exit(1);
    }
    ratio = 255 / (float)max;
  }

  pPic_ = (Picture*)malloc(sizeof(Picture));
  if (pPic_ == NULL) {
    perror("pPic_ malloc");
    exit(1);
  }
    
  pPic_->x = x_size_;
  pPic_->y = y_size_;
  pPic_->r = r = (unsigned char*)malloc(sizeof(unsigned char) * x_size_ * y_size_);
  pPic_->g = g = (unsigned char*)malloc(sizeof(unsigned char) * x_size_ * y_size_);
  pPic_->b = b = (unsigned char*)malloc(sizeof(unsigned char) * x_size_ * y_size_);
  pPic_->a = NULL;
      
  if (r == NULL || g == NULL || b == NULL) {
    free(pPic_->r);
    free(pPic_->g);
    free(pPic_->b);
    free(pPic_->a);
    free(pPic_);
    pPic_ = NULL;

    exit(1);
  }

  switch(type) {
  case 1:
    for (i = 0; i < y_size_; i++) {
      for (j = 0; j < y_size_; j++) {
	if(fscanf(input_fp_, "%d", &tmp) != 1) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(r++) = *(g++) = *(b++) = (unsigned char)((1 - tmp) * 255);
      }
    }
    break;
  case 2:
    for (i = 0; i < y_size_; i++) {
      for (j = 0; j < x_size_; j++) {
	if (fscanf(input_fp_, "%d", &tmp) != 1) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(r++) = *(g++) = *(b++) = (unsigned char)(tmp * ratio);
      }
    }
    break;
  case 3:
    for (i = 0; i < y_size_; i++) {
      for (j = 0; j < x_size_; j++) {
	if (fscanf(input_fp_, "%d", &tmp) != 1) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(r++) = (unsigned char)(tmp * ratio);
	  
	if (fscanf(input_fp_, "%d", &tmp) != 1) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(g++) = (unsigned char)(tmp * ratio);

	if (fscanf(input_fp_, "%d", &tmp) != 1) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(b++) = (unsigned char)(tmp * ratio);
      }
    }
    break;
  case 4:
    for (i = 0; i < y_size_; i++) {
      for (j = 0; j < (x_size_ - 1) / 8; j++) {
	if ((tmp = getc(input_fp_)) == EOF) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	  
	for (k = 7; k >= 0; k--)
	  *(r++) = *(g++) = *(b++) = (unsigned char)((1 - ((tmp >> k) % 2)) * 255); 
      }
      if ((tmp = getc(input_fp_)) == EOF) {
	free(pPic_->r);
	free(pPic_->g);
	free(pPic_->b);
	free(pPic_->a);
	free(pPic_);
	pPic_ = NULL;
	  
	exit(1);
      }
      for (k = 7; k >= 7 - (x_size_ - 1) % 8; k--)
	*(r++) = *(g++) = *(b++) = (unsigned char)((1 - ((tmp >> k) & 2)) * 255);
    }
    break;
  case 5:
    for (i = 0; i < y_size_; i++) {
      for (j = 0; j < x_size_; j++) {
	if ((tmp = getc(input_fp_)) == EOF) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;

	  exit(1);
	}
	*(r++) = *(g++) = *(b++) = (unsigned char)(tmp * ratio);
      }
    }
    break;
  case 6:
    for (i = 0; i < y_size_; i++) {
      for (j = 0; j < x_size_; j++) {
	if ((tmp = getc(input_fp_)) == EOF) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(r++) = (unsigned char)(tmp * ratio);

	if ((tmp = getc(input_fp_)) == EOF) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(g++) = (unsigned char)(tmp * ratio);

	if ((tmp = getc(input_fp_)) == EOF) {
	  free(pPic_->r);
	  free(pPic_->g);
	  free(pPic_->b);
	  free(pPic_->a);
	  free(pPic_);
	  pPic_ = NULL;
	    
	  exit(1);
	}
	*(b++) = (unsigned char)(tmp * ratio);
      }
    }
    break;
  }
}
 
void Jpegencoder::makeHeader() {
  unsigned char x0, x1, y0, y1;
  x0 = (unsigned char)(x_size_ / 256);
  x1 = (unsigned char)(x_size_ % 256);
  y0 = (unsigned char)(y_size_ / 256);
  y1 = (unsigned char)(y_size_ % 256);
  unsigned char buf[N] = {
    /* SOI marker */
    0xff, 0xd8,
    /* APP0 segment */
    0xff, 0xe0,
    0x00, 0x10,
    0x4a, 0x46, 0x49, 0x46, 0x00,
    0x01, 0x01,
    0x00,		/* 14 */
    0x00, 0x01,
    0x00, 0x01,
    0x00, 0x00,	/* 22 */
    /* DQT segment */
    0xff, 0xdb, /* marker */
    0x00, 0x84, /* length of segment */
    0x00,	/* element accuracy | ID */ //27
    /* quantify table */
    16,11,10,16,24,40,51,61,
    12,12,14,19,26,58,60,55,
    14,13,16,24,40,57,69,56,
    14,17,22,29,51,87,80,62,
    18,22,37,56,68,109,103,77,
    24,35,55,64,81,104,113,92,
    49,64,78,87,103,121,120,110,
    72,92,95,98,112,100,103,99,	// 91
    0x01,	/* element accuracy | ID */
    /* quantify table */
    17,18,24,47,99,99,99,99,
    18,21,26,66,99,99,99,99,
    24,26,56,99,99,99,99,99,
    47,66,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,	// 156
    /* DHTAC segment */
    0xff, 0xc4, /* marker */
    0x00, 0xb5, /* length of segment */
    0x10,	     /* DC=0,AC=1 | ID */
    0, 2, 1, 3, /* L1~L16 */
    3, 2, 4, 3,
    5, 5, 4, 4,
    0, 0, 1, 125,
    0x01,0x02, /* V2 */
    0x03,	    /* V3 */
    0x00,0x04,0x11, /* V4 */
    0x05,0x12,0x21, /* V5 */
    0x31,0x41,	 /* V6 */
    0x06,0x13,0x51,0x61, /* V7 */
    0x07,0x22,0x71,      /* V8 */
    0x14,0x32,0x81,0x91,0xa1, /* V9 */
    0x08,0x23,0x42,0xb1,0xc1, /* V10 */
    0x15,0x52,0xd1,0xf0,	   /* V11 */
    0x24,0x33,0x62,0x72,	   /* V12 */
    0x82,			   /* V15 */
    0x09,0x0a,0x16,0x17,0x18,0x19,
    0x1a,0x25,0x26,0x27,0x28,0x29,
    0x2a,0x34,0x35,0x36,0x37,0x38,
    0x39,0x3a,0x43,0x44,0x45,0x46,
    0x47,0x48,0x49,0x4a,0x53,0x54,
    0x55,0x56,0x57,0x58,0x59,0x5a,0x63,
    0x64,0x65,0x66,0x67,0x68,0x69,
    0x6a,0x73,0x74,0x75,0x76,0x77,
    0x78,0x79,0x7a,0x83,0x84,0x85,
    0x86,0x87,0x88,0x89,0x8a,0x92,
    0x93,0x94,0x95,0x96,0x97,0x98,
    0x99,0x9a,0xa2,0xa3,0xa4,0xa5,
    0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,
    0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,
    0xb9,0xba,0xc2,0xc3,0xc4,0xc5,
    0xc6,0xc7,0xc8,0xc9,0xca,0xd2,
    0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,
    0xd9,0xda,0xe1,0xe2,0xe3,0xe4,
    0xe5,0xe6,0xe7,0xe8,0xe9,0xea,
    0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,
    0xf7,0xf8,0xf9,0xfa,
    /* DHTDC segment */
    0xff, 0xc4, /* marker */
    0x00, 0x1f, /* length of segment */
    0x00,       /* DC=0,AC=1 | ID */
    0, 3, 1, 1, /* L1~L16 */
    1, 1, 1, 1,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0x00,0x01,0x02, /* V2 */
    0x03,	    /* V3 */
    0x04, /* V4 */
    0x05, /* V5 */
    0x06, /* V6 */
    0x07, /* V7 */
    0x08, /* V8 */
    0x09, /* V9 */
    0x0a, /* V10 */
    0x0b,
    /* SOF segment */
    0xff, 0xc0, /* marker */
    0x00, 0x11, /* length of segment */
    0x08,       /* sample accuracy */
    y0, y1,     /* vertical size */
    x0, x1,     /* horizontal size */
    0x03,       /* element number */
    0x01,       /* element ID */
    0x22,       /* v-sample | h-sample */
    0x00,       /* q-table ID */
    0x02,       /* element ID */
    0x11,
    0x01,
    0x03,
    0x11,
    0x01,
    /* SOS segment */
    0xff, 0xda, /* marker */
    0x00, 0x0c, /* length of segment */
    0x03,       /* element number */
    0x01, 0x00,
    0x02, 0x00,
    0x03, 0x00,
    0x00, 0x3f, 0x00};

  if (fwrite(buf, sizeof(unsigned char), N, output_fp_) < N)
    perror("fwrite in makeHeader()");

  return;
}

void Jpegencoder::convertYCbCr()
{
  int i, j, tmp, tmp2, tmp3;

  fixed_x_ = x_size_;
  fixed_y_ = y_size_;

  if ((fixed_x_ % 16) != 0) fixed_x_ = fixed_x_ + (16 - (fixed_x_ % 16));
  if ((fixed_y_ % 16) != 0) fixed_y_ = fixed_y_ + (16 - (fixed_y_ % 16));

  col_y_ = (int*)malloc(sizeof(int)*fixed_x_*fixed_y_);
  col_cb_ = (int*)malloc(sizeof(int)*fixed_x_*fixed_y_/4);
  col_cr_ = (int*)malloc(sizeof(int)*fixed_x_*fixed_y_/4);

  for (i = 0; i < y_size_; i++) {
    for (j = 0; j < x_size_; j++) {
      tmp = j + i * fixed_x_;
      tmp2 = j + i * x_size_;
      col_y_[tmp] = (int)((0.2990 * pPic_->r[tmp2]) + (0.5870 * pPic_->g[tmp2]) 
			  + (0.1140 * pPic_->b[tmp2]) - 128);
      if ((i % 2 == 0) && (j % 2 == 0)) {
		  tmp3 = j / 2 + (i / 2) * (fixed_x_ / 2);
		  col_cb_[tmp3] = (int)(-(0.1687 * pPic_->r[tmp2]) - (0.3313 * pPic_->g[tmp2])
								+ (0.5000 * pPic_->b[tmp2]));
		  col_cr_[tmp3] = (int)((0.5000 * pPic_->r[tmp2]) - (0.4187 * pPic_->g[tmp2])
								- (0.0813 * pPic_->b[tmp2]));
      }
    }
  }

  for (i = 0; i < y_size_; i++) {
    tmp = x_size_ - 1 + fixed_x_ * i;
    for (j = x_size_; j < fixed_x_; j++) {
      tmp2 = j + i * fixed_x_;
      col_y_[tmp2] = col_y_[tmp];
    }
  }
  for (j = 0; j < x_size_; j++) {
    tmp = j + fixed_x_ * (y_size_ - 1);
    for (i = y_size_; i < fixed_y_; i++) {
      col_y_[tmp2] = col_y_[tmp];
    }
  }
  tmp = (x_size_ - 1) * (y_size_ - 1);
  for (i = y_size_; i < fixed_y_; i++) {
    for (j = x_size_; j < fixed_x_; j++) {
      tmp2 = j + i * fixed_x_;
      col_y_[tmp2] = col_y_[tmp];
    }
  }

  for (i = 0; i < y_size_/2; i++) {
    tmp = x_size_ / 2 - 1 + j * fixed_x_ / 2;
    for (j = x_size_/2; j < fixed_x_/2; j++) {
      tmp2 = j + i * fixed_x_/2;
      col_cb_[tmp2] = col_cb_[tmp];
      col_cr_[tmp2] = col_cr_[tmp];
    }
  }
  for (j = 0; j < x_size_/2; j++) {
    tmp = j + (y_size_ / 2 - 1) * fixed_x_ / 2;
    for (i = y_size_/2; i < fixed_y_/2; i++) {
		tmp2 = j + i * fixed_x_/2;
		col_cb_[tmp2] = col_cb_[tmp];
		col_cr_[tmp2] = col_cr_[tmp];
    }
  }
  tmp = (x_size_/2 - 1) + (y_size_/2 - 1) * fixed_x_/2;
  for (i = y_size_/2; i < fixed_y_/2; i++) {
    for (j = x_size_/2; j < fixed_x_/2; j++) {
      tmp2 = j + i * fixed_x_/2;
      col_cb_[tmp2] = col_cb_[tmp];
      col_cr_[tmp2] = col_cr_[tmp];
    }
  }

  return;
}

void Jpegencoder::freeYCbCr()
{
  free(col_y_);
  free(col_cb_);
  free(col_cr_);

  return;
}

void Jpegencoder::DCTconvert()
{
  dct_y_ = (int*)malloc(sizeof(int)*fixed_x_*fixed_y_);
  dct_cb_ = (int*)malloc(sizeof(int)*fixed_x_*fixed_y_/4);
  dct_cr_ = (int*)malloc(sizeof(int)*fixed_x_*fixed_y_/4);

  int p = fixed_x_ / 8, q = fixed_y_ / 8;
  int u, v, x, y, n, m;
  int index;
  double tmp, tmp2;
  double cu, cv;
  double long c, d;

  for (n = 0; n < q; n++) {
    for (m = 0; m < p; m++) {

      // 8*8 block
      for (v = 0; v < 8; v++) {
	if (!v) cv = 1.0 / sqrt(2);
	else cv = 1.0;

	for (u = 0; u < 8; u++) {
	  if (!u) cu = 1.0 / sqrt(2);
	  else cu = 1.0;
	  tmp = 0;

	  for (y = 0; y < 8; y++) {
	    for (x = 0; x < 8; x++) {
	      index = (y+8*n) * fixed_x_ + (x+8*m);
	      c = cos((2.0*x+1.0)*u*M_PI/16.0)*cos((2.0*y+1.0)*v*M_PI/16.0);
	      tmp += c * col_y_[index];
	    }
	  }
	  index = (v+8*n) * fixed_x_ + (u+8*m);
	  dct_y_[index] = (int)(floor(cu*cv*tmp/4+0.5));
	}
      }
    }
  }

  p = p / 2;
  q = q / 2;
  
  for (n = 0; n < q; n++) {
    for (m = 0; m < p; m++) {

      // 8*8 block
      for (v = 0; v < 8; v++) {
	if (!v) cv = 1.0 / sqrt(2);
	else cv = 1.0;

	for (u = 0; u < 8; u++) {
	  if (!u) cu = 1.0 / sqrt(2);
	  else cu = 1.0;
	  tmp = 0.0;
	  tmp2 = 0.0;

	  for (y = 0; y < 8; y++) {
	    for (x = 0; x < 8; x++) {
	      index = (y+8*n) * fixed_x_/2 + (x+8*m);
	      c = cos((2.0*x+1.0)*u*M_PI/16.0)*cos((2.0*y+1.0)*v*M_PI/16.0);
	      tmp += c * col_cb_[index];
	      tmp2 += c * col_cr_[index];
	    }
	  }
	  index = (v+8*n) * fixed_x_/2 + (u+8*m);
	  dct_cb_[index] = (int)(floor(cu*cv*tmp/4+0.5));
	  dct_cr_[index] = (int)(floor(cu*cv*tmp2/4+0.5));
	}
      }
    }
  }

  return;
}

void Jpegencoder::freeDCT()
{
  free(dct_y_);
  free(dct_cb_);
  free(dct_cr_);

  return;
}

void Jpegencoder::quantization()
{
  const int q1[8][8] = {
    { 16, 11, 10, 16, 24, 40, 51, 61},
    { 12, 12, 14, 19, 26, 58, 60, 55},
    { 14, 13, 16, 24, 40, 57, 69, 56},
    { 14, 17, 22, 29, 51, 87, 80, 62},
    { 18, 22, 37, 56, 68,109,103, 77},
    { 24, 35, 55, 64, 81, 04,113, 92},
    { 49, 64, 78, 87,103,121,120,101},
    { 72, 92, 95, 98,112,100,103, 99}};

  const int q2[8][8] = {
    { 17, 18, 24, 47, 99, 99, 99, 99},
    { 18, 21, 26, 66, 99, 99, 99, 99},
    { 24, 26, 56, 99, 99, 99, 99, 99},
    { 47, 66, 99, 99, 99, 99, 99, 99},
    { 99, 99, 99, 99, 99, 99, 99, 99},
    { 99, 99, 99, 99, 99, 99, 99, 99},
    { 99, 99, 99, 99, 99, 99, 99, 99},
    { 99, 99, 99, 99, 99, 99, 99, 99}};

  int i, j, m, n;
  int x1 = fixed_x_ / 8;
  int y1 = fixed_y_ / 8;
  int index;

  for (m = 0; m < y1; m++) {
    for (n = 0; n < x1; n++) {
      for (i = 0; i < 8; i++) {
	for (j = 0; j < 8; j++) {
	  index = j+8*n + (i+8*m)*fixed_x_;
	  dct_y_[index] = dct_y_[index] / q1[i][j];
	}
      }
    }
  }

  x1 = x1 / 2;
  y1 = y1 / 2;

  for (m = 0; m < y1; m++) {
    for (n = 0; n < x1; n++) {
      for (i = 0; i < 8; i++) {
	for (j = 0; j < 8; j++) {
	  index = j+8*n + (i+8*m)*fixed_x_/2;
	  dct_cb_[index] = dct_cb_[index] / q2[i][j];
	  dct_cr_[index] = dct_cr_[index] / q2[i][j];
	}
      }
    }
  }

  return;
}

int Jpegencoder::digit(int x)
{
  int i = 0;
  if (x < 0) x = - x;
  while (x > 0) {
    x = x >> 1;
    i++;
  }

  return i;
}

void Jpegencoder::outstream(unsigned int size, unsigned int x)
{
  int i, tmp;
  unsigned int buf[size];
  unsigned char a[1];

  unsigned int binary[16] = {0x8000, 0x4000, 0x2000, 0x1000,
			     0x0800, 0x0400, 0x0200, 0x0100,
			     0x0080, 0x0040, 0x0020, 0x0010,
			     0x0008, 0x0004, 0x0002, 0x0001};

  for (i = 0; i < size; i++) {
    tmp = i + (16 - size);
    if (x >= binary[tmp]) {
      buf[i] = 1;
      x = x - binary[tmp];
    }
    else buf[i] = 0;
  }

  for (i = 0; i < size; i++) {
    str_[i+count_] = buf[i];
  }
  count_ = count_ + size;

  while (count_ >= 8) {
    a[0] = 0;
    for (i = 0; i < 8; i++) {
      if (str_[i]) a[0] = a[0] + binary[i+8];
    }
    if (fwrite(a, sizeof(unsigned char), 1, output_fp_) < 1) {
      perror("fwrite in Jpegencoder::outstream");
      exit(1);
    }
    if (a[0] == 0xff) {
      a[0] = 0x00;
      if (fwrite(a, sizeof(unsigned char), 1, output_fp_) < 1) {
	perror("fwrite in Jpegencoder::outstream a[0] == 0xff");
	exit(0);
      }
    }
    for (i = 8; i < count_; i++) {
      str_[i-8] = str_[i];
    }
    count_ = count_ - 8;
  }

  return;
}

int Jpegencoder::bit_inv(int size, int x)
{
  int i;
  int y = 0;

  unsigned int binary[16] = {0x8000, 0x4000, 0x2000, 0x1000,
			     0x0800, 0x0400, 0x0200, 0x0100,
			     0x0080, 0x0040, 0x0020, 0x0010,
			     0x0008, 0x0004, 0x0002, 0x0001};
  
  for (i = 0; i < size; i++) {
    if (x >= binary[i + (16 - size)])
      x = x - binary[i + (16 - size)];
    else
      y = y + binary[i + (16 - size)];
  }

  return y;
}

void Jpegencoder::huffman()
{
  const int zig_x[64] = {0,1,0,0,1,2,3,2, /* zigzag sequence */
			 1,0,0,1,2,3,4,5,
			 4,3,2,1,0,0,1,2,
			 3,4,5,6,7,6,5,4,
			 3,2,1,0,1,2,3,4,
			 5,6,7,7,6,5,4,3,
			 2,3,4,5,6,7,7,6,
			 5,4,5,6,7,7,6,7};
  
  const int zig_y[64] = {0,0,1,2,1,0,0,1,
			 2,3,4,3,2,1,0,0,
			 1,2,3,4,5,6,5,4,
			 3,2,1,0,0,1,2,3,
			 4,5,6,7,7,6,5,4,
			 3,2,1,2,3,4,5,6,
			 7,7,6,5,4,3,4,5,
			 6,7,7,6,5,6,7,7};

  const unsigned int dc_table[12][2] = {{2,0x0000},
					{2,0x0001},
					{2,0x0002},
					{3,0x0006},
					{4,0x000E},
					{5,0x001E},
					{6,0x003E},
					{7,0x007E},
					{8,0x00FE},
					{9,0x01FE},
					{10,0x03FE},
					{11,0x07FE}};
  
  const unsigned int ac_table[16][10][2] = {{{2,0x0000},{2,0x0001},
					     {3,0x0004},{4,0x000B},
					     {5,0x001A},{7,0x0078},
					     {8,0x00F8},{10,0x03F6},
					     {16,0xFF82},{16,0xFF83}},
					    {{4,0x000C},{5,0x001B},
					     {7,0x0079},{9,0x01F6},
					     {11,0x07F6},{16,0xFF84},
					     {16,0xFF85},{16,0xFF86},
					     {16,0xFF87},{16,0xFF88}},
					    {{5,0x001C},{8,0x00F9},
					     {10,0x03F7},{12,0x0FF4},
					     {16,0xFF89},{16,0xFF8A},
					     {16,0xFF8B},{16,0xFF8C},
					     {16,0xFF8D},{16,0xFF8E}},
					    {{6,0x003A},{9,0x01F7},
					     {12,0x0FF5},{16,0xFF8F},
					     {16,0xFF90},{16,0xFF91},
					     {16,0xFF92},{16,0xFF93},
					     {16,0xFF94},{16,0xFF95}},
					    {{6,0x003B},{10,0x03F8},
					     {16,0xFF96},{16,0xFF97},
					     {16,0xFF98},{16,0xFF99},
					     {16,0xFF9A},{16,0xFF9B},
					     {16,0xFF9C},{16,0xFF9D}},
					    {{7,0x007A},{11,0x07F7},
					     {16,0xFF9E},{16,0xFF9F},
					     {16,0xFFA0},{16,0xFFA1},
					     {16,0xFFA2},{16,0xFFA3},
					     {16,0xFFA4},{16,0xFFA5}},
					    {{7,0x007B},{12,0x0FF6},
					     {16,0xFFA6},{16,0xFFA7},
					     {16,0xFFA8},{16,0xFFA9},
					     {16,0xFFAA},{16,0xFFAB},
					     {16,0xFFAC},{16,0xFFAD}},
					    {{8,0x00FA},{12,0x0FF7},
					     {16,0xFFAE},{16,0xFFAF},
					     {16,0xFFB0},{16,0xFFB1},
					     {16,0xFFB2},{16,0xFFB3},
					     {16,0xFFB4},{16,0xFFB5}},
					    {{9,0x01F8},{15,0x7FC0},
					     {16,0xFFB6},{16,0xFFB7},
					     {16,0xFFB8},{16,0xFFB9},
					     {16,0xFFBA},{16,0xFFBB},
					     {16,0xFFBC},{16,0xFFBD}},
					    {{9,0x01F9},{16,0xFFBE},
					     {16,0xFFBF},{16,0xFFC0},
					     {16,0xFFC1},{16,0xFFC2},
					     {16,0xFFC3},{16,0xFFC4},
					     {16,0xFFC5},{16,0xFFC6}},
					    {{9,0x01FA},{16,0xFFC7},
					     {16,0xFFC8},{16,0xFFC9},
					     {16,0xFFCA},{16,0xFFCB},
					     {16,0xFFCC},{16,0xFFCD},
					     {16,0xFFCE},{16,0xFFCF}},
					    {{10,0x03F9},{16,0xFFD0},
					     {16,0xFFD1},{16,0xFFD2},
					     {16,0xFFD3},{16,0xFFD4},
					     {16,0xFFD5},{16,0xFFD6},
					     {16,0xFFD7},{16,0xFFD8}},
					    {{10,0x03FA},{16,0xFFD9},
					     {16,0xFFDA},{16,0xFFDB},
					     {16,0xFFDC},{16,0xFFDD},
					     {16,0xFFDE},{16,0xFFDF},
					     {16,0xFFE0},{16,0xFFE1}},
					    {{11,0x07F8},{16,0xFFE2},
					     {16,0xFFE3},{16,0xFFE4},
					     {16,0xFFE5},{16,0xFFE6},
					     {16,0xFFE7},{16,0xFFE8},
					     {16,0xFFE9},{16,0xFFEA}},
					    {{16,0xFFEB},{16,0xFFEC},
					     {16,0xFFED},{16,0xFFEE},
					     {16,0xFFEF},{16,0xFFF0},
					     {16,0xFFF1},{16,0xFFF2},
					     {16,0xFFF3},{16,0xFFF4}},
					    {{16,0xFFF5},{16,0xFFF6},
					     {16,0xFFF7},{16,0xFFF8},
					     {16,0xFFF9},{16,0xFFFA},
					     {16,0xFFFB},{16,0xFFFC},
					     {16,0xFFFD},{16,0xFFFE}}};

  const unsigned int ac_special[2][2] = {{4,0x000A},
					 {11,0x07F9}};

  unsigned int binary[16] = {0x8000, 0x4000, 0x2000, 0x1000,
			     0x0800, 0x0400, 0x0200, 0x0100,
			     0x0080, 0x0040, 0x0020, 0x0010,
			     0x0008, 0x0004, 0x0002, 0x0001};  

  int h, i, j, k, l, m, n, p;
  int x1 = fixed_x_ / 8;
  int y1 = fixed_y_ / 8;
  int px, py, index;

  int ***y = new int**[y1];
  for (i = 0; i < y1; i++) {
    y[i] = new int*[x1];
    for (j = 0; j < x1; j++) {
      y[i][j] = new int[64];
    }
  }

  int s = (x1 / 2) * (y1 / 2);
  
  int **cb = new int*[s];
  for (i = 0; i < s; i++) {
    cb[i] = new int[64];
  }

  int **cr = new int*[s];
  for (i = 0; i < s; i++) {
    cr[i] = new int[64];
  }

  for (j = 0; j < y1; j++) {
    for (i = 0; i < x1; i++) {
      for (l = 0; l < 8; l++) {
	for (k = 0; k < 8; k++) {
	  n = k + l * 8;
	  px = zig_x[n] + 8 * i;
	  py = zig_y[n] + 8 * j;
	  index = px + py * fixed_x_;
	  y[j][i][n] = dct_y_[index];
	}
      }
    }
  }


  x1 = x1 / 2;
  y1 = y1 / 2;

  for (j = 0; j < y1; j++) {
    for (i = 0; i < x1; i++) {
      m = i + j * x1;
      for (l = 0; l < 8; l++) {
	for (k = 0; k < 8; k++) {
	  n = k + l * 8;
	  px = zig_x[n] + 8 * i;
	  py = zig_y[n] + 8 * j;
	  index = px + py * fixed_x_ / 2;
	  cb[m][n] = dct_cb_[index];
	  cr[m][n] = dct_cr_[index];
	}
      }
    }
  }

  int pre = 0;
  int dif, dig, run;
  int eob = 0;
  int pre_cb = 0, pre_cr = 0;
  int tmp;

  x1 = x1 * 2;
  y1 = y1 * 2;
  
  for (n = 0; n < y1; n = n + 2) {
    for (m = 0; m < x1; m = m + 2) {
      for (p = 0; p < 4; p++) {
	i = m + (p % 2);
	h = n + (p / 2);

	dif = y[h][i][0] - pre;
	pre = y[h][i][0];
	dig = digit(dif);

	outstream(dc_table[dig][0], dc_table[dig][1]);

	if (dif > 0) outstream(dig, dif);
	else outstream(dig, bit_inv(dig, -dif));

	run = 0;

	for (j = 1; j < 64; j++) {
	  if (y[h][i][j] == 0) {
	    if (j == 63)
	      outstream(ac_special[0][0], ac_special[0][1]);
	    else if (run == 15) {
	      for (k = j +1; k < 64; k++) {
		if (y[h][i][k] != 0) eob = 1;
	      }
	      if (eob == 0) {
		outstream(ac_special[0][0], ac_special[0][1]);
		break;
	      }
	      else {
		outstream(ac_special[1][0], ac_special[1][1]);
		eob = 0;
		run = 0;
	      }
	    }
	    else
	      run++;
	  }

	  else if (y[h][i][j] > 0) {
	    dig = digit(y[h][i][j]);
	    outstream(ac_table[run][dig-1][0], ac_table[run][dig-1][1]);
	    outstream(dig, y[h][i][j]);
	    run = 0;
	  }

	  else {
	    dig = digit(-y[h][i][j]);
	    outstream(ac_table[run][dig-1][0], ac_table[run][dig-1][1]);
	    outstream(dig, bit_inv(dig, -y[h][i][j]));
	    run = 0;
	  }
	}
      }

      // cb
      tmp = (m / 2) + (n / 2) * (x1 / 2);
      dif = cb[tmp][0] - pre_cb;
      pre_cb = cb[tmp][0];
      dig = digit(dif);
      outstream(dc_table[dig][0], dc_table[dig][1]);

      if (dif > 0) outstream(dig, dif);
      else outstream(dig, bit_inv(dig, -dif));

      run = 0;

      for (j = 1; j < 64; j++) {
	if (cb[tmp][j] == 0) {
	  if (j == 63)
	    outstream(ac_special[0][0], ac_special[0][1]);
	  else if (run == 15) {
	    for (k = j + 1; k < 64; k++) {
	      if  (cb[tmp][k] != 0) eob = 1;
	    }
	    if (eob == 0) {
	      outstream(ac_special[0][0], ac_special[0][1]);
	      break;
	    }
	    else {
	      outstream(ac_special[1][0], ac_special[1][1]);
	      eob = 0;
	      run = 0;
	    }
	  }
	  else run++;
	}

	else if (cb[tmp][j] > 0) {
	  dig = digit(cb[tmp][j]);
	  outstream(ac_table[run][dig-1][0], ac_table[run][dig-1][1]);
	  outstream(dig, cb[tmp][j]);
	  run = 0;
	}
	else {
	  dig = digit(-cb[tmp][j]);
	  outstream(ac_table[run][dig-1][0], ac_table[run][dig-1][1]);
	  outstream(dig, bit_inv(dig, -cb[tmp][j]));
	  run = 0;
	}
      }
      // cb end

      // cr start
      dif = cr[tmp][0] - pre_cr;
      pre_cr = cr[tmp][0];
      dig = digit(dif);
      outstream(dc_table[dig][0], dc_table[dig][1]);
      if (dif > 0) outstream(dig, dif);
      else outstream(dig, bit_inv(dig, -dif));

      run = 0;

      for (j = 1; j < 64; j++) {
	if (cr[tmp][j] == 0) {
	  if (j == 63)
	    outstream(ac_special[0][0], ac_special[0][1]);
	  else if (run == 15) {
	    for (k = j + 1; k < 64; k++) {
	      if (cr[tmp][k] != 0) eob = 1;
	    }
	    if (eob == 0) {
	      outstream(ac_special[0][0], ac_special[0][1]);
	      break;
	    }
	    else {
	      outstream(ac_special[1][0], ac_special[1][1]);
	      eob = 0;
	      run = 0;
	    }
	  }
	  else run++;
	}

	else if (cr[tmp][j] > 0) {
	  dig = digit(cr[tmp][j]);
	  outstream(ac_table[run][dig-1][0], ac_table[run][dig-1][1]);
	  outstream(dig, cr[tmp][j]);
	  run = 0;
	}
	else {
	  dig = digit(-cr[tmp][j]);
	  outstream(ac_table[run][dig-1][0], ac_table[run][dig-1][1]);
	  outstream(dig, bit_inv(dig, -cr[tmp][j]));
	  run = 0;
	}
      }
      // cr end
    }
  }

  if (count_ > 0) {
    tmp = 0;
    for (i = 0; i < (8 - count_); i++) {
      tmp = tmp + binary[15-i];
    }
    outstream((8-count_), tmp);
  }

  for (i = 0; i < y1; i++) {
    for (j = 0; j < x1; j++) {
        delete[] y[i][j];
    }
    delete[] y[i];
  }
  delete[] y;

  for (i = 0; i < s; i++) { 
    delete[] cb[i];
    delete[] cr[i];
  }
  delete[] cb;
  delete[] cr;

  return;
}

void Jpegencoder::endHeader()
{
  unsigned char buf[2] = {0xff, 0xd9};

  if (fwrite(buf, sizeof(unsigned char), 2, output_fp_) < 2)
    perror("EOI_fwrite");

  return;
}

void Jpegencoder::printQuant()
{
  int i, j, index;
  FILE* fp = fopen("quant.txt", "w");

  for (i = 0; i < fixed_y_; i++) {
    for (j = 0; j < fixed_x_; j++) {
      index = j + i * fixed_x_;
      fprintf(fp, "%d ", dct_y_[index]);
    }
    fprintf(fp, "\n");
  }

  fprintf(fp, "\n");

  for (i = 0; i < fixed_y_ / 2; i++) {
    for (j = 0; j < fixed_x_ / 2; j++) {
      index = j + i * fixed_x_ / 2;
      fprintf(fp, "%d ", dct_cb_[index]);
    }
    fprintf(fp, "\n");
  }

  fprintf(fp, "\n");

  for (i = 0; i < fixed_y_ / 2; i++) {
    for (j = 0; j < fixed_x_ / 2; j++) {
      index = j + i * fixed_x_ / 2;
      fprintf(fp, "%d ", dct_cr_[index]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);

  return;
}

void Jpegencoder::printPic()
{
  FILE* fp = fopen("pic.txt", "w");
  int i, j, index;

  for (i = 0; i < fixed_y_; i++) {
    for (j = 0; j < fixed_x_; j++) {
      index = j + i * fixed_x_;
      fprintf(fp, "%3d ", col_y_[index]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "\n");
  for (i = 0; i < fixed_y_ / 2; i++) {
    for (j = 0; j < fixed_x_ / 2; j++) {
      index = j + i * fixed_x_ / 2;
      fprintf(fp, "%3d ", col_cb_[index]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "\n");
  for (i = 0; i < fixed_y_ / 2; i++) {
    for (j = 0; j < fixed_x_ / 2; j++) {
      index = j + i * fixed_x_ / 2;
      fprintf(fp, "%3d ", col_cr_[index]);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);

  return;
}
