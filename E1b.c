#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * read_char(void *);
void * check_hit(void *); 

int filedes[2];

int main(void) {

	int i;
	pthread_t tid1, tid2;
	
	pipe(filedes);

	/* Create thread for reading characters. */
	i = pthread_create(&tid1, NULL, read_char, NULL);

	/* Create thread for checking hitting of any keyboard key. */
	i = pthread_create(&tid2, NULL, check_hit, NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	return 0;
}

void * read_char(void * param) {

	char c;
	int num =0;

	printf("Entering routine to read character.........\nType a character: \n");

	c = getchar();

	while (c != '0'){

		if ((num = write(filedes[1], &c, 1)) == -1){
			perror("write");
			exit(1);
		}
		else {
			if (c != '\n'){
				printf("Writer: wrote character: %c\n",c);
				printf("Writer: Type a character: \n");
			}
		}
		
		c = getchar();
	}
	
	write(filedes[1], &c, 1);
	
	pthread_exit(0);
}

void * check_hit(void * param){
	
	char c;
	int num =0;

	printf("Entering routine to check hit.........\n");

	do {
		if ((num = read(filedes[0], &c, 1)) == -1)
			perror("read");
		else {
			if (c != '\n')
				printf("Reader: bytes %d read \"%c\"\n",num, c);
		}
	} while (c != '0');

	printf("Reader exiting\n");
	exit(0);
	
	pthread_exit(0);

}
