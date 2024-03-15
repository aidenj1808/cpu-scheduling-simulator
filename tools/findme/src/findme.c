#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


int option_index = 0;
char *short_options = "t:n:m:u:";
static struct option long_options[] = {
	{"type", optional_argument, NULL, 't'},
	{"name", required_argument, NULL, 'n'},
	{"maxdepth", optional_argument, NULL, 'm'},
	{"user", optional_argument, NULL, 'u'},
	{NULL, 0, NULL, 0}
};

int main(int argc, char *argv[]) {
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
