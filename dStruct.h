#ifndef DSTRUCT_H
#define DSTRUCT_H

typedef struct Data {
    unsigned int pid;
    unsigned int status;
    int niceness;
    float cpuTime;
    float procTime;
	struct Data *next;
    //possibly need a node type idk check notes 
} Data;

/*
 *
 */
Data *createData();

/*
 *
 */
void destroyData(Data *dataArray);

/*
 *
 */
Data getEntry(Data *dataArray, unsigned int pid, int index);

/*
 *
 */
void pushEntry(Data *dataArray, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime);

/*
 *
 */
Data *popData();

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
int searchEntryPID(Data *dataArray, unsigned int pid);

/*
 *
 */
int searchEntryNicenessStatus(Data *dataArray, int niceness, unsigned int status);

/*
 *
 */
void printAllEntries(Data *dataArray);

/*
 *
 */
void printEntryPID(Data *dataArray, unsigned int pid);

/*
 *
 */
void printEntryNicenessStatus(Data *dataArray, int niceness, unsigned int status);

#endif /* DSTRUCT_H */
