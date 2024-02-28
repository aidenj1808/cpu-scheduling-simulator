#ifndef DSTRUCT_H
#define DSTRUCT_H

typedef struct Node {
    unsigned int pid;
    unsigned int status;
    int niceness;
    float cpuTime;
    float procTime;
	struct Node *next;
} Node;

typedef struct DataList {
	struct Node *first;
	int size;
} DataList;

typedef struct Levels{
    struct DataList *five;
    struct DataList *four;
    struct DataList *three;
    struct DataList *two;
    struct DataList *one;
}Levels;


DataList *createDataList();

void destroyDataList(DataList **dataList);

Node *getEntry(DataList *dataList, unsigned int pid, int index);

void pushEntry(DataList **dataList, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime);

Node popEntry(DataList *dataList, int index);

float getCPUTime(DataList *dataList, unsigned int pid);

void setCPUTime(DataList *dataList, unsigned int pid, float cpuTime);

int getNiceness(DataList *dataList, unsigned int pid);

void setNiceness(DataList *dataList, unsigned int pid, int niceness);

unsigned int getStatus(DataList *dataList, unsigned int pid);

void setStatus(DataList *dataList, unsigned int pid, unsigned int status);

int searchEntryPID(DataList *dataList, unsigned int pid);

int searchEntryNicenessStatus(DataList *dataList, int niceness, unsigned int status);

void printAllEntries(DataList *dataList);

void printEntryPID(DataList *dataList, unsigned int pid);

void printEntryNicenessStatus(DataList *dataList, int niceness, unsigned int status);

#endif /* DSTRUCT_H */
