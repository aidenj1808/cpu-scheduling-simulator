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
void destroyData(Data *dataList);

/*
 *
 */
Data *getEntry(Data *dataList, unsigned int pid, int index);

/*
 *
 */
void pushEntry(Data **dataList, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime);

/*
 *
 */
Data *popData();

/*
 *
 */
float getCPUTime(Data *dataList, unsigned int pid);

/*
 *
 */
void setCPUTime(Data *dataList, unsigned int pid, float cpuTime);

/*
 *
 */
int getNiceness(Data *dataList, unsigned int pid);

/*
 *
 */
void setNiceness(Data *dataList, unsigned int pid, int niceness);

/*
 *
 */
unsigned int getStatus(Data *dataList, unsigned int pid);

/*
 *
 */
void setStatus(Data *dataList, unsigned int pid, unsigned int status);

/*
 *
 */
int searchEntryPID(Data *dataList, unsigned int pid);

/*
 *
 */
int searchEntryNicenessStatus(Data *dataList, int niceness, unsigned int status);

/*
 *
 */
void printAllEntries(Data *dataList);

/*
 *
 */
void printEntryPID(Data *dataList, unsigned int pid);

/*
 *
 */
void printEntryNicenessStatus(Data *dataList, int niceness, unsigned int status);

#endif /* DSTRUCT_H */
