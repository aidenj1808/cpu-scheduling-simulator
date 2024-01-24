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
do we need to have a check to see if new files added to proc directory idk...
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
push entries to ready??? do we do it all at once or like one at a time?
file removed after pushed
added "type" to struct and changed pushentry function to check if it is type 1 not allow more than 1
*********/

int logFile(Node data){
    ;
    //i think here update the log file passign the popped data
    //change popped status to 3
    //idk if we are just adding the completed or if we add all further reading suggests add all?
}

int openDirTest(DataList *ready_queue){
    DIR *dirp;
    FILE *fp; 
    char *line = NULL;
    size_t len = 0;
    //char *token;  
    int tmpNice;
    int tmpProc;


    struct dirent *dir;

    dirp = opendir("../newProc/"); //pathname??? yes...
    //printf("files:\n");
    if(dirp){
        while ((dir = readdir(dirp)) != NULL){
            //since it also looks at the current and parent 
            if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")){
                char filename[100] = "../newProc/";

                strcat(filename, dir->d_name);
                
                fp = fopen(filename, "r");
                if(fp == NULL){
                    fprintf(stderr, "file cannot be openned\n");
                    exit(EXIT_FAILURE);
                }
                //it is only one line long 
                getline(&line, &len, fp);

                //printf("%s\n", dir->d_name);//prints can be removed later just here to test
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
        closedir(dirp);
    }

    free(line);
    return 0;
}

int main(){
    Node readyPop, runningPop;
    DataList *ready_queue = createDataList();
    DataList *running_queue = createDataList();//maybe we can add type? to struct??
    running_queue->type = 1;

    openDirTest(ready_queue);//reads all the files at once into ready idk if it is supposed to be done in the loop one at a time??

    printAllEntries(ready_queue);
    //pushEntry(&running_queue, 8, 1, 1, 0, 12);
    //pushEntry(&running_queue, 3, 1, 12, 0, 3);
    //printAllEntries(running_queue);


    //just testing stuff below will need to check times and all that
        //maybe could do a while using ready->size??
    readyPop = popEntry(ready_queue);
    pushEntry(&running_queue, readyPop.pid, 2, readyPop.niceness, readyPop.cpuTime, readyPop.procTime);
    //while ready && running not empty?
    while(ready_queue->size > 0 && running_queue->size > 0){
        //will need to do an if check for cputime just testing this atm
        runningPop = popEntry(running_queue);
        readyPop = popEntry(ready_queue);
        //maybe pass the ready pop to a function that will add to log file 
        pushEntry(&running_queue, readyPop.pid, 2, readyPop.niceness, readyPop.cpuTime, readyPop.procTime);
        printAllEntries(ready_queue);
        printAllEntries(running_queue);
    }


    return 0;
}