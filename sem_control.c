#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

/*
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
}
*/

int create_semaphore() {
  int semid;
  int key = ftok("makefile", 5);
  int sc;
  
  semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
      
    if (semid >= 0) {

      printf("semaphore created %d\n", semid);
      union semun su;
      su.val = -1;

      sc = semctl(semid, 0, SETVAL, su);
      printf("value set: %d\n", sc);
    } else {
      printf("semaphore already created/n");
    }

    return semid;
}
    

int remove_semaphore() {
  int semid;
  int key = ftok("makefile", 5);
  int sc;

  semid = semget(key, 1, 0);

  //remove sempahore
  sc = semctl(semid, 0, IPC_RMID);
  printf("semaphore removed: %d\n", sc);

  return semid;
}

int create_shmem() {
  int shd;
  
  
  return 0;
}

int remove_shmem() {
  return 0;
}

int main(int argc, char *argv[]) {  
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
    create_semaphore();  
  } 

  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    remove_semaphore();
  }
  
}
