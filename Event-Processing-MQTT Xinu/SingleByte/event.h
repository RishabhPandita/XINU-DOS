
#define MAX 256

struct eventent1 {
void (*func)(topic16,uint32);
pid32 pid;
int32 isvalid;
int32 group_id;
};

struct eventent {
int32 function_count;
struct eventent1 pid_callback[8];
};

struct broker_struct {
uint32 topic_data;
topic16 topic;
};
