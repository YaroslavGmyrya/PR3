all:
	make -C mySimpleComputer
	make -C myTerm

	gcc ./myTerm/console.c -o main.out -L./myTerm -L./mySimpleComputer -lmyTermLib -lmySimpleLib -lm

clean:
	rm -f *.a
	make clean -C mySimpleComputer
	make clean -C myTerm

run:
	./main.out