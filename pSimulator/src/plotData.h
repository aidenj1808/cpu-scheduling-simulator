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
	struct Info *first;
	int size;//number of procs
    //maybe in here save a total for Tturn and Tresp?? then will be easy to calc avg at end?
} InfoList;

InfoList *createInfoList();

void destroyInfoList(InfoList **infoList);

int searchEntryPID(InfoList *infoList, unsigned int pid);

void printInfoList(InfoList *infoList);

//int plotData(int length, float *data);//old
int plotData(InfoList *infoList);//new

void setEXTTime(InfoList *infoList, unsigned int pid, float globalTime, float cpuTime);

void setRUNTime(InfoList *infoList, unsigned int pid, float globalTime);

float randCol();

//float *getGraphData(int fileCount); //old call
void getGraphData(int fileCount, InfoList *infoList);//new call

int getFileCount(char *path);

float setScale(InfoList *infoList);

#endif /* PLOTDATA_H */