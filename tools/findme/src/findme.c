#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <glob.h>
#include <stdbool.h>


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int thread_index = 0;

glob_t globStruct;

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
	int option;
	char *start_dir;
	int maxdepth; 
	char *user;
	char *filename;
} ThreadData;

typedef struct {
	int find_all_types;
	int find_with_max_depth;
	int find_everything;
	int find_with_user;
	bool find_with_glob;
} Cases;

void *find_file(void *arg){
	ThreadData *data = (ThreadData *)arg;
	return NULL;
}

char *getUserName(int uidNumber) {
	FILE *output;
	char *buffer = malloc(100);
	char command[100];

	char search[] = "ldapsearch -x -LLL";
	char grepCut[] = "| grep uid: | cut -d \":\" -f2 | cut -d \" \" -f2";
	snprintf(command, 100, "%s uidNumber=%d %s", search, uidNumber, grepCut);

	output = popen(command, "r");
	if (output == NULL) {
		printf("POPEN: failed to execute command.\n");
		exit(EXIT_FAILURE);
	}

	fgets(buffer, 100, output);
	buffer[strcspn(buffer, "\n")] = 0;
	pclose(output);
	return buffer;
}

int getUserIDNumber(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("getUserIDNumber: Cannot find file.\n");
		exit(EXIT_FAILURE);
	}
	int uid = buf.st_uid;
	return uid;
}

int isRegularFile(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("isRegularFile: Cannot find file: %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (S_ISREG(buf.st_mode)) {
		return 1;
	}
	return 0;
}

int isDirectory(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("isDirectory: Cannot find file: %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (S_ISDIR(buf.st_mode)) {
		return 1;
	}
	return 0;
}

int isSymbolicLink(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("isSymbolicLink: Cannot find file: %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (S_ISLNK(buf.st_mode)) {
		return 1;
	}
	return 0;
}

int isCharacterDevice(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("isCharacterDevice: Cannot find file: %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (S_ISCHR(buf.st_mode)) {
		return 1;
	}
	return 0;
}

int isBlockDevice(char *file_name) {
	int fs;
	struct stat buf;
	
	if ( (fs = stat(file_name, &buf) ) < 0 ) {
		printf("isBlockDevice: Cannot find file: %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	if (S_ISBLK(buf.st_mode)) {
		return 1;
	}
	return 0;
}

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

void printFilesOrGlob(char *directory, char *file_name, struct dirent *file, Cases cases, glob_t *globStruct) {
	if (cases.find_with_glob) {
		char current_path[500];
		snprintf(current_path, 500, "%s/%s", directory, file_name);
		if (globStruct->gl_pathc == 0) {
			glob(current_path, GLOB_ERR, NULL, &(*globStruct));
		} else {
			glob(current_path, GLOB_ERR | GLOB_APPEND, NULL, &(*globStruct));
		}
	} else {
		printf("%s/%s\n", directory, file->d_name);
	}
}

void printFiles(char *directory, Cases cases, char *file_type, char *user,
		char *file_name, int max_depth) {

	if (max_depth == 0) {
		return;
	}

	DIR *dp = opendir(directory);
	if (dp == NULL) {
		printf("printFiles: Could not open directory: %s\n", directory);
		exit(EXIT_FAILURE);
	}
	
	struct dirent *file;
	for (int i = 0; i < 2; i++) {file = readdir(dp);} //skips . & ..
	
	while ( (file = readdir(dp)) != NULL ) {
		char current_path[500];
		snprintf(current_path, 500, "%s/%s", directory, file->d_name);
		if (cases.find_everything) {
			printFilesOrGlob(directory, file_name, file, cases, &globStruct);
		} else if (cases.find_all_types && cases.find_with_max_depth && cases.find_with_user
				&& !strcmp(file->d_name, file_name)) {
			printFilesOrGlob(directory, file_name, file, cases, &globStruct);
		} else if (cases.find_all_types && cases.find_with_max_depth && !cases.find_with_user) {
			printFilesOrGlob(directory, file_name, file, cases, &globStruct);
		} else if (cases.find_all_types && !cases.find_with_max_depth && cases.find_with_user &&
				!strcmp(file->d_name, file_name)) {
			int userID = getUserIDNumber(current_path);
			char *username = getUserName(userID);
			if (!strcmp(user, username)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			free(username);
		} else if (cases.find_all_types && !cases.find_with_max_depth && !cases.find_with_user) {
			if (!strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
		} else if (!cases.find_all_types && cases.find_with_max_depth && cases.find_with_user &&
				!strcmp(file->d_name, file_name)) {
			int userID = getUserIDNumber(current_path);
			char *username = getUserName(userID);
			if (!strcmp(file_type, "f") && isRegularFile(current_path) 
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "d") && isDirectory(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "s") && isSymbolicLink(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "c") && isCharacterDevice(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "b") && isBlockDevice(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			free(username);
		} else if (!cases.find_all_types && cases.find_with_max_depth && !cases.find_with_user) {
			if (file_name == NULL) {
				if (!strcmp(file_type, "f") && isRegularFile(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "d") && isDirectory(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "s") && isSymbolicLink(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "c") && isCharacterDevice(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "b") && isBlockDevice(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
			}
			else if (!strcmp(file_type, "f") && isRegularFile(current_path) 
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "d") && isDirectory(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "s") && isSymbolicLink(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "c") && isCharacterDevice(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "b") && isBlockDevice(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
		} else if (!cases.find_all_types && !cases.find_with_max_depth && cases.find_with_user &&
				!strcmp(file->d_name, file_name)) {
			int userID = getUserIDNumber(current_path);
			char *username = getUserName(userID);
			if (!strcmp(file_type, "f") && isRegularFile(current_path) &&
					!strcmp(user, username)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "d") && isDirectory(current_path) &&
					!strcmp(user, username)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "s") && isSymbolicLink(current_path) &&
					!strcmp(user, username)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "c") && isCharacterDevice(current_path) &&
					!strcmp(user, username)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "b") && isBlockDevice(current_path) &&
					!strcmp(user, username)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			free(username);
		} else if (!cases.find_all_types && !cases.find_with_max_depth && !cases.find_with_user) {
			if (file_name == NULL) {
				if (!strcmp(file_type, "f") && isRegularFile(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "d") && isDirectory(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "s") && isSymbolicLink(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "c") && isCharacterDevice(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
				else if (!strcmp(file_type, "b") && isBlockDevice(current_path)) {
					printFilesOrGlob(directory, file_name, file, cases, &globStruct);
				}
			}
			else if (!strcmp(file_type, "f") && isRegularFile(current_path) 
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "d") && isDirectory(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "s") && isSymbolicLink(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "c") && isCharacterDevice(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
			else if (!strcmp(file_type, "b") && isBlockDevice(current_path)
					&& !strcmp(file->d_name, file_name)) {
				printFilesOrGlob(directory, file_name, file, cases, &globStruct);
			}
		}

		if (isDirectory(current_path)) {
			char path[500] = {0};
			strcat(path, directory);
			strcat(path, "/");
			strcat(path, file->d_name);
			if (cases.find_with_glob) {
				printFilesOrGlob(path, file_name, file, cases, &globStruct);
			}
			printFiles(path, cases, file_type, user, file_name, max_depth - 1);
		}
	}
	closedir(dp);
}

int main(int argc, char *argv[]) {
	int nprocs = get_nprocs();
	pthread_t tid[nprocs];
	int find_all_types = 1;
	int find_with_max_depth = 0;
	int find_with_user = 0;
	int find_everything = 0;
	
	char *start_file_path = argv[1]; // must be a directory
	isDirectoryCheck(start_file_path);

	int option;
	char *type;
	char *file_name = NULL;
	char *user;
	long int max_depth = -1;
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
	

	if (find_with_user && file_name == NULL) {
		printf("When using --user filter please also use --name filter\n");
		printf("Sorry about that one :(\n");
		exit(EXIT_FAILURE);
	}


	ThreadData data;
	data.start_dir = start_file_path;
	data.maxdepth = max_depth;
	data.user = user;
	data.filename = file_name;

	Cases cases;
	cases.find_all_types = find_all_types;
	cases.find_with_max_depth = find_with_max_depth;
	cases.find_with_user = find_with_user;

	if (argc == 2) {
		cases.find_everything = 1;
		cases.find_with_glob = false;
	} else {
		cases.find_everything = 0;
	}

	if (file_name) {
		cases.find_with_glob = strpbrk(file_name, "*?[]^") != NULL;
	}

	printFiles(start_file_path, cases, type, user, file_name, max_depth);
	
	char **foundFiles = globStruct.gl_pathv;
	if (cases.find_with_glob && globStruct.gl_pathc != 0) {
		while (*foundFiles) {
			printf("%s\n", *foundFiles);
			foundFiles++;
		}
	}

	return 0;
}
