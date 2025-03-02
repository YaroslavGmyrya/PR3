all: console.out pr2.out
	./console/build/console.out
	./mySimpleComputer/pr2.out

console.out:
	$(MAKE) -C console

pr2.out:
	$(MAKE) -C mySimpleComputer