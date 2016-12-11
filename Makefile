all:
	gcc sem_control.c -o sem_control
	gcc sem_write.c -o sem_write

clean:
	rm sem_control
	rm sem_write
	rm file.txt

