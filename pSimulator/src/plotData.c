
#include <stdio.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 400

//LOG FILE FORMAT
//log time, pid, status, niceness, cputime, proctime

float randCol(float pid){
  srand((unsigned int)pid);
  return (float)rand() / RAND_MAX;
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
  
  //loop through to create the squares
  for (int i = 0; i < length; i = i+2) {
    double width = data[i+1] * 20;

    float r = randCol(data[i]) + 0.2;
    float g = randCol(data[i]) + 0.1;
    float b = randCol(data[i]) + 0.3;

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
  strftime(date, sizeof(date), "%m-%d-%Y", tm_info);
  char filename[50];
  snprintf(filename, sizeof(filename), "../log/log-%s.png", date);


  // save the image  
  cairo_surface_write_to_png(surface, filename);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  return 0;
}

