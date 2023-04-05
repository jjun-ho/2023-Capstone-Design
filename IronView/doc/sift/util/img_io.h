#ifndef _IMG_IO_H_
#define _IMG_IO_H_

typedef struct {
  int w;
  int h;
  unsigned char * img_r;
  unsigned char * img_g;
  unsigned char * img_b;
} PPM_IMG;

unsigned char* read_pgm(int* w, int* h, const char* filename);
void write_pgm(unsigned char* data, int w, int h, const char* filename);
void write_ppm(unsigned char* r, unsigned char* g, unsigned char* b,
               int w, int h, const char * filename);
void write_float_pgm(float* data, int w, int h, const char* filename, int mode);

void draw_red_circle(PPM_IMG* imgPPM, int x, int y, int cR);
void draw_red_orientation(PPM_IMG* imgPPM, int x, int y, float ori, int cR);

#endif//_IMG_IO_H_
