#ifndef __DEBUG_H__
#define __DEBUG_H__
#define USE_DEBUG


#ifdef USE_DEBUG

#define PRINTF printf

#define DEBUG(arg...) do{								\
	PRINTF("\r\n[%s:%s:%d] ", __FILE__,__FUNCTION__, __LINE__);		\
	PRINTF(arg);										\
}while(0)

#else

#define PRINTF
#define DEBUG

#endif


#endif
