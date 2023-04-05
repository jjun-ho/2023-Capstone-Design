#include "sift.h"
#include "img_io.h"

void  render(unsigned char* img, char* out_filename,
             int O, int S, 
             int* octavesW, int* octavesH,
             pointList* keyHead, int keyN, 
             float sigmas[MAX_O][MAX_S]){
  
    // common variables
    int i, j;
    // pointer for linked list traversal
    pointList* keyPtr = keyHead;
    // color imag ein PPM format
    PPM_IMG imgPPM;
    // image width and height of octave base
    int w, h;
    // x, y position of keypoint scaled to octave base
    int x, y;

    /*******************************
     * cR:
     * radius of the circle
     * cR = sigma * 4 * (2^O)
     *******************************/
    int cR;

    // assign width and height
    w = octavesW[0];
    h = octavesH[0];
  
    // initialize the imgPPM
    imgPPM.w =  w;
    imgPPM.h =  h;
    imgPPM.img_r = 
        (unsigned char*)
        malloc( w * h * sizeof(unsigned char) );
    imgPPM.img_g = 
        (unsigned char*)
        malloc( w * h * sizeof(unsigned char) );
    imgPPM.img_b = 
        (unsigned char*)
        malloc( w * h * sizeof(unsigned char) );

    // copy gray PGM img to color PPM img
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            imgPPM.img_r[i*w+j] = img[i*w+j];
            imgPPM.img_g[i*w+j] = img[i*w+j];
            imgPPM.img_b[i*w+j] = img[i*w+j];
        }
    }

    for(i = 0; i < keyN; i++){
        // derive circle radius cR
        cR = 
            (int)
            ( sigmas
              [keyPtr->self.o]
              [(int)keyPtr->self.s] * 
              pow(2,keyPtr->self.o) );
        if(cR <= 1){ // avoid zero radius
            cR = 1;
        }
        x = (int)(keyPtr->self.x) * 
            (w/octavesW[keyPtr->self.o]);
        y = (int)(keyPtr->self.y) * 
            (h/octavesH[keyPtr->self.o]);
        draw_red_circle(&imgPPM, (int)x, (int)y, cR);
        draw_red_orientation(&imgPPM, (int)x, (int)y, keyPtr->self.ori, cR);
        keyPtr = keyPtr->next;
    }

    // write rendered image to output
    write_ppm(imgPPM.img_r, imgPPM.img_g, imgPPM.img_b, w, h, out_filename);

    // free allocated memory
    free(imgPPM.img_r);
    free(imgPPM.img_g);
    free(imgPPM.img_b);
}// render()
