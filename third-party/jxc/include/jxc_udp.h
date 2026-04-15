#ifndef __JXC_UDP_H
#define __JXC_UDP_H

#include <stdint.h>

/**
 * 默认IPV4 UDP
 * 未实现线程保护!!!
*/

typedef void* jxc_udp_handle;

typedef struct {
    int8_t      *local_ip;          // 绑定IP（NULL=INADDR_ANY）
    uint16_t    local_port;         // 本地端口
    int8_t      *mcast_group;       // 组播地址（NULL=单播）
    int8_t      *iface_ip;          // 绑定网卡IP（用于组播/发送）
    int32_t     recv_timeout_ms;    // 接收超时（-1=阻塞）
}jxc_udp_cfg_t;


#ifdef __cplusplus
extern "C" {
#endif

jxc_udp_handle jxc_udp_creat(jxc_udp_cfg_t *cfg);
void jxc_udp_destroy(jxc_udp_handle handle);
int32_t jxc_udp_send(jxc_udp_handle handle, int8_t *dst_ip, uint16_t dst_port, void *data, uint32_t len);
int32_t jxc_udp_recv(jxc_udp_handle handle, void *buf, uint32_t buf_len, char *src_ip, uint16_t *src_port);

#ifdef __cplusplus
}
#endif



#endif
