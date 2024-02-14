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

//old version
/*float *getGraphData(int fileCount) {
	FILE *fptr;
	struct dirent *dp;
	DIR *dirptr = opendir("../log/");
	int index = 0;
    float *data = malloc(sizeof(float) * (fileCount * 2));

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
			strtok(buffer, ",");
            unsigned int pid = atoi(strtok(NULL, ","));
			unsigned int status = atoi(strtok(NULL, ","));
			strtok(NULL, ",");
			float cpuTime = atoi(strtok(NULL, ","));
			strtok(NULL, ",");

			if (status == 3) {
                data[index++] = pid;
				data[index++] = cpuTime;
				break;
			}
		}
		fclose(fptr);
		free(path);
		free(buffer);
	}
	closedir(dirptr);
	return data;
}*/
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
			float globalTime = atoi(strtok(buffer, ","));
            unsigned int pid = atoi(strtok(NULL, ","));
			unsigned int status = atoi(strtok(NULL, ","));
			strtok(NULL, ",");//niceness dont need for now...
			float cpuTime = atoi(strtok(NULL, ","));//as of now cpu and proc time the same
			float procTime = atoi(strtok(NULL, ","));

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
					newProc->cpuTime = cpuTime;
					newProc->procTime = procTime;
					newProc->r = -1;
					newProc->g = -1;
					newProc->b = -1;
					newProc->Trun = -1;

					if (infoList->size == 0) {
						newProc->next = NULL;
						infoList->first = newProc;
					} else {
						newProc->next = infoList->first;
						infoList->first = newProc;
					}
					infoList->size++;
				}
				break;
			}

			if (status == 2) {
				//will need to get the entry and set Trun = globalTime
				setRUNTime(infoList, pid, globalTime);//in func it checks if it has been set already if it has it doesnt
				break;
			}
			if (status == 3) {//should only appear as 3 once so dont need to check? can just set
				//will need to get the entry and set Text = globalTime
				setEXTTime(infoList, pid, globalTime);
				break;
			}
		}
		fclose(fptr);
		free(path);
		free(buffer);
	}
	closedir(dirptr);
	//TODO delete this after just doing a test
	if(infoList->size == fileCount){
		printf("size of list equal to file count\n");
	}
	return;
}

void setEXTTime(InfoList *infoList, unsigned int pid, float globalTime){
    if(!infoList){
        printf("infoList is empty.\n");
        return;
    }
	Info *node = infoList->first;
	while (node != NULL) {
		if (node->pid == pid) {
			node->Text = globalTime;
			break;
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
			if(node->Trun == -1){
				node->Trun = globalTime;
			}
			break;
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

//TODO this is still old plot data will need to change to use struct 
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

	Info *node = infoList->first;
  	//loop through to create the squares
	while (node != NULL) {

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
		node = node->next;  
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

  	time_t t = time(NULL);
  	struct tm *tm_info = localtime(&t);
  	char date[20]; 
  	strftime(date, sizeof(date), "%m-%d-%Y-%H:%M:%S", tm_info);
  	char filename[50];
  	snprintf(filename, sizeof(filename), "../log/log-%s.png", date);


  	// save the image  
  	cairo_surface_write_to_png(surface, filename);

  	cairo_destroy(cr);
  	cairo_surface_destroy(surface);

  	return 0;
}

int main() {
	//create infolist
	InfoList *infoList = createInfoList();

	//didnt change getFileCount
	//TODO do we want to keep it so it counts how many processes finish? do we assume all finish?
	int fileCount = getFileCount("../log/");

	//will need to change getGrapData, no return just pass the infoList and parse file 
	getGraphData(fileCount, infoList);
	//TODO change plot data to work with the struct now
	plotData(infoList);//TODO test

	/*previous calls using just an array
	//int fileCount = getFileCount("../log/");
	float *values = getGraphData(fileCount);
    plotData(fileCount * 2, values);
	free(values);*/

	//destroy info list
	destroyInfoList(&infoList);

	return 0;
}
