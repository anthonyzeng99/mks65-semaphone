all:
	gcc sem_control.c -o sem_control
	gcc sem_write.c -o sem_write

run: sem_control
	./sem_control.c

clean:
	rm *~

