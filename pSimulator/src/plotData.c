#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <cairo/cairo.h>
#include <time.h>

#include "plotData.h"

#define WIDTH 1000
#define HEIGHT 400
#define MAXWIDTH 845

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
	double totalTime = 0;
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
			float globalTime = atof(strtok(buffer, ","));
            unsigned int pid = atoi(strtok(NULL, ","));
			unsigned int status = atoi(strtok(NULL, ","));
			strtok(NULL, ",");//niceness dont need for now...
			float cpuTime = atof(strtok(NULL, ","));//as of now cpu and proc time the same
			float procTime = atof(strtok(NULL, ","));

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
					//newProc->Trun = -1;//TODO switched to very large number for testing... maybe try with -1 again 
					newProc->Trun = 100;
					newProc->totalProc = procTime;
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
					//newProc->Trun = -1;//TODO switched to very large number for testing...maybe try with -1 again 
					newProc->Trun = 100;
					newProc->totalProc = procTime;
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
				//will need to get the entry and set Trun = globalTime
				setRUNTime(infoList, pid, globalTime);//in func it checks if it has been set already if it has it doesnt
			}
			if (status == 3) {//should only appear as 3 once so dont need to check? can just set
				//will need to get the entry and set Text = globalTime
				setEXTTime(infoList, pid, globalTime, cpuTime);
			}
		}
		fclose(fptr);
		free(path);
		free(buffer);
	}
	closedir(dirptr);
	//printInfoList(infoList);//TODO delete this after just doing a test
	calcAVG(infoList);

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
			//TODO i think us the check func over this
			/*if(node->Text - node->Trun > node->procTime){
				//printf("RR used ext-run = %f  > %f",node->Text - node->Trun, node->procTime); //TODO
				infoList->flag = 1;
			}*/
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
	return (float) rand() / ((float) (RAND_MAX)) * max; //TODO sometimes colour given is dark making label hard to read would be nice to check for this
}

//TODO testing.... see if pid 2 still not going in... might have to do a case that if there is a status 2 and not in then just put in
//plot data for SJF and FIFO, searches for the min proctime to run, should be fine if it searches for that while FIFO...
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
  	float scale = setScale(infoList); 

	int counter = 0;
	Info *node = infoList->first;
 
	while(counter < infoList->size ){

		Info *minNode = infoList->first;
		Info *tmpNode = infoList->first;
		float currMin = node->Text + 1;//the last exit time +1 should be greater than the next proctime....
		//look for the next min proc time to plot 
		while(minNode != NULL){
			//
			if(minNode->Trun < currMin && !(minNode->drawn)){
				currMin = minNode->Trun; 
				tmpNode = minNode;	
			}
			minNode = minNode->next;
		}
		node = tmpNode;

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
		cairo_set_font_size(cr, 22);//maybe try a smaller font... or scale the font...
		cairo_move_to(cr, x + (width * 0.3), y + 25); 
		cairo_set_source_rgba(cr, 0, 0, 0, 1);
		cairo_show_text(cr, pidLabel);

		x += width; //for starting position of next 
		label = x;  
		node->drawn = 1;
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
	sprintf(turnLabel, "Average Turn-Around Time: %.3f", infoList->Tturn);
  	cairo_show_text(cr, turnLabel);

	//number of procs
	cairo_set_font_size(cr, 20);
  	cairo_move_to(cr, 100, HEIGHT - 50);
  	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	char procsLabel[50];
	sprintf(procsLabel, "Number of Processes: %.d", infoList->size);
  	cairo_show_text(cr, procsLabel);
	

  	time_t t = time(NULL);
  	struct tm *tm_info = localtime(&t);
  	char date[20]; 
  	strftime(date, sizeof(date), "%m-%d-%Y-%H:%M:%S", tm_info);
  	char filename[50];
  	snprintf(filename, sizeof(filename), "../log/log-%s.png", date);


  	// save the image  
  	cairo_surface_write_to_png(surface, filename); //TODO uncomment for hand in
	//cairo_surface_write_to_png(surface, "../log/atest.png");//TODO erase test file name

  	cairo_destroy(cr);
  	cairo_surface_destroy(surface);

  	return 0;
}

//this will be the RR plotting if the flag is set
int RRplotData(InfoList *infoList){
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
  	float scale = setScale(infoList);

	int counter = 0;
	Info *node = infoList->first;
	
	//set the timeq
	infoList->timeQuantum = node->next->Trun - node->Trun;

	while(counter < infoList->size ){

		double width;
		//if the time left is less than the time q just use the remaining proc time for the width
		if(node->totalProc <= infoList->timeQuantum){
			width = node->totalProc * 25 * scale; //change the scale here or in the function?
		}
		else{
			width = infoList->timeQuantum * 25 * scale; //scaling when jiffy .5
		}

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
		cairo_set_font_size(cr, 7);//try a smaller size but make sure it isnt too small (22 tried)
		cairo_move_to(cr, x + (width * 0.3), y + 25); 
		cairo_set_source_rgba(cr, 0, 0, 0, 1);
		cairo_show_text(cr, pidLabel);

		x += width; //for starting position of next 
		label = x;  
		
		if(node->totalProc <= infoList->timeQuantum){
			node->totalProc = 0;//if the time is less than the time q just set it to 0
			node->drawn = 1;//might not need this flag here i dont think it gets used tbh
			counter++;//counter to track how many completed procs have been done 
		}
		else{
			node->totalProc = node->totalProc - infoList->timeQuantum;//else subtract the time q from total for the next use
		}
		//printf("counter: %d, time left for pid %d: %f\n", counter, node->pid, node->totalProc);//TODO test print

		//now need to find the next node 
		//if at the end of the list
		if(node->next == NULL){
			//go back to the start
			node = infoList->first;
		}
		else{
			//else just go to the next 
			node = node->next;
		}

		//check it has time left to be drawn
		if(node->totalProc == 0){
			//if no time left and the next node is not the end go to the next node that has remaining time 
			while(node->totalProc == 0 && node->next != NULL){
				node = node->next;
			}
			//if search throught the whole thing then break 
			//should exit anyways once the counter hits 
			if(node->totalProc == 0 && node->next == NULL){
				break;
			}
		}
	}

	//everything below is the same as other....
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
  	cairo_surface_write_to_png(surface, filename); //TODO uncomment for hand in
	//cairo_surface_write_to_png(surface, "../log/RRtest.png");//TODO erase test file name

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

//TODO need to test this
void RRcheck(InfoList *infoList){
	if(!infoList){
    	printf("infoList is empty.\n");
        return;
    }
	Info *node = infoList->first;
	while (node != NULL) {
		//see if tquantums are used...
		//-0.10 cause the log is off?? TODO if the log file changes would need to change this 
		if (node->Text > node->Trun + node->procTime){//TODO 
			//printf("exit time is greater than runtime+proctime\n"); //TODO erase this test print 
			infoList->flag = 1;	
		}
		node = node->next;
	}
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

	//check if RR is used
	RRcheck(infoList);//TODO test logic works

	//*****FORCE TO USE RR*******/
	//infoList->flag = 1; //TODO remove this just here for RR testing while algo isnt ready in procSim
	
	//TODO remove test prints
	if(infoList->flag == 1){
		//printf("RR used\n");
		RRplotData(infoList);
		//might need to adjust the avgCalc to work with RR too????
	}
	if(infoList->flag == 0){
		//printf("SJF or FIFO used\n");
		plotData(infoList);//TODO test
	}
	//printInfoList(infoList); //TODO erase test


	//destroy info list
	destroyInfoList(&infoList);

	return 0;
}
