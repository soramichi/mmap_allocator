#if !defined(_MMAP_ALLOCATOR_H)
#define _MMAP_ALLOCATOR_H

#include <stdio.h> // size_t

void* mm_malloc_normal(size_t size);
void* mm_calloc_normal(size_t nmemb, size_t size);
void* mm_realloc_normal(void* ptr, size_t size);
void mm_free_normal(void* ptr);

void* mm_malloc_approximate(size_t size);
void* mm_calloc_approximate(size_t nmemb, size_t size);
void* mm_realloc_approximate(void* ptr, size_t size);
void mm_free_approximate(void* ptr);

// c_region and a_region must be on the cache!! (how can we guarantee this??)
// -O3 puts `__index' and `__target' onto registers thus there is no extra memory ld/st for them.
#define fetch(c_value, c_ptr, c_region, a_region) do {	\
    int __index = ((c_ptr) - (c_region));		\
    typeof(a_region) __target = (a_region) + __index;	\
    __builtin_prefetch(__target);			\
    c_value = *(c_ptr);					\
  } while(0)

#define fetch2(c_value, c_ptr, c_region, a_region, a_region2) do {	\
    int __index  = ((c_ptr) - (c_region));				\
    typeof(a_region) __target = (a_region) + __index;			\
    __builtin_prefetch(__target);					\
    typeof(a_region) __target2 = (a_region2) + __index;			\
    __builtin_prefetch(__target2);					\
    c_value = *(c_ptr);							\
  } while(0)

#endif
