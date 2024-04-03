testGlob: testGlob.c
	set -o noglob
	gcc -Wall -g testGlob.c -o testGlob
findme: findme.c
	gcc -Wall -g -pthread findme.c -o findme

clean:
	rm -f findme
	rm -f testGlob
