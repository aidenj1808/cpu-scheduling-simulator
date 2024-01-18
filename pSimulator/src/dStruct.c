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

void destroyDataList(DataList **dataList) {
    //loop through save position to the next free current, do for each
    if (!*dataList){
        printf("Datalist is empty\n");
        return;
    }
    DataList *list = *dataList;
	Node *node = list->first;
    while (node != NULL){
        Node *temp = node;
        node = node->next;
        free(temp);
    }
	free(list);
    //now point to NULL
    *dataList = NULL;
}

Node *getEntry(DataList *dataList, unsigned int pid, int index) {
  if(!dataList){
      printf("Datalist is empty.\n");
      return NULL;//return NULL??
    }
	Node *node = dataList->first;
	if (index == -1) {
		while (node != NULL) {
			if (node->pid == pid) {
				return node;
			} else {
				node = node->next;
			}
		}
		if (!node) {
			printf("Data does not exist with PID: %u\n", pid);
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
            return node;
		}
	}
	return node;
}

void pushEntry(DataList **dataList, unsigned int pid, unsigned int status, int niceness, float cpuTime, float procTime) {
	if ((*dataList)->size == 100) {
		printf("DataList is full.\n");
		return;
	}

	Node *newNode = malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("could not allocate memory for new node.\n");
		return;
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

Node popEntry(DataList *dataList) {	
  if(!dataList){
     printf("Datalist is empty.\n");
     Node errorNode = {};
     return errorNode;//return NULL??
    }
    
	if (dataList->size == 1) {
		Node lastNode = *(dataList->first);
		return lastNode;	
	}
  
	Node *node = dataList->first;
	while (node->next->next != NULL) {
		node = node->next;
	}
	Node lastNode = *(node->next);
	free(node->next);
	node->next = NULL;
	dataList->size--;
	return lastNode;
}

float getCPUTime(DataList *dataList, unsigned int pid) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return -0.0;//return 
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return node->cpuTime;
		}
		node = node->next;
	}
	if (!node) {
		printf("Data for PID: %u does not exist.\n", pid);
        return -1;
	}
	return -1;
}

void setCPUTime(DataList *dataList, unsigned int pid, float cpuTime) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->cpuTime = cpuTime;
			break;
		}
		node = node->next;
	}
  
	if (node == NULL) {
		printf("PID: %u does not exist\n", pid);
		return;
	}
}

int getNiceness(DataList *dataList, unsigned int pid) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return -1;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return node->niceness;
		}
		node = node->next;
	}
	if (!node) {
		printf("Data for PID: %u does not exist.\n", pid);
	}
	return -1; 
}

void setNiceness(DataList *dataList, unsigned int pid, int niceness) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->niceness = niceness;
			break;
		}
		node = node->next;
	}

	if (node == NULL) {
		printf("PID: %u does not exist.\n", pid);
	}
}

unsigned int getStatus(DataList *dataList, unsigned int pid) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return 0;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return node->status;
		}
		node = node->next;
	}
	if (!node) {
		printf("Data for PID: %u does not exist.\n", pid);
	}
	return 0;
}

void setStatus(DataList *dataList, unsigned int pid, unsigned int status) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->status = status;
			break;
		}
		node = node->next;
	}
	if (node == NULL) {
		printf("PID: %u does not exist.\n", pid);
	}
}

int searchEntryPID(DataList *dataList, unsigned int pid) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return 0;
    }
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
    if(!dataList){
        printf("Datalist is empty.\n");
        return 0;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->niceness == niceness && status == -1) {
			return 1;
		} else if (node->status == status && niceness == -1) {
			return 1;
		} else if (node->niceness == niceness || node->status == status) { //should this be &&
			return 1;//how will you know which exists?? do you just run this again 50/50 get right??
		}
		node = node->next;
	}
	return 0;
}

void printAllEntries(DataList *dataList) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
	Node *node = dataList->first;
	printf("%s%15s%15s%15s%15s\n", "pid", "status", "niceness", "cputime", "proctime");
	while (node != NULL) {
		printf("%u%12u%13d%23f%14f\n", node->pid, node->status, \
				node->niceness, node->cpuTime, node->procTime);
		node = node->next;
	}
}

void printEntryPID(DataList *dataList, unsigned int pid) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
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
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
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

