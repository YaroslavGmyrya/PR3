all: console.out main.out

console.out: console.c
	gcc -o console.out console.c

main.out: main.c
	gcc -o main.out main.c -L../mySimpleComputer/ -lmylib

clean:
	rm -f console.out main.out

run: all
	./console.out
	./main.out

.PHONY: all clean run