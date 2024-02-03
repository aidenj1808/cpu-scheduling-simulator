#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <cairo/cairo.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 400
#define MAXWIDTH 700

//LOG FILE FORMAT
//log time, pid, status, niceness, cputime, proctime

float setScale(int length, float *data){
	double totalTime;
	float scale = 1.0; 

	for(int i = 1; i < length; i = i + 2){
		totalTime += data[i] * 20;
	}

	if(totalTime > MAXWIDTH){
		scale = MAXWIDTH / totalTime;
	}
	return scale;
}

float *getGraphData(int fileCount) {
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

int plotData(int length, float *data){
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
  float scale = setScale(length, data); 
  //loop through to create the squares
  for (int i = 0; i < length; i = i+2) {
    double width = data[i+1] * 20 * scale;

    float r = randCol();
    float g = randCol();
    float b = randCol();

    cairo_rectangle(cr, x, y, width, 50);
    //given colour based off of pid since pid unique
    cairo_set_source_rgb(cr, r, g, b);
    cairo_fill(cr);

    //write pid label
    char pidLabel[15];
    sprintf(pidLabel, "%.f", data[i]);
    cairo_set_font_size(cr, 22);
    cairo_move_to(cr, x + (width * 0.3), y + 25); 
    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_show_text(cr, pidLabel);

    x += width; //for starting position of next 
    label = x;    
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
	int fileCount = getFileCount("../log/");
	float *values = getGraphData(fileCount);
    plotData(fileCount * 2, values);
	free(values);
	return 0;
}
