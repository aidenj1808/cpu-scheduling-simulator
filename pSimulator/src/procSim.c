#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dStruct.h"

float GLOBALTIME = 0.00;
float DELTAT = 0.10;

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
	struct dirent *dp;
	for (int i = 0; i < 3; i++) {dp = readdir(dirptr);} //skips . & .. & .gitkeep
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
	char *buffer = malloc(50);
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

void pushRunningQueue(DataList *running_queue, Node entry) {
	pushEntry(&running_queue, entry.pid, 2, entry.niceness, entry.cpuTime,
							  entry.procTime);
}

int main() {
	DataList *ready_queue = createDataList();
	DataList *running_queue = createDataList();
	unsigned int nextPID = 1;

	do {
		GLOBALTIME = GLOBALTIME + DELTAT;

		int check = pushReadyQueue(ready_queue, nextPID);
		if (GLOBALTIME == 0.00 + DELTAT) {
			nextPID++;
			logFile(ready_queue, running_queue, GLOBALTIME);
			continue;
		}

		if (running_queue->size == 0 && ready_queue->size != 0) {
			Node entry = popEntry(ready_queue);
			pushRunningQueue(running_queue, entry);
			float cpuTime = getCPUTime(running_queue, entry.pid);
			float newTime = (int) ( (cpuTime + DELTAT) * 100 + .5) / 100.0;
			setCPUTime(running_queue, entry.pid, newTime);
		} else {
			unsigned int runningProcPID = running_queue->first->pid;
			float cpuTime = getCPUTime(running_queue, runningProcPID);
			float newTime = (int) ( (cpuTime + DELTAT) * 100 + .5) / 100.0;
			setCPUTime(running_queue, runningProcPID, newTime);
		}

		unsigned int runningProcPID = running_queue->first->pid;
		if (getCPUTime(running_queue, runningProcPID) >= running_queue->first->procTime) {
			setStatus(running_queue, runningProcPID, 3);
			logFile(ready_queue, running_queue, GLOBALTIME);
			popEntry(running_queue);
		} else {
			logFile(ready_queue, running_queue, GLOBALTIME);
		}

		if (!(check == -1)) {
			nextPID++;	
		}
	} while (ready_queue->size != 0 || running_queue->size != 0);

	destroyDataList(&ready_queue);
	destroyDataList(&running_queue);
    return 0;
}
