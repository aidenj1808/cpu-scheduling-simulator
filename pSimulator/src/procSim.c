#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dStruct.h"

float GLOBALTIME = 0.00; //do we want this name? or

/*******
 * loops adding and removing processes from running queue,
 *  adding and removing from a ready queue
 * updating log files until the queues are empty
 * main program contains 2 queues ready_queue and running_queue
 * file/directory structure outlined 
 * 
 * new process added by files in newProc folder (proctime, niceness)
 * 
 * each cycle of the simulator
 *  increment global time t
 *  read in information from the files in the folder creating new PID, status of READY, cputime 0
 *      status: 1=ready, 2=running, 3=complete
 *  information then stored by push onto ready queue
 *  once read in file can be removed 
 * when running is empty a new proccess is popped from ready and pushed to running
 * when proccess is done running (cputime>=proctime) 
 * each cycle the cputime should be incremented by a certain amount
 * 
 * a logfile should be kept containing all processes in both queues
 *********/

/*TODO
loop of removing and adding to running
updating log file 
increment global t
checking running to pop (cpu>proc)
increment cpu
.h for procSim???
*********/

/*DONE
made global t dont use it...
file structure 
file creation script
create queues
push entries to ready
file removed after pushed
*********/

int logFile(Node data){
    return 0;
    //i think here update the log file passign the popped data
    //change popped status to 3
    //idk if we are just adding the completed or if we add all further reading suggests add all?
}

int openDirTest(DataList *ready_queue){
    //maybe just open it each time so that if a new file appears it will be accounted for ??
    DIR *dirpointer;

    FILE *fp; 
    char *line = NULL;
    size_t len = 0;  
    int tmpNice, tmpProc;
    int empty = 0;

    struct dirent *directory;

    dirpointer = opendir("../newProc/");
    printf("files:\n");

    if(dirpointer){
        directory = readdir(dirpointer);//do it twice cause there will always be a current and parent file...
        directory = readdir(dirpointer);//if dont do these it just ends without pushing file
        if((directory = readdir(dirpointer)) != NULL){
        //while ((directory = readdir(dirpointer)) != NULL){
            //since it also looks at the current and parent 
            if(strcmp(directory->d_name, ".") && strcmp(directory->d_name, "..")){
                char filename[100] = "../newProc/";

                strcat(filename, directory->d_name);
                
                fp = fopen(filename, "r");
                if(fp == NULL){
                    fprintf(stderr, "file cannot be openned\n");
                    exit(EXIT_FAILURE);
                }
                //it is only one line long 
                getline(&line, &len, fp);

                printf("%s\n", directory->d_name);//prints can be removed later just here to test
                //printf("Contents: %s", line);
                //token = strtok(line, ", "); //idk what way is better?
                //tmpProc = atoi(token);
                tmpProc = atoi(strtok(line, ", "));//technically an int but seems fine... maybe do another cast?
                //printf("First token: %d\n", tmpProc);
                //token = strtok(NULL, "\n");
                //tmpNice = atoi(token);
                tmpNice = atoi(strtok(NULL, "\n"));
                //printf("Second token: %d\n", tmpNice);

                pushEntry(&ready_queue, ready_queue->size + 1, 1, tmpNice, 0, tmpProc);

                fclose(fp); //close file then remove 
                remove(filename);
            }
        }
        else {
            empty = 1;//nothing other than current and parent files, if file is added will change to 0 again on entry and return such
        }
        closedir(dirpointer);
    }
    if(line){
        free(line);
    }
    return empty;
}

int main(){
    int emptyDir = 0;//if the flag anything other than 0 the directory is empty

    Node readyPop, runningPop;
    DataList *ready_queue = createDataList();
    DataList *running_queue = createDataList();


    emptyDir = openDirTest(ready_queue);//reads all the files at once into ready idk if it is supposed to be done in the loop one at a time??
    emptyDir = openDirTest(ready_queue);
    printAllEntries(ready_queue);
    //pushEntry(&running_queue, 8, 1, 1, 0, 12);
    //pushEntry(&running_queue, 3, 1, 12, 0, 3);
    //printAllEntries(running_queue);


    readyPop = popEntry(ready_queue);
    pushEntry(&running_queue, readyPop.pid, 2, readyPop.niceness, readyPop.cpuTime, readyPop.procTime);
    
    while(ready_queue->size > 0 && running_queue->size > 0 && emptyDir == 0){
        //may need to adjust or have an initial call of open dir then pop ready and push to running 
        //otherwise will have to change the entry case
        
        emptyDir = openDirTest(ready_queue);
        //should continue to check if new file until both queues are empty
        runningPop = popEntry(running_queue);
        readyPop = popEntry(ready_queue);
        //maybe pass the ready pop to a function that will add to log file 
        pushEntry(&running_queue, readyPop.pid, 2, readyPop.niceness, readyPop.cpuTime, readyPop.procTime);
        printAllEntries(ready_queue);
        printAllEntries(running_queue);
        if(!emptyDir){
            printf("Directory not empty\n");
        }
        else{
            printf("Directory is empty\n");
        }
    }


    return 0;
}