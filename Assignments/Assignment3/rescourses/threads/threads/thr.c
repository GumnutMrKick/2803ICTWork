#include <stdio.h>
#include <pthread.h>

struct test{
 int a;
 int b;
};

void *myThreadFn(void *arg)
{
 struct test *ptr = (struct test *)arg;
 sleep(1);
 printf("In Thread Fn with ptr->a:%d and ptr->b:%d\n",ptr->a, ptr->b);
 
 //pthread_exit(&ptr->b);
 //pthread_exit(&ptr->a);
 
 return (void *)(ptr->a+ ptr->b);

}



int main(int argc, char *argv[])
{
 struct test test1 = {1,2};
 pthread_t tid;
 
 void *res;
 
 printf("Before Thread\n");
 pthread_create(&tid, NULL, myThreadFn,(void *) &test1);
 
 pthread_join(tid, &res);
 printf("Thread returned :%d\n", (int)res);
 
 /*
   *pthread_join blocks the calling thread until the
  * thread with identifier equals to the first argument
  * terminates
   */
 //pthread_join(tid, NULL);
 
 printf("After Thread\n");
 return 0;
}
