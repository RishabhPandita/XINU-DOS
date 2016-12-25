#include <xinu.h> 
 
void foo(topic16 ,int32 *, int32);
void foo1(topic16 ,int32 *,int32);
pid32 subscriber1; 
pid32 subscriber22; 
pid32 publisher1;
pid32 handler1; 
 
process subscriber(void) 
{
if (subscribe(0x0101,foo) == SYSERR)
	printf("\nSystemError");

if (subscribe(0x0102,foo1) == SYSERR)
	printf("\nSystemError");

sleep(20);
unsubscribe(0x0101);
unsubscribe(0x0102);
}
 
process publisher(void) 
{ 
int data[] = {1,2,3,4,5};
int data1[] = {100,200,300,400,500};

if(publish(0x0101,(void *)data ,5) == SYSERR)
	printf("SystemError");

if(publish(0x0102,(void *)data1,5) == SYSERR)
	printf("SystemError");
}

process handler(void)
{
sleep(10);
broker();
}

void foo(topic16 topic,int32* data,int32 size) {
	int32 i =0;
	int32 topicid,groupid;
        topicid = topic & 0x00FF;
        groupid = (topic >> 8) & 0x00FF;
	printf("\nGroupID %d, TopicId %d",groupid,topicid);
	while(i<size) {
		printf("\tData[%d]=%d",i,data[i++]);
	}
}
 
void foo1(topic16 topic,int32* data,int32 size)
{
	int32 i=0;
	int32 topicid,groupid;
        topicid = topic & 0x00FF;
        groupid = (topic >> 8) & 0x00FF;
	printf("\nGroupID %d, TopicId %d",groupid,topicid);
	while(i<size) {
		printf("\tD[%d]=[%d]",i,data[i++]);	
	}


} 
process main(void) 
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
