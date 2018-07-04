#if !defined(_BMALLOC_H)
#define _BMALLOC_H

#include <stdio.h> // size_t
#include <inttypes.h> // uintXX_t

void mm_init();
void mm_finalize();
void* mm_malloc(size_t size);
void* mm_calloc(size_t nmemb, size_t size);
void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);

#endif
