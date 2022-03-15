/* ----------------------------------------------------------------- */
/* PROGRAM  server.c                                                 */
/* ----------------------------------------------------------------- */

#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

#include  "header.h"

void  main(void)
{
     key_t          ShmKEY;
     int            ShmID;
     struct Memory  *ShmPTR;
     
     ShmKEY = ftok(".", 'x');
     ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("   Server has received a shared memory of four integers...\n");
     
     ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
     if ((int) ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("   server has attached the shared memory...\n");
     
     while (ShmPTR->status != FILLED)
          ;
     printf("   server found the data is ready...\n");
     printf("   server found %d %d %d %d in shared memory...\n",
                ShmPTR->data[0], ShmPTR->data[1], 
                ShmPTR->data[2], ShmPTR->data[3]);

     ShmPTR->status = TAKEN;
     printf("   server has informed client data have been taken...\n");
     shmdt((void *) ShmPTR);
     printf("   server has detached its shared memory...\n");
     printf("   server exits...\n");
     exit(0);
}
