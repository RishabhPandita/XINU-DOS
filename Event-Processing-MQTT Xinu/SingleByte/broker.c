#include <xinu.h>
struct eventent grptop[MAX];
struct broker_struct *broker_dude;
int32 broker_head,broker_tail,broker_count;
sid32 mutex;
int32 brokerq_size= 3;

process broker (){
	while (1) {
		wait(mutex);
		int16 i=0,j=0;
		int32 topic,topicid,groupid;
		uint32 data;	
	
		if(broker_count > 0) {
		
			topic= broker_dude[broker_head].topic;
			data = broker_dude[broker_head].topic_data;
			broker_count--;
			broker_head ++;	
			broker_head = broker_head%brokerq_size;
		
			topicid= topic & 0x00FF;
			groupid= (topic >> 8) & 0x00FF;
	
			if(groupid!=0) {
				while(j<8) {
					if(grptop[topicid].pid_callback[j].isvalid!=0 && grptop[topicid].pid_callback[j].group_id == groupid){
						printf("\nBroker running : group [ %d ] topic [ %d ].function %d data",groupid,topicid,data);
						grptop[topicid].pid_callback[j].func(topicid,data);
						break;
					}
				j++;
				}
			}
			else {
				int k=0;
				while(k<8) {
					if(grptop[topicid].pid_callback[k].isvalid!=0){	
						printf("\nBroker running:broadcasting to all groups topic [ %d ].function",topicid);
						grptop[topicid].pid_callback[k].func(topicid,data);
					}
				k++;
				}
			}
			
		}
	signal(mutex);
	}

}


syscall subscribe (topic16 topic, void(*handler)(topic16,uint32)) {
	wait(mutex);

	uint32 topicid,groupid;
	topicid= topic & 0x00FF;
	groupid= (topic >> 8) & 0x00FF;
	int32 i = 0,k=0;
	
	int32 l=0;
	while(l<8) {	
		if(grptop[topicid].pid_callback[l].pid == currpid && grptop[topicid].pid_callback[l].isvalid == 1 && grptop[topicid].pid_callback[l].group_id == groupid)  		  { 
			signal(mutex);
			return SYSERR;
		}
	l++;
	}
	if(grptop[topicid].function_count<8)
		while (i<8) {
       			if( grptop[topicid].pid_callback[i].isvalid == 0) {
       		         	grptop[topicid].pid_callback[i].func =handler;
	                        grptop[topicid].pid_callback[i].isvalid = 1;
        	        	grptop[topicid].pid_callback[i].pid=currpid;
          		      	grptop[topicid].function_count++;
				grptop[topicid].pid_callback[i].group_id=groupid;
       				printf("\nSubscribing to GroupID %d TopicID %d",groupid,topicid);	
				break;
			}	
		i++;
		}
	else { 
		signal(mutex);
		return SYSERR;
	}
		

	signal(mutex);
return OK;
}


syscall unsubscribe (topic16 topic) {
	wait(mutex);
	int32 topicid,groupid;
	topicid= topic & 0x00FF;
	groupid= (topic >> 8) & 0x00FF;
	int32 i=0;

        while(i<8) {
                if(grptop[topicid].pid_callback[i].pid == currpid && grptop[topicid].pid_callback[i].isvalid == 1 && grptop[topicid].pid_callback[i].group_id == groupid) 		  {	
                        grptop[topicid].pid_callback[i].isvalid = 0;
                        grptop[topicid].function_count--;
       			printf("\nUnsubscribing from GroupID %d TopicID %d",groupid,topicid);	
			break;
	 	}
        i++;
        }
signal(mutex);
return OK;
}


syscall publish (topic16 topic,uint32 data) {

	wait(mutex);
        if(broker_count >= brokerq_size) {
        
	
        	struct broker_struct *broker_dude_2,*tmp;
		int32 temp_brokerq_size = brokerq_size*2;
		broker_dude_2 = (struct broker_struct *)getmem(temp_brokerq_size * sizeof(struct broker_struct));
		int32 i;

		for(i=0;i<brokerq_size;i++) {
			broker_dude_2[i].topic = broker_dude[i].topic;
			broker_dude_2[i].topic_data = broker_dude[i].topic_data;
		}


		tmp = broker_dude;
		broker_dude = broker_dude_2;
		freemem((char *)tmp,brokerq_size*sizeof(struct broker_struct));
		
		brokerq_size = temp_brokerq_size;
		printf("\nMemory extended two times");
		broker_tail = broker_count;
	
	}	
	
        broker_dude[broker_tail].topic_data=data;
        broker_dude[broker_tail].topic=topic;
	printf("\nPublishing data %d in Broker Queue %d tail %d ",broker_dude[broker_tail].topic_data,broker_count,broker_tail);
	broker_count++;
	broker_tail++;
	broker_tail = broker_tail%brokerq_size;
	signal(mutex);

return OK;
}


syscall init_all() {

	mutex = semcreate(1);
	int32 i,j;
	wait(mutex);
	broker_count =0;
	broker_head  =0;
	broker_tail  =0;

	for (i=0;i<MAX;i++)
	{
			grptop[i].function_count = 0;
			int32 k = 0;
			while(k<8) {
				grptop[i].pid_callback[k].group_id = 300;
				grptop[i].pid_callback[k++].isvalid = 0;	
			}		
	}
	broker_dude = (struct broker_struct*) getmem(brokerq_size*sizeof(struct broker_struct));
	signal(mutex);

return OK;
}
