#ifndef __JXC_QUEUE_H
#define __JXC_QUEUE_H

/**
 * QUEUE 线程保护
 * 不建议在出队入队时destory
*/

#include <stdint.h>
#include <jxc_type.h>

typedef void* jxc_queue_handle;

#ifdef __cplusplus
extern "C" {
#endif

jxc_queue_handle jxc_queue_create(uint32_t max_num, uint32_t elem_size);
void jxc_queue_destroy(jxc_queue_handle handle);
jxc_status jxc_queue_enqueue(jxc_queue_handle handle, uint8_t *data, int timeout_ms);
jxc_status jxc_queue_dequeue(jxc_queue_handle handle, uint8_t *data, int timeout_ms);
jxc_status jxc_queue_get_latest(jxc_queue_handle handle, uint8_t *data, int timeout_ms);
int jxc_queue_get_cur_num(jxc_queue_handle handle);
void jxc_queue_clear(jxc_queue_handle handle);

#ifdef __cplusplus
}
#endif


#endif

