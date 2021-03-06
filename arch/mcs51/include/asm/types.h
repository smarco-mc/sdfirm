#ifndef __TYPES_MCS51_H_INCLUDE__
#define __TYPES_MCS51_H_INCLUDE__

#ifdef SDCC
/* Memory Model & Storage Classes:
 *
 * Small memory model is forced to put the default variables into the data
 * section.
 * We can use __near__ to put variables into a nearer storage 'pdata'
 * section and __far__ to put variables into a farer storage 'xdata'
 * section.
 */
#define __text__		__code
#define __near__		__pdata
#define __far__			__xdata

#ifndef __SDCC_LONG_LONG
typedef unsigned char uint64_t[8];
#define ARCH_HAVE_UINT64_T	1
#endif

#define __TEXT_TYPE__(__type__, __name__)	\
	typedef __text__ __type__ __name__
#endif

typedef void *caddr_t;
#define ARCH_HAVE_CADDR_T 1

#endif /* __TYPES_MCS51_H_INCLUDE__ */
