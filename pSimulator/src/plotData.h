#ifndef PLOTDATA_H
#define PLOTDATA_H

typedef struct Info {
    unsigned int pid;
    //not exactly sure what will be needed..
    float r;
    float g;
    float b;
    float cpuTime;
    float procTime;
    float Tarr;//first appearance s=1
    float Trun;//when s=2
    float Text;//when s=3
    //do we want to save these or just do the calcs?
    float Tturn; //= Text - Tarr
    float Tresp; //= Trun - Tarr
	struct Info *next;
} Info;

typedef struct InfoList {
	struct Node *first;
	int size;//number of procs
    //maybe in here save a total for Tturn and Tresp?? then will be easy to calc avg at end?
} InfoList;

InfoList *createInfoList();

void destroyInfoList(InfoList **infoList);

int searchEntryPID(InfoList *infoList, unsigned int pid);

int plotData(int length, float *data);

void setEXTTime(InfoList *infoList, unsigned int pid, float globalTime);

void setRUNTime(InfoList *infoList, unsigned int pid, float globalTime);

float randCol(float pid);

//float *getGraphData(int fileCount); //old call
void getGraphData(int fileCount, InfoList *infoList);//new call

int getFileCount(char *path);

float setScale(int length, float *data);

#endif /* PLOTDATA_H */