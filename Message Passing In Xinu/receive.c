/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}

umsg32 receiveMsg(void)
{
	intmask mask1;
	struct procent *prptr1;
	umsg32 msg1;
	
	mask1 = disable();
	prptr1 =&proctab[currpid];

		if(prptr1->count<=0)
		{
				prptr1->prstate = PR_RECV;
				printf("\nResched");
				resched(); 
		}
		
		if(prptr1->count!=0){
			msg1 = prptr1->queue_pr[++prptr1->rec_ptr];
			prptr1->rec_ptr = prptr1->rec_ptr%10;
			printf("\nRec[%d] %d", prptr1->rec_ptr, prptr1->queue_pr[prptr1->rec_ptr]);
			prptr1->count--;
			} 
		
restore(mask1);
return msg1;
}

syscall receiveMsgs(umsg32* msg, uint32 msg_count)
{
	struct procent *prptr;
	prptr = &proctab[currpid];
	uint32 no_rec = msg_count;

	while(prptr->count < msg_count) {
		printf("\nresched");
		prptr->prstate = PR_RECV;
		resched();
	}
	if (msg_count <= prptr->count) {
		while(no_rec >0) {
			*msg = receiveMsg();
			msg++;
			no_rec--;
		}
	}
}

