testprog: test.c findme.c
	make findme
	gcc -Wall -g test.c -o testprog

findme: findme.c
	gcc -Wall -g -pthread findme.c -o findme

testGlob: testGlob.c
	set -o noglob
	gcc -Wall -g testGlob.c -o testGlob

clean:
	rm -f findme
	rm -f testGlob
	rm -f testprog

tar:
	tar --exclude .git -czvf ../../lab10.tar.gz -C ../../ lab10
