#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM 10
int counter = 0;

void *printer(void *arg)
{
    for(int i=0; i<NUM; i++)
    {
        printf("count = %d\n", counter);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, printer, NULL);
    for (int i=0;i<NUM;i++)
    {
        counter++; //- increment count
        sleep(1);
    }
    //pthread_join(tid, NULL);//- wait
    return 0;
}
