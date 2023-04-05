#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "sift.h"


void read_skey(pointList** keyHead, int* keyN, char* filename);

void match_skey(pointList* keyHead1, int keyN1,
                pointList* keyHead2, int keyN2,
                int* nrMatch);

void diff(SiftKeypoint p1, SiftKeypoint p2, int* flagMatch);

int main(int argc, char* argv[]){

    pointList* keyHead1;
    pointList* keyHead2;
    int keyN1 = 0;
    int keyN2 = 0;
    int nrMatch1;
    int nrMatch2;
    
    if (argc != 3) {
        fprintf(
            stderr, 
            "Usage: match skey1 skey2. File name should not contain '-', e.g. skey-1.\n");
        exit(1);
    }

    read_skey(&keyHead1, &keyN1, argv[1]);
    read_skey(&keyHead2, &keyN2, argv[2]);
    match_skey(keyHead1, keyN1, keyHead2, keyN2, &nrMatch1);
    match_skey(keyHead2, keyN2, keyHead1, keyN1, &nrMatch2);

    printf("points in first skey: %d\n", keyN1);
    printf("points in second skey: %d\n", keyN2);
    printf("match percentage for fisrt skey: %d%%\n", (int)((float)(nrMatch1)*100.0f/(float)keyN1));
    printf("match percentage for second skey: %d%%\n", (int)((float)(nrMatch2)*100.0f/(float)keyN2));

    return 0;
    
}


void read_skey(pointList** keyHead, int* keyN, char* filename){

    // common variables
    int i;
    // file pointer for skey
    FILE *skey;
    // number of elements per key
    int elements;

    // open file for read
    skey = fopen(filename, "r");
    // number of keys
    fscanf(skey, "%d", keyN);
    // elements per key (not used)
    fscanf(skey, "%d", &elements);

    // read all keys
    for(i = 0; i < *keyN; i++){
        // create poitner and memory for new key
        pointList* tmp;
        tmp = (pointList*) 
            malloc(sizeof(pointList));
        // insert new node before the head
        if(i == 0){ // head empty, create it
            tmp->next = NULL;
        }else{ // head not empty, insert new node
            tmp->next = *keyHead;
        }
        // update the head
        *keyHead = tmp;
        // fill in the contents
        fscanf(skey, "%f", &((*keyHead)->self.x));
        fscanf(skey, "%f", &((*keyHead)->self.y));
        fscanf(skey, "%f", &((*keyHead)->self.s));
        fscanf(skey, "%f", &((*keyHead)->self.ori));
    }
    
    fclose(skey);
    
}

void match_skey(pointList* keyHead1, int keyN1,
                pointList* keyHead2, int keyN2,
                int* nrMatch){
    
    int i,j;
    pointList* keyPtr1;
    pointList* keyPtr2;
    int flagMatch;

    *nrMatch = 0;
    keyPtr1 = keyHead1;
    for(i = 0; i < keyN1; i++){
        flagMatch = 0;
        keyPtr2 = keyHead2;
        for(j = 0; j < keyN2; j++){
            diff(keyPtr1->self, keyPtr2->self, &flagMatch);
            if(flagMatch == 1){
                *nrMatch = *nrMatch + 1;
                break;
            } else{
                keyPtr2 = keyPtr2->next;
            }
        }
        keyPtr1 = keyPtr1->next;
    }
    
}


void diff(SiftKeypoint p1, SiftKeypoint p2, int* flagMatch)
{

    float xErrThres = 0.01;
    float yErrThres = 0.01;
    float oErrThres = 0.01;
    float sErrThres = 0.01;
    float oriErrThres = 0.1;

    if( fabs(p1.x - p2.x) <= xErrThres &&
        fabs(p1.y - p2.y) <= yErrThres && 
        fabs(p1.o - p2.o) <= oErrThres && 
        fabs(p1.s - p2.s) <= sErrThres && 
        fabs(p1.ori - p2.ori) <= oriErrThres ){
        *flagMatch = 1;
    }else{
        *flagMatch = 0;
    }
    
}
