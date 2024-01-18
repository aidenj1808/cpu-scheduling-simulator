#include <stdio.h>
#include <stdlib.h>
#include "dStruct.h"

/******************************
 * TODO
 * 
*/

int main(void) {
    printf("  ****************************\n");
 	printf("  *  test: createDataList()  *\n");
    printf("  ****************************\n");
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
	printf("Try to pop an empty Datalist\n");
	popEntry(emptyList);
	printf("\n");

	printf("popEntry() will pop off the oldest process and return the data\n");
	Node pop1 = popEntry(list); 
	printf("First pop information:\n");
	printf("PID: %u\n", pop1.pid);
	printf("Status: %u\n", pop1.status);
	printf("Niceness: %d\n", pop1.niceness);
	printf("cpuTime: %f\n", pop1.cpuTime);
	printf("procTime: %f\n", pop1.procTime);
	printf("DataList:\n");
	printAllEntries(list);
	
	printf("Pop entry with one entry in list:\n");
	DataList *list2 = createDataList();
	pushEntry(&list2, 1, 1, 1, 1.0, 1.0);
	Node pop2 = popEntry(list2);
	printf("Pop entry with one entry in list information:\n");
	printf("PID: %u\n", pop2.pid);
	printf("Status: %u\n", pop2.status);
	printf("Niceness: %d\n", pop2.niceness);
	printf("cpuTime: %f\n", pop2.cpuTime);
	printf("procTime: %f\n", pop2.procTime);
	printf("\n");

    printf("  ************************\n");
 	printf("  *  test: getCPUTime()  *\n");
    printf("  ************************\n");
	printf("Try to get CPU from an empty Datalist\n");
	getCPUTime(emptyList,1);
	printf("\n");
  
	float time1 = getCPUTime(list, 9); 
	printf("Get CPU time for PID 9: %f\n", time1);
	float time2 = getCPUTime(list, 1); 
	printf("Get CPU for PID 1 (which does not exist) %f\n\n", time2); // returns -1 (invalid pid)
	printf("\n");

    printf("  ************************\n");
 	printf("  *  test: setCPUTime()  *\n");
    printf("  ************************\n");
	printf("Try to set CPU for an empty Datalist\n");
	setCPUTime(emptyList,1,10.0);
	printf("\n");
  
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
	printf("Try to get niceness from an empty Datalist\n");
	getNiceness(emptyList,1);
	printf("\n");
  
	int niceness1 = getNiceness(list, 1);
	printf("Get niceness with pid that does not exist (1) returns: %d\n", niceness1);
	int niceness2 = getNiceness(list, 8);
	printf("Get niceness that does exist (8): %d\n", niceness2);
	printf("\n");

    printf("  *************************\n");
 	printf("  *  test: setNiceness()  *\n");
    printf("  *************************\n");
  printf("Try to set niceness for an empty Datalist\n");
	setNiceness(emptyList,1,2);
	printf("\n");

	printf("Setting PID 1 to niceness 10 (which PID does not exist)\n");
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
	printf("Try to get status from an empty Datalist\n");
	getStatus(emptyList,1);
	printf("\n");

	unsigned int status1 = getStatus(list, 1);
	printf("Getting status of PID 1 (does not exist): %u\n", status1);
	unsigned int status2 = getStatus(list, 5);
	printf("Getting status of PID 5: %u\n", status2);
	printf("\n");

    printf("  ***********************\n");
 	printf("  *  test: setStatus()  *\n");
    printf("  ***********************\n");
  printf("Try to set status for an empty Datalist\n");
	setStatus(emptyList,1,1);
	printf("\n");

	printf("Setting PID 1 to status 10(which PID does not exist)\n");
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
  printf("Try to search entry from an empty Datalist\n");
	searchEntryPID(emptyList,1);
	printf("\n");

	int isvalid1 = searchEntryPID(list, 1);
	int isvalid2 = searchEntryPID(list, 8);
	printf("Searching for PID 1:\n");
	if(isvalid1){
		printf("PID 1 exists\n");
	}
	else{
		printf("PID 1 does not exist\n");
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
	printf("Try to search entry from an empty Datalist\n");
	searchEntryNicenessStatus(emptyList,1,1);
	printf("\n");

	//testing by niceness
	isvalid1 = searchEntryNicenessStatus(list, 12, -2); 
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
	isvalid1 = searchEntryNicenessStatus(list, -1, 1);
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

	//search by niceness or status 
	isvalid1 = searchEntryNicenessStatus(list, 4, 1);
	isvalid2 = searchEntryNicenessStatus(list, 1, 8);
	printf("\nSearching for status 2 or niceness 4:\n");
	if(isvalid1){
		printf("status 2 exists\n");
	}
	else{
		printf("status 2 does not exist\n");
	}
	printf("\nSearching for status 8 or niceness 2:\n");
	if(isvalid2){
		printf("status 8 or niceness 2 exists\n");
	}
	else{
		printf("status 8 or niceness 2 does not exist\n");
	}
	printf("\n");
	printf("\nSearching for status 1 or niceness 1:\n");
	isvalid2 = searchEntryNicenessStatus(list, 1, 1);
	if(isvalid2){
		printf("status 1 or niceness 1 exists\n");
	}
	else{
		printf("status 1 or niceness 1 does not exist\n");
	}
	printf("\n");

    printf("  *****************************\n");
 	printf("  *  test: printAllEntries()  *\n");
    printf("  *****************************\n");
	//Test here 
	printf("Try to print an empty Datalist\n");
	printAllEntries(emptyList);
	printf("\n");	

	printAllEntries(list);
	printf("\n");

    printf("  ***************************\n");
 	printf("  *  test: printEntryPID()  *\n");
    printf("  ***************************\n");
	//Test here
	printf("Try to print from an empty Datalist\n");
	printEntryPID(emptyList,1);
	printf("\n");

	printf("Print entry of PID 1 (does not exist) and PID 9 (exists):\n");
	printEntryPID(list, 1);
	printEntryPID(list, 9);
	printf("\n");

    printf("  **************************************\n");
 	printf("  *  test: printEntryNicenessStatus()  *\n");
    printf("  **************************************\n");
	//Test here
	printf("Try to print from an empty Datalist\n");
	printEntryNicenessStatus(emptyList,1,1);
	printf("\n");

	//fail tests
	printf("Try to search for niceness 1, then try status 2, then try niceness 2 and status 3:\nAll combinations do not exist\n\n");
	printEntryNicenessStatus(list, 1, -1);
	printf("\n");
	printEntryNicenessStatus(list, -1, 12);
	printf("\n");
	printEntryNicenessStatus(list, 20, 1);
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
	 * Do we want to allow all to be popped? 
	**************/ 
	//printf("First will pop until empty\n");
	
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	pop1 = popEntry(list);
	
	pop1 = popEntry(list); //last entry
	printAllEntries(list);

	printf("If try to have over 100 entries entry will not be pushed\n");
	for (int i = 1; i < 101; i++) {
		pushEntry(&list, i, i, i, (float) i, (float) i);
	}
	pushEntry(&list, 101, 101, 101, 101.0, 101.0);
	printAllEntries(list);

	printf("\n");

	printf("  *****************************\n");
 	printf("  *  test: destoryDataList()  *\n");
    printf("  *****************************\n");
	//Test here
	destroyDataList(&list);
	destroyDataList(&list2);
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
