#include <stdio.h>
#include <stdlib.h>
#include "dStruct.h"

Data *createData() {
	Data *dataList = malloc(sizeof(Data));
	if (dataList != NULL) {
		dataList->pid = NULL;
		dataList->next = NULL;
	}
	return dataList;
}

void destroyData(Data *dataList) {
    //loop through save position to the next free current, do for each
    while (dataList != NULL){
        Data *temp = dataList;
        dataList = dataList->next;
        free(temp);
    }
}

Data *getEntry(Data *dataList, unsigned int pid, int index) {
	if (index == -1) {
		while (dataList->pid != pid) {
			dataList = dataList->next;
		}
		return dataList;
	} else {
		int i = 0;
		while (dataList != NULL) {
			if (i == index) {
				return dataList;
			}
			i++;
		}	
	}
}

void pushEntry(Data **dataList, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime) {
	if (!((*dataList)->pid)) {
		(*dataList)->pid = pid;
		(*dataList)->status = status;
		(*dataList)->niceness = niceness;
		(*dataList)->cpuTime = cpuTime;
		(*dataList)->procTime = procTime;

	} else {
		Data *newNode = malloc(sizeof(Data));
		newNode->pid = pid;
		newNode->status = status;
		newNode->niceness = niceness;
		newNode->cpuTime = cpuTime;
		newNode->procTime = procTime;
		
		newNode->next = *dataList;
		*dataList = newNode;
	}
}

Data *popData() {
	;
}

float getCPUTime(Data *dataList, unsigned int pid) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			return dataList->pid;
		}
		dataList = dataList->next;
	}
}

void setCPUTime(Data *dataList, unsigned int pid, float cpuTime) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			dataList->cpuTime = cpuTime;
		}
	}
}

int getNiceness(Data *dataList, unsigned int pid) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			return dataList->niceness;
		}
		dataList = dataList->next;
	}
}

void setNiceness(Data *dataList, unsigned int pid, int niceness) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			dataList->niceness = niceness;
		}
	}
}

unsigned int getStatus(Data *dataList, unsigned int pid) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			return dataList->status;
		}
		dataList = dataList->next;
	}
}

void setStatus(Data *dataList, unsigned int pid, unsigned int status) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			dataList->status = status;
		}
	}
}

int searchEntryPID(Data *dataList, unsigned int pid) {
	while (dataList != NULL) {
		if (dataList->pid == pid) {
			return 1;
		}
		dataList = dataList->next;
	}
	return 0;
}

int searchEntryNicenessStatus(Data *dataList, int niceness, unsigned int status) {
	while (dataList != NULL) {
		if (dataList->niceness == niceness && status == -1) {
			return 1
		} else if (dataList->status == status && niceness == -1) {
			return 1
		} else if (dataList->niceness == niceness && dataList->status == status) {
			return 1;
		}
		dataList = dataList->next;
	}
	return 0;
}

void printAllEntries(Data *dataList) {
	printf("%s%15s%15s%15s%15s\n", "pid", "status", "niceness", "cputime", "proctime");
	Data *current = dataList;
	while (current != NULL) {
		printf("%u%12u%13d%23f%14f\n", current->pid, current->status, \
				current->niceness, current->cpuTime, current->procTime);
		current = current->next;
	}
}

void printEntryPID(Data *dataList, unsigned int pid) {
	;
}

void printEntryNicenessStatus(Data *dataList, int niceness, unsigned int status) {
	;
}

int main(void) {
	Data *list = createData();

  	pushEntry(&list, 1, 1, 1, 1.0, 1.0);
	pushEntry(&list, 2, 2, 2, 2.0, 2.0);
	pushEntry(&list, 3, 3, 3, 3.0, 3.0);

	printAllEntries(list);

	Data *entry1 = getEntry(list, 1, -1);
	Data *entry2 = getEntry(list, -1, 2);
	printf("%d\n", entry1->pid);
	printf("%d\n\n", entry2->pid);

	float time1 = getCPUTime(list, 1); 
	float time2 = getCPUTime(list, 3); 
	printf("%f\n", time1);
	printf("%f\n\n", time2);

	int niceness1 = getNiceness(list, 1);
	int niceness2 = getNiceness(list, 3);
	printf("%d\n", niceness1);
	printf("%d\n\n", niceness2);

	unsigned int status1 = getStatus(list, 1);
	unsigned int status2 = getStatus(list, 3);
	printf("%u\n", status1);
	printf("%u\n\n", status2);
	destroyData(list);
	return 0;
}
