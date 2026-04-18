#ifndef __JXC_TYPE_H
#define __JXC_TYPE_H

#ifndef true
#define true    1
#endif

#ifndef false
#define false   0
#endif

typedef enum{
    jxcStatusSuccess            = 0,        //操作成功
    jxcStatusFailed             = -1,       //操作失败
    jxcStatusParaInvalid        = -2,       //参数无效
    jxcStatusTimeout            = -3,       //超时

    jxcStatusLogFileExist       = -11,      //file存在
    jxcStatusLogFileOpenErr     = -12,      //file打开失败

}jxc_status;


#endif

