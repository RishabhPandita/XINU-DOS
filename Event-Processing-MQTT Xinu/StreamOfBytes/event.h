
#define MAX 256
#define MAXX 100

struct eventent1 {
void (*func)(topic16,void *,uint32);
pid32 pid;
int32 group_id;
int32 isvalid;
};

struct eventent {
int32 function_count;
struct eventent1 pid_callback[8];
};

struct broker_struct {
int* topic_data;
int32 size;
topic16 topic;
};

