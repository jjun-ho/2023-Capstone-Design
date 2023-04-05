#include "sift.h"

/**********************************************
 * Difference of Gaussians (DoG)
 * Only diff neighboring scales in same octave
 * Do not diff across two octaves
 *
 * Parameters
 * dog: output, Ox(S-1) images in floating point
 * scaleSpace: input, OxS images in floating pt
 * O: number of octaves
 * S: scales per octave
 * octavesW: output, width of each octave
 * octavesH: output, height of each octave
 **********************************************/

void DoG(float *dog[MAX_O][MAX_S-1], 
         float *scaleSpace[MAX_O][MAX_S], 
         int O, int S,
         int* octavesW, int* octavesH){
  
    // comon variables
    int i, j, ii, jj;
  
    for(i = 0; i < O; i++){
        for(j = 0; j < S-1; j++){
            for(ii = 0; ii < octavesH[i]; ii++){
                for(jj = 0; jj < octavesW[i]; jj++){
                    // dog = upper scale - lower scale
                    dog
                        [i]
                        [j]
                        [ii*octavesW[i]+jj] 
                        =
                        scaleSpace
                        [i]
                        [j+1]
                        [ii*octavesW[i]+jj] 
                        -
                        scaleSpace
                        [i]
                        [j]
                        [ii*octavesW[i]+jj];
                }
            }
        }
    }

}
