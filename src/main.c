#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

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
    double temp = jxc_get_us();
    struct timespec ts;
    ts.tv_sec = 0;          //s
    ts.tv_nsec = 1000*1000; //ns
    nanosleep(&ts, NULL);
    // usleep(500);
    JXC_INFO("set sleep 1000 us with real %f us\n",jxc_get_us()-temp);
    temp = jxc_get_ms();
    usleep(1000);
    JXC_INFO("set sleep 1000 us with real %f ms\n",jxc_get_ms()-temp);
    temp = jxc_get_s();
    usleep(1000);
    JXC_INFO("set sleep 1000 us with real %f s\n",jxc_get_s()-temp);
}

//----------------------------------------------------------------------------------------------------jxc_fifo
//----------------------------------------------------------------------------------------------------jxc_queue
//----------------------------------------------------------------------------------------------------jxc_udp
//----------------------------------------------------------------------------------------------------jxc_tcp



//----------------------------------------------------------------------------------------------------MAIN
int main(int argc, char **argv)
{
    printf("this is jxc test\n");

    //----------test jxc_log
    test_jxc_log();

    //----------test jxc_util
    test_jxc_util();


    //----------test jxc_fifo
    //----------test jxc_queue
    //----------test jxc_udp
    //----------test jxc_tcp
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

    JXC_INFO("test finish\n");
    return 0;
}

