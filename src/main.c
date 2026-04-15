#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "jxc.h"
#include "jxc_log.h"
#include "jxc_fifo.h"
#include "jxc_util.h"
#include "jxc_udp.h"

//全局变量
bool is_running = true;

//线程
pthread_t thread1;
pthread_t thread2;

pthread_t thread3;
pthread_t thread4;

pthread_t thread_udp;
pthread_t thread_udp_groupS;
pthread_t thread_udp_groupR;

//FIFO
jxc_fifo_handle fifo = NULL;

//UDP
jxc_udp_handle  udp_hndl = NULL;
jxc_udp_handle  udp_groupS_hndl = NULL;
jxc_udp_handle  udp_groupR_hndl = NULL;
//UDP 单播
#define UDP_LOCAL_IP        "192.168.10.128"
#define UDP_LOCAL_PORT      19990
//UDP 组播发
#define UDP_GROUPS_IP       "226.0.0.1"
#define UDP_GROUPS_PORT     12345
#define UDP_GROUPS_NET_CARD "192.168.10.128"
//UDP 组播收
#define UDP_GROUPR_IP       "239.0.0.1"
#define UDP_GROUPR_PORT     12346
#define UDP_GROUPR_NET_CARD "192.168.3.11"


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
    while(is_running)
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

void *thread_udp_fun(void *para)
{
    uint8_t data[1024] = {0};
    uint8_t ip[256] = {0};
    uint16_t port = 0;
    while(is_running)
    {
        if(jxc_udp_recv(udp_hndl, data, sizeof(data), ip, &port) > 0){
            if(data[0] == 0x5a && data[1] == 0x5a){
                return NULL;
            }else{
                JXC_INFO("udp recv from %s:%d\n",ip,port);
                JXC_INFO("udp recv: %s\n",data);
            }
        }else{
            // JXC_INFO("udp recv timeout\n");
            jxc_udp_send(udp_hndl, "192.168.10.128", 19991, "i am 19990\n", sizeof("i am 19990\n"));
        }
    }
    
}

void *thread_udp_groupS_fun(void *para)
{
    while(is_running)
    {
        jxc_udp_send(udp_groupS_hndl, UDP_GROUPS_IP, UDP_GROUPS_PORT, "i am udp group", sizeof("i am udp group"));
        sleep(1);
    }
}

void *thread_udp_groupR_fun(void *para)
{
    uint8_t data[1024] = {0};
    uint8_t ip[256] = {0};
    uint16_t port = 0;
    while(is_running)
    {
        if(jxc_udp_recv(udp_groupR_hndl, data, sizeof(data), ip, &port) > 0){
            JXC_INFO("udp recv from %s:%d\n",ip,port);
            JXC_INFO("udp recv: %s\n",data);
        }else{
            // JXC_INFO("udp group recv timeout\n");
            usleep(20000);
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

//----------------------------------------------------------------------------------------------------jxc_queue
void test_jxc_queue(void)
{
    JXC_INFO("--------------------TEST JXC QUEUE\n");
    JXC_INFO("NULL\n");
}

void test_jxc_queue_destory(void)
{

}

//----------------------------------------------------------------------------------------------------jxc_udp
void test_jxc_udp(void)
{
    JXC_INFO("--------------------TEST JXC UDP\n");
    
    int ret = 0;

    //单播发送&接收
    jxc_udp_cfg_t cfg;
    cfg.local_ip = UDP_LOCAL_IP;
    cfg.local_port = UDP_LOCAL_PORT;
    cfg.mcast_group = NULL;
    cfg.iface_ip = NULL;
    cfg.recv_timeout_ms = 2000;
    udp_hndl = jxc_udp_creat(&cfg);
    ret = pthread_create(&thread_udp, 0, thread_udp_fun, NULL);

    //组播发送
    cfg.local_ip = NULL;
    cfg.local_port = 0;
    cfg.mcast_group = NULL;
    cfg.iface_ip = NULL;
    cfg.recv_timeout_ms = -1;
    udp_groupS_hndl = jxc_udp_creat(&cfg);
    jxc_udp_set_mcast_if(udp_groupS_hndl, UDP_GROUPS_NET_CARD);
    ret = pthread_create(&thread_udp_groupS, 0, thread_udp_groupS_fun, NULL);

    //组播接收
    cfg.local_ip = NULL;
    cfg.local_port = UDP_GROUPR_PORT;
    cfg.mcast_group = UDP_GROUPR_IP;
    cfg.iface_ip = UDP_GROUPR_NET_CARD;
    cfg.recv_timeout_ms = 2000;
    udp_groupR_hndl = jxc_udp_creat(&cfg);
    if(!udp_groupR_hndl){
        JXC_ERROR("udp group recv err\n");
    }
    ret = pthread_create(&thread_udp_groupR, 0, thread_udp_groupR_fun, NULL);
}

void test_jxc_udp_destory(void)
{
    pthread_join(thread_udp, NULL);
    pthread_join(thread_udp_groupS, NULL);
    pthread_join(thread_udp_groupR, NULL);

    jxc_udp_destroy(udp_hndl);
    jxc_udp_destroy(udp_groupS_hndl);
    jxc_udp_destroy(udp_groupR_hndl);

}

//----------------------------------------------------------------------------------------------------jxc_tcp
void test_jxc_tcp(void)
{
    JXC_INFO("--------------------TEST JXC TCP\n");
    JXC_INFO("NULL\n");
}

void test_jxc_tcp_destory(void)
{

}

//----------------------------------------------------------------------------------------------------ALL DESTORY
void test_destory(void)
{
    //log destory
    test_jxc_log_destory();
    //fifo destory
    test_jxc_fifo_destory();
    //queue destory
    test_jxc_queue_destory();
    //udp destory
    test_jxc_udp_destory();
    //tcp destory
    test_jxc_tcp_destory();
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

    printf("test app close\n");
    is_running = false;
    test_destory();
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

    //quit
    // test_destory();
    while(is_running)
    {
        sleep(1);
    }
    
    printf("test finish\n");

    return 0;
}

