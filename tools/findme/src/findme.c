#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

bool found = false;

int option_index = 0;
char *short_options = "t:n:u:m:";
static struct option long_options[] = {
	{"type", required_argument, NULL, 't'},
	{"name", required_argument, NULL, 'n'},
	{"user", required_argument, NULL, 'u'},
	{"maxdepth", required_argument, NULL, 'm'},
	{NULL, 0, NULL, 0}
};

//data for threads
typedef struct{
	int find_all_types;
	int find_with_user;

	int option;
	char *start_dir;
	char *type; 
	int maxdepth; 
	long int curr_depth;
	char *user;
	char *filename;
}ThreadData;

//TODO --maybe , found flag as ptr, but the have to deal with race cond and all other stuff...

//used to check flags, if no other flags set its a match return 1
//else send to other helper functions for flags if ever not a match returns 0
int verify(ThreadData *data, char *curr_path){
	//ThreadData *data = (ThreadData *)arg;
	printf("flags--> user: %d type: %d\n",data->find_with_user, data->find_all_types);//TODO erase

	//check if flags not set
	//dont need to check max depth that is checked in prev func
	if(data->find_with_user == 0 && data->find_all_types == 1){
		printf("No other flags have been set, it is a match at %s\n", curr_path);//TODO erase
		return 1;
	}
	//TODO if user flag
	//TODO if type flag
	return 0;
}

//threads passed to function?
void *search_dir(void *arg){
	ThreadData *data = (ThreadData *)arg;
	printf("\nStarting search of %s\n", data->start_dir); //TODO erase
	DIR *dirptr = opendir(data->start_dir);
	if(dirptr == NULL){
		printf("Error opening directory\n");
		exit(EXIT_FAILURE);
	}

	struct dirent *dp;
	char *self = ".";
	char *par = "..";

	while((dp = readdir(dirptr)) != NULL){
		//skip over the self and parent ?? if always first two could just skip?
		//is there a case they arent the first two opened...
		//for (int i = 0; i < 2; i++) {dp = readdir(dirptr);} //skips . & .. 
		//dp = readdir(dirptr);

		while((strcmp(dp->d_name, self) == 0 )|| (strcmp(dp->d_name, par) == 0)){
			dp = readdir(dirptr);
		}
		
		//if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
			//continue;
		//} 
		if (!dp){
			//if end of dir and no match return
			closedir(dirptr);
			return NULL;//TODO return NULL to show nothing found??
		}
		char *curr_path = malloc(strlen(data->start_dir) + strlen("/") + strlen(dp->d_name) + 1);
		if(curr_path == NULL){
			printf("Error allocating memory for pathname\n");
			exit(EXIT_FAILURE);
		}	

		sprintf(curr_path, "%s/%s", data->start_dir, dp->d_name); //becomes path to the curr file //TODO or should be in new entry?
		
		printf("Current file: %s, full path: %s\n", dp->d_name, curr_path);//TODO erase

		//if directory, start a new thread to search in directory
		if(dp->d_type == DT_DIR){
			//first check if it matches the filename? before opening it, if it is a hit go to verify
			if(strcmp(data->filename, dp->d_name) == 0){
				//name of dir is filename match
				printf("Found at %s now to verify\n", curr_path);//TODO erase
				int ver = verify(data, curr_path); 
				if(ver){
					//TODO set gloab var to notify other threads
					printf("Verify success!\n");//TODO erase
					//TODO return curr_path?? add to struct??
					break;
				}
				//TODO or maybe do verify if yes save path in struct and break else go to next file?
				printf("Verify Fail\n");//TODO erase
			}
			//if maxdepth flag not set or max not reached go into the dir
			else if(data->maxdepth == -1 || data->maxdepth > data->curr_depth){//not a match and create  new thread to search new dir
			printf("Current depth: %ld, Max depth: %d\n", data->curr_depth, data->maxdepth);//TODO erase
			printf("going into a new dir!\n\n");//TODO erase
			//TODO implement max threads??? have them wait if at max to not use too much resources
				ThreadData newthread;
				newthread.find_all_types = data->find_all_types;
				newthread.find_with_user = data->find_with_user;
				newthread.start_dir = curr_path;
				newthread.type = data->type;
				newthread.maxdepth = data->maxdepth; 
				newthread.user = data->user;
				newthread.filename = data->filename;
				newthread.curr_depth = data->curr_depth + 1;

				/********
				pthread_t tid; 
				pthread_create(&tid, NULL, search_dir, (void *)&newthread);
				pthread_join(tid, NULL);
				********///with threads

				//testing without thread call
				search_dir((void *)&newthread);
			}
			else if(data->maxdepth == data->curr_depth){
				//TODO dont go into the dir.. so do nothing? could erase this check?
				printf("Max depth reached! \n");//TODO erase
			}

		}
		else{
			if(strcmp(dp->d_name, data->filename) == 0){
				//TODO filename match go to verify...
				printf("Found at %s now to verify\n", curr_path);//TODO erase
				int ver = verify(data, curr_path); 
				if(ver){
					//TODO set glob var to notify other threads
					printf("Verify success!\n");//TODO erase
					//TODO return curr_path?? add to struct??
					break;
				}
				//TODO or maybe do verify if yes save path in struct and break else go to next file?
				printf("Verify Fail\n");//TODO erase
			}
		}
		free(curr_path); //make sure free
		
	}
	//TODO need to free the pathname and close the dir
	closedir(dirptr);
	return NULL;
}


int main(int argc, char *argv[]) {
	//TODO usage message case??

	int find_all_types = 1;
	int find_with_max_depth = 0;
	int find_with_user = 0;
	
	int option;
	char *start_file_path = argv[1]; // must be a directory
	char *type;
	char *file_name;
	char *user;
	long int max_depth = -1; //TODO changed to default to -1 for comparisons (if ==-1 or max>curr)
	long int curr_depth;
	while ( (option = getopt_long(argc, argv, short_options, 
								  long_options, &option_index)) != -1) {
		switch (option) {
			case 't':
				find_all_types = 0;
				type = optarg;
				break;

			case 'n':
				file_name = optarg;
				break;

			case 'u':
				find_with_user = 1;
				user = optarg;
				break;

			case 'm':
				find_with_max_depth = 1;
				char *ptr;
				max_depth = strtol(optarg, &ptr, 10);
				break;
		}
	}
	printf("Starting file path: %s\n", start_file_path);
	printf("find all types? %d\n", find_all_types);
	printf("type: %s\n", type);
	printf("file name to find: %s\n", file_name);
	printf("find with max depth? %d\n", find_with_max_depth);
	printf("max depth: %ld\n", max_depth);
	printf("find with user? %d\n", find_with_user);
	printf("user: %s\n", user);

	//TODO to do how many threads created? start with one for each dir? to a max #??
	ThreadData data;
	data.find_all_types = find_all_types;
	data.find_with_user = find_with_user;
	data.start_dir = start_file_path;
	data.type = type;
	data.maxdepth = max_depth; 
	data.user = user;
	data.filename = file_name;
	data.curr_depth = 0;

	//TODO maybe global flag to tell threads if file found, maybe if name found other threads go to wait and if match threads will break ??
	/************
	//pthread_t tid;
	//pthread_create(&tid, NULL, search_dir, (void *)&data);
	//pthread_join(tid, NULL);
	*********///commented these out for now testing without thread...

	//testing without thread call
	search_dir((void *)&data);

	//TODO need to remember to close dir!!


	if (find_all_types && find_with_max_depth && find_with_user) {
		;
	} else if (find_all_types && find_with_max_depth && !find_with_user) {
		;
	} else if (find_all_types && !find_with_max_depth && find_with_user) {
		;
	} else if (find_all_types && !find_with_max_depth && !find_with_user) {
		;
	} else if (!find_all_types && find_with_max_depth && find_with_user) {
		;
	} else if (!find_all_types && find_with_max_depth && !find_with_user) {
		;
	} else if (!find_all_types && !find_with_max_depth && find_with_user) {
		;
	} else if (!find_all_types && !find_with_max_depth && !find_with_user) {
		;
	}

	return 0;
}
