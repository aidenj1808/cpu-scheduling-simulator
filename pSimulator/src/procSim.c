#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "dStruct.h"

int openDirTest(){
    DIR *dirp;

    struct dirent *dir;
    dirp = opendir("../newProc/"); //pathname??? yes...
    if(dirp){
        while ((dir = readdir(dirp)) != NULL){
            printf("%s\n", dir->d_name);
        }
        closedir(dirp);
    }

    return 0;
}

int main(){
    openDirTest();
    return 0;
}