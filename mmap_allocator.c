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

static const size_t max_size = 1024 * 1024 * 64; // 64 MB

static struct {
  size_t capacity, current_used, total_size;
  void** regions;
  size_t* sizes;
  void* memory;
} _handler;

void mm_init(){
  _handler.capacity = 32; // for now, 32 regions
  _handler.current_used = 0;
  _handler.total_size = 0;

  // note we use normal malloc here because these are meta-data
  _handler.regions = (void**)malloc(sizeof(void*) * _handler.capacity);
  _handler.sizes = (size_t*)malloc(sizeof(size_t) * _handler.capacity);

  {
    int fd;
    char* addr;    

    fd = open("/dev/zero", O_RDONLY);
    _handler.memory = mmap(NULL, max_size, PROT_WRITE , MAP_PRIVATE, fd, 0);

    close(fd);
  }

  srand(time(NULL));
}

void mm_finalize(){
  // do nothing
}

void* mm_malloc(size_t size){
  if (_handler.total_size + size >= max_size) {
    fprintf(stderr, "Out of memory: total_size exceeds max_size.\n");
    exit(1);
  }

  void* mem = _handler.memory + _handler.total_size;
  _handler.total_size += size;

  if(_handler.current_used == _handler.capacity){
    // note we use normal realloc here because these are meta-data
    _handler.regions = (void**)realloc(_handler.regions, sizeof(void*) * _handler.capacity * 2);
    _handler.sizes = (size_t*)realloc(_handler.sizes, sizeof(size_t) * _handler.capacity * 2);
    _handler.capacity *= 2;
  }

  _handler.regions[_handler.current_used] = mem;
  _handler.sizes[_handler.current_used] = size;
  _handler.current_used++;

  return mem;
}

void* mm_calloc(size_t nmemb, size_t size){
  // allocate
  void* mem = mm_malloc(nmemb * size);

  // initialize with 0?
  // memset(mem, 0, nmemb * size);

  return mem;
}

void* mm_realloc(void* ptr, size_t size){
  fprintf(stderr, "mm_realloc not implemented yet");
  return NULL;
}

void mm_free(void* ptr){
  int i = 0;

  for(i=0; i<_handler.current_used; i++){
    if(_handler.regions[i] == ptr){
      _handler.sizes[i] = 0;
      _handler.regions[i] == NULL;
      // no memory reclamation
    }
  }
}
