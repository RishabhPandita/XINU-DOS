#include <xinu.h> 
 
void foo(topic16 topic,uint32 data);
void foo1(topic16 topic,uint32 data);
pid32 subscriber1; 
pid32 subscriber22; 
pid32 publisher1;
pid32 handler1; 
 
process subscriber(void) 
{
subscribe(0x0101,foo); 
subscribe(0x0201,foo1);

sleep(10);
unsubscribe(0x0101);
unsubscribe(0x0201);
}
 
process publisher(void) 
{ 
//publish to group 1 and topic 1
publish(0x0101,1);
publish(0x0101,2);
publish(0x0101,3);
publish(0x0101,4);
publish(0x0101,5);
publish(0x0101,6);
//publish to group2 and topic 2
publish(0x0201,7);
publish(0x0201,8);
publish(0x0201,9);
publish(0x0201,10);
publish(0x0201,11);
//brodcast
publish(0x0001,100);
}

void foo(topic16 topic,uint32 data) {
	int32 topicid,groupid;
	topicid = topic & 0x00FF;
	groupid = (topic >> 8) & 0x00FF;
	printf("\nHandler Functoin :Groupid = %d,Topicid = %d,Data Recieved = %d\n",groupid,topicid,data);
} 
 
void foo1(topic16 topic,uint32 data) {
	int32 topicid,groupid;
	topicid= topic & 0x00FF;
	groupid= (topic >> 8) & 0x00FF;
	printf("\nHandler function: G = %d,T = %d,D = %d\n",groupid,topicid,data);
}

process    main(void) 
{ 
    recvclr(); 
    subscriber1 = create(subscriber, 4096, 50, "subscriber", 0); 
    publisher1 = create(publisher, 4096, 50, "publisher", 0); 
 
    resched_cntl(DEFER_START); 
    resume(subscriber1); 
    resume(publisher1);     
    resched_cntl(DEFER_STOP); 
     
    return OK; 
}
