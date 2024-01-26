#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dStruct.h"

float GLOBALTIME = 0.00; //do we want this name? or

/*******
 * loops adding and removing processes from running queue,
 *  adding and removing from a ready queue
 * updating log files until the queues are empty
 * main program contains 2 queues ready_queue and running_queue
 * file/directory structure outlined 
 * 
 * new process added by files in newProc folder (proctime, niceness)
 * 
 * each cycle of the simulator
 *  increment global time t
 *  read in information from the files in the folder creating new PID, status of READY, cputime 0
 *      status: 1=ready, 2=running, 3=complete
 *  information then stored by push onto ready queue
 *  once read in file can be removed 
 * when running is empty a new proccess is popped from ready and pushed to running
 * when proccess is done running (cputime>=proctime) 
 * each cycle the cputime should be incremented by a certain amount
 * 
 * a logfile should be kept containing all processes in both queues
 *********/

/*TODO
updating log file 
increment global t
checking running to pop (cpu>proc)
increment cpu
.h for procSim???
*********/



void logFile(DataList *ready_queue, DataList *running_queue, float logTime){
	int len = snprintf(NULL, 0, "../log/log%.2f.txt", logTime);
	char *fileName = malloc(len + 1);
	snprintf(fileName, len + 1, "../log/log%.2f.txt", logTime);
	
	FILE *fptr;
	fptr = fopen(fileName, "w");
	if (fptr == NULL) {
		printf("File cannot be created.\n");
		exit(EXIT_FAILURE);
	}

	Node *readyNode = ready_queue->first;
	while (readyNode != NULL) {
		fprintf(fptr, "%.2f,%u,%u,%d,%.2f,%.2f\n", logTime, readyNode->pid,
												   readyNode->status,
												   readyNode->niceness,
												   readyNode->cpuTime,
												   readyNode->procTime);
		readyNode = readyNode->next;
	}

	Node *runningNode = running_queue->first;
	while (runningNode != NULL) {
		fprintf(fptr, "%.2f,%u,%u,%d,%.2f,%.2f\n", logTime, runningNode->pid,
												   runningNode->status,
												   runningNode->niceness,
												   runningNode->cpuTime,
												   runningNode->procTime);
		runningNode = runningNode->next;
	}
	fclose(fptr);
	free(fileName);
}

int pushReadyQueue(DataList *ready_queue, unsigned int pid) {
	DIR *dirptr = opendir("../newProc/");
	struct dirent *dp;
	for (int i = 0; i < 2; i++) {dp = readdir(dirptr);} //skips . & ..
	dp = readdir(dirptr);
	if (!dp) {
		closedir(dirptr);
		return -1;
	}

	int newProcDirLen = strlen("../newProc/");
	char *path = malloc(newProcDirLen + 1);
 	strcpy(path, "../newProc/");

	int procFileLen = strlen(dp->d_name);
	path = realloc(path, newProcDirLen + procFileLen + 1);
	strcat(path, dp->d_name);
	closedir(dirptr);

	FILE *fptr = fopen(path, "r");
	char line[50];
	fgets(line, 50, fptr);
	int niceness = atoi(strtok(line, ", "));
	float procTime = atoi(strtok(NULL, ", "));
	pushEntry(&ready_queue, pid, 1, niceness, 0.0, procTime);
	fclose(fptr);
	remove(path);
	return 0;
} 

int openDirTest(DataList *ready_queue){
    //maybe just open it each time so that if a new file appears it will be accounted for ??
    DIR *dirpointer;

    FILE *fp; 
    char *line = NULL;
    size_t len = 0;  
    int tmpNice, tmpProc;
    int empty = 0;

    struct dirent *directory;

    dirpointer = opendir("../newProc/");
    //printf("files:\n");

    if(dirpointer){
        directory = readdir(dirpointer);//do it twice cause there will always be a current and parent file...
        directory = readdir(dirpointer);//if dont do these it just ends without pushing file
        if((directory = readdir(dirpointer)) != NULL){
        //while ((directory = readdir(dirpointer)) != NULL){
            //since it also looks at the current and parent 
            if(strcmp(directory->d_name, ".") && strcmp(directory->d_name, "..")){
                char filename[100] = "../newProc/";

                strcat(filename, directory->d_name);
                
                fp = fopen(filename, "r");
                if(fp == NULL){
                    fprintf(stderr, "file cannot be openned\n");
                    exit(EXIT_FAILURE);
                }
                //it is only one line long 
                getline(&line, &len, fp);

                //printf("%s\n", directory->d_name);//prints can be removed later just here to test
                //printf("Contents: %s", line);
                //token = strtok(line, ", "); //idk what way is better?
                //tmpProc = atoi(token);
                tmpProc = atoi(strtok(line, ", "));//technically an int but seems fine... maybe do another cast?
                //printf("First token: %d\n", tmpProc);
                //token = strtok(NULL, "\n");
                //tmpNice = atoi(token);
                tmpNice = atoi(strtok(NULL, "\n"));
                //printf("Second token: %d\n", tmpNice);

                pushEntry(&ready_queue, ready_queue->size + 1, 1, tmpNice, 0, tmpProc);

                fclose(fp); //close file then remove 
                remove(filename);
            }
        }
        else {
            empty = 1;//nothing other than current and parent files, if file is added will change to 0 again on entry and return such
        }
        closedir(dirpointer);
    }
    if(line){
        free(line);
    }
    return empty;
}

int main(){
	/*
    int emptyDir = 0;//if the flag anything other than 0 the directory is empty

    Node readyPop, runningPop; 
    Node *currEntry;
    DataList *ready_queue = createDataList();
    DataList *running_queue = createDataList();


    //below just putting an entry on not sure how we want to initialize stuff
    //*********
    emptyDir = openDirTest(ready_queue);
    emptyDir = openDirTest(ready_queue);
    //readyqueue will have 2 entries here and running will have 0
    printAllEntries(ready_queue);

    readyPop = popEntry(ready_queue);
    pushEntry(&running_queue, readyPop.pid, 2, readyPop.niceness, readyPop.cpuTime, readyPop.procTime);
    //both running and ready have one entry now
    //************
    
    /*TODO loop logic idk what is best lol im dumb logic is hard*/
    while(ready_queue->size > 0 || running_queue->size > 0 ){//&& emptyDir == 0){
        GLOBALTIME++; //not sure what we want to increment by? or if its supposed to be done here

        //get entry by pid
        if(running_queue->size == 1){
            currEntry = getEntry(running_queue, -1, 0);//to find pid

            //print statements just for testing
            printf("BEFORE:\n");
            printf("PID: %u\n", currEntry->pid);
	        printf("cpuTime: %f\n", currEntry->cpuTime);
	        printf("procTime: %f\n\n", currEntry->procTime);

            //are we supposed to use the global time or is it a different delta t?
            setCPUTime(running_queue, currEntry->pid, ((currEntry->cpuTime) + GLOBALTIME));

            printf("AFTER:\n");
            printf("PID: %u\n", currEntry->pid);
	        printf("cpuTime: %f\n", currEntry->cpuTime);//same as getcpu
	        printf("procTime: %f\n\n", currEntry->procTime);
            //just testing to see if both are the same not sure which we prefer to use
            float time1 = getCPUTime(running_queue ,currEntry->pid); //this and above is the same thing
            printf("Getting CPUTIME: %f\n", time1);

            //checking to see if greater if yes pop and push running
            if(currEntry->cpuTime > currEntry->procTime){
                printf("Process finished\n");

                //or log before?
                runningPop = popEntry(running_queue);
                //log info? do something with finished
                readyPop = popEntry(ready_queue);
                //do we set global back to 0??
                pushEntry(&running_queue, readyPop.pid, 2, readyPop.niceness, readyPop.cpuTime, readyPop.procTime);
            }
        }

        //may need to adjust or have an initial call of open dir then pop ready and push to running 
        //otherwise will have to change the entry case
        
        emptyDir = openDirTest(ready_queue);

        printAllEntries(ready_queue);
        printAllEntries(running_queue);

        //did this to exit loop cause it goes inf otherwise... idk
        if(ready_queue->size == 0 == running_queue->size > 0 ){

            if(!emptyDir){
                printf("Directory not empty\n");
            }
            else{
                printf("Directory is empty\n");
                return 0;
            }
        }
    }
	*/
	/*
	//Testing logFile
	DataList *ready_queue = createDataList();
	DataList *running_queue = createDataList();
	pushEntry(&ready_queue, 2, 1, 10, 0.0, 15.0);
	pushEntry(&ready_queue, 3, 1, 10, 0.0, 10.0);
	pushEntry(&ready_queue, 4, 1, 10, 0.0, 5.0);
	pushEntry(&running_queue, 1, 2, 10, 5.0, 15.0);
	logFile(ready_queue, running_queue, 5.00);
	*/
	//Testing pushReadyQueue and logFile
	DataList *ready_queue = createDataList();
	DataList *running_queue = createDataList();
	pushReadyQueue(ready_queue, 1);
	logFile(ready_queue, running_queue, 1.00);
	pushReadyQueue(ready_queue, 2);
	logFile(ready_queue, running_queue, 2.00);
	printAllEntries(ready_queue);
    return 0;
}
