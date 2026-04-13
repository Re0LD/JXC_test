#ifndef __JXC_FIFO_H
#define __JXC_FIFO_H

#include <stdint.h>
#include "jxc.h"

typedef void* jxc_fifo_handle;

#ifdef __cplusplus
extern "C" {
#endif

jxc_fifo_handle jxc_fifo_create(int maxLen);
jxc_status jxc_fifo_destroy(jxc_fifo_handle handle);
uint32_t jxc_fifo_write(jxc_fifo_handle handle, uint8_t *data, uint32_t len);
uint32_t jxc_fifo_read(jxc_fifo_handle handle, uint8_t *data, uint32_t len);
jxc_status jxc_fifo_clear(jxc_fifo_handle handle);
uint32_t jxc_fifo_get_count(jxc_fifo_handle handle);


#ifdef __cplusplus
}
#endif


#endif

