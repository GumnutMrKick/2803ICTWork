#include <pthread.h>
#include <stdio.h>

void* ThrdFn(void *arg) {
    printf("Hi I’m %d\n",pthread_self());
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1,NULL, ThrdFn, NULL);
    pthread_create(&t2,NULL, ThrdFn, NULL);
    pthread_join(t1,NULL); //- wait for t1
    pthread_join(t2,NULL); //- wait for t2
    return 0;
}
