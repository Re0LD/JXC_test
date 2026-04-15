#include "test_udp.h"
#include "jxc_udp.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

/**
 * test udp lib
 * 组播收发的port不应该一样，否则会导致后创建的socket bind异常
*/

static bool is_running = true;

//线程
pthread_t thread_udp;
pthread_t thread_udp_groupS;
pthread_t thread_udp_groupR;
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

void *thread_udp_fun(void *para)
{
    uint8_t data[1024] = {0};
    uint8_t ip[256] = {0};
    uint16_t port = 0;
    while(is_running)
    {
        if(jxc_udp_recv(udp_hndl, data, sizeof(data), ip, &port) > 0){
            printf("udp recv from %s:%d\n",ip,port);
            printf("udp recv: %s\n",data);
        }else{
            // printf("udp recv timeout\n");
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
            printf("udp recv from %s:%d\n",ip,port);
            printf("udp recv: %s\n",data);
        }else{
            // printf("udp group recv timeout\n");
        }
    }
    
}

void test_jxc_udp(void)
{
    printf("--------------------TEST JXC UDP\n");
    
    int ret = 0;

    //单播发送&接收
    //单播 必需 配置本地ip和port
    jxc_udp_cfg_t cfg;
    cfg.local_ip = UDP_LOCAL_IP;
    cfg.local_port = UDP_LOCAL_PORT;
    cfg.mcast_group = NULL;
    cfg.iface_ip = NULL;
    cfg.recv_timeout_ms = 2000;
    udp_hndl = jxc_udp_creat(&cfg);
    if(!udp_hndl){
        printf("udp send/recv err\n");
    }
    ret = pthread_create(&thread_udp, 0, thread_udp_fun, NULL);

    //组播发送
    //组播发送 需要 指定网卡，否则多网口可能异常
    cfg.local_ip = NULL;
    cfg.local_port = 0;
    cfg.mcast_group = NULL;
    cfg.iface_ip = NULL;
    cfg.recv_timeout_ms = -1;
    udp_groupS_hndl = jxc_udp_creat(&cfg);
    if(!udp_groupS_hndl){
        printf("udp group send err\n");
    }
    jxc_udp_set_mcast_if(udp_groupS_hndl, UDP_GROUPS_NET_CARD);
    ret = pthread_create(&thread_udp_groupS, 0, thread_udp_groupS_fun, NULL);

    //组播接收
    //组播接收 必需 配置本地端口(组播的端口) 组播网段 组播网卡
    cfg.local_ip = NULL;
    cfg.local_port = UDP_GROUPR_PORT;
    cfg.mcast_group = UDP_GROUPR_IP;
    cfg.iface_ip = UDP_GROUPR_NET_CARD;
    cfg.recv_timeout_ms = 2000;
    udp_groupR_hndl = jxc_udp_creat(&cfg);
    if(!udp_groupR_hndl){
        printf("udp group recv err\n");
    }
    ret = pthread_create(&thread_udp_groupR, 0, thread_udp_groupR_fun, NULL);
}

void test_jxc_udp_destory(void)
{
    is_running = false;

    pthread_join(thread_udp, NULL);
    pthread_join(thread_udp_groupS, NULL);
    pthread_join(thread_udp_groupR, NULL);

    jxc_udp_destroy(udp_hndl);
    jxc_udp_destroy(udp_groupS_hndl);
    jxc_udp_destroy(udp_groupR_hndl);
}


