#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <cairo/cairo.h>
#include <time.h>

#include "plotData.h"

#define WIDTH 900
#define HEIGHT 400
#define MAXWIDTH 700

//LOG FILE FORMAT
//log time, pid, status, niceness, cputime, proctime

InfoList *createInfoList(){
	InfoList *infoList = malloc(sizeof(InfoList));
	if (infoList == NULL) {
		printf("could not allocate memory for info list.\n");
		exit(EXIT_FAILURE);
	}
	infoList->first = NULL;
	infoList->size = 0;
	infoList->flag = 0;
	return infoList;
}

void destroyInfoList(InfoList **infoList){
    //loop through save position to the next free current, do for each
    if (!*infoList){
        printf("Infolist is empty\n");
        return;
    }
    InfoList *list = *infoList;
	Info *info = list->first;
    while (info != NULL){
        Info *temp = info;
        info = info->next;
        free(temp);
    }
	free(list);
    //now point to NULL
    *infoList = NULL;
}

float setScale(InfoList *infoList){
	double totalTime;
	float scale = 1.0; 

	if(!infoList){
        printf("infolist is empty.\n");
        return 0;
    }
	Info *node = infoList->first;
	while (node != NULL) {
		totalTime += node->cpuTime * 20;
		node = node->next;
	}

	if(totalTime > MAXWIDTH){
		scale = MAXWIDTH / totalTime;
	}
	return scale;
}

int searchEntryPID(InfoList *infoList, unsigned int pid){
    if(!infoList){
        printf("infolist is empty.\n");
        return 0;
    }
	Info *node = infoList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

void printInfoList(InfoList *infoList){
    Info *current = infoList->first;
    printf("PID   R     G     B     CPU Time   Proc Time   Tarr   Trun   Text drawn\n");
    while (current != NULL) {
        printf("%-6u%-6.2f%-6.2f%-6.2f%-12.2f%-12.2f%-8.2f%-8.2f%-8.2f%-8d\n",
               current->pid, current->r, current->g, current->b,
               current->cpuTime, current->procTime, current->Tarr,
               current->Trun, current->Text, current->drawn);
        current = current->next;
    }
}

//new getGraphData
void getGraphData(int fileCount, InfoList *infoList){
	FILE *fptr;
	struct dirent *dp;
	DIR *dirptr = opendir("../log/");

	for (int i = 0; i < 2; i++) {dp = readdir(dirptr);} //skips . & ..
	while ((dp = readdir(dirptr)) != NULL) {
	 	int len = strlen("../log/");
		char *path = malloc(len + 1);
 		strcpy(path, "../log/");

		int len2 = strlen(dp->d_name);
		path = realloc(path, len + len2 + 1);
		strcat(path, dp->d_name);

		fptr = fopen(path, "r");
		char *buffer = malloc(100);
		fgets(buffer, 100, fptr); // Skips header
		while (fgets(buffer, 100, fptr) != NULL) {
			//printf("Getgraphdata function: \n");//TODO erase test 
			float globalTime = atof(strtok(buffer, ","));
			//printf("global time: %.2f ", globalTime);//TODO erase test
            unsigned int pid = atoi(strtok(NULL, ","));
			//printf("PID: %u ", pid);//TODO erase test
			unsigned int status = atoi(strtok(NULL, ","));
			//printf("Status: %u", status);//TODO erase test
			strtok(NULL, ",");//niceness dont need for now...
			float cpuTime = atof(strtok(NULL, ","));//as of now cpu and proc time the same
			//printf("CPU Time: %.2f\n", cpuTime);//TODO erase test
			float procTime = atof(strtok(NULL, ","));
			//printf("Proc Time: %.2f\n", procTime);//TODO erase test

			//because pid 1 gets pushed immed there is no status == 1 so..//TODO is this still the case?? otherwise dont need this
			if(infoList->size == 0){
				Info *newProc = malloc(sizeof(Info));
					if (newProc == NULL) {
						printf("could not allocate memory for new proc.\n");
						return;
					}
					newProc->pid = pid;
					newProc->Tarr = 0.0;
					newProc->procTime = procTime;
					newProc->r = -1;
					newProc->g = -1;
					newProc->b = -1;
					//newProc->Trun = -1;//TODO switched to very large number for testing...
					newProc->Trun = 100;
					newProc->totalProc = -1;
					newProc->drawn = 0;

					infoList->first = newProc;
					infoList->size++;

			}

			//need to check stat == 1 AND pid !in infoList
			if (status == 1) {
				//check if already in infolist 0 (not in) 1 (in)
				int check = searchEntryPID(infoList, pid);
				if(!check){
					Info *newProc = malloc(sizeof(Info));
					if (newProc == NULL) {
						printf("could not allocate memory for new proc.\n");
						return;
					}
					newProc->pid = pid;
					newProc->Tarr = globalTime;
					newProc->procTime = procTime;
					newProc->r = -1;
					newProc->g = -1;
					newProc->b = -1;
					//newProc->Trun = -1;//TODO switched to very large number for testing...
					newProc->Trun = 100;
					newProc->totalProc = -1;
					newProc->drawn = 0;

					//linked list not a queue (insert at the back)
					if (infoList->size == 0) {
						infoList->first = newProc;
					} else {
						//find the last
						Info *current = infoList->first;
            			while (current->next != NULL) {
                			current = current->next;
            			}
            			current->next = newProc;
					}
					infoList->size++;
				}
				
			}
			
			if (status == 2) {
				//printf("status is 2 and runtime is %f\n", globalTime);//TODO erase test
				//will need to get the entry and set Trun = globalTime
				setRUNTime(infoList, pid, globalTime);//in func it checks if it has been set already if it has it doesnt
			}
			if (status == 3) {//should only appear as 3 once so dont need to check? can just set
				//will need to get the entry and set Text = globalTime
				setEXTTime(infoList, pid, globalTime, cpuTime);
				//TODO should only update once out? anyways only will switch between 1 and 2
			}
		}
		fclose(fptr);
		free(path);
		free(buffer);
	}
	closedir(dirptr);
	printInfoList(infoList);//TODO delete this after just doing a test
	//qsort(infoList->first, infoList->size, sizeof(Info), cmpfunc);//for arrays
	//bubbleSort(infoList);
	printInfoList(infoList);//TODO delete this after just doing a test
	calcAVG(infoList);

	//TODO delete this after just doing a test
	if(infoList->size == fileCount){
		printf("size of list equal to file count\n");
	}
	return;
}

void setEXTTime(InfoList *infoList, unsigned int pid, float globalTime, float cpuTime){
    if(!infoList){
        printf("infoList is empty.\n");
        return;
    }
	Info *node = infoList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->cpuTime = cpuTime;
			node->Text = globalTime;
			if(node->Text - node->Trun > node->procTime){
				printf("RR used"); //
				infoList->flag = 1;
			}
			printf("Text-Trun: %f, proc: %f\n", node->Text - node->Trun, node->procTime);//TODO erase test
			return;
		}
		node = node->next;
	}
  
	if (node == NULL) {
		printf("setEXTTime error: PID: %u does not exist\n", pid);
		return;
	}
}

void setRUNTime(InfoList *infoList, unsigned int pid, float globalTime){
    if(!infoList){
        printf("infoList is empty.\n");
        return;
    }
	Info *node = infoList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			//if(node->Trun == -1){//TODO changed for testing
			if(node->Trun == 100.00){
				node->Trun = globalTime;
			}
			return;
		}
		node = node->next;
	}
  
	if (node == NULL) {
		printf("setRUNTime error: PID: %u does not exist\n", pid);
		return;
	}
}

int getFileCount(char *path) {
	int count = 0;
	DIR *dirptr = opendir(path);
	struct dirent *dp;

	for (int i = 0; i < 2; i++) {dp = readdir(dirptr);} //skips . & ..

	while ((dp = readdir(dirptr)) != NULL) {
		int len = strlen(path);
		char *fullpath = malloc(len + 1);
 		strcpy(fullpath, path);

		int len2 = strlen(dp->d_name);
		fullpath = realloc(fullpath, len + len2 + 1);
		strcat(fullpath, dp->d_name);

		FILE *fptr = fopen(fullpath, "r");
		char *buffer = malloc(100);
		while (fgets(buffer, 100, fptr) != NULL) {
			strtok(buffer, ",");
			strtok(NULL, ",");
			unsigned int status = atoi(strtok(NULL, ","));
			strtok(NULL, ",");
			atoi(strtok(NULL, ","));
			strtok(NULL, ",");

			if (status == 3) {
				count++;
				break;
			}
		}
		fclose(fptr);
		free(fullpath);
		free(buffer);
	}
	closedir(dirptr);
	return count;
}

float randCol(){
	float max = 1.0;
	return (float) rand() / ((float) (RAND_MAX)) * max;
}

void bubbleSort(InfoList *infoList){
	if(infoList == NULL || infoList->first == NULL){
		//nothing to sort
		return;
	}

	int swapped;
	Info *currPtr;
	Info *last = NULL;
	Info *head = infoList->first;


	do{
		swapped = 0;
		currPtr = head;

		while(currPtr != last && currPtr != NULL){
			
			//check if current is greater than the next swap if is
			if(currPtr->Trun > currPtr->next->Trun){
				//is this line causing fault?
				//printf("%.2f greater than %.2f\n", currPtr->Trun, currPtr->next->Trun);//TODO test print
				swapNodes(&head, currPtr, currPtr->next);
				swapped = 1;
			}
			currPtr = currPtr->next;
		}
		//move last pointer to second last
		last = currPtr;
	}while (swapped);
}

void swapNodes(Info **headRef, Info *a, Info *b){

	if(a == b || *headRef == NULL){
		return;
	}

	/*Info temp = *a;
	*a = *b;
	*b = temp;*/

	Info *prevA = NULL;
	Info *currA = *headRef;
	while(currA && currA != a){
		prevA = currA;
		currA = currA->next;
	}

	Info *prevB = NULL;
	Info *currB = *headRef;
	while(currB && currB != b){
		prevB = currB;
		currB = currB->next;
	}

	//if a isnt in list or a is first
	if(currA == NULL || prevB == NULL || currA->next == currB || currB->next == currA){
		//node not in list
		return;
	}
	if(prevA != NULL){
		prevA->next = b;
	}
	else{
		*headRef = b;
	}

	if(prevB != NULL){
		prevB->next = a;
	}
	else{
		*headRef = a;
	}

	//swap next ptrs
	Info *temp = a->next;
	a->next = b->next;
	b->next = temp;

}

//TODO testing, RR...
int plotData(InfoList *infoList){
	if(!infoList || infoList->size == 0){
        printf("infolist is empty.\n");
        return 0;
    }
	//initialize cairo
  	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);

  	cairo_t *cr = cairo_create(surface);

  	//set the background to white
  	cairo_set_source_rgb(cr, 1, 1, 1);
  	cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
  	cairo_fill(cr);

  	//initial positions
  	double x = 100;
  	double y = 200;
  	double label = 0;
  
  	srand((unsigned int) time(NULL));
  	float scale = setScale(infoList); //TODO test

	int counter = 0;
	Info *node = infoList->first;
	//Info *node = NULL;
	//node->drawn = 1;
	printf("while counter is less than %d\n", infoList->size + 1);
	while(counter < infoList->size ){
		 /*if (node == NULL) {
        node = infoList->first; // Start from the head node
    }*/
		Info *minNode = infoList->first;
		Info *tmpNode = infoList->first;
		float currMin = node->Text + 1;
		while(minNode != NULL){
			if(minNode->Trun < currMin && !(minNode->drawn)){
				currMin = minNode->Trun; 
				tmpNode = minNode;	
			}
			minNode = minNode->next;
		}
		node = tmpNode;
		//Info *node = infoList->first;
		//currMin = node->Trun;

		//loop through to create the squares
		//while (node != NULL) {

		double width = node->cpuTime * 20 * scale;

		//for RR set and save so that same colour used throughout
		if(node->r == -1){
			node->r = randCol();
			node->g = randCol();
			node->b = randCol();
		}
		float r = node->r;
		float g = node->g;
		float b = node->b;

		cairo_rectangle(cr, x, y, width, 50);
		//given colour based off of pid since pid unique
		cairo_set_source_rgb(cr, r, g, b);
		cairo_fill(cr);

		//write pid label
		char pidLabel[15];
		sprintf(pidLabel, "%.d", node->pid);
		cairo_set_font_size(cr, 22);
		cairo_move_to(cr, x + (width * 0.3), y + 25); 
		cairo_set_source_rgba(cr, 0, 0, 0, 1);
		cairo_show_text(cr, pidLabel);

		x += width; //for starting position of next 
		label = x;  
		node->drawn = 1;
		//node = node->next;  
	//}
		//search for the next min
		//TODO
		/*Info *minNode = infoList->first;
		Info *tmpNode = infoList->first;
		float currMin = node->Text + 1;
		while(minNode != NULL){
			if(minNode->Trun < currMin && minNode->drawn != 1){
				currMin = minNode->Trun; 
				tmpNode = minNode;	
			}
			minNode = minNode->next;
		}
		node = tmpNode;*/
		//node->drawn = 1;
		counter++;
		printf("Counter is %d\n", counter);
	}

  	//line and label
  	cairo_set_source_rgb(cr, 0, 0, 0);
  	cairo_move_to(cr, 100, HEIGHT - 150);
  	cairo_line_to(cr, label + 50, HEIGHT - 150);
  	cairo_stroke(cr);

  	//arrow head
  	cairo_move_to(cr, label + 50, HEIGHT - 150);
  	cairo_line_to(cr, label + 30, HEIGHT - 155);
  	cairo_stroke(cr);
  	cairo_move_to(cr, label + 50, HEIGHT - 150);
  	cairo_line_to(cr, label + 30, HEIGHT - 145);
  	cairo_stroke(cr);

  	cairo_set_font_size(cr, 18);
  	cairo_move_to(cr, label + 50, HEIGHT - 130);
  	cairo_set_source_rgba(cr, 0, 0, 0, 1);
  	cairo_show_text(cr, "t");

	//draw the calculations 
	//initial positions
  	//double x = 100;
  	//double y = 200;
  	//double label = 0;
	//Tresp
	cairo_set_font_size(cr, 20);
  	cairo_move_to(cr, 100, HEIGHT - 115);
  	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	char respLabel[50];
	sprintf(respLabel, "Average Response Time: %.3f", infoList->Tresp);//TODO 3 decimal places idk if want more or less
  	cairo_show_text(cr, respLabel);

	//Tturn
	cairo_set_font_size(cr, 20);
  	cairo_move_to(cr, 100, HEIGHT - 80);
  	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	char turnLabel[50];
	sprintf(turnLabel, "Average Turn-Around Time: %.3f", infoList->Tturn);//TODO 3 decimal places idk if want more or less
  	cairo_show_text(cr, turnLabel);

	//number of procs
	cairo_set_font_size(cr, 20);
  	cairo_move_to(cr, 100, HEIGHT - 50);
  	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	char procsLabel[50];
	sprintf(procsLabel, "Number of Processes: %.d", infoList->size);//TODO if change for RR may need to change this
  	cairo_show_text(cr, procsLabel);
	

  	time_t t = time(NULL);
  	struct tm *tm_info = localtime(&t);
  	char date[20]; 
  	strftime(date, sizeof(date), "%m-%d-%Y-%H:%M:%S", tm_info);
  	char filename[50];
  	snprintf(filename, sizeof(filename), "../log/log-%s.png", date);


  	// save the image  
  	//cairo_surface_write_to_png(surface, filename); //TODO uncomment for hand in
	cairo_surface_write_to_png(surface, "../log/atest.png");//TODO erase test file name

  	cairo_destroy(cr);
  	cairo_surface_destroy(surface);

  	return 0;
}

void calcAVG(InfoList *infoList){
	Info *current = infoList->first;
	float tmp;
    while (current != NULL){
		//tresp = trun - tarr
		tmp = current->Trun - current->Tarr;
		infoList->Tresp += tmp;
		//tturn = text - tarr
		tmp = current->Text - current->Tarr;
		infoList->Tturn += tmp;
		current = current->next;
	}

	infoList->Tresp = infoList->Tresp / infoList->size;
	infoList->Tturn = infoList->Tturn / infoList->size;

	return;
}

int main() {
	//create infolist
	InfoList *infoList = createInfoList();

	//didnt change getFileCount
	//TODO do we want to keep it so it counts how many processes finish? do we assume all finish?
	int fileCount = getFileCount("../log/");

	//will need to change getGrapData, no return just pass the infoList and parse file 
	getGraphData(fileCount, infoList);
	//TODO remove test prints
	if(infoList->flag == 1){
		printf("RR used\n");
	}
	if(infoList->flag == 0){
		printf("SJF or FIFO used\n");
	}
	//TODO change plot data to work with the struct now
	plotData(infoList);//TODO test
	//testt print function
	printInfoList(infoList); //TODO erase test


	//destroy info list
	destroyInfoList(&infoList);

	return 0;
}
