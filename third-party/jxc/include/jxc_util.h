#ifndef __JXC_UTIL_H
#define __JXC_UTIL_H

/**
 * 
*/

#define JXC_ABS(x)                      ((x) >= 0 ? (x) : (-(x)))
#define JXC_MAX2(x, y)                  ((x) > (y) ? (x) : (y))
#define JXC_MIN2(x, y)                  ((x) < (y) ? (x) : (y))
#define JXC_MAX3(x, y, z)               ((x) > (y) ? JXC_MAX2(x, z) : JXC_MAX2(y, z))
#define JXC_MIN3(x, y, z)               ((x) < (y) ? JXC_MIN2(x, z) : JXC_MIN2(y, z))
#define JXC_ALIGN_UP(x, a)              ((((x) + ((a) - 1)) / (a)) * (a))
#define JXC_ALIGN_DOWN(x, a)            (((x) / (a)) * (a))
#define JXC_VALUE_BETWEEN(x, min, max)  (((x) >= (min)) && ((x) <= (max)))

#ifdef __cplusplus
extern "C" {
#endif

double jxc_get_us(void);
double jxc_get_ms(void);
double jxc_get_s(void);


#ifdef __cplusplus
}
#endif


#endif

