#include <math.h>
#include "sift.h"

int current_octave = 0;
float* grad_buffer = NULL;

void
keypoint_orientation(SiftKeypoint* key, float *scaleImage,
                     float sigma, int octaveW, int octaveH);

// Calculate the orientations of all the keypoints
void
orientation(pointList* keyHead, float *scaleSpace[MAX_O][MAX_S],
            float sigmas[MAX_O][MAX_S], int* octavesW, int* octavesH) {
    pointList* keyP = keyHead;
    while (keyP != NULL) {
        SiftKeypoint* key = &keyP->self;
        int octave  = key->o;
        float scale = key->s;
        float* scaleImage = scaleSpace[octave][(int)scale];
        float sigma = sigmas[octave][(int)scale];
        int octW = octavesW[octave];
        int octH = octavesH[octave];
        keypoint_orientation(key, scaleImage, sigma, octW, octH);
        keyP = keyP->next;
    }// while (keyP != NULL)
}// orientation()

void
keypoint_orientation(SiftKeypoint* key, float *scaleImage,
                     float sigma, int octaveW, int octaveH) {
    float PI = 3.1415926f;
    float ori_hist[ORI_BINS] = {0.0f};
    int bin = ORI_BINS;
    int x = (int)(key->x);
    int y = (int)(key->y);
    float radius = 1.5f * sigma;

    int i, j;
    float maxBin = 0.0f;
	int maxOri = 0;
    for(i = -1 * (int)(radius); i < (int)(radius) + 1; ++i){
        for(j = -1 * (int)(radius); j < (int)(radius) + 1; ++j){
            // compute magnitude and orientation
            int index = (y + i) * octaveW + (x + j);
            int voteIndex;
            float mag, ori, weight;
            float dx = scaleImage[index+1] - scaleImage[index-1];
            float dy = scaleImage[index-octaveW] - scaleImage[index+octaveW];
            if(dx == 0.0f && dy == 0.0f){
                continue;
            }
            mag = sqrt(dx*dx + dy*dy);
            ori = atan2(dy, dx);
            // voting to histo bins
            // weight for smoothing
            weight = exp( -1.0f * (i*i+j*j) / (2.0f*sigma*sigma) );
            // oriHisto represent orientations from - PI to PI.
            // (index == 0) means - PI
            // (index == bin-1) means PI
            voteIndex = (int)(bin * ((ori + PI) / (2.0f * PI)) );
            // avoid exceeding the boundary of the histogram
            if(voteIndex < 0 ){
                voteIndex = 0;
            } else if(voteIndex > (ORI_BINS - 1) ){
                voteIndex = (ORI_BINS - 1);
            }
            // weight the vote with Gaussian smoothing
            ori_hist[voteIndex] += weight * mag;
            // update the major orientation
            if(ori_hist[voteIndex] > maxBin){
                maxBin = ori_hist[voteIndex];
                maxOri = voteIndex;
            }
        }// for j
    }// for i
    // ((maxOri*10) - 180) * PI/180
    key->ori = maxOri*PI/18 - PI;
    key->mag = maxBin;
}// keypoint_orientation()
