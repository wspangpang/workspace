#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>

pthread_t ntid;
pthread_mutex_t mutex;
int value;

void *func(void *arg)
{
	//printf("%s %d %u \n", "new", (unsigned int)getpid(), (unsigned int)pthread_self());
	//printf("%u \n", (unsigned int)ntid);
    pthread_mutex_lock(&mutex);
    sleep(2);
    value = 10;
    pthread_mutex_unlock(&mutex);
    printf("%d\n", value);
	return ((void *)0);
}

int main()
{
	
    pthread_create(&ntid, NULL, func, NULL);
    sleep(1);
	//printf("%s %d %u \n", "main", (unsigned int)getpid(), (unsigned int)pthread_self());
	//printf("%u \n", (unsigned int)ntid);
	//int fd = open("/dev/pts/1",O_RDWR);
	//write(fd, "h", 1);
	//close(fd);
    pthread_mutex_lock(&mutex);
    value = 20;
	printf("%d\n", value);
    pthread_mutex_unlock(&mutex);
	pthread_join(ntid,NULL);
	return 0;
}
