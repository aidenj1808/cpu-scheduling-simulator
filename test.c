#include <stdio.h>
#include "dStruct.h"

/******************************
 * TODO
 * 
*/

int main(void) {
    printf("  ****************************\n");
 	printf("  *  test: createDataList()  *\n");
    printf("  ****************************\n");
	//Test here
	/**************
	 * TODO 
	 * i think can't do more testing than this?
	**************/
	DataList *list = createDataList();
	DataList *emptyList = NULL; //for use testing NULL DataList

	if(list){
		printf("Success creating DL\n");
	}
	else{
		printf("Failure creating DL\n");
	}
	printf("\n");

    printf("  ***********************\n");
 	printf("  *  test: pushEntry()  *\n");
    printf("  ***********************\n");
	//Test here
	/**************
	 * TODO 
	 * i think can't do more testing than this?
	**************/
	printf("Pushing entries 1-9 in format x, x, x, x.0, x.0\n");
	pushEntry(&list, 1, 1, 1, 1.0, 1.0);
	pushEntry(&list, 2, 2, 2, 2.0, 2.0);
	pushEntry(&list, 3, 3, 3, 3.0, 3.0);
	pushEntry(&list, 4, 4, 4, 4.0, 4.0);
	pushEntry(&list, 5, 5, 5, 5.0, 5.0);
	pushEntry(&list, 6, 6, 6, 6.0, 6.0);
	pushEntry(&list, 7, 7, 7, 7.0, 7.0);
	pushEntry(&list, 8, 8, 8, 8.0, 8.0);
	pushEntry(&list, 9, 9, 9, 9.0, 9.0);
	//show was success by printing all 
	printAllEntries(list); 
	printf("\n");
	
	printf("  **********************\n");
 	printf("  *  test: getEntry()  *\n");
    printf("  **********************\n");
	//Test here

	printf("Get entry from empty DataList\n");
	getEntry(emptyList, 1, -1);
	printf("\n");

	printf("Get entry by pid 1\n");
	Node *entry1 = getEntry(list, 1, -1);//get entry by pid
	printf("Entry information from getEntry() with pid 1\n");
	printf("PID: %u\n", entry1->pid);
	printf("Status: %u\n", entry1->status);
	printf("Niceness: %d\n", entry1->niceness);
	printf("cpuTime: %f\n", entry1->cpuTime);
	printf("procTime: %f\n\n", entry1->procTime);
	
	printf("Get entry by pid 20, which does not exist\n");
	Node *entry2 = getEntry(list, 20, -1);
	printf("Get entry with pid 20 that does not exist returns NULL: %p\n", entry2);
	printf("\n");

	printf("Get entry by index 2\n");
	Node *entry3 = getEntry(list, -1, 2);//get entry by index
	printf("Entry information from getEntry() with index 2\n");
	printf("PID: %u\n", entry3->pid);
	printf("Status: %u\n", entry3->status);
	printf("Niceness: %d\n", entry3->niceness);
	printf("cpuTime: %f\n", entry3->cpuTime);
	printf("procTime: %f\n\n", entry3->procTime);

	printf("Get entry by index 15, which does not exist\n");
	Node *entry4 = getEntry(list, -1, 15);//get entry by index
	printf("Get entry with index 15 that does not exist returns NULL: %p\n", entry4);
	printf("\n");
	
    printf("  **********************\n");
 	printf("  *  test: popEntry()  *\n");
    printf("  **********************\n");
	//Test here
	/**************
	 * TODO 
	 * down at bottom try to pop when only one entry get seg fault
	 * not sure what it is returning
	 * should we have a function to print the popped entry or?? 
	 * do something with the popped data? if statements just there to stop the warnings
	**************/
	printf("popEntry() will pop off the oldest process and return the data\n");
	printf("Pop pid 1\n");
	Node *pop1 = popEntry(list); 
	printf("Popped entry: %u	%u	%d	%f	%f\n",pop1->pid, pop1->status, pop1->niceness, pop1->cpuTime, pop1->procTime);
	Node *pop2 = popEntry(list);
	if(pop2){
		;
	}
	Node *pop3 = popEntry(list);
	if(pop3){
		;
	}
	printAllEntries(list);
	printf("\n");
	
    printf("  ************************\n");
 	printf("  *  test: getCPUTime()  *\n");
    printf("  ************************\n");
	//Test here
	/**************
	 * TODO 
	 * i think nothing left unless we want to add a way to handle invalid args and test that
	**************/ 
	float time1 = getCPUTime(list, 9); 
	printf("Get CPU time for PID 9: %f\n", time1);
	float time2 = getCPUTime(list, 1); 
	printf("Get CPU for PID 1 (which does not exist) %f\n\n", time2); 
	printf("\n");
	
    printf("  ************************\n");
 	printf("  *  test: setCPUTime()  *\n");
    printf("  ************************\n");
	//Test here
	/**************
	 * TODO 
	 * again nothing else unless we do valid arg check
	**************/ 
	printf("Set CPUTime for PID 1 to 4.5(which does not exist)\n");
	setCPUTime(list, 1, 4.5); 
	printf("\n");
	printf("The entry for PID 7 before:\n");
	printEntryPID(list, 7);
	setCPUTime(list, 7, 10.254376);
	printf("\nThe entry for PID 7 after changing CPU time to 10.254376:\n");
	printEntryPID(list, 7);
	printf("\n");

    printf("  *************************\n");
 	printf("  *  test: getNiceness()  *\n");
    printf("  *************************\n");
	//Test here
	/**************
	 * TODO 
	 * make sure the DNE return of -1 works
	 * again nothing else unless we do valid arg check
	**************/ 
	int niceness1 = getNiceness(list, 1);
	printf("Get niceness that does not exist (1): %d\n", niceness1);
	int niceness2 = getNiceness(list, 8);
	printf("Get niceness that does exist (8): %d\n", niceness2);
	printf("\n");
	
    printf("  *************************\n");
 	printf("  *  test: setNiceness()  *\n");
    printf("  *************************\n");
	//Test here
	/**************
	 * TODO 
	 * add a DNE statement to function like others
	 * again nothing else unless we do valid arg check
	**************/ 
	printf("Setting PID 1 to niceness 10 (which does not exist)\n");
	setNiceness(list, 1, 10);
	printf("PID 8 before: \n");
	printEntryPID(list, 8);
	setNiceness(list, 8, 300);
	printf("\nPID 8 after changing niceness to 300: \n");
	printEntryPID(list, 8);
	printf("\n");
	
    printf("  ***********************\n");
 	printf("  *  test: getStatus()  *\n");
    printf("  ***********************\n");
	//Test here
	/**************
	 * TODO 
	 * make sure the DNE return of 0 works
	 * again nothing else unless we do valid arg check
	**************/ 
	unsigned int status1 = getStatus(list, 1);
	printf("Getting status of PID 1 (does not exist): %u\n", status1);
	unsigned int status2 = getStatus(list, 5);
	printf("Getting status of PID 5: %u\n", status2);
	printf("\n");
	
    printf("  ***********************\n");
 	printf("  *  test: setStatus()  *\n");
    printf("  ***********************\n");
	//Test here
	/**************
	 * TODO 
	 * add DNE statement to function like others
	 * again nothing else unless we do valid arg check
	**************/ 
	printf("Setting PID 1 to status 10(which does not exist)\n");
	setStatus(list, 1, 10);
	printf("PID 5 before: \n");
	printEntryPID(list, 5);
	setStatus(list, 5, 25);
	printf("\nPID 5 after changing status to 25: \n");
	printEntryPID(list, 5);
	printf("\n");

    printf("  ****************************\n");
 	printf("  *  test: searchEntryPID()  *\n");
    printf("  ****************************\n");
	//Test here
	/**************
	 * TODO 
	 * print a DNE statement in the function? or if exists print the entry?
	 * again nothing else unless we do valid arg check
	**************/ 
	int isvalid1 = searchEntryPID(list, 2);
	int isvalid2 = searchEntryPID(list, 8);
	printf("Searching for PID 2:\n");
	if(isvalid1){
		printf("PID 2 exists\n");//should we put statement in the function like the others?
	}
	else{
		printf("PID 2 does not exist\n");
	}
	printf("Searching for PID 8:\n");
	if(isvalid2){
		printf("PID 8 exists\n");
	}
	else{
		printf("PID 8 does not exist\n");
	}
	printf("\n");

    printf("  ***************************************\n");
 	printf("  *  test: searchEntryNicenessStatus()  *\n");
    printf("  ***************************************\n");
	//Test here
	/**************
	 * TODO 
	 * print a DNE statement in the function? or if exists print the entry?
	 * again nothing else unless we do valid arg check
	 * when searching by both have it better show which one exists since it is an OR not AND??
	**************/
	//testing by niceness
	isvalid1 = searchEntryNicenessStatus(list, 2, -2); //same as above should we add a statement to function
	isvalid2 = searchEntryNicenessStatus(list, 7, -1);
	printf("Searching for niceness 2:\n");
	if(isvalid1){
		printf("niceness 2 exists\n");
	}
	else{
		printf("niceness 2 does not exist\n");
	}
	printf("\nSearching for niceness 7:\n");
	if(isvalid2){
		printf("niceness 7 exists\n");
	}
	else{
		printf("niceness 7 does not exist\n");
	}

	//testing by status
	isvalid1 = searchEntryNicenessStatus(list, -1, 2);
	isvalid2 = searchEntryNicenessStatus(list, -1, 8);
	printf("\nSearching for status 2:\n");
	if(isvalid1){
		printf("status 2 exists\n");
	}
	else{
		printf("status 2 does not exist\n");
	}
	printf("\nSearching for status 8:\n");
	if(isvalid2){
		printf("status 8 exists\n");
	}
	else{
		printf("status 8 does not exist\n");
	}

	//search by niceness or status //should the function be && 
	isvalid1 = searchEntryNicenessStatus(list, 4, 2);
	isvalid2 = searchEntryNicenessStatus(list, 2, 8);
	printf("\nSearching for status 2 and niceness 4:\n");
	if(isvalid1){
		printf("status 2 exists\n");
	}
	else{
		printf("status 2 does not exist\n");
	}
	printf("\nSearching for status 8 and niceness 2:\n");
	if(isvalid2){
		printf("status 8 exists\n");//TODO
	}
	else{
		printf("status 8 does not exist\n");//TODO
	}
	printf("\n");

    printf("  *****************************\n");
 	printf("  *  test: printAllEntries()  *\n");
    printf("  *****************************\n");
	//Test here
	/**************
	 * TODO 
	 * i think all we can test
	**************/ 
	printAllEntries(list);
	printf("\n");

    printf("  ***************************\n");
 	printf("  *  test: printEntryPID()  *\n");
    printf("  ***************************\n");
	//Test here
	/**************
	 * TODO 
	 * again nothing else unless we do valid arg check
	**************/ 
	printf("Print entry of PID 1 (does not exist) and PID 9 (exists):\n");
	printEntryPID(list, 1);
	printEntryPID(list, 9);
	printf("\n");

    printf("  **************************************\n");
 	printf("  *  test: printEntryNicenessStatus()  *\n");
    printf("  **************************************\n");
	//Test here
	/**************
	 * TODO 
	 * print a DNE statement in the function? or if exists print the entry?
	 * again nothing else unless we do valid arg check
	 * for pass tests what combinations do we need?
	**************/ 
	//fail tests
	printf("Try to search for niceness 1, then try status 2, then try niceness 2 and status 3:\nAll combinations do not exist\n\n");
	printEntryNicenessStatus(list, 1, -1);
	printf("\n");
	printEntryNicenessStatus(list, -1, 2);
	printf("\n");
	printEntryNicenessStatus(list, 2, 3);
	printf("\n");

	//pass tests
	printf("Passing tests:\n");

	printf("Try to search for niceness 6:\n");
	printEntryNicenessStatus(list, 6, -1);//niceness exist
	printf("\n");

	printf("Try to search for status 4\n");
	printEntryNicenessStatus(list, -1, 4);//status exist
	printf("\n");

	printf("Try to search for niceness 9 (exists) and status 3 (does not exist)\n");
	printEntryNicenessStatus(list, 9, 3);//niceness exists but status doesnt
	printf("\n");

	printf("Try to search for niceness 2 (does not exist) and status 25 (exists)\n");
	printEntryNicenessStatus(list, 2, 25);//niceness DNE and status exists
	printf("\n");

	printf("Try to search for niceness 300 (exists) and status 8 (exists)\n");
	printEntryNicenessStatus(list, 300, 8);//both niceness and status exist in the same entry
	printf("\n");

	printf("Try to search for niceness 4 (exists) and status 25 (exists)\n");
	printEntryNicenessStatus(list, 4, 25);//both niceness and status exist but in different entries
	printf("\n");

	printf("Changing multiple entries to status 9 then search:\n");
	setStatus(list, 6, 9);
	setStatus(list, 4, 9);
	printEntryNicenessStatus(list, -1, 9);//double status
	printf("\n");

	printf("Changing multiple entries to niceness 4 then search:\n");
	setNiceness(list, 8, 4);
	printEntryNicenessStatus(list, 4, -1);//double niceness
	printf("\n");
	printf("\n");

	printf("  *****************************\n");
 	printf("  *  test: max capacity  *\n");
    printf("  *****************************\n");
	//Test here
	/**************
	 * TODO 
	 * should have a null check at the start of function? maybe make sure all functions have?
	 * print a DNE statement in the function? or if exists print the entry?
	 * 
	 * Popping the last one results in seg fault 
	 * only get told full if doing a bunch concurrently??? or so it seems idk maybe i am missing
	 * 	something but it lets me keep adding entries...
	**************/ 
	printf("First will pop until empty\n");
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	//pop1 = popEntry(list); //last entry
	//TODO seg faults if trying to pop all off
	printAllEntries(list);

	printf("If try to have over 100 entries entry will not be pushed\n");
	for (int i = 1; i < 101; i++) {
		pushEntry(&list, i, i, i, (float) i, (float) i);
	}//here we get told datalist is full because there is 100 consecutive?? why does it allow more below 
	pushEntry(&list, 101, 101, 101, 101.0, 101.0);
	pushEntry(&list, 102, 101, 101, 101.0, 101.0);
	pushEntry(&list, 103, 101, 101, 101.0, 101.0);//after this it will have 104 entires it keeps letting entries 
	printAllEntries(list);

	printf("\n");

	printf("  *****************************\n");
 	printf("  *  test: destoryDataList()  *\n");
    printf("  *****************************\n");
	//Test here
	/**************
	 * TODO 
	 * i think good no other tests
	**************/ 
	destroyDataList(&list);
	if(!list){
		printf("Datalist destroyed.\n");
	}
	else{
		printf("Datalist could not be destroyed.\n");
	}
	printf("\n");
	printf("Attempt to destroy empty Datalist:\n");
	destroyDataList(&list);

	return 0;
}




/*void setCPUTime(DataList *dataList, unsigned int pid, float cpuTime) {
    if(!dataList){
        printf("Datalist is empty.\n");
        return;
    }
	Node *node = dataList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->cpuTime = cpuTime;
		}
		node = node->next;
	}
    if (!node) {
		printf("Data for PID: %u does not exist.\n", pid);
        return;
    }
}*/