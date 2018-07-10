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

#endif
