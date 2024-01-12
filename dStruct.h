#ifndef DSTRUCT_H
#define DSTRUCT_H

typedef struct{
    unsigned int pid;
    unsigned int status;
    int niceness;
    float cpuTime;
    float procTime;
    //add the next thing pointer 
    //possibly need a node type idk check notes 
} Data;

/*
 *
 */
Data createData();

/*
 *
 */
void destroyData(Data *dataArray);

/*
 *
 */
Data getEntryByPID(Data *dataArray, unsigned int pid);

/*
 *
 */
Data getEntryByIndex(Data *dataArray, int index);

/*
 *
 */
void pushData(unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime);

/*
 *
 */
Data popData();

/*
 *
 */
float getCPUTime(Data *dataArray, unsigned int pid);

/*
 *
 */
void setCPUTime(unsigned int pid, float cpuTime);

/*
 *
 */
int getNiceness(Data *dataArray, unsigned int pid);

/*
 *
 */
void setNiceness(unsigned int pid, int niceness);

/*
 *
 */
unsigned int getStatus(Data *dataArray, unsigned int pid);

/*
 *
 */
void setStatus(Data *dataArray, unsigned int pid);

/*
 *
 */
_Bool searchEntryByPID(Data *dataArray, unsigned int pid);

/*
 *
 */
_Bool searchEntryByNiceness(Data *dataArray, int niceness);

/*
 *
 */
_Bool searchEntryByStatus(Data *dataArray, unsigned int status);

/*
 *
 */
void printAllEntries(Data *dataArray);

/*
 *
 */
void printEntryByPID(Data *dataArray, unsigned int pid);

/*
 *
 */
void printEntryByNiceness(Data *dataArray, int niceness);

/*
 *
 */
void printEntryByStatus(Data *dataArray, unsigned int status);
#endif /* DSTRUCT_H */
