CFLAGS= -Wall -g
CC= gcc 

test: procSim plotData
	./files2.sh
	./procSim
	#./plotData

test2: procSim 
	./files2.sh

procSim: procSim.o dStruct.o
	$(CC) $(CFLAGS) procSim.o dStruct.o -o procSim

plotData: plotData.c
	$(CC) $(CFLAGS) -lcairo plotData.c -o plotData

dStruct.o: dStruct.c dStruct.h
	$(CC) $(CFLAGS) -c dStruct.c 

procSim.o: procSim.c dStruct.h plotData.h
	$(CC) $(CFLAGS) -lm -c procSim.c 

plotData.o: plotData.c plotData.h
	$(CC) $(CFLAGS) -c plotData.c

valgrindTestProcSim: procSim
	valgrind -s --leak-check=full --show-leak-kinds=all ./procSim

# Memory leaks are outputed out of the hands of the user using Cairo in clearing
# up. We talked about this not being a problem.
valgrindTestPlotData: plotData
	valgrind -s --leak-check=full --show-leak-kinds=all ./plotData

clean:
	rm -f *.o *~ *.txt
	rm -f ../newProc/*.txt
	rm -f ../log/log*
	rm -f procSim
	rm -f plotData
	rm -f ../log/atest.png
	rm -f ../log/RRtest.png
	touch ../log/.gitkeep
	touch ../newProc/.gitkeep
testing: test #if we want a file to diff good.txt 
	./procSim > testOut.txt 

tar:
	tar --exclude .git -czvf ../../../lab06.tar.gz -C ../../../ lab06
