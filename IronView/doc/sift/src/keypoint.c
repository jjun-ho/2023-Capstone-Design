#include <assert.h>
#include <stdlib.h>
#include "sift.h"

void
write_keypoints(FILE* fp, SiftKeypoint* keys, int n) {
    int i, j;
    assert(fp != NULL);
    fprintf(fp, "%d %d\n", n, NUM_OF_DESCRIPTORS);
    for (i = 0; i < n; ++i) {
        fprintf(fp, "%.3f %.3f %.3f %.3f\n", keys[i].x, keys[i].y,
                keys[i].s, (float)keys[i].o);
        for (j = 0; j < NUM_OF_DESCRIPTORS; ++j) {
            fprintf(fp, " %d", (int)(keys[i].descriptors[j]));
            if ((j%19) == 0)
                fprintf(fp, "\n");
        }// for j = 0 to NUM_OF_DESCRIPTORS
        fprintf(fp, "\n");
    }// for i = 0 to n
}// write_keypoints()
