#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <time.h>
#include "dStruct.h"


#define ALGOR "ALGOR_FIFO"
//#define ALGOR "ALGOR_SJF"
//#define ALGOR "ALGOR_RR"
//#define ALGOR "ALGOR_MLFQ" //5 level priority but uses RR 

#define TIME_DT 0.10
#define TIME_JIFFY 0.50
#define MAX_Q 5 //TODO for testing use 5 but should be 20 (20*5=100)
#define MAX_RESET 5.00 //TODO not sure when we want to reset but can use this fo ease of value change
float GLOBAL_TIME = 0.00;
float T_RESET = 0.00; //when hits max time rest the queues and add them to proper priority level

char *createLogFile() {
  	time_t t = time(NULL);
  	struct tm *tm_info = localtime(&t);
  	char date[20]; 
  	strftime(date, sizeof(date), "%m-%d-%Y-%H:%M:%S", tm_info);
  	char *filename = malloc(50);
  	snprintf(filename, 50, "../log/log-%s.txt", date);

	FILE *fptr;
	fptr = fopen(filename, "w");
	if (fptr == NULL) {
		printf("File cannot be created.\n");
		exit(EXIT_FAILURE);
	}

	fprintf(fptr, "time,PID,status,niceness,cputime,proctime\n");
	fclose(fptr);
	return filename;
}

void logFile(DataList *ready_queue, DataList *running_queue, float logTime, char *fileName) {
	FILE *fptr;
	fptr = fopen(fileName, "a");
	if (fptr == NULL) {
		printf("File cannot be opened.\n");
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
	if(runningNode) {
		fprintf(fptr, "%.2f,%u,%u,%d,%.2f,%.2f\n", logTime,
												   runningNode->pid,
												   runningNode->status,
												   runningNode->niceness,
												   runningNode->cpuTime,
												   runningNode->procTime);
	}
	fclose(fptr);
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
	char *ptr;
	float procTime = strtol(strtok(buffer, ","), &ptr, 10);
	int niceness = strtol(strtok(NULL, ","), &ptr, 10);
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

//TODO do we want this here or in dStruct.c?
Levels *createLevels(){
	Levels *levelsList = malloc(sizeof(Levels));
	if(levelsList == NULL){
		printf("Could not allocate memory for levels list.\n");
		exit(EXIT_FAILURE);
	}
	DataList *five_queue = createDataList();
	levelsList->five = five_queue;
	DataList *four_queue = createDataList();
	levelsList->four = four_queue;
	DataList *three_queue = createDataList();
	levelsList->three = three_queue;
	DataList *two_queue = createDataList();
	levelsList->two = two_queue;
	DataList *one_queue = createDataList();
	levelsList->one = one_queue;

	return levelsList;
}
//TODO need to test
void destroyLevels(Levels **levelsList){
	if(!*levelsList){
		printf("Levels list already empty.\n");
		return;
	}

	Levels *levels = *levelsList;
	destroyDataList(&(levels->five));
	destroyDataList(&(levels->four));
	destroyDataList(&(levels->three));
	destroyDataList(&(levels->two));
	destroyDataList(&(levels->one));

	free(levels);
	*levelsList = NULL;
	return;
}

DataList *checkLevel(DataList *level, int priority){
	Node *node = level->first;
	DataList *newQueue = createDataList();
	int index = 0;
	while(node != NULL){
		//TODO need to find out if he will be using niceness 1-5 or if he will be using others and
		//we need to define a range fitting into the priority levels
		if(node->niceness == priority){
			//get index and pop then push to the new queue
			Node popped = popEntry(level, index);
			pushEntry(&newQueue, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		index ++;
		node = node->next;
	}
	return newQueue;
}

//TODO function to reset queues when treset hits
void resetQueues(Levels *levels){
	DataList *fiveNew = checkLevel(levels->five, 5);
	DataList *fourNew = checkLevel(levels->four, 4);
	DataList *threeNew = checkLevel(levels->three, 3);
	DataList *twoNew = checkLevel(levels->two, 2);
	DataList *oneNew = checkLevel(levels->one, 1);

	//free and assign
	destroyDataList(&(levels->five));
	destroyDataList(&(levels->four));
	destroyDataList(&(levels->three));
	destroyDataList(&(levels->two));
	destroyDataList(&(levels->one));

	levels->five = fiveNew;
	levels->four = fourNew;
	levels->three = threeNew;
	levels->two = twoNew;
	levels->one = oneNew;

	return;
}

int main() {
	DataList *ready_queue = createDataList();
	DataList *running_queue = createDataList();
	char *fileName = createLogFile();
	unsigned int nextPID = 1;

	if (!strcmp(ALGOR, "ALGOR_FIFO")) {
		do {
			GLOBAL_TIME = GLOBAL_TIME + TIME_DT;

			int check = pushReadyQueue(ready_queue, nextPID);
			if (fabs(GLOBAL_TIME - (0.00 + TIME_DT)) < 0.0001) {
				nextPID++;
				Node *first = ready_queue->first;
				printf("%f %u %u %d %f %f\n", GLOBAL_TIME, first->pid,
						first->status, first->niceness, first->cpuTime, first->procTime);
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
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
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
				popEntry(running_queue, 0);
			} else {
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
			}

			if (!(check == -1)) {
				nextPID++;	
			}
		} while (ready_queue->size != 0 || running_queue->size != 0);
	} else if (!strcmp(ALGOR, "ALGOR_SJF")) {	
		do {
			GLOBAL_TIME = GLOBAL_TIME + TIME_DT;

			if (fabs(GLOBAL_TIME - (0.00 + TIME_DT)) < 0.0001) {	
				pushReadyQueue(ready_queue, nextPID);
				nextPID++;
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
				continue;
			} else {
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
			}
			int check = pushReadyQueue(ready_queue, nextPID);

			unsigned int runningProcPID = running_queue->first->pid;
			if (getCPUTime(running_queue, runningProcPID) >= running_queue->first->procTime) {
				setStatus(running_queue, runningProcPID, 3);
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
				popEntry(running_queue, 0);
			} else {
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
			}

			if (!(check == -1)) {
				nextPID++;	
			}
		} while (ready_queue->size != 0 || running_queue->size != 0);
	} else if (!strcmp(ALGOR, "ALGOR_RR")) {
		float current_running;
		do {
			GLOBAL_TIME = GLOBAL_TIME + TIME_DT;

			int check = pushReadyQueue(ready_queue, nextPID);
			if (fabs(GLOBAL_TIME - (0.00 + TIME_DT)) < 0.0001) {
				nextPID++;
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
				continue;
			}

			if (running_queue->size == 0 && ready_queue->size != 0) {
				Node entry = popEntry(ready_queue, ready_queue->size - 1);
				pushRunningQueue(running_queue, entry);
				float cpuTime = getCPUTime(running_queue, entry.pid);
				float newTime = (int) ( (cpuTime + TIME_DT) * 100 + .5) / 100.0;
				setCPUTime(running_queue, entry.pid, newTime);
				current_running = TIME_DT;
			} else {
				unsigned int runningProcPID = running_queue->first->pid;
				float cpuTime = getCPUTime(running_queue, runningProcPID);
				float newTime = (int) ( (cpuTime + TIME_DT) * 100 + .5) / 100.0;
				setCPUTime(running_queue, runningProcPID, newTime);
				current_running += TIME_DT;
			}

			unsigned int runningProcPID = running_queue->first->pid;
			if (getCPUTime(running_queue, runningProcPID) >= running_queue->first->procTime) {
				setStatus(running_queue, runningProcPID, 3);
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
				popEntry(running_queue, 0);
			} else if (current_running >= TIME_JIFFY) {
				setStatus(running_queue, runningProcPID, 1);
				Node poppedNode = popEntry(running_queue, 0);
				pushEntry(&ready_queue, poppedNode.pid, poppedNode.status,
						  poppedNode.niceness, poppedNode.cpuTime, poppedNode.procTime);
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
			} else {
				logFile(ready_queue, running_queue, GLOBAL_TIME, fileName);
			}

			if (!(check == -1)) {
				nextPID++;	
			}
		} while (ready_queue->size != 0 || running_queue->size != 0);
	} else if (!strcmp(ALGOR, "ALGOR_MLFQ")) {
		//TODO will do similar to RR but incorporate the levels and treset
		//TODO create levels list 
		Levels *levelsList = createLevels();


		//TODO destroy the datalists in levels list
		destroyLevels(&levelsList);
	}

	destroyDataList(&ready_queue);
	destroyDataList(&running_queue);
	free(fileName);

    return 0;
}
