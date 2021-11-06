#ifndef MEMPOOLCONF_H
#define MEMPOOLCONF_H
#include "ipethernet-conf.h"
#include "ipopt.h"
#include "enc28j60.h"
#include <stdint.h>

typedef uint16_t memaddress;
typedef uint8_t memhandle;

#if IP_SOCKET_NUMPACKETS and IP_CONNS
#define NUM_TCP_MEMBLOCKS (IP_SOCKET_NUMPACKETS*2)*IP_CONNS
#else
#define NUM_TCP_MEMBLOCKS 0
#endif

#if IP_UDP and IP_UDP_CONNS
#define NUM_UDP_MEMBLOCKS ((2+IP_UDP_BACKLOG)*IP_UDP_CONNS)
#else
#define NUM_UDP_MEMBLOCKS 0
#endif

#define MEMPOOL_NUM_MEMBLOCKS (NUM_TCP_MEMBLOCKS+NUM_UDP_MEMBLOCKS)

#define MEMPOOL_STARTADDRESS TXSTART_INIT+1
#define MEMPOOL_SIZE TXSTOP_INIT-TXSTART_INIT

void enc28J60_mempool_block_move_callback(memaddress,memaddress,memaddress);

#define MEMPOOL_MEMBLOCK_MV(dest,src,size) enc28J60_mempool_block_move_callback(dest,src,size)

#endif