#ifndef __JXC_LOG_H
#define __JXC_LOG_H

#include <stdio.h>
#include "jxc_type.h"

//----------全局开关(注释掉可禁用所有日志)----------
#define ENABLE_PRINTF

//----------各等级日志开关(可分别控制)----------
#define ENABLE_LOG_DEBUG    1
#define ENABLE_LOG_INFO     1
#define ENABLE_LOG_ERROR    1
#define DEFAULT_FILE_NAME   "log_file.txt"

#ifdef __cplusplus
extern "C" {
#endif

jxc_status jxc_log_init(char *filename);
jxc_status jxc_log_close(void);
jxc_status jxc_log_write(const char *fmt, ...);

#ifdef ENABLE_PRINTF
    #if ENABLE_LOG_DEBUG
        #define JXC_DEBUG(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
    #else
        #define JXC_DEBUG(fmt, ...) ((void)0)
    #endif

    #if ENABLE_LOG_INFO
        #define JXC_INFO(fmt, ...)  printf("[INFO] " fmt, ##__VA_ARGS__)
    #else
        #define JXC_INFO(fmt, ...)  ((void)0)
    #endif

    #if ENABLE_LOG_ERROR
        #define JXC_ERROR(fmt, ...) printf("[ERROR] " fmt, ##__VA_ARGS__)
    #else
        #define JXC_ERROR(fmt, ...) ((void)0)
    #endif
#else
    #define JXC_DEBUG(fmt, ...) ((void)0)
    #define JXC_INFO(fmt, ...)  ((void)0)
    #define JXC_ERROR(fmt, ...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif


#endif

