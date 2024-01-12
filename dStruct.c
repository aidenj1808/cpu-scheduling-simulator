#include "stdio.h"
#include "stdlib.h"
#include "dStruct.h"

Data *createData() {
	Data *array = malloc(sizeof(Data));
	return array;
}

void destroyData(Data *dataArray) {
	;
}


Data getEntryByPID(Data *dataArray, unsigned int pid) {
	;
}

Data getEntryByIndex(Data *dataArray, int index) {
	;
}

void pushEntry(Data *dataArray, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime) {
	int length = sizeof(dataArray) / sizeof(dataArray[0]);
	if (length == 0) {
		dataArray[0].pid = pid;
		dataArray[0].status = status;
		dataArray[0].niceness = niceness;
		dataArray[0].cpuTime = cpuTime;
		dataArray[0].procTime = procTime;
		dataArray[0].next = NULL;
	}


	
}

Data popData() {
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

_Bool searchEntryByPID(Data *dataArray, unsigned int pid) {
	;
}

_Bool searchEntryByNiceness(Data *dataArray, int niceness) {
	;
}

_Bool searchEntryByStatus(Data *dataArray, unsigned int status) {
	;
}

void printAllEntries(Data *dataArray) {
	;
}

void printEntryByPID(Data *dataArray, unsigned int pid) {
	while (dataArray != NULL) {
		if (dataArray->pid == pid) {
			printf("Entry with pid %d\n", dataArray->pid);
			printf("%u\n", dataArray->pid);
			printf("%u\n", dataArray->status);
			printf("%d\n", dataArray->niceness);
			printf("%f\n", dataArray->cpuTime);
			printf("%f\n", dataArray->procTime);
			dataArray = dataArray->next;
		} else {
			dataArray = dataArray->next;
		}
	}
}

void printEntryByNiceness(Data *dataArray, int niceness) {
	;
}

void printEntryByStatus(Data *dataArray, unsigned int status) {
	;
}

int main(void) {
	Data *my_array = createData();
	pushEntry(my_array, 1, 1, 1, 1.0, 1.0);
	printEntryByPID(my_array, 1);
	pushEntry(my_array, 2, 2, 2, 2.0, 2.0);
	printEntryByPID(my_array, 2);
	return 0;
}
