#include "mempool.h"

#define POOLOFFSET 1

struct memblock MemoryPool_blocks[MEMPOOL_NUM_MEMBLOCKS+1];

/**
 * @brief 
 * 
 * @param mp 
 */
void MemoryPool_init(MemoryPool *mp) {
    memset(mp->blocks, 0, sizeof(mp->blocks));
    mp->blocks[POOLSTART].begin = MEMPOOL_STARTADDRESS;
    mp->blocks[POOLSTART].size = 0;
    mp->blocks[POOLSTART].nextblock = NOBLOCK;
}

/**
 * @brief 
 * 
 * @param mp 
 * @param size 
 * @return memhandle 
 */
memhandle MemoryPool_allocBlock(MemoryPool *mp, memaddress size) {
    memblock *best = NULL;
    memhandle cur = POOLSTART;
    memblock* block = &mp->blocks[POOLSTART];
    memaddress bestsize = MEMPOOL_SIZE + 1;

    do {
        memhandle next = block->nextblock;
        memaddress freesize = ( next == NOBLOCK ? mp->blocks[POOLSTART].begin + MEMPOOL_SIZE : mp->blocks[next].begin) - block->begin - block->size;
        if (freesize == size) {
            best = &mp->blocks[cur];
            goto found;
        }
        if (freesize > size && freesize < bestsize) {
            bestsize = freesize;
            best = &mp->blocks[cur];
        }
        if (next == NOBLOCK) {
            if (best)
            goto found;
            else
            goto collect;
        }
        block = &mp->blocks[next];
        cur = next;
    }
    while (true);

    collect: {
        cur = POOLSTART;
        block = &mp->blocks[POOLSTART];
        memhandle next;
        while ((next = block->nextblock) != NOBLOCK)
        {
            memaddress dest = block->begin + block->size;
            memblock* nextblock = &mp->blocks[next];
            memaddress* src = &nextblock->begin;
            if (dest != *src)
            {
    #ifdef MEMPOOL_MEMBLOCK_MV
                MEMPOOL_MEMBLOCK_MV(dest,*src,nextblock->size);
    #endif
                *src = dest;
            }
            block = nextblock;
        }
        if (mp->blocks[POOLSTART].begin + MEMPOOL_SIZE - block->begin - block->size >= size)
        best = block;
        else
        goto notfound;
    }

    found:
    {
        block = &mp->blocks[POOLOFFSET];
        for (cur = POOLOFFSET; cur < MEMPOOL_NUM_MEMBLOCKS + POOLOFFSET; cur++)
        {
            if (block->size)
            {
                block++;
                continue;
            }
            memaddress address = best->begin + best->size;
    #ifdef MEMBLOCK_ALLOC
            MEMBLOCK_ALLOC(address,size);
    #endif
            block->begin = address;
            block->size = size;
            block->nextblock = best->nextblock;
            best->nextblock = cur;
            return cur;
        }
    }

    notfound: return NOBLOCK;
}

/**
 * @brief 
 * 
 * @param mp 
 */
void MemoryPool_freeBlock(MemoryPool *mp, memhandle handle) {
    if (handle == NOBLOCK)
        return;
    memblock *b = &mp->blocks[POOLSTART];

    do {
        memhandle next = b->nextblock;
        if (next == handle) {
            memblock *f = &mp->blocks[next];
    #ifdef MEMBLOCK_FREE
            MEMBLOCK_FREE(f->begin,f->size);
    #endif
            b->nextblock = f->nextblock;
            f->size = 0;
            f->nextblock = NOBLOCK;
            return;
        }
        if (next == NOBLOCK)
        return;
        b = &mp->blocks[next];
    }
    while (true);
}

void MemoryPool_resizeBlock(MemoryPool *mp, memhandle handle, memaddress position, memaddress size) {
    memblock *block = mp->blocks[handle];
    block->begin += position;
    block->size = size;
}

/**
 * @brief 
 * 
 * @param mp 
 * @return memaddress 
 */
memaddress MemoryPool_blockSize(MemoryPool *mp, memhandle handle) {
    return mp->blocks[handle].size;
}