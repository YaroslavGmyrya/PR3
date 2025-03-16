all:
	gcc -c myTerm.c -o myTerm.o

	gcc -c mt_variables.c -o var.o
	
	ar rcs libmyTermLib.a myTerm.o var.o

clean:
	rm -f *.o *.a