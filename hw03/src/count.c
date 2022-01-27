#include "libvc.h"

#include <211.h>

#include <stdio.h>

#define OOM_MESSAGE     "%s: out of memory\n"
#define DROP_MESSAGE    "%s: vote dropped: %s\n"
#define FINAL_MESSAGE   "%s: %zu vote(s) dropped\n"

int main(int argc, char* argv[])
{
    int dropped = 0;
    vote_count_t start = vc_create();
    if(start==NULL){
        fprintf(stderr,OOM_MESSAGE, argv[0]);
        exit(1);
    }
    else{
        char* line;
        while((line=read_line())!=NULL){
            size_t* count = vc_update(start,line);
            if(count==NULL){
                dropped++;
                fprintf(stderr, DROP_MESSAGE, argv[0], line);
            }
            *count ++;
        }
        vc_print(start);
        vc_destroy(start);
        if(dropped>0){
            fprintf(stderr, FINAL_MESSAGE,argv[0], dropped);
            exit(2);
        }
    }

    return 0;
}

