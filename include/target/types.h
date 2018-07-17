#ifndef __TYPES_H_INCLUDE__
#define __TYPES_H_INCLUDE__

#ifndef __ASSEMBLY__
typedef signed char		int8_t;
typedef unsigned char		uint8_t;
typedef signed short		int16_t;
typedef unsigned short		uint16_t;

#include <asm/types.h>

typedef uint8_t			boolean;
typedef uint16_t		wchar_t;
#ifndef ARCH_HAVE_UINT32_T
typedef signed int		int32_t;
typedef unsigned int		uint32_t;
#endif
#ifndef ARCH_HAVE_UINT64_T
typedef signed long long	int64_t;
typedef unsigned long long	uint64_t;
#endif
#ifndef ARCH_HAVE_CADDR_T
typedef unsigned long		caddr_t;
#endif
#ifndef ARCH_HAVE_SIZE_T
typedef unsigned long		size_t;
#endif
#ifndef ARCH_HAVE_LOFF_T
typedef unsigned long		loff_t;
#endif

#ifndef NULL
#define	NULL			((void *)0L)
#endif /* NULL */

#define false			(unsigned char)0
#define true			!false

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER)	__compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)
#endif
#endif /* __ASSEMBLY__ */

#define _BV(bit)		(1 << (bit))
#define _SET_FV(name, value)	\
	(((value) & (name##_MASK)) << (name##_OFFSET))
#define _GET_FV(name, value)	\
	(((value) >> (name##_OFFSET)) & (name##_MASK))
/* Default to _SET_FV() as by default _FV() is used to generate field
 * values to be written to the registers.
 */
#define _FV(name, value)	_SET_FV(name, value)

#ifndef __ASSEMBLY__
__TEXT_TYPE__(char, text_char_t);
__TEXT_TYPE__(const uint8_t, text_byte_t);
__TEXT_TYPE__(uint16_t, text_word_t);
__TEXT_TYPE__(uint32_t, text_quad_t);
#endif /* __ASSEMBLY__ */

#endif /* __TYPES_H_INCLUDE__ */
