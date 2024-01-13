#include <stdio.h>
#include <stdlib.h>
#include "dStruct.h"

Data *createData() {
	Data *data = malloc(sizeof(Data));
	if (data != NULL) {
		data->next = NULL;
	}
	return data;
}

void destroyData(Data *dataArray) {
    //loop through save position to the next free current, do for each
    while (dataArray != NULL){
        Data * temp = dataArray;
        dataArray = dataArray->next;
        free(temp);
    }
}


Data getEntry(Data *dataArray, unsigned int pid, int index) {
	;
}

void pushEntry(Data **dataList, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime) {
	/*if ((*dataList)->pid == NULL) {
		(*dataList)->pid = pid;
		(*dataList)->status = status;
		(*dataList)->niceness = niceness;
		(*dataList)->cpuTime = cpuTime;
		(*dataList)->procTime = procTime;
        
	} else {*/
		Data *newNode = malloc(sizeof(Data));

        if (newNode == NULL){
            //print fail??
            //return NULL;
        }

		newNode->pid = pid;
		newNode->status = status;
		newNode->niceness = niceness;
		newNode->cpuTime = cpuTime;
		newNode->procTime = procTime;
		
		newNode->next = *dataList;
		*dataList = newNode;
       //dataList->next = NULL;
        //return newNode;
	//}
}

Data *popData() {
	;
}

float getCPUTime(Data *dataArray, unsigned int pid) {
	;
}

void setCPUTime(unsigned int pid, float cpuTime) {
	;
}

int getNiceness(Data *dataArray, unsigned int pid) {
	;
}

void setNiceness(unsigned int pid, int niceness) {
	;
}

unsigned int getStatus(Data *dataArray, unsigned int pid) {
	;
}

void setStatus(Data *dataArray, unsigned int pid) {
	;
}

int searchEntryPID(Data *dataArray, unsigned int pid) {
	;
}

int searchEntryNicenessStatus(Data *dataArray, int niceness, unsigned int status) {
	;
}

void printAllEntries(Data *dataList) {
	printf("%s%15s%15s%15s%15s\n", "pid", "status", "niceness", "cputime", "proctime");
	//for (dataList; dataList->next == NULL; dataList = dataList->next) {
	Data *current = dataList;
	//printf("%u\n", current->pid);
	while (current != NULL) {
		printf("%u%12u%13d%23f%14f\n", current->pid, current->status, \
				current->niceness, current->cpuTime, current->procTime);
		current = current->next;
	}
}

void printEntryPID(Data *dataArray, unsigned int pid) {
	;
}

void printEntryNicenessStatus(Data *dataArray, int niceness, unsigned int status) {
	;
}

int main(void) {
	Data *list = createData();
	//list = pushEntry(&list, 1, 1, 1, 1.0, 1.0);
	//list = pushEntry(&list, 2, 2, 2, 2.0, 2.0);
	//list = pushEntry(&list, 3, 3, 3, 3.0, 3.0);

    pushEntry(&list, 1, 1, 1, 1.0, 1.0);
	pushEntry(&list, 2, 2, 2, 2.0, 2.0);
	pushEntry(&list, 3, 3, 3, 3.0, 3.0);
	printAllEntries(list);
	return 0;
}
