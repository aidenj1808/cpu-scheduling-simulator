#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>


int option_index = 0;
char *short_options = "t:n:m:u:";
static struct option long_options[] = {
	{"type", optional_argument, NULL, 't'},
	{"name", required_argument, NULL, 'n'},
	{"maxdepth", optional_argument, NULL, 'm'},
	{"user", optional_argument, NULL, 'u'},
	{NULL, 0, NULL, 0}
};

//data for threads
typedef struct{
	int option;
	char *start_dir;
	char type; 
	int maxdepth; 
	char *user;
	char *filename;
}ThreadData;

//threads passed to function
void *find_file(void *arg){
	ThreadData *data = (ThreadData *)arg;
	//do the logic...
	return NULL;
}

int main(int argc, char *argv[]) {
	//usage message if not all provided
	//>>./findme [start_dir] [type] [maxdepth] [user] [filename]
	if(argc != 6){//TODO do we need all options?//do you need this with getopt??
		//not sure the format your doing tbh
		printf("Usage: ./findme [start_dir] [type] [maxdepth] [user] [filename]\n");
		exit(EXIT_FAILURE);
	}

	int find_all_types = 1;
	int find_with_max_depth = 0;
	int find_with_user = 0;
	
	int option;
	char type;
	char *start_file_path = argv[1]; // must be a directory
	char *file_name;
	int max_depth;
	char *user;
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

			case 'm':
				find_with_max_depth = 1;
				max_depth = optarg;
				//char *ptr = NULL;
				//max_depth = strtol(optarg, &ptr, 10);
				//max_depth = atoi(optarg);
				break;

			case 'u':
				find_with_user = 1;
				user = optarg;
				break;
		}
	}
	printf("Starting file path: %s\n", start_file_path);
	printf("find all types? %d\n", find_all_types);
	printf("type: %c\n", type);
	printf("file name to find: %s\n", file_name);
	printf("find with max depth? %d\n", find_with_max_depth);
	printf("max depth: %d\n", max_depth);
	printf("find with user? %d\n", find_with_user);
	printf("user: %s\n", user);

	//not fully sure what you have in progress so just starting thread stuff..
	//to do how many threads created?
	ThreadData data;
	data.start_dir = start_file_path;
	data.type = type;
	data.maxdepth = max_depth;
	data.user = user;
	data.filename = file_name;
	//TODO just creates one thread could do a loop to create more at a max
	pthread_t tid;
	pthread_create(&tid, NULL, find_file, (void *)&data);

	//if do multiple will need to use the tid arr may need to switch up struct
	pthread_join(tid, NULL);


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
