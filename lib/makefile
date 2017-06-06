all:tst.o libadd.so
	gcc tst.o  -L ./ -ladd -lsub  -o a.out

tst.o:tst.c 
	gcc -c tst.c -o tst.o

libadd.so:add.c  libsub.so
	gcc -c -fPIC add.c -o add.o
	gcc -shared add.o  -o libadd.so

libsub.so:sub.c
	gcc -c -fPIC sub.c -o sub.o
	gcc -shared sub.o -o libsub.so

clean:
	rm *.o *.so a.out
