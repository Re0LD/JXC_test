#include "test_fifo.h"
#include "jxc_fifo.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


static bool is_running = true;
jxc_fifo_handle fifo = NULL;
pthread_t thread3;
pthread_t thread4;

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
    while(is_running)
    {
        if(jxc_fifo_get_count(fifo)>=10){
            jxc_fifo_read(fifo, data, sizeof(data));
            if(memcmp(data, checkdata, sizeof(data)) == 0){
                // jxc_log_write("check ok");
                break;
            }else{
                // jxc_log_write("%d--%d",data[0],data[9]);
            }
        }
    }
}


void test_jxc_fifo(void)
{
    printf("--------------------TEST JXC FIFO\n");

    //init fifo
    fifo = jxc_fifo_create(1024*1024);

    int ret = 0;
    ret = pthread_create(&thread3, 0, thread3_fun, NULL);
    ret = pthread_create(&thread4, 0, thread4_fun, NULL);
}

void test_jxc_fifo_destory(void)
{
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    jxc_fifo_destroy(fifo);
}

