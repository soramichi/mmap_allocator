#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "mmap_allocator.h"

static const int MAP_APPROXIMATE = 4;

static const size_t max_size = 1024 * 1024 * 32; // 32 MB

typedef struct {
  size_t capacity, current_used, total_size;
  void** regions;
  size_t* sizes;
  void* memory;
  int mmap_flags;
} Handler;

static Handler _handler_normal, _handler_approximate;

static void do_mm_init(Handler* _handler){
  _handler->capacity = 32; // for now, 32 regions
  _handler->current_used = 0;
  _handler->total_size = 0;

  // note we use normal malloc here because these are meta-data
  _handler->regions = (void**)malloc(sizeof(void*) * _handler->capacity);
  _handler->sizes = (size_t*)malloc(sizeof(size_t) * _handler->capacity);

  {
    int fd;

    fd = open("/dev/zero", O_RDONLY);
    _handler->memory = mmap(NULL, max_size, PROT_WRITE , _handler->mmap_flags, fd, 0);

    // XXX: No error check here because giving MAP_APPROXIMATE in a real machine
    // always returns MAP_FAILED, but it is as intended.

    close(fd);
  }

  srand(time(NULL));
}

static void* do_mm_malloc(Handler* _handler, size_t size){
  if (_handler->total_size + size >= max_size) {
    fprintf(stderr, "Out of memory: total_size exceeds max_size.\n");
    exit(1);
  }

  void* mem = _handler->memory + _handler->total_size;
  _handler->total_size += size;

  if(_handler->current_used == _handler->capacity){
    // note we use normal realloc here because these are meta-data
    _handler->regions = (void**)realloc(_handler->regions, sizeof(void*) * _handler->capacity * 2);
    _handler->sizes = (size_t*)realloc(_handler->sizes, sizeof(size_t) * _handler->capacity * 2);
    _handler->capacity *= 2;
  }

  _handler->regions[_handler->current_used] = mem;
  _handler->sizes[_handler->current_used] = size;
  _handler->current_used++;

  return mem;
}

static void* do_mm_calloc(Handler* _handler, size_t nmemb, size_t size){
  // allocate
  void* mem = do_mm_malloc(_handler, nmemb * size);

  // initialize with 0?
  // memset(mem, 0, nmemb * size);

  return mem;
}

/******** init (automatically called before the `main') *****************/
static void __attribute__((constructor)) mm_init() {
  _handler_normal.mmap_flags = MAP_PRIVATE;
  do_mm_init(&_handler_normal);

#if defined(USE_APPROXIMATE)
  _handler_approximate.mmap_flags = MAP_PRIVATE | MAP_APPROXIMATE;
  do_mm_init(&_handler_approximate);
#endif
}

/******** public functions  ********************************************/
void* mm_malloc_normal(size_t size) {
  return do_mm_malloc(&_handler_normal, size);
}

void* mm_calloc_normal(size_t nmemb, size_t size) {
  return do_mm_calloc(&_handler_normal, nmemb, size);
}

void* mm_malloc_approximate(size_t size) {
  return do_mm_malloc(&_handler_approximate, size);
}

void* mm_calloc_approximate(size_t nmemb, size_t size) {
  return do_mm_calloc(&_handler_approximate, nmemb, size);
}

void* mm_realloc_normal(void* ptr, size_t size){
  // do nothing
  return NULL;
}

void* mm_realloc_approximate(void* ptr, size_t size){
  // do nothing
  return NULL;
}

void mm_free_normal(void* ptr){
  // do nothing
}

void mm_free_approximate(void* ptr){
  // do nothing
}
