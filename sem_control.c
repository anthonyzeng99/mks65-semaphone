#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};


void create_semaphore() {
  int sc;
  int key = ftok("Makefile", 50);
  int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
      
    if (semid >= 0) { 

      printf("semaphore created %d\n", semid);
      union semun su;
      su.val = 1;

      sc = semctl(semid, 0, SETVAL, su);
      //printf("value set: %d\n", sc);
    } else {
      printf("semaphore already created: %s\n", strerror(errno));
    }
}
    

void remove_semaphore() {
  int sc;
  int key = ftok("Makefile", 50);
  int semid = semget(key, 1, 0);

  //remove sempahore
  sc = semctl(semid, 0, IPC_RMID);
  if (sc >= 0)
    printf("semaphore successfully removed\n");
  else
    printf("failed to remove semaphore: %s\n", strerror(errno));
}

void create_shmem() {
  int key = ftok("Makefile",70);
  int shmemid = shmget(key, 1024, IPC_CREAT | IPC_EXCL | 0644);
  
  //printf("key: %d\n", key);
  //printf("shmemid : %d\n", shmemid);
  
  if (shmemid >= 0)
    printf("shared memory created %d\n", shmemid);
  else
    printf("shared memory already exists: %s\n", strerror(errno));
}

void remove_shmem() {
  int sd;
  int key = ftok("Makefile",70);
  int shmemid = shmget(key, 1024, 0644);
  
  sd = shmctl(shmemid, IPC_RMID, 0);
  
  if (sd >= 0)
    printf("shared memory successfully removed\n");
  else
    printf("failed to remove shared memory: %s\n", strerror(errno));
}

void create_file(){
  int file = open("file.txt", O_CREAT | O_TRUNC, 0644);
  
  if (file >= 0)
    printf("file successfully created\n");
  else
    printf("file already exists: %s\n", strerror(errno));
    
  close(file);
}

void view_file(){
  int file = open("file.txt", O_RDONLY, 0644);
  
  struct stat info;
  stat("file.txt", &info);
  
  char story[info.st_size];
  read(file, info, info.st_size);
  
  printf("STORY\n%s\n", story);
  
  close(file);
  
}

int main(int argc, char *argv[]) {  
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
    create_semaphore();  
    create_shmem();
    create_file();
  } 
  
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0) {
    view_file();
  }

  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    view_file();
    remove_semaphore();
    remove_shmem();
  }
  
  return 0;
}
