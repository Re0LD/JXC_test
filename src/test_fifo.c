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
#define TOTAL 1000000*10
#define FIFO_TIMEOUT    20

static void *thread_fifo_w_fun(void *para)
{
    for(int i=0; i<TOTAL; i++){ 
        if(jxc_fifo_write(fifo, (uint8_t *)&i, sizeof(i), FIFO_TIMEOUT) == jxcStatusSuccess){

        }else{
            printf("jxc_fifo wirte timeout\n");
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
            if(jxc_fifo_read(fifo, (uint8_t *)&value, sizeof(value), FIFO_TIMEOUT) == jxcStatusSuccess){
                if(value != expect){
                    printf("jxc_fifo read err with v=%d e=%d\n",value, expect);
                }
                expect++;
            }else{
                printf("jxc_fifo wirte timeout\n");
            }
        }
    }
    printf("jxc_fifo finish\n");
}


void test_jxc_fifo(void)
{
    printf("--------------------TEST JXC FIFO\n");

    //init fifo
    fifo = jxc_fifo_create(1024*1024);

    int s = 0;
    int ss = 0;

    //连续写入
    for(int i=0; i<10; i++){
        if(jxc_fifo_write(fifo, (uint8_t *)&s, sizeof(s), FIFO_TIMEOUT) == jxcStatusSuccess){
            s++;
        }else{
            printf("[ERR] fifo write err\n");
        }
    }

    for(int i=0; i<11 ; i++){
        if(jxc_fifo_read(fifo, (uint8_t *)&ss, sizeof(ss), FIFO_TIMEOUT) == jxcStatusSuccess){
            printf("get fifo value = %d\n",ss);
        }else{
            printf("get fifo timeout, fifo size is %d\n",jxc_fifo_get_count(fifo));
            break;
        }
    }

    for(int i=0; i<100; i++){
        jxc_fifo_write(fifo, (uint8_t *)&i, sizeof(i), FIFO_TIMEOUT);
    }
    printf("fifo write 100*4 bytes, fifo size is %d\n",jxc_fifo_get_count(fifo));
    jxc_fifo_clear(fifo);
    printf("fifo clear, fifo size is %d\n",jxc_fifo_get_count(fifo));

    int ret = 0;
    ret = pthread_create(&thread2, 0, thread_fifo_r_fun, NULL);
    ret = pthread_create(&thread1, 0, thread_fifo_w_fun, NULL);
    
}

void test_jxc_fifo_destory(void)
{
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    jxc_fifo_destroy(fifo);
}

