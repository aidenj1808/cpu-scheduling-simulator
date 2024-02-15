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
	struct Info *next;
} Info;

typedef struct InfoList {
	struct Info *first;
	int size;//number of procs
    float Tturn; //= Text - Tarr (this is the avg saved)
    float Tresp; //= Trun - Tarr (this is the avg saved)
} InfoList;

//just creates the initial entry
InfoList *createInfoList();

//loops through freeing memory then frees the main struct
void destroyInfoList(InfoList **infoList);

//searches if a pid already recorded if yes returns 1 else returns 0
int searchEntryPID(InfoList *infoList, unsigned int pid);

//just for test purposes to print out everything that has been recorded 
void printInfoList(InfoList *infoList);

//int plotData(int length, float *data);//old
//driver of plotting the graph and saving to png
int plotData(InfoList *infoList);//new

//finds the associated pid entry and sets the Text and cputime from the log files 
void setEXTTime(InfoList *infoList, unsigned int pid, float globalTime, float cpuTime);

//finds the entry associated with the pid and sets the time it is first ran
void setRUNTime(InfoList *infoList, unsigned int pid, float globalTime);

//returns a random float value for use in rgb
float randCol();

//float *getGraphData(int fileCount); //old call
//enters the directory and each files adding processes and information to the InfoList
void getGraphData(int fileCount, InfoList *infoList);//new call

//counts the number of completed proccesses by looking for status = 3
int getFileCount(char *path);

//calculates the total size of procs and will scale it to fit within the canvas borders returns this scale factor
float setScale(InfoList *infoList);

//calculates the avg Tresp and Tturn for the given procs 
void calcAVG(InfoList *infoList); 

#endif /* PLOTDATA_H */