all:
	gcc -O3 gemv.c -o gemv -DDEBUG
	gcc -O3 gemv_block.c ../mmap_allocator.c -o gemv_block -DDEBUG

clean:
	rm -f gemv gemv_block gemv_block_bmalloc *~

