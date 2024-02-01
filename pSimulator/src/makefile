
CFLAGS= -Wall -std=c11 
CC= gcc 

proc: files procSim	
	./procSim

procSim: procSim.o dStruct.o plotData.o
	$(CC) $(CFLAGS) procSim.o dStruct.o plotData.o -o procSim -lcairo

dStruct.o: dStruct.c dStruct.h
	$(CC) $(CFLAGS) -g -c dStruct.c 

procSim.o: procSim.c dStruct.h plotData.h
	$(CC) $(CFLAGS) -g -c procSim.c 

plotData.o: plotData.c plotData.h
	$(CC) $(CFLAGS) -g -c plotData.c

files: files.sh
	chmod +x files.sh
	./files.sh

valgrindTest: procSim
	valgrind -s --leak-check=yes ./procSim

clean:
	rm -f *.o *~ *.txt
	rm ../log/log*
	rm procSim 
	touch ../log/.gitkeep
testing: test #if we want a file to diff good.txt 
	./procSim > testOut.txt 

tar:
	tar --exclude .git -czvf ../lab02.tar.gz -C ../ lab02
