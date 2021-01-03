#ifndef _L0_USARTH_
#define _L0_USARTH_

#include "sio.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _l031k6Usart {
        SioBase super;
        struct {
            SIO_PARAM param;
        }Protected;
    }l031k6Usart;

   void l031k6UsartCreate(l031k6Usart* sio, SIO_PARAM *param);
   void l031k6UsartGetParam(l031k6Usart* sio, SIO_PARAM *param);

#ifdef __cplusplus
}
#endif

#endif  /* sioh */