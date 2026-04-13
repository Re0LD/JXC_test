#ifndef __JXC_TYPE_H
#define __JXC_TYPE_H

#ifndef true
#define true    1
#endif

#ifndef false
#define false   0
#endif

typedef enum{
    jxcStatusParaInvalid        = -2,       //参数无效
    jxcStatusFailed             = -1,       //操作失败
    jxcStatusSuccess            = 0,        //操作成功

    jxcStatusLogFileExist       = -11,      //file存在
    jxcStatusLogFileOpenErr     = -12,      //file打开失败

    jxcStatusFifoTimeout        = -21,      //fifo超时
    jxcStatusFifoParaErr        = -22,      //fifo参数异常

}jxc_status;


#endif

