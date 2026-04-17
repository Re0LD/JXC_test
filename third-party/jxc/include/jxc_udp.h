#ifndef __JXC_UDP_H
#define __JXC_UDP_H

/**
 * 默认IPV4 UDP
 * 未实现线程保护!!!
*/

#include <stdint.h>
#include <jxc_type.h>

typedef void* jxc_udp_handle;

typedef struct{
    char        *local_ip;          //绑定IP(NULL=INADDR_ANY)
    uint16_t    local_port;         //本地端口
    char        *mcast_group;       //组播地址(NULL=单播)
    char        *iface_ip;          //绑定网卡IP(用于组播接收)
    int         recv_timeout_ms;    //接收超时(-1=阻塞)
}jxc_udp_cfg_t;

#ifdef __cplusplus
extern "C" {
#endif

jxc_udp_handle jxc_udp_creat(jxc_udp_cfg_t *cfg);
void jxc_udp_destroy(jxc_udp_handle handle);
int jxc_udp_send(jxc_udp_handle handle, char *dst_ip, uint16_t dst_port, uint8_t *data, uint32_t len);
int jxc_udp_recv(jxc_udp_handle handle, uint8_t *buf, uint32_t buf_len, char *src_ip, uint16_t *src_port);
jxc_status jxc_udp_set_mcast_if(jxc_udp_handle handle, char *iface_ip);  //指定组播发送网卡

#ifdef __cplusplus
}
#endif



#endif
