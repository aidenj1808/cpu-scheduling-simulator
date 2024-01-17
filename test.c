#include <stdio.h>
#include "dStruct.h"

/******************************
 * TODO
 * test searching through empty datalist 
 * test searching for non existent
 * test for something at the start/middle/end
 * test for max entries of 100
 * do we need to add stuff to test for proper args?
*/

int main(void) {
    printf("  ****************************\n");
 	printf("  *  test: createDataList()  *\n");
    printf("  ****************************\n");
	//Test here
	DataList *list = createDataList();
	//if list isnt null and successfully created will print success
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
	//TODO test max of 100 but maybe do it after testing print? so dont have to print out all entries
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
	//TODO search for pid not in 
	printf("Search for entry by pid 1\n");
	Node *entry1 = getEntry(list, 1, -1);//search by pid
	printEntryPID(list, entry1->pid);
	printf("Search for entry by pid 20, which does not exist\n");
	//TODO this seg faults when passed pid that dne
	
	Node *entry2 = getEntry(list, 20, -1);//search by pid
	if(entry2){
		printEntryPID(list, entry2->pid);
	}
	
	printf("Search for entry by index 2\n");
	Node *entry3 = getEntry(list, -1, 2);//search by index
	printEntryPID(list, entry3->pid);
	printf("Search for entry by index 15, which does not exist\n");
	Node *entry4 = getEntry(list, -1, 15);//search by index
	if(entry4){
		printEntryPID(list, entry4->pid);
	}
	printf("\n");
	
    printf("  **********************\n");
 	printf("  *  test: popEntry()  *\n");
    printf("  **********************\n");
	//Test here
	//TODO pop empty?, pop until past empty?
	//*********** im dub what is this returning....
	printf("popEntry() will pop off the oldest process and return the data\n");
	printf("Pop pid 1\n");
	Node *pop1 = popEntry(list);//pop the last and return the data 
	printf("Popped entry: %u	%u	%d	%f	%f\n",pop1->pid, pop1->status, pop1->niceness, pop1->cpuTime, pop1->procTime);
	Node *pop2 = popEntry(list);
	Node *pop3 = popEntry(list);
	printAllEntries(list);
	printf("\n");
	
    printf("  ************************\n");
 	printf("  *  test: getCPUTime()  *\n");
    printf("  ************************\n");
	//Test here
	//TODO test pid that dne
	float time1 = getCPUTime(list, 9); //are these pids popped already?
	printf("Get CPU time for PID 9: %f\n", time1);
	float time2 = getCPUTime(list, 1); 
	printf("Get CPU for PID 1 (which was popped) %f\n\n", time2); // maybe dont need to print idk
	printf("\n");
	
    printf("  ************************\n");
 	printf("  *  test: setCPUTime()  *\n");
    printf("  ************************\n");
	//Test here
	//TODO test pid dne ****add a statement to the function that notifies it dne 
	//maybe print out then change then print the change?or i guess just change the ones pulled from above
	setCPUTime(list, 1, 4.5); 
	//pid 1 dne so maybe add a statement like all the other ones to say it dne ??
	
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
	//TODO chnage the pids cause pretty sure these gone, keep one to test for DNE
	int niceness1 = getNiceness(list, 1);
	printf("Get niceness that does not exist (1): %d\n", niceness1);
	int niceness2 = getNiceness(list, 8);
	printf("Get niceness that does exist (8): %d\n", niceness2);
	printf("\n");
	
    printf("  *************************\n");
 	printf("  *  test: setNiceness()  *\n");
    printf("  *************************\n");
	//Test here
	printf("Testing pid 1 which does not exist, nothing will happen program will continue..\n");
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
	//TODO test DNE
	unsigned int status1 = getStatus(list, 1);
	printf("Getting status of PID 1 (does not exist): %u\n", status1);
	unsigned int status2 = getStatus(list, 5);
	printf("Getting status of PID 5: %u\n", status2);
	printf("\n");
	
    printf("  ***********************\n");
 	printf("  *  test: setStatus()  *\n");
    printf("  ***********************\n");
	//Test here
	//same notes as the last few...
	printf("Testing pid 1 which does not exist, nothing will happen program will continue..\n");
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
	//TODO DNE, some statement using the return to show if found or not
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
	//TODO DNE, some statement using the return to show if found or not
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
	printAllEntries(list);
	printf("\n");

    printf("  ***************************\n");
 	printf("  *  test: printEntryPID()  *\n");
    printf("  ***************************\n");
	//Test here
	printf("Print entry of PID 1 (does not exist) and PID 9 (exists):\n");
	printEntryPID(list, 1);
	printEntryPID(list, 9);
	printf("\n");

    printf("  **************************************\n");
 	printf("  *  test: printEntryNicenessStatus()  *\n");
    printf("  **************************************\n");
	//Test here
	//TODO outliers usual...
	//fail tests
	printf("Try to search for niceness 1, then try status 2, then try niceness 2 and status 3:\nAll combinations do not exist\n\n");
	printEntryNicenessStatus(list, 1, -1);
	printf("\n");
	printEntryNicenessStatus(list, -1, 2);
	printf("\n");
	printEntryNicenessStatus(list, 2, 3);
	//TODO STILL
	printf("Try to search for niceness 1, then try status 2, then try niceness 2 and status 3:\nAll combinations do not exist\n\n");
	printEntryNicenessStatus(list, 1, -1);
	printf("\n");
	printEntryNicenessStatus(list, -1, 2);
	printf("\n");
	printEntryNicenessStatus(list, 2, 3);
	printf("\n");
	printf("\n");
/*
	//TODO test the max capacity here??

	printf("  *****************************\n");
 	printf("  *  test: destoryDataList()  *\n");
    printf("  *****************************\n");
	//Test here
	//TODO like another if null success type print
	destroyDataList(list);
	printf("\n");*/

	return 0;
}
