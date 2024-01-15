#include <stdio.h>
#include <stdlib.h>
#include "dStruct.h"

DataList *createDataList() {
	DataList *dataList = malloc(sizeof(DataList));
	if (dataList == NULL) {
		printf("could not allocate memory for data list.\n");
		exit(EXIT_FAILURE);
	}
	dataList->first = NULL;
	dataList->size = 0;
	return dataList;
}

void destroyDataList(DataList *dataList) {
    //loop through save position to the next free current, do for each
	Node *node = dataList->first;
    while (node != NULL){
        Node *temp = node;
        node = node->next;
        free(temp);
    }
	free(dataList);
}

Node *getEntry(DataList *dataList, unsigned int pid, int index) {
	Node *node = dataList->first;
	if (index == -1) {
		while (node->pid != pid) {
			node = node->next;
		}
		if (!node) {
			printf("Data does not exist with PID: %u\n", pid);
			exit(EXIT_FAILURE);
		} else {
			return node;
		}
	} else {
		int i = 0;
		while (node != NULL) {
			if (i == index) {
				return node;
			}
			node = node->next;
			i++;
		}	

		if (!node) {
			printf("Data does not exist with Index: %d\n", index);
			exit(EXIT_FAILURE);
		}
	}
}

void pushEntry(DataList **dataList, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime) {
	if ((*dataList)->size == 100) {
		printf("DataList is full.\n");
		exit(EXIT_FAILURE);
	}

	Node *newNode = malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("could not allocate memory for new node.\n");
		exit(EXIT_FAILURE);
	}
	newNode->pid = pid;
	newNode->status = status;
	newNode->niceness = niceness;
	newNode->cpuTime = cpuTime;
	newNode->procTime = procTime;
	if ((*dataList)->size == 0) {
		newNode->next = NULL;
		(*dataList)->first = newNode;
	} else {
		newNode->next = (*dataList)->first;
		(*dataList)->first = newNode;
	}
	(*dataList)->size++;
}

Node *popData(DataList *dataList) {	
	Node *node = dataList->first;
	while (node->next->next != NULL) {
		node = node->next;
	}
	Node *lastNode = node->next;
	free(node->next);
	node->next = NULL;
	dataList->size--;
	return lastNode;
}

float getCPUTime(DataList *dataList, unsigned int pid) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return node->cpuTime;
		}
		node = node->next;
	}
}

void setCPUTime(DataList *dataList, unsigned int pid, float cpuTime) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->cpuTime = cpuTime;
		}
		node = node->next;
	}
}

int getNiceness(DataList *dataList, unsigned int pid) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return node->niceness;
		}
		node = node->next;
	}
}

void setNiceness(DataList *dataList, unsigned int pid, int niceness) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->niceness = niceness;
		}
		node = node->next;
	}
}

unsigned int getStatus(DataList *dataList, unsigned int pid) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return node->status;
		}
		node = node->next;
	}
}

void setStatus(DataList *dataList, unsigned int pid, unsigned int status) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->status = status;
		}
		node = node->next;
	}
}

int searchEntryPID(DataList *dataList, unsigned int pid) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

int searchEntryNicenessStatus(DataList *dataList, int niceness, unsigned int status) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->niceness == niceness && status == -1) {
			return 1;
		} else if (node->status == status && niceness == -1) {
			return 1;
		} else if (node->niceness == niceness || node->status == status) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

void printAllEntries(DataList *dataList) {
	Node *node = dataList->first;
	printf("%s%15s%15s%15s%15s\n", "pid", "status", "niceness", "cputime", "proctime");
	while (node != NULL) {
		printf("%u%12u%13d%23f%14f\n", node->pid, node->status, \
				node->niceness, node->cpuTime, node->procTime);
		node = node->next;
	}
}

void printEntryPID(DataList *dataList, unsigned int pid) {
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			break;
		}
		node = node->next;
	}

	if (node == NULL) {
		printf("Data for PID:%u does not exist.\n", pid);
		return;
	}
	printf("%s%15s%15s%15s%15s\n", "pid", "status", "niceness", "cputime", "proctime");
	printf("%u%12u%13d%23f%14f\n", node->pid, node->status, \
		   node->niceness, node->cpuTime, node->procTime);
}

void printEntryNicenessStatus(DataList *dataList, int niceness, unsigned int status) {
	Node *node = dataList->first;
	printf("%s%15s%15s%15s%15s\n", "pid", "status", "niceness", "cputime", "proctime");
	while (node != NULL) {
		if (node->niceness == niceness && status == -1) {
			printf("%u%12u%13d%23f%14f\n", node->pid, node->status, \
		           node->niceness, node->cpuTime, node->procTime);

		} else if (node->status == status && niceness == -1) {
			printf("%u%12u%13d%23f%14f\n", node->pid, node->status, \
		           node->niceness, node->cpuTime, node->procTime);

		} else if (node->niceness == niceness || node->status == status) {
			printf("%u%12u%13d%23f%14f\n", node->pid, node->status, \
		            node->niceness, node->cpuTime, node->procTime);
		}
		node = node->next;
	}

	if (!searchEntryNicenessStatus(dataList, niceness, status)) {
		printf("niceness: %d or status: %u does not exist.\n", niceness, status);
	}
}

int main(void) {
	DataList *list = createDataList();
	/*
	pushEntry(&list, 1, 1, 1, 1.0, 1.0);
	pushEntry(&list, 2, 2, 2, 2.0, 2.0);
	pushEntry(&list, 3, 3, 3, 3.0, 3.0);
	pushEntry(&list, 4, 4, 4, 4.0, 4.0);
	pushEntry(&list, 5, 5, 5, 5.0, 5.0);
	pushEntry(&list, 6, 6, 6, 6.0, 6.0);
	pushEntry(&list, 7, 7, 7, 7.0, 7.0);
	pushEntry(&list, 8, 8, 8, 8.0, 8.0);
	pushEntry(&list, 9, 9, 9, 9.0, 9.0);
	printAllEntries(list);
	popData(list);
	popData(list);
	popData(list);
	printAllEntries(list);
	destroyDataList(list);
	*/
	for (int i = 1; i < 101; i++) {
		pushEntry(&list, i, i, i, (float) i, (float) i);
	}
	pushEntry(&list, 101, 101, 101, 101.0, 101.0);
	/*
	Data *pop1 = popData(list);
	Data *pop2 = popData(list);
	Data *pop3 = popData(list);
	Data *pop4 = popData(list);
	Data *pop5 = popData(list);
	printAllEntries(list);
	destroyData(list);
	*/
	/*
	Data *list = createData();

  	pushEntry(&list, 1, 1, 1, 1.0, 1.0);
	pushEntry(&list, 2, 2, 2, 2.0, 2.0);
	pushEntry(&list, 3, 3, 3, 3.0, 3.0);

	printAllEntries(list);
	printf("\n");
	printEntryPID(list, 1);
	printf("\n");
	printEntryNicenessStatus(list, 1, -1);
	printf("\n");
	printEntryNicenessStatus(list, -1, 2);
	printf("\n");
	printEntryNicenessStatus(list, 2, 3);
	printf("\n");

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
	*/
	return 0;
}
