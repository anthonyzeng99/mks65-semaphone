#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>


union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};


void create_semaphore() {
  int semid;
  int key = ftok("Makefile", 50);
  int sc;
  
  semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
      
    if (semid >= 0) { 

      printf("semaphore created %d\n", semid);
      union semun su;
      su.val = 1;

      sc = semctl(semid, 0, SETVAL, su);
      printf("value set: %d\n", sc);
    } else {
      printf("semaphore already created\n");
    }
}
    

void remove_semaphore() {
  int semid;
  int key = ftok("Makefile", 50);
  int sc;

  semid = semget(key, 1, 0);

  //remove sempahore
  sc = semctl(semid, 0, IPC_RMID);
  if (sc >= 0)
    printf("semaphore successfully removed\n");
  else
    printf("failed to remove semaphore\n");
}

void create_shmem() {
  int key = ftok("Makefile",60);
  int shmemid = shmget(key, 1024, IPC_CREAT | IPC_EXCL);
  
  printf("key: %d\n", key);
  printf("shmemid : %d\n", shmemid);
  
  if (shmemid >= 0)
    printf("shared memory created %d\n", shmemid);
  else
    printf("shared memory already exists: %s\n", strerror(errno));
}

void remove_shmem() {
  int sd;
  int key = ftok("Makefile",60);
  
  sd = shmctl(key, IPC_RMID, 0);
  
  if (sd >= 0)
    printf("shared memory successfully removed\n");
  else
    printf("failed to remove shared memory\n");
}

void create_file(){
  int file = open("file.txt", O_CREAT | O_EXCL | O_TRUNC, 0644);
  
  if (file >= 0)
    printf("file successfully created\n");
  else
    printf("file already exists\n");
    
  close(file);
}

int main(int argc, char *argv[]) {  
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
    create_semaphore();  
    create_shmem();
    create_file();
  } 

  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    remove_semaphore();
    remove_shmem();
  }
  
}
