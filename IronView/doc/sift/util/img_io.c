#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "img_io.h"

unsigned char*
read_pgm(int* w, int* h, const char * filename){
    unsigned char* data;
    FILE* in_file;
    char ch, type;
    int i;
    
    in_file = fopen(filename, "rb");
    if (! in_file){
        fprintf(stderr, "ERROR(0): Fail to open file %s\n", filename);
        exit(1);
    }
    /* Determine pgm image type (only type three can be used)*/
    ch = getc(in_file);
    if(ch != 'P'){
        printf("ERROR(1): Not valid pgm/ppm file type\n");
        exit(1);
    }
    ch = getc(in_file);
    /* Convert the one digit integer currently represented as a character to
     * an integer(48 == '0') */
    type = ch - 48;
    if(type != 5)
    {
        printf("ERROR(2): this file type (P%d) is not supported!\n", type);
        exit(1);
    }
    while(getc(in_file) != '\n');      /* Skip to end of line*/
    while (getc(in_file) == '#'){       /* Skip comment lines */
        while (getc(in_file) != '\n');
    }
    fseek(in_file, -1, SEEK_CUR);     /* Backup one character*/

    fscanf(in_file,"%d", w);
    fscanf(in_file,"%d", h);
    fscanf(in_file,"%d", &i);  /* Skipped here */
    while(getc(in_file) != '\n');
    data = (unsigned char*)malloc((*w)*(*h)*sizeof(unsigned char));

    fread(data, sizeof(unsigned char), (*w)*(*h), in_file);
    return data;
}// read_pgm()

void
write_pgm(unsigned char* data, int w, int h, const char* filename){
    FILE* out_file;
    assert(w > 0);
    assert(h > 0);

    out_file = fopen(filename, "wb");
    if (! out_file){
        fprintf(stderr, "Fail to open file: %s\n", filename);
        exit(1);
    }
    
    fprintf(out_file, "P5\n");
    fprintf(out_file, "%d %d\n255\n", w, h);
    fwrite(data, sizeof(unsigned char), w * h, out_file);
    fclose(out_file);
}// write_pgm ()

void
write_ppm(unsigned char* r, unsigned char* g, unsigned char* b, int w, int h,
          const char * filename) {
    FILE * out_file;
    int i;

    unsigned char* obuf = (unsigned char*)malloc(3*w*h*sizeof(unsigned char));

    for(i = 0; i < w*h; i ++){
        obuf[3*i + 0] = r[i];
        obuf[3*i + 1] = g[i];
        obuf[3*i + 2] = b[i];
    }
    out_file = fopen(filename, "wb");
    fprintf(out_file, "P6\n");
    fprintf(out_file, "%d %d\n255\n", w, h);
    fwrite(obuf,sizeof(unsigned char), 3*w*h, out_file);
    fclose(out_file);
    free(obuf);
}// write_ppm()


void
write_float_pgm(float* data, int w, int h, const char* filename, int mode){
    int i, j;
    unsigned char* charImg;
    int tmpInt;
    charImg = (unsigned char*)
        malloc(w*h*sizeof(unsigned char));
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            if(mode == 1){ // clop
                if( data[i*w+j] >= 255.0){
                    charImg[i*w+j] = 255;
                } else if(data[i*w+j] <= 0.0){
                    charImg[i*w+j]=0;
                } else{
                    charImg[i*w+j]=(int)data[i*w+j];
                }
            }else if(mode == 2){ // abs, x10, clop
                tmpInt = 
                    (int)(fabs(data[i*w+j])*10.0);
                if( fabs(data[i*w+j]) >= 255){
                    charImg[i*w+j] = 255;
                } else if(tmpInt <= 0){
                    charImg[i*w+j] = 0;
                } else{
                    charImg[i*w+j] = tmpInt;
                }
            }else{
                return;
            }
        }
    }
    write_pgm(charImg, w, h, filename);
    free(charImg);
}// write_float_pgm()

void
setPixelRed(PPM_IMG* img, int x, int y) {
    if( (y >= 0) && (y < img->h) && (x >= 0) && (x < img->w)){
        img->img_r[y*img->w + x] = 255;
        img->img_g[y*img->w + x] = 0;
        img->img_b[y*img->w + x] = 0;
    }
}// setPixelRed()

void
draw_red_circle(PPM_IMG* imgPPM, int x, int y, int cR){
    int cx = -cR, cy = 0, err = 2-2*cR; /* II. Quadrant */
    do {
        setPixelRed(imgPPM, x-cx, y+cy); /*	  I. Quadrant */
        setPixelRed(imgPPM, x-cy, y-cx); /*	 II. Quadrant */
        setPixelRed(imgPPM, x+cx, y-cy); /* III. Quadrant */
        setPixelRed(imgPPM, x+cy, y+cx); /*	 IV. Quadrant */
        cR = err;
        if (cR >  cx)
            err += ++cx*2+1; /* e_xy+e_x > 0 */
        if (cR <= cy)
            err += ++cy*2+1; /* e_xy+e_y < 0 */
    } while (cx < 0);
}// draw_red_circle()

void
draw_red_orientation(PPM_IMG* imgPPM, int x, int y, float ori, int cR) {
    int xe = x + cos(ori)*cR, ye = y + sin(ori)*cR;
    // Bresenham's line algorithm
    int dx =  abs(xe-x), sx = x<xe ? 1 : -1;
    int dy = -abs(ye-y), sy = y<ye ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */

    for(;;){  /* loop */
        setPixelRed(imgPPM, x,y);
        if (x==xe && y==ye) break;
        e2 = 2*err;
        if (e2 >= dy) { err += dy; x += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y += sy; } /* e_xy+e_y < 0 */
    }
}// draw_red_orientation()
