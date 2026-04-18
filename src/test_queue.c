#include "test_queue.h"
#include "jxc_queue.h"
#include "jxc_util.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

/**
 * test queue lib
*/

static bool is_running = true;

//线程
static pthread_t thread1;
static pthread_t thread2;
//QUEUE
jxc_queue_handle handle = NULL;
#define QUEUE_MAX_NUM   10
#define QUEUE_TIMEOUT   500

typedef struct{
    int     num;
    int     fix; 
}test_t;

static void *thread1_fun(void *para)
{
    test_t s = {0};

    while(is_running)
    {
        jxc_queue_enqueue(handle, (uint8_t *)&s, 20);
        s.num++;
        s.fix++;
        usleep(1000*20);
    }
}

static void *thread2_fun(void *para)
{
    test_t s = {0};

    while(is_running)
    {
        if(jxc_queue_dequeue(handle, (uint8_t *)&s, QUEUE_TIMEOUT) == jxcStatusSuccess){
            static int i=0;
            static double time=0;
            if(i%1000 == 0){
                printf("queue downSample1000 i=%d num=%d fix=%d cur queue size is %d els time %f\n",
                        i,s.num,s.fix,jxc_queue_get_cur_num(handle),(jxc_get_ms() - time));
                time = jxc_get_ms();
            }
            i++;
        }else{
            printf("jxc_queue_dequeue timeout\n");
        }
    }
}

void test_jxc_queue(void)
{
    printf("--------------------TEST JXC QUEUE\n");

    handle = jxc_queue_create(QUEUE_MAX_NUM, sizeof(test_t));
    test_t s = {0};
    test_t ss = {0};

    //测试连续写入
    for(int i=0; i<QUEUE_MAX_NUM*2; i++){
        if(jxc_queue_enqueue(handle, (uint8_t *)&s, QUEUE_TIMEOUT) == jxcStatusSuccess){
            s.num++;
            s.fix++;
        }else{
            printf("queue enqueue timeout, cur queue size is %d\n",jxc_queue_get_cur_num(handle));
            jxc_queue_dequeue(handle, (uint8_t *)&ss, QUEUE_TIMEOUT);
            printf("get oldest para num=%d fix=%d\n",ss.num,ss.fix);
            jxc_queue_get_latest(handle, (uint8_t *)&ss, QUEUE_TIMEOUT);
            printf("get newest para num=%d fix=%d and queue size is %d\n",ss.num,ss.fix,jxc_queue_get_cur_num(handle));
            break;
        }
    }

    jxc_queue_enqueue(handle, (uint8_t *)&s, QUEUE_TIMEOUT);
    printf("queue enqueue 1 elem, queue size is %d\n",jxc_queue_get_cur_num(handle));
    jxc_queue_clear(handle);
    printf("clear queue, queue size is %d\n",jxc_queue_get_cur_num(handle));

    for(int i=0; i<QUEUE_MAX_NUM; i++){
        if(jxc_queue_enqueue(handle, (uint8_t *)&s, QUEUE_TIMEOUT) == jxcStatusSuccess){
            s.num++;
            s.fix++;
        }else{
            printf("[ERR] queue enqueue timeout, queue size is %d\n",jxc_queue_get_cur_num(handle));
            break;
        }
    }

    for(int i=0; i<QUEUE_MAX_NUM+1; i++){
        if(jxc_queue_dequeue(handle, (uint8_t *)&ss, QUEUE_TIMEOUT) == jxcStatusSuccess){
            printf("queue enqueue num=%d fix=%d\n",ss.num,ss.fix);
        }else{
            printf("queue enqueue timeout, queue size is %d\n",jxc_queue_get_cur_num(handle));
            break;
        }
    }

    int ret = 0;
    ret = pthread_create(&thread1, 0, thread1_fun, NULL);
    ret = pthread_create(&thread2, 0, thread2_fun, NULL);

}

void test_jxc_queue_destory(void)
{
    is_running = false;
    jxc_queue_destroy(handle);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
}