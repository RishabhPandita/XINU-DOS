/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/
	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}


syscall sendMsg(
		pid32	pid1,
		umsg32 msg1
		)
{

	intmask mask1;
	struct procent *prptr1;
	
	mask1 = disable();
	if(isbadpid(pid1)) {
		restore(mask1);
		return SYSERR;
	}

	prptr1 = &proctab[pid1];
		if(prptr1->count !=10 ){
			prptr1->queue_pr[++prptr1->send_ptr]=msg1;
			printf("\nSend[%d] ->  %d PID %d",prptr1->send_ptr,prptr1->queue_pr[prptr1->send_ptr],pid1);
			prptr1->send_ptr=prptr1->send_ptr%10;
			prptr1->prhasmsg = TRUE;
			prptr1->count++;
		}
	if (prptr1->prstate == PR_RECV) {
		ready(pid1);
	} else if (prptr1->prstate == PR_RECTIM) {
		unsleep(pid1);
		ready(pid1);
	}

	restore(mask1);		/* Restore interrupts */
	return OK;
}

uint32 sendMsgs(
		pid32 pid,
		umsg32* msgs,
		uint32 msg_count
		)
{
	uint32 msg_sent=0;
	while(msg_count-- >0) {
	uint32 status = sendMsg(pid,*msgs);
	if(status !=-1)
		msg_sent++;
	msgs++;
	}
return msg_sent;	
}

/**Alternate approach for sendMsgs send none or send all
uint32 sendMsgs(
                pid32 pid,
                umsg32* msgs,
                uint32 msg_count
                )
{

	prptr = &proctab[pid];
	if((10-prptr->count)>=msg_count) {
        	uint32 msg_sent=0;
		while(msg_count-- >0) {
		       	uint32 status = sendMsg(pid,*msgs);
		        	if(status !=-1)
	        	       	 msg_sent++;
	        	msgs++;
	        	}
		return msg_sent;
	}
	else
		return SYSERR;
}
**/

uint32 sendnMsg(
		uint32 pid_count,
		pid32* pids,
		umsg32 msg
		)
{
	uint32 count = 0;
	while(pid_count >0)	{
		uint32 success =sendMsg(*pids,msg);
		if(success!=-1)
			count++;
		pids++;
		pid_count--;
	}

	if(count == 0)
		return SYSERR;
	else
		return count;
}
