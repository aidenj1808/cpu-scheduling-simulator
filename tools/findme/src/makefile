findme: findme.c
	gcc -Wall -g -pthread findme.c -o findme

testGlob: testGlob.c
	set -o noglob
	gcc -Wall -g testGlob.c -o testGlob

clean:
	rm -f findme
	rm -f testGlob

tar:
	tar --exclude .git -czvf ../../lab10.tar.gz -C ../../ lab10
