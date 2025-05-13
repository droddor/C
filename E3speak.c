#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "fifoPractica"

void main(void){
    char s[300];
    int num,fd;

    mknod(FIFO_NAME, S_IFIFO | 0666,0);

    printf("Esperando lectores...\n");

    fd = open(FIFO_NAME,O_WRONLY);

    printf("Lector, escribe: \n");

    while(fgets(s,sizeof(s),stdin),!feof(stdin)){
        if((num = write(fd,s,strlen(s))) == -1)
            perror("write");
        else
            printf("speak: wrote %d bytes\n",num);
    }

}