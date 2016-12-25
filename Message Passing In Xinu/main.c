/*  main.c  - main */

#include <xinu.h>

pid32 producer_id;
pid32 consumer_id;
pid32 consumer_id1;
pid32 consumer_id2;
pid32 consumer_id3;
pid32 timer_id;

pid32 p[5];

void chk_sendMsg(){
	
	sendMsg(consumer_id,0);
	sleep(1);
	sendMsg(consumer_id,1);
	sleep(1);
	sendMsg(consumer_id,2);
	sleep(1);
	sendMsg(consumer_id,3);
	sleep(1);
	sendMsg(consumer_id,4);
	sleep(1);
	sendMsg(consumer_id,5);
	sleep(1);
	sendMsg(consumer_id,6);
	sleep(1);
	sendMsg(consumer_id,7);
	sleep(1);
	sendMsg(consumer_id,8);
	sleep(1);
	sendMsg(consumer_id,9);

}

void chk_sendMsgs() {

	printf("\n\n\n\n Checking SendMsgs:\n");
	umsg32 msg[] = {0,9,5,3,4,6};
	uint32 msg_sent = sendMsgs(consumer_id,&msg,6);
	printf("\n Messages sent: %d",msg_sent);
}

void chk_sendnMsg() {

	uint32 n =sendnMsg(4,&p,121);
	printf("\nNumber of messages sent%d ",n);
}

process producer(void)
{

	chk_sendMsg();			//Uncomment chk_sendMsg() and chk_receiveMsg() to  test
//	chk_sendMsgs();			//comment the consumer functions
//	chk_sendnMsg();			//comment the consumer functions

	return OK;
}


void chk_receiveMsg(){

	umsg32 msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
	msg = receiveMsg();
	printf("\nReceived : %d",msg);
}

void chk_receiveMsgs() {

	umsg32 temp [12];
	umsg32* msg1=&temp; 
	receiveMsgs(msg1,5);	
}

process consumer(void)
{
	chk_receiveMsg();  //uncomment chk_sendMsg to check
//	chk_receiveMsgs(); //comment producer functions and above funtion 

	return OK;
}


process	main(void)
{
	recvclr();

	producer_id = create(producer, 4096, 50, "producer", 0);
	consumer_id = create(consumer, 4096, 50, "consumer", 0);
/*	consumer_id1 = create(consumer, 4096, 50, "consumer", 0); // Uncomment following block for testing sendnMsg()  
	consumer_id2 = create(consumer, 4096, 50, "consumer", 0);
	consumer_id3 = create(consumer, 4096, 50, "consumer", 0);

	p[0]=consumer_id;
	p[1]=consumer_id1;
	p[2]=consumer_id2;
	p[3]=consumer_id3;
*/
	resched_cntl(DEFER_START);
		resume(producer_id);
		resume(consumer_id);
	resched_cntl(DEFER_STOP);

	return OK;
}
