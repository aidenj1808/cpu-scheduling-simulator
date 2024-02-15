#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dStruct.h"
/*TODO noticed that 10 procs made and only 9 are going in 1-8 and 10, but 
9 is left in the newProc dir??
--> when FIFO it doesnt read in the first proc so 9/10 are there, seg faults on first run
--> when SJF it will go like 4 rounds and seg fault BUT it reads in 10/10 procs
*/


//#define ALGOR "ALGOR_FIFO"
#define ALGOR "ALGOR_SJF"
//#define ALGOR "ALGOR_RR"

#define TIME_DT 0.10
#define TIME_JIFFY 0.10

float GLOBAL_TIME = 0.00;
/*
float TIME_DT = 0.10;
float TIME_JIFFY = 0;*/

void logFile(DataList *ready_queue, DataList *running_queue, float logTime) {
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
	fprintf(fptr, "time,PID,status,niceness,cputime,proctime\n");
	while (readyNode != NULL) {
		fprintf(fptr, "%.2f,%u,%u,%d,%.2f,%.2f\n", logTime, readyNode->pid,
												   readyNode->status,
												   readyNode->niceness,
												   readyNode->cpuTime,
												   readyNode->procTime);
		readyNode = readyNode->next;
	}

	Node *runningNode = running_queue->first;
	if(runningNode) {
		fprintf(fptr, "%.2f,%u,%u,%d,%.2f,%.2f\n", logTime,
												   runningNode->pid,
												   runningNode->status,
												   runningNode->niceness,
												   runningNode->cpuTime,
												   runningNode->procTime);
	}
	fclose(fptr);
	free(fileName);
}

int pushReadyQueue(DataList *ready_queue, unsigned int pid) {
	DIR *dirptr = opendir("../newProc/");
	if (dirptr == NULL) {
		printf("Error opening directory.\n");
		return -1;
	}
	struct dirent *dp;
	for (int i = 0; i < 3; i++) {dp = readdir(dirptr);} //skips . & .. & .gitkeep
	dp = readdir(dirptr);
	if (!dp) {
		closedir(dirptr);
		return -1;
	}

	int newProcDirLen = strlen("../newProc/");
	char *path = malloc(newProcDirLen + 1);
	if (path == NULL) {
		printf("could not allocate memory for path.\n");
		return -1;
	}
 	strcpy(path, "../newProc/");

	int procFileLen = strlen(dp->d_name);
	path = realloc(path, newProcDirLen + procFileLen + 1);
	if (path == NULL) {
		printf("Error reallocing path.\n");
		return -1;
	}
	strcat(path, dp->d_name);
	closedir(dirptr);

	FILE *fptr = fopen(path, "r");
	if (fptr == NULL) {
		printf("Error opening file.\n");
		return -1;
	}
	char *buffer = malloc(50);
	if (buffer == NULL) {
		printf("could not allocate memory for path.\n");
		return -1;
	}

	fgets(buffer, 50, fptr);
	float procTime = atoi(strtok(buffer, ","));
	int niceness = atoi(strtok(NULL, ","));
	pushEntry(&ready_queue, pid, 1, niceness, 0.0, procTime);
	fclose(fptr);
	remove(path);
	free(path);
	free(buffer);
	return 0;
} 

int getShortestJobIndex(DataList *ready_queue) {
	int shortestJobIndex = 0;
	int index = 0;
	Node *node = ready_queue->first;
	float shortestProcTime = node->procTime;
	while (node != NULL) {
		if (node->procTime <= shortestProcTime) {
			shortestProcTime = node->procTime;
			shortestJobIndex = index;
		}
		node = node->next;
		index++;
	}
	return shortestJobIndex;
}

void pushRunningQueue(DataList *running_queue, Node entry) {
	pushEntry(&running_queue, entry.pid, 2, entry.niceness, entry.cpuTime,
							  entry.procTime);
}

int main() {
	DataList *ready_queue = createDataList();
	DataList *running_queue = createDataList();
	unsigned int nextPID = 1;

	if (!strcmp(ALGOR, "ALGOR_FIFO")) {
		do {
			GLOBAL_TIME = GLOBAL_TIME + TIME_DT;

			int check = pushReadyQueue(ready_queue, nextPID);
			if (GLOBAL_TIME == 0.00 + TIME_DT) {
				nextPID++;
				logFile(ready_queue, running_queue, GLOBAL_TIME);
				continue;
			}

			if (running_queue->size == 0 && ready_queue->size != 0) {
				Node entry = popEntry(ready_queue, ready_queue->size - 1);
				pushRunningQueue(running_queue, entry);
				float cpuTime = getCPUTime(running_queue, entry.pid);
				float newTime = (int) ( (cpuTime + TIME_DT) * 100 + .5) / 100.0;
				setCPUTime(running_queue, entry.pid, newTime);
			} else {
				unsigned int runningProcPID = running_queue->first->pid;
				float cpuTime = getCPUTime(running_queue, runningProcPID);
				float newTime = (int) ( (cpuTime + TIME_DT) * 100 + .5) / 100.0;
				setCPUTime(running_queue, runningProcPID, newTime);
			}

			unsigned int runningProcPID = running_queue->first->pid;
			if (getCPUTime(running_queue, runningProcPID) >= running_queue->first->procTime) {
				setStatus(running_queue, runningProcPID, 3);
				logFile(ready_queue, running_queue, GLOBAL_TIME);
				popEntry(running_queue, 0);
			} else {
				logFile(ready_queue, running_queue, GLOBAL_TIME);
			}

			if (!(check == -1)) {
				nextPID++;	
			}
		} while (ready_queue->size != 0 || running_queue->size != 0);
	} else if (!strcmp(ALGOR, "ALGOR_SJF")) {	
		do {
			GLOBAL_TIME = GLOBAL_TIME + TIME_DT;

			int check = pushReadyQueue(ready_queue, nextPID);
			if (GLOBAL_TIME == 0.00 + TIME_DT) {
				nextPID++;
				logFile(ready_queue, running_queue, GLOBAL_TIME);
				continue;
			}

			if (running_queue->size == 0 && ready_queue->size != 0) {
				int index = getShortestJobIndex(ready_queue);
				Node entry = popEntry(ready_queue, index);
				pushRunningQueue(running_queue, entry);
				float cpuTime = getCPUTime(running_queue, entry.pid);
				float newTime = (int) ( (cpuTime + TIME_DT) * 100 + .5) / 100.0;
				setCPUTime(running_queue, entry.pid, newTime);
			} else {
				unsigned int runningProcPID = running_queue->first->pid;
				float cpuTime = getCPUTime(running_queue, runningProcPID);
				float newTime = (int) ( (cpuTime + TIME_DT) * 100 + .5) / 100.0;
				setCPUTime(running_queue, runningProcPID, newTime);
			}

			unsigned int runningProcPID = running_queue->first->pid;
			if (getCPUTime(running_queue, runningProcPID) >= running_queue->first->procTime) {
				setStatus(running_queue, runningProcPID, 3);
				logFile(ready_queue, running_queue, GLOBAL_TIME);
				popEntry(running_queue, 0);
			} else {
				logFile(ready_queue, running_queue, GLOBAL_TIME);
			}

			if (!(check == -1)) {
				nextPID++;	
			}
		} while (ready_queue->size != 0 || running_queue->size != 0);
	} //else if (!strcmp(ALGOR, "ALGOR_RR")) {
		
	//}

	destroyDataList(&ready_queue);
	destroyDataList(&running_queue);

    return 0;
}
