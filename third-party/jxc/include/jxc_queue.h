#ifndef __JXC_QUEUE_H
#define __JXC_QUEUE_H

/**
 * 
 * 
*/

#include <stdint.h>

typedef void* jxc_queue_handle;

#ifdef __cplusplus
extern "C" {
#endif

jxc_queue_handle jxc_queue_create(uint32_t max_num, uint32_t elem_size);
void jxc_queue_destroy(jxc_queue_handle handle);

int jxc_queue_enqueue(jxc_queue_handle handle, void *data, int timeout_ms);
int jxc_queue_dequeue(jxc_queue_handle handle, void *data, int timeout_ms);


void jxc_queue_is_empty();
void jxc_queue_is_full();
void jxc_queue_size();

void jxc_queue_clear();



#ifdef __cplusplus
}
#endif


#endif

