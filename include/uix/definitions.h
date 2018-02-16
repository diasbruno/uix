#ifndef __UIX_DEFINITIONS_H__
#define __UIX_DEFINITIONS_H__ 1

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define UNUSED(x) UNUSED_ ## x
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif

#endif // __UIX_DEFINITIONS_H__
