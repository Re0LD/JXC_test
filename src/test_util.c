#include "test_util.h"
#include "jxc_util.h"

#include <stdio.h>
#include <unistd.h>

/**
 * test util lib
*/

void test_jxc_util(void)
{
    printf("--------------------TEST JXC UTIL\n");
    double tempus = jxc_get_us();
    double tempms = jxc_get_ms();
    double temps = jxc_get_s();
    usleep(1000*2);
    printf("set sleep 1000*2 us with real %f us\n",jxc_get_us()-tempus);
    printf("set sleep 1000*2 us with real %f ms\n",jxc_get_ms()-tempms);
    printf("set sleep 1000*2 us with real %f s\n",jxc_get_s()-temps);

    printf("x=-1 y=1 after ABS a=%d b=%d\n",JXC_ABS(-1), JXC_ABS(1));
    printf("x=-1 y=1 after MAX2 res=%d\n",JXC_MAX2(-1, 1));
    printf("x=-1 y=1 after MIN2 res=%d\n",JXC_MIN2(-1, 1));
    printf("x=-1 y=1 z=10 after MAX3 res=%d\n",JXC_MAX3(-1, 1, 10));
    printf("x=-1 y=1 z=10 after MIN3 res=%d\n",JXC_MIN3(-1, 1, 10));
    printf("x=44 y=16 after ALIGN_UP res=%d\n",JXC_ALIGN_UP(44, 16));
    printf("x=44 y=16 after ALIGN_DOWN res=%d\n",JXC_ALIGN_DOWN(44, 16));
    printf("value=10 min=0 max=20 after BETWEEN res=%d\n",JXC_VALUE_BETWEEN(10, 0, 20));
    printf("value=-1 min=0 max=20 after BETWEEN res=%d\n",JXC_VALUE_BETWEEN(-1, 0, 20));
}
