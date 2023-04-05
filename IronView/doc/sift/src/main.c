#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "img_io.h"
#include "sift.h"

int
main(int argc, char* argv[]) {
    int w, h;
    unsigned char* in_img;
    if (argc != 3) {
        fprintf(
            stderr, 
            "Usage: eca-sift input-image output-image\n");
        exit(1);
    }
    in_img = read_pgm(&w, &h, argv[1]);
    assert(in_img != NULL);
    sift(in_img, w, h, argv[2]);
    return 0;
}// main()

