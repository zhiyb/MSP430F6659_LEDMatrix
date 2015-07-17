#ifndef CC3000_H
#define CC3000_H

#include "CC3000HostDriver/wlan.h"

#ifdef  __cplusplus
extern "C" {
#endif

void cc3000_init(tWlanCB sWlanCB);
INT16 cc3000_read(const INT32 sockfd, void *buf, INT32 len);
INT16 cc3000_write(const INT32 sockfd, const void *buf, INT32 len);

#ifdef  __cplusplus
}
#endif

#endif
