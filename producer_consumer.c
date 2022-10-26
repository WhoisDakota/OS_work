#include <stdio.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

//static int success = 0;

#define AUTHOR "Alexandr Zincenko"
MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);


//struct semaphore empty, full, mutex;
struct semaphore empty;
struct semaphore full;
struct semaphore mutex;


//void up(struct semaphore *sem){};

//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct task_struct *ts[2];
struct params { char name[100]; int proc_seq; int buf_index; int pid; };
struct params threads[2];


struct task_struct *con[1];


static int delay = .0003;

int in = 0;
int out = 0;


static int buffSize = 10000;
int buffer[10000];
static int prod = 0;
static int cons = 0;
static int uuid = 0;
module_param(buffSize, int, 0);
module_param(prod,int, 0);
module_param(cons,int, 0);
module_param(uuid, int, 0);

static int thread_func(void *data){
   	printk(KERN_INFO "%s: Ding", ((struct params*)data)->name);
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(delay*HZ);
	return 0;
}

void main1(int s){
	printk("s value = %d\n", s);
}

static int producer(void *pno){
	
	FILE *f = popen( "ps -u $(id -u -n)", "r" );
	
	
	int item = 2;
	int i=0;
		
	for (i = 0; i < buffSize; i++){
	
	printk("ENTERED FOR LOOP");
	printk("Produced Item#-%d at buffer index:%d for PID:%d\n", item, in, *((int *) pno));
	item = i;
	buffer[in] = item;
	in = (in + 1) % buffSize;
	}
	return 0;
}


static int __init initialize(void){
    printk(KERN_INFO "value of buffer_size = %d\n", buffSize);
    printk(KERN_INFO "value of prod = %d\n", prod);
    printk(KERN_INFO "value of cons = %d\n", cons);
    printk(KERN_INFO "value of uuid = %d\n", uuid);
    
    if (prod != 0 ){
	
	ts[0] = kthread_run(producer, (void*)&threads[0], "thread1");
	}
	
    	sema_init(&empty,buffSize);
	sema_init(&full,0);
	sema_init(&mutex,0);
    
    /* Initialize the parameter structures */
   strncpy(threads[0].name,"thread 1 producer speaking",26);
   strncpy(threads[1].name,"thread 2 consumer speaking",26);
   threads[0].pid = uuid;
   threads[1].pid = uuid;

   /* Create and start two kernel threads */
   printk(KERN_EMERG "Starting..");
   ts[0] = kthread_run(producer, (void*)&threads[0], "spawn1");
  // ts[1] = kthread_run(consumer, (void*)&threads[1], "spawn2");
    
    return 0;
}

static void __exit clean_exit(void){
    printk(KERN_INFO "Exited");
}

module_init(initialize);
module_exit(clean_exit);

