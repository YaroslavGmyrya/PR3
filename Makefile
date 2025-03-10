all:
	$(MAKE) -C mySimpleComputer
	$(MAKE) -C console

clean:
	make clean -C console
	make clean -C mySimpleComputer

run:
	make run -C console