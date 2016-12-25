/*  main.c  - main */

#include <xinu.h>

#define BUFFER_SIZE 10                     /*Buffer Size for user manipulation*/ 
pid32 producer_id;
pid32 consumer_id;
pid32 timer_id;

int32 consumed_count = 0;
const int32 CONSUMED_MAX = 100;

/* Define your circular buffer structure and semaphore variables here */
/* */ 
sid32 semempty; 			/*number of empty slots in buffer*/
sid32 semfull;				/*number of full slots in buffer*/		
sid32 sem_buff_mutex;			/*mutex for buffer*/	


struct data {				/*structure that multiple threads can access*/		
int32 index;
};

struct data buffer[BUFFER_SIZE];	/*an array of structure that multiple threads can access and manipulate*/

//Global Variables required
int32 piter,citer;			/*producer and consumer iterators*/


/* Place your code for entering a critical section here */
void mutex_acquire(sid32 mutex)
{
	/*semaphore wait*/
	wait(mutex);
}

/* Place your code for leaving a critical section here */
void mutex_release(sid32 mutex)
{
	/*semaphore signal*/
	signal(mutex);
}

/* Place the code for the buffer producer here */
process producer(void)
{
	/* */
	while(1) {
	mutex_acquire(semempty);
		mutex_acquire(sem_buff_mutex);
			buffer[piter].index = (piter)*2;	
			printf("Produced [%d]: %d \t",piter,buffer[piter].index);
			piter++;
			piter=piter%BUFFER_SIZE;
		mutex_release(sem_buff_mutex);
	mutex_release(semfull);
	}
	return OK;
}

/* Place the code for the buffer consumer here */
process consumer(void)
{
	/* Every time your consumer consumes another buffer element,
	 * make sure to include the statement:
	 *   consumed_count += 1;
	 * this will allow the timing function to record performance */
	/* */
	while(1) {
		mutex_acquire(semfull);
			mutex_acquire(sem_buff_mutex);
				printf("Consumed [%d]:%d \n ",citer,buffer[citer].index);
				citer++;
				citer = citer%BUFFER_SIZE;
				consumed_count+=1;	
			mutex_release(semempty);
		mutex_release(sem_buff_mutex);
		}
	return OK;
}

/* Timing utility function - please ignore */
process time_and_end(void)
{
	int32 times[5];
	int32 i;

	for (i = 0; i < 5; ++i)
	{
		times[i] = clktime_ms;
		yield();

		consumed_count = 0;
		while (consumed_count < CONSUMED_MAX * (i+1))
		{
			yield();
		}

		times[i] = clktime_ms - times[i];
	}

	kill(producer_id);
	kill(consumer_id);

	for (i = 0; i < 5; ++i)
	{
          kprintf("TIME ELAPSED (%d): %d\n", (i+1) * CONSUMED_MAX, times[i]);
	}
}

process	main(void)
{
	recvclr();

	/* Create the shared circular buffer and semaphores here */
	/* create mutex */
	semempty = semcreate(10);
	semfull =  semcreate(0);
	sem_buff_mutex = semcreate(1);
	piter = 0;citer=0;
	producer_id = create(producer, 4096, 50, "producer", 0);
	consumer_id = create(consumer, 4096, 50, "consumer", 0);
	timer_id = create(time_and_end, 4096, 50, "timer", 0);

	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	/* Uncomment the following line for part 3 to see timing results */
        resume(timer_id);
	resched_cntl(DEFER_STOP);

	return OK;
}
