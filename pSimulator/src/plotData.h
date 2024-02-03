#ifndef PLOTDATA_H
#define PLOTDATA_H

int plotData(int length, float *data);

float randCol(float pid);

float *getGraphData(int fileCount); 

int getFileCount(char *path);

float setScale(int length, float *data);

#endif /* PLOTDATA_H */