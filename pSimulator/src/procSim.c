#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <time.h>
#include "dStruct.h"


//#define ALGOR "ALGOR_FIFO"
//#define ALGOR "ALGOR_SJF"
//#define ALGOR "ALGOR_RR"
#define ALGOR "ALGOR_MLFQ" //5 level priority but uses RR 

#define TIME_DT 0.10
#define TIME_JIFFY 0.50
#define MAX_Q 2 //TODO for testing use 5 but should be 20 (20*5=100)
#define MAX_RESET 5.00 //TODO not sure when we want to reset but can use this fo ease of value change
float GLOBAL_TIME = 0.00;
float T_RESET = 0.00; //when hits max time rest the queues and add them to proper priority level

void printLevels(Levels *levels);

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
	Queues *queuesFive = malloc(sizeof(Queues));
	if(queuesFive == NULL){
		printf("Could not allocate memory for queues list.\n");
		exit(EXIT_FAILURE);
	}
	Queues *queuesFour = malloc(sizeof(Queues));
	if(queuesFour == NULL){
		printf("Could not allocate memory for queues list.\n");
		exit(EXIT_FAILURE);
	}
	Queues *queuesThree = malloc(sizeof(Queues));
	if(queuesThree == NULL){
		printf("Could not allocate memory for queues list.\n");
		exit(EXIT_FAILURE);
	}
	Queues *queuesTwo = malloc(sizeof(Queues));
	if(queuesTwo == NULL){
		printf("Could not allocate memory for queues list.\n");
		exit(EXIT_FAILURE);
	}
	Queues *queuesOne = malloc(sizeof(Queues));
	if(queuesOne == NULL){
		printf("Could not allocate memory for queues list.\n");
		exit(EXIT_FAILURE);
	}

	levelsList->five = queuesFive;
	levelsList->four = queuesFour;
	levelsList->three = queuesThree;
	levelsList->two = queuesTwo;
	levelsList->one = queuesOne;


	DataList *five_ready_queue = createDataList();
	levelsList->five->ready_queue = five_ready_queue;
	DataList *four_ready_queue = createDataList();
	levelsList->four->ready_queue = four_ready_queue;
	DataList *three_ready_queue = createDataList();
	levelsList->three->ready_queue = three_ready_queue;
	DataList *two_ready_queue = createDataList();
	levelsList->two->ready_queue = two_ready_queue;
	DataList *one_ready_queue = createDataList();
	levelsList->one->ready_queue = one_ready_queue;

	DataList *five_running_queue = createDataList();
	levelsList->five->running_queue = five_running_queue;
	DataList *four_running_queue = createDataList();
	levelsList->four->running_queue = four_running_queue;
	DataList *three_running_queue = createDataList();
	levelsList->three->running_queue = three_running_queue;
	DataList *two_running_queue = createDataList();
	levelsList->two->running_queue = two_running_queue;
	DataList *one_running_queue = createDataList();
	levelsList->one->running_queue = one_running_queue;

	return levelsList;
}
//TODO need to test, implement after working
void destroyLevels(Levels **levelsList){
	if(!*levelsList){
		printf("Levels list already empty.\n");
		return;
	}

	Levels *levels = *levelsList;
	destroyDataList(&(levels->five->ready_queue));
	destroyDataList(&(levels->five->running_queue));
	destroyDataList(&(levels->four->ready_queue));
	destroyDataList(&(levels->four->running_queue));
	destroyDataList(&(levels->three->ready_queue));
	destroyDataList(&(levels->three->running_queue));
	destroyDataList(&(levels->two->ready_queue));
	destroyDataList(&(levels->two->running_queue));
	destroyDataList(&(levels->one->ready_queue));
	destroyDataList(&(levels->one->running_queue));

	free(levels->five);
	free(levels->four);
	free(levels->three);
	free(levels->two);
	free(levels->one);
	levels->five = NULL;
	levels->four = NULL;
	levels->three = NULL;
	levels->two = NULL;
	levels->one = NULL;

	free(levels);
	*levelsList = NULL;
	return;
}

//TODO function to reset queues when treset hits
void resetQueues(Levels *levels){
	//printf("Starting Queue reset\n");//TODO erase
	DataList *fiveNew = createDataList();
	DataList *fourNew = createDataList();
	DataList *threeNew = createDataList();
	DataList *twoNew = createDataList();
	DataList *oneNew = createDataList();
	
	//level 5
	Node *node = levels->five->ready_queue->first;
	while(node != NULL){
		Node popped = popEntry(levels->five->ready_queue, levels->five->ready_queue->size - 1);
		if(popped.niceness == 5){
			pushEntry(&fiveNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 4){
			pushEntry(&fourNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 3){
			pushEntry(&threeNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 2){
			pushEntry(&twoNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 1){
			pushEntry(&oneNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		node = levels->five->ready_queue->first;
	}
	//level 4
	node = levels->four->ready_queue->first;
	while(node != NULL){
		Node popped = popEntry(levels->four->ready_queue, levels->four->ready_queue->size - 1);
		if(popped.niceness == 5){
			pushEntry(&fiveNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 4){
			pushEntry(&fourNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 3){
			pushEntry(&threeNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 2){
			pushEntry(&twoNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 1){
			pushEntry(&oneNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		node = levels->four->ready_queue->first;
	}
	//level 3
	node = levels->three->ready_queue->first;
	while(node != NULL){
		Node popped = popEntry(levels->three->ready_queue, levels->three->ready_queue->size - 1);
		if(popped.niceness == 5){
			pushEntry(&fiveNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 4){
			pushEntry(&fourNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 3){
			pushEntry(&threeNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 2){
			pushEntry(&twoNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 1){
			pushEntry(&oneNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		node = levels->three->ready_queue->first;
	}
	//level 2
	node = levels->two->ready_queue->first;
	while(node != NULL){
		Node popped = popEntry(levels->two->ready_queue, levels->two->ready_queue->size - 1);
		if(popped.niceness == 5){
			pushEntry(&fiveNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 4){
			pushEntry(&fourNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 3){
			pushEntry(&threeNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 2){
			pushEntry(&twoNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 1){
			pushEntry(&oneNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		node = levels->two->ready_queue->first;
	}
	//level 1
	node = levels->one->ready_queue->first;
	while(node != NULL){
		Node popped = popEntry(levels->one->ready_queue, levels->one->ready_queue->size - 1);
		if(popped.niceness == 5){
			pushEntry(&fiveNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 4){
			pushEntry(&fourNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 3){
			pushEntry(&threeNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 2){
			pushEntry(&twoNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		else if(popped.niceness == 1){
			pushEntry(&oneNew, popped.pid, popped.status, popped.niceness, popped.cpuTime, popped.procTime);
		}
		node = levels->one->ready_queue->first;
	}

	//free and assign
	destroyDataList(&(levels->five->ready_queue));
	destroyDataList(&(levels->four->ready_queue));
	destroyDataList(&(levels->three->ready_queue));
	destroyDataList(&(levels->two->ready_queue));
	destroyDataList(&(levels->one->ready_queue));

	levels->five->ready_queue = fiveNew;
	levels->four->ready_queue = fourNew;
	levels->three->ready_queue = threeNew;
	levels->two->ready_queue = twoNew;
	levels->one->ready_queue = oneNew;

	return;
}

int pushLevelQueue(Levels *levels, unsigned int pid) {
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
  
	//printf("Niceness for pid %d: %d\n", pid, niceness);//TODO erase

	if (niceness == 5) {
		pushEntry(&(levels->five->ready_queue), pid, 1, niceness, 0.00, procTime);
	}
	else if (niceness == 4) {
		pushEntry(&(levels->four->ready_queue), pid, 1, niceness, 0.00, procTime);
	}
	else if (niceness == 3) {
		pushEntry(&(levels->three->ready_queue), pid, 1, niceness, 0.00, procTime);
	}
	else if (niceness == 2) {
		pushEntry(&(levels->two->ready_queue), pid, 1, niceness, 0.00, procTime);
	}
	else if (niceness == 1) {
		pushEntry(&(levels->one->ready_queue), pid, 1, niceness, 0.00, procTime);
	}

	fclose(fptr);
	remove(path);
	free(path);
	free(buffer);
	return 0;
}

void initLevels(Levels *levels) {
	unsigned int pid = 1;
	int check;
	while ((check = pushLevelQueue(levels, pid)) != -1) {
		pid++;
	}
}

void printLevels(Levels *levels) {
	printf("Level 5:\n");
	printf("Ready:");
	printAllEntries(levels->five->ready_queue);
	printf("Running:");
	printAllEntries(levels->five->running_queue);
	printf("\n");

	printf("Level 4:\n");
	printf("Ready:");
	printAllEntries(levels->four->ready_queue);
	printf("Running:");
	printAllEntries(levels->four->running_queue);
	printf("\n");

	printf("Level 3:\n");
	printf("Ready:");
	printAllEntries(levels->three->ready_queue);
	printf("Running:");
	printAllEntries(levels->three->running_queue);
	printf("\n");

	printf("Level 2:\n");
	printf("Ready:");
	printAllEntries(levels->two->ready_queue);
	printf("Running:");
	printAllEntries(levels->two->running_queue);
	printf("\n");

	printf("Level 1:\n");
	printf("Ready:");
	printAllEntries(levels->one->ready_queue);
	printf("Running:");
	printAllEntries(levels->one->running_queue);
	printf("\n");

}

int runRR(DataList *queue) {
	;//timeStart = 0
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
		initLevels(levelsList);
		printLevels(levelsList);
		resetQueues(levelsList);
		printLevels(levelsList);
		/*
		do {
			GLOBAL_TIME = GLOBAL_TIME + TIME_DT;

		} while (1);
		*/

		//TODO destroy the datalists in levels list
		//destroyLevels(&levelsList);
	}

	destroyDataList(&ready_queue);
	destroyDataList(&running_queue);
	free(fileName);

    return 0;
}
