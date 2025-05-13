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
    int num, fd;

    mknod(FIFO_NAME,S_IFIFO | 0666,0);

    printf("Esperando escritores...\n");

    fd = open(FIFO_NAME, O_RDONLY);

    printf("Tenemos escritor: \n");

    do{
        if((num = read(fd,s,300)) == -1)
            perror("read");
        
        else{
            s[num-1] = '\0';
            printf("tick : read %d bytes: \"%s\"\n",num,s);
        }
    }while(num > 0);
}