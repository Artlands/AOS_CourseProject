/*
 * _TRACE_C_
 *
 * GC64 Microcode Driver Tracing
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "dmc_ucode.h"

/* ---------------------------------------------- TRACE_READ */
extern void trace_read( int sz, uint64_t addr){

  /* TODO : make this block more efficient */
  if( sz%16 != 0 ){
    sz = ((sz/16)+1)*16;
    if( sz>128){sz=256;}
  }


  if( (__config.conf & CONF_TRACE) > 0 ){
    // coalesced memory, send to hmc-sim
    printf( "HMC_READ:%d:0x%016lx\n", sz, addr );
    // printf( "RD:8:0:0x%016lx\n", addr );
    // printf( "Coalesced\n" );
    dmctrace->type = 1;
    dmctrace->size = sz;
    dmctrace->update = 1;
    dmctrace->addr = addr;
  }

  /* update the counters */
  switch( sz ){
    case 16:
      h_RD16++;
      break;
    case 32:
      h_RD32++;
      break;
    case 48:
      h_RD48++;
      break;
    case 64:
      h_RD64++;
      break;
    case 80:
      h_RD80++;
      break;
    case 96:
      h_RD96++;
      break;
    case 112:
      h_RD112++;
      break;
    case 128:
      h_RD128++;
      break;
    case 256:
      h_RD256++;
      break;
    default:
      break;
  }

}

/* ---------------------------------------------- TRACE_WRITE */
extern void trace_write(int sz, uint64_t addr){

  /* TODO : make this block more efficient */
  if( sz%16 != 0 ){
    sz = ((sz/16)+1)*16;
    if( sz>128){sz=256;}
  }

  if( (__config.conf & CONF_TRACE) > 0 ){
    printf( "HMC_WRITE:%d:0x%016lx\n", sz, addr );
   	// printf( "WR:8:0:0x%016lx\n",addr );
    // printf( "Coalesced\n" );
    dmctrace->type = 0;
    dmctrace->size = sz;
    dmctrace->update = 1;
    dmctrace->addr = addr;
  }

  /* update the counters */
  switch( sz ){
    case 16:
      h_WR16++;
      break;
    case 32:
      h_WR32++;
      break;
    case 48:
      h_WR48++;
      break;
    case 64:
      h_WR64++;
      break;
    case 80:
      h_WR80++;
      break;
    case 96:
      h_WR96++;
      break;
    case 112:
      h_WR112++;
      break;
    case 128:
      h_WR128++;
      break;
    case 256:
      h_WR256++;
      break;
    default:
      break;
  }
}

/* ---------------------------------------------- TRACE */
extern void trace() {
  //if( (__config.conf & DEEP_TRACE) > 0 ){
    //printf( "total_read_bytes:total_write_bytes:read_timeout:write_timeout\t%d:%d:%d:%d\n",
      //      total_read_bytes, total_write_bytes, read_timeout, write_timeout );
  //}
	;
}
/* EOF */
