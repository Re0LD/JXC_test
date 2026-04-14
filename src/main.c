#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "jxc_log.h"
#include "jxc_fifo.h"
#include "jxc_util.h"

//线程
pthread_t thread1;
pthread_t thread2;

pthread_t thread3;
pthread_t thread4;

//
jxc_fifo_handle fifo = NULL;

//----------------------------------------------------------------------------------------------------
void *thread1_fun(void *para)
{
    for(int i=0; i<1000; i++){
        // srand(time(NULL));
        // int ms = rand() % 10000;
        // usleep(100);
        jxc_log_write("i am t1 %d",i);
    }
}

void *thread2_fun(void *para)
{
    for(int i=0; i<1000; i++){
        // srand(time(NULL));
        // int ms = rand() % 10000;
        // usleep(100000);
        jxc_log_write("i am t2 %d",i);
    }

    jxc_log_write("i am done");
}


void *thread3_fun(void *para)
{
    uint8_t data[10]={0};

    for(int i=0; i<1000; i++){
        memset(data, i%255, sizeof(data));    
        jxc_fifo_write(fifo, data, sizeof(data));
        usleep(1000);
    }

    for(int i=0; i<sizeof(data); i++){
        data[i] = i;
    }
    jxc_fifo_write(fifo, data, sizeof(data));

}

void *thread4_fun(void *para)
{
    uint8_t data[10] = {0};
    uint8_t checkdata[10] = {0,1,2,3,4,5,6,7,8,9};
    while(1)
    {
        if(jxc_fifo_get_count(fifo)>=10){
            jxc_fifo_read(fifo, data, sizeof(data));
            if(memcmp(data, checkdata, sizeof(data)) == 0){
                jxc_log_write("check ok");
                break;
            }else{
                jxc_log_write("%d--%d",data[0],data[9]);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------jxc_log
void test_jxc_log(void)
{
    JXC_INFO("--------------------TEST JXC LOG\n");
    jxc_log_init("testlog.txt");
    for(int i=0; i<5; i++)
        jxc_log_write("log_%d\n",i);

    JXC_DEBUG("JXC_DEBUG\n");
    JXC_INFO("JXC_INFO\n");
    JXC_ERROR("JXC_ERROR\n");
}

//----------------------------------------------------------------------------------------------------jxc_util
void test_jxc_util(void)
{
    JXC_INFO("--------------------TEST JXC UTIL\n");
    double tempus = jxc_get_us();
    double tempms = jxc_get_ms();
    double temps = jxc_get_s();
    usleep(1000*2);
    JXC_INFO("set sleep 1000*2 us with real %f us\n",jxc_get_us()-tempus);
    JXC_INFO("set sleep 1000*2 us with real %f ms\n",jxc_get_ms()-tempms);
    JXC_INFO("set sleep 1000*2 us with real %f s\n",jxc_get_s()-temps);

    int a=-1, b=1;
    JXC_INFO("a=-1 b=1 after ABS a=%d b=%d\n",JXC_ABS(a), JXC_ABS(b));
    JXC_INFO("x=-1 y=1 after MAX2 res=%d\n",JXC_MAX2(-1, 1));
    JXC_INFO("x=-1 y=1 after MIN2 res=%d\n",JXC_MIN2(-1, 1));
    JXC_INFO("x=-1 y=1 z=10 after MAX3 res=%d\n",JXC_MAX3(-1, 1, 10));
    JXC_INFO("x=-1 y=1 z=10 after MIN3 res=%d\n",JXC_MIN3(-1, 1, 10));
    JXC_INFO("x=44 y=16 after ALIGN_UP res=%d\n",JXC_ALIGN_UP(44, 16));
    JXC_INFO("x=44 y=16 after ALIGN_DOWN res=%d\n",JXC_ALIGN_DOWN(44, 16));
    JXC_INFO("value=10 min=0 max=20 after BETWEEN res=%d\n",JXC_VALUE_BETWEEN(10, 0, 20));
    JXC_INFO("value=-1 min=0 max=20 after BETWEEN res=%d\n",JXC_VALUE_BETWEEN(-1, 0, 20));
}

//----------------------------------------------------------------------------------------------------jxc_fifo
void test_jxc_fifo(void)
{
    JXC_INFO("--------------------TEST JXC FIFO\n");
    JXC_INFO("NULL\n");
}

//----------------------------------------------------------------------------------------------------jxc_queue
void test_jxc_queue(void)
{
    JXC_INFO("--------------------TEST JXC QUEUE\n");
    JXC_INFO("NULL\n");
}

//----------------------------------------------------------------------------------------------------jxc_udp
void test_jxc_udp(void)
{
    JXC_INFO("--------------------TEST JXC UDP\n");
    JXC_INFO("NULL\n");
}

//----------------------------------------------------------------------------------------------------jxc_tcp
void test_jxc_tcp(void)
{
    JXC_INFO("--------------------TEST JXC TCP\n");
    JXC_INFO("NULL\n");
}

//----------------------------------------------------------------------------------------------------SIGNAL
void signal_cb(int sig)
{
    if(sig == SIGINT){
        printf("Get ctrl c\n");
    }else if(sig == SIGTERM){
        printf("Get kill/reboot\n");
    }else{
        printf("unknow sig %d\n",sig);
    }

}


//----------------------------------------------------------------------------------------------------MAIN
int main(int argc, char **argv)
{
    printf("this is jxc test\n");

    signal(SIGINT, signal_cb);   // Ctrl+C
    signal(SIGTERM, signal_cb);  // kill 或 reboot

    //----------test jxc_log
    test_jxc_log();

    //----------test jxc_util
    test_jxc_util();

    //----------test jxc_fifo
    test_jxc_fifo();

    //----------test jxc_queue
    test_jxc_queue();

    //----------test jxc_udp
    test_jxc_udp();

    //----------test jxc_tcp
    test_jxc_tcp();

    //----------test jxc_XXX


    //init fifo
    fifo = jxc_fifo_create(1024*1024);
    
    //creat
    int ret = 0;
    ret = pthread_create(&thread1, 0, thread1_fun, NULL);
    ret = pthread_create(&thread2, 0, thread2_fun, NULL);
    ret = pthread_create(&thread3, 0, thread3_fun, NULL);
    ret = pthread_create(&thread4, 0, thread4_fun, NULL);

    //quit
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    jxc_log_close();
    jxc_fifo_destroy(fifo);

    printf("test finish\n");

    return 0;
}

