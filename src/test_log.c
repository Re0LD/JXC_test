#include "test_log.h"
#include "jxc_log.h"

#include <stdio.h>
#include <pthread.h>

/**
 * test log lib
*/

//线程
static pthread_t thread1;
static pthread_t thread2;

static void *thread1_fun(void *para)
{
    for(int i=0; i<1000; i++){
        jxc_log_write("i am t1 %d",i);
    }
    jxc_log_write("i am t1 done");
}

static void *thread2_fun(void *para)
{
    for(int i=0; i<1000; i++){
        jxc_log_write("i am t2 %d",i);
    }
    jxc_log_write("i am t2 done");
}

void test_jxc_log(void)
{
    printf("--------------------TEST JXC LOG\n");
    jxc_log_init("testlog.txt");
    for(int i=0; i<5; i++)
        jxc_log_write("log_%d\n",i);

    JXC_DEBUG("JXC_DEBUG\n");
    JXC_INFO("JXC_INFO\n");
    JXC_ERROR("JXC_ERROR\n");

    int ret = 0;
    ret = pthread_create(&thread1, 0, thread1_fun, NULL);
    ret = pthread_create(&thread2, 0, thread2_fun, NULL);
}

void test_jxc_log_destory(void)
{
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    jxc_log_close();
}
