#ifndef _SIFT_H_
#define _SIFT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/****************************************
 * Flags
 ***************************************/
// print information
#define VERBOSE 1

/****************************************
 * Global Definitions
 ***************************************/

// maximum number of octaves
#define MAX_O (20)
// maximum nuber of scales per octave
#define MAX_S (10)
// number of bins for orientation histogram
#define ORI_BINS (36)

/****************************************
 * Data Structures
 ***************************************/

// 128-element descriptor
#define NUM_OF_DESCRIPTORS (128)
typedef struct SiftKeypoint_t {
    int o; // octave number
    float x; // x coordinate
    float y; // y coordinate
    float s; // scale
    float ori; // orientation in degrees.
    float mag; // magnitude
    float descriptors [NUM_OF_DESCRIPTORS];
} SiftKeypoint;

// a linked list for descriptors
typedef struct pointList_t{
    SiftKeypoint self;
    struct pointList_t* next;
} pointList;

// Gradient buffer for orientation and descriptor calculation
extern int current_octave;
extern float* grad_buffer;

/****************************************
 * Functions
 ***************************************/

void sift(unsigned char* img, int w, int h, char* out_filename);

void buildOB(unsigned char* img, 
             unsigned char* octaves[MAX_O], 
             int O, int S, 
             int* octavesW, int* octavesH);

void buildSS(unsigned char* octaves[MAX_O], 
             float* scaleSpace[MAX_O][MAX_S], 
             int O, int S, 
             int* octavesW, int* octavesH, 
             float sigmas[MAX_O][MAX_S]);

void DoG(float *dog[MAX_O][MAX_S-1], 
         float *scaleSpace[MAX_O][MAX_S], 
         int O, int S,
         int* octavesW, int* octavesH);

void extreme(pointList** keyHead, int* keyN,
             float* dog[MAX_O][MAX_S-1], 
             int O, int S,
             int* octavesW, int* octavesH);

void orientation(pointList* keyHead, 
                 float *scaleSpace[MAX_O][MAX_S],
                 float sigmas[MAX_O][MAX_S], 
                 int* octavesW, int* octavesH);

void write_keypoints(FILE* fp, 
                     SiftKeypoint* keys, 
                     int n);

void  render(unsigned char* img, char* out_filename,
             int O, int S, 
             int* octavesW, int* octavesH,
             pointList* keyHead, int keyN, 
             float sigmas[MAX_O][MAX_S]);

#endif//_SIFT_H_

