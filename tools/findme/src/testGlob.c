#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <glob.h>


int option_index = 0;
char *short_options = "t:n:u:m:";
static struct option long_options[] = {
	{"type", required_argument, NULL, 't'},
	{"name", required_argument, NULL, 'n'},
	{"user", required_argument, NULL, 'u'},
	{"maxdepth", required_argument, NULL, 'm'},
	{NULL, 0, NULL, 0}
};

void isDirectoryCheck(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("Given starting file path is not a directory: %s\n", file_name);
		printf("Usage: ./findme [starting directory]");
		printf(" [--type or -t]");
		printf(" [--user or -u]");
		printf(" [--name or -n]");
		printf(" [--maxdepth or -m]\n");
		exit(EXIT_FAILURE);
	}

	if (!S_ISDIR(buf.st_mode)) {
		printf("Given starting file path is not a directory: %s\n", file_name);
		printf("Usage: ./findme [starting directory]");
		printf(" [--type or -t]");
		printf(" [--user or -u]");
		printf(" [--name or -n]");
		printf(" [--maxdepth or -m]\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv) {
	int option;
	char *type;
	char *file_name = NULL;
	char *user;
	long int max_depth = -1;

	int find_all_types = 1;
	int find_with_max_depth = 0;
	int find_with_user = 0;
	int find_everything = 0;

	char *start_file_path = argv[1]; // must be a directory
	isDirectoryCheck(start_file_path);

	while ( (option = getopt_long(argc, argv, short_options, 
								  long_options, &option_index)) != -1) {
		switch (option) {
			case 't':
				find_all_types = 0;
				type = optarg;
				break;

			case 'n':
				file_name = optarg;
				system("set +o noglob");
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
	
	if (argc == 2) {
		find_everything = 1;
	}

	if (find_with_user && file_name == NULL) {
		printf("When using --user filter please also use --name filter\n");
		printf("Sorry about that one :(\n");
		exit(EXIT_FAILURE);
	}
	glob_t globStruct;
	int result;
	char **foundFiles;
	
	printf("file_name: %s\n", file_name);
	result = glob(file_name, GLOB_ERR, NULL, &globStruct);

	foundFiles = globStruct.gl_pathv;
	/*
	if (result != 0) {
		printf("glob didn't work\n");
		exit(EXIT_FAILURE);
	} else {
		while(*foundFiles) {
			printf("%s\n", *foundFiles);
			foundFiles++;
		}
	}
	*/
	while(*foundFiles) {
		printf("%s\n", *foundFiles);
		foundFiles++;
	}

	return 0;
}
