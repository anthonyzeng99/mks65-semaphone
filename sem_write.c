#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main() {
    int semid = semget(ftok("Makefile", 50), 1, 0644);
    int shmemid = shmget(ftok("Makefile", 70), 1024, 0644);
    int *shmem_ptr = shmat(shmemid, 0, 0);
    int file = open("file.txt", O_RDONLY | 0644);
    
    struct sembuf sb; 
    sb.sem_op = -1; 
    sb.sem_num = 0; 
    sb.sem_flg = SEM_UNDO; 
    
    //down
    semop(semid, &sb, 1);
    
    //read previous line
    char old_line[*shmem_ptr + 1];
    lseek(file, -1 * (*shmem_ptr), SEEK_END);
    read(file, old_line, *shmem_ptr);
    old_line[*shmem_ptr + 1] = 0;
    
    close(file);
    
    if (*shmem_ptr){
        printf("Previous writer wrote:\n%s\n", old_line);
        printf("What would you like to add on?\n");
    }
    else
        printf("There was no previous writer. What would you like to write?\n");
        
    file = open("file.txt", O_RDWR | O_APPEND | 0644);
      
    //write new line
    char new_line[100];
    fgets(new_line, 100, stdin);
    write(file, new_line, strlen(new_line));
    *shmem_ptr = strlen(new_line);
    
    close(file);
    shmdt(shmem_ptr);
    
    //up
    sb.sem_op = 1;
    semop(semid, &sb, 1);
    
    return 0;
}
