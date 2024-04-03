#include <stdio.h>
#include <stdlib.h>

//test program to test glob functionality

int main() {
    //"t:n:u:m:" 

    printf("Looking for file *.c\n");
    system("./findme . --name \"*.c\"");

    printf("Looking for file [rb]at.c\n");
    system("./findme . --name \"[rb]at.c\"");

    printf("Looking for file ?at.c\n");
    system("./findme . --name \"?at.c\"");

    printf("Looking for file *.txt\n");
    system("./findme . --name \"*.txt\"");

    printf("Looking for file *2.c\n");
    system("./findme . --name \"*2.c\"");

    printf("Looking for file [^t]at.*\n");
    system("./findme . --name \"[^t]at.*\"");

    printf("Looking for file *.?og\n");
    system("./findme . --name \"*.?og\"");

    printf("Looking for file ?at.c\n");
    system("./findme . --name \"?at.c\"");

    printf("Looking for file *2.?\n");
    system("./findme . --name \"*2.?\"");

    printf("Looking for file *2.*\n");
    system("./findme . --name \"*2.*\"");

    printf("Looking for file test.*\n");
    system("./findme . --name \"test.*\"");

    printf("Looking for file [c]??.?\n");
    system("./findme . --name \"[c]??.?\"");
 

    return 0;
}
