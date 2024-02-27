#include <stdio.h>
#include <stdlib.h>


// Program can be run after changing the pre-processor algorithm definition
// in procSim.c to create the graph thats needed.
int main(void) {
	printf("Creating Graph...\n");
	system("./files2.sh");
	system("make procSim");
	system("make plotData");
	system("./procSim");
	system("./plotData");
	system("mv ../log/*.png testGraphs");
	system("rm -f ../log/log*");
	system("rm -f ../log/*.png");
	system("rm procSim");
	system("rm plotData");
	system("touch ../log/.gitkeep");
	system("touch ../newProc/.gitkeep");
}
