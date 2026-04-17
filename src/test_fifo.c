#include "test_fifo.h"
#include "jxc_fifo.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


static bool is_running = true;

static pthread_t thread1;
static pthread_t thread2;
jxc_fifo_handle fifo = NULL;
#define TOTAL 1000000

static void *thread_fifo_w_fun(void *para)
{
    uint32_t len = 0;

    for(int i=0; i<TOTAL; i++){ 
        len = jxc_fifo_write(fifo, (uint8_t *)&i, sizeof(i));
        if(len != sizeof(i)){
            printf("jxc_fifo wirte err\n");
        }
    }

}

static void *thread_fifo_r_fun(void *para)
{
    uint32_t expect = 0;
    uint32_t value = 0;

    while(expect < TOTAL)
    {
        if(jxc_fifo_get_count(fifo)>0){
            jxc_fifo_read(fifo, (uint8_t *)&value, sizeof(value));
            if(value != expect){
                printf("jxc_fifo read err with v=%d e=%d\n",value, expect);
            }
            expect++;
        }
    }
    printf("jxc_fifo finish\n");
}


void test_jxc_fifo(void)
{
    printf("--------------------TEST JXC FIFO\n");

    //init fifo
    fifo = jxc_fifo_create(1024*1024*4);

    int ret = 0;
    ret = pthread_create(&thread1, 0, thread_fifo_w_fun, NULL);
    ret = pthread_create(&thread2, 0, thread_fifo_r_fun, NULL);
}

void test_jxc_fifo_destory(void)
{
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    jxc_fifo_destroy(fifo);
}

