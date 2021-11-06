#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stdint.h>
#include <string.h>

#define POOLSTART 0
#define NOBLOCK 0

#include "mempool_conf.h"

struct memblock {
    memaddress begin;
    memaddress size;
    memhandle nextblock;
};

typedef struct {
    struct memblock blocks[MEMPOOL_NUM_MEMBLOCKS+1]; 
} MemoryPool;

// Funciones
void MemoryPool_init(MemoryPool *mp);
memhandle MemoryPool_allocBlock(MemoryPool *mp, memaddress);
void MemoryPool_freeBlock(MemoryPool *mp, memhandle);
void MemoryPool_resizeBlock(MemoryPool *mp, memhandle handle, memaddress position, memaddress size);
memaddress MemoryPool_blockSize(MemoryPool *mp, memhandle);

#endif /* MEMPOOL_H */