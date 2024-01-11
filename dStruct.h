#ifndef DSTRUCT_H
#define DSTRUCT_H

typedef struct{
    int pid;
    int status;
    int niceness;
    float cpuTime;
    float procTime;
    //add the next thing pointer 
    //possibly need a node type idk check notes 
} Data;
#endif /* DSTRUCT_H */