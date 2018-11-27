/*
 * _TRACE_C_
 *
 * GC64 Microcode Driver Tracing
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "dmc_ucode.h"

static int get_hmcmemop( int type, int nbytes, MEMOP *op ){

	if( type == 0 ){
		switch( nbytes )
		{
			case 1 :
				*op = WR16;
				break;
			case 2 :
				*op = WR16;
				break;
			case 4 :
				*op = WR16;
				break;
			case 8 :
				*op = WR16;
				break;
			case 16:
				*op = WR16;
				break;
			case 32:
				*op = WR32;
				break;
			case 48:
				*op = WR48;
				break;
			case 64:
				*op = WR64;
				break;
			case 80:
				*op = WR80;
				break;
			case 96:
				*op = WR96;
				break;
			case 112:
				*op = WR112;
				break;
			case 128:
				*op = WR128;
				break;
			// case 256:
			// 	*op = WR256;
			// 	break;
			default:
				return -1;
				break;
		}
		return 0;
	}else if( type == 1 ){
		switch( nbytes )
		{
			case 1 :
				*op = RD16;
				break;
			case 2 :
				*op = RD16;
				break;
			case 4 :
				*op = RD16;
				break;
			case 8 :
				*op = RD16;
				break;
			case 16:
				*op = RD16;
				break;
			case 32:
				*op = RD32;
				break;
			case 48:
				*op = RD48;
				break;
			case 64:
				*op = RD64;
				break;
			case 80:
				*op = RD80;
				break;
			case 96:
				*op = RD96;
				break;
			case 112:
				*op = RD112;
				break;
			case 128:
				*op = RD128;
				break;
			// case 256:
			// 	*op = RD256;
			// 	break;
			default:
				return -1;
				break;
		}
		return 0;
	}else if ( type == 2 ){
		*op = FLOW_NULL;
	}else{
		return -1;
	}
	return 0;
}

/* ---------------------------------------------- TRACE_READ */
extern void trace_read( int sz, uint64_t addr){
  MEMOP op_rd;

  /* TODO : make this block more efficient */
  if( sz%16 != 0 ){
    sz = ((sz/16)+1)*16;
    if( sz>128){sz=256;}
  }

  if( (__config.conf & CONF_TRACE) > 0 ){

    get_hmcmemop(1, sz, &op)

    //printf( "HMC_READ:%d:0x%016llx\n", sz, addr );
    // printf( "RD:8:0:0x%016llx\n", addr );
    /* send data to an extern object */
    hmctrace->proc = 0;
    hmctrace->op = op_rd;
    hmctrace->addr = addr;
    hmctrace->type = 1;
    hmctrace->update = 1;

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
  MEMOP op_wr;
  /* TODO : make this block more efficient */
  if( sz%16 != 0 ){
    sz = ((sz/16)+1)*16;
    if( sz>128){sz=256;}
  }

  if( (__config.conf & CONF_TRACE) > 0 ){
    get_hmcmemop(0, sz, &op)

    //printf( "HMC_READ:%d:0x%016llx\n", sz, addr );
    // printf( "RD:8:0:0x%016llx\n", addr );
    /* send data to an extern object */
    hmctrace->proc = 0;
    hmctrace->op = op_wr;
    hmctrace->addr = addr;
    hmctrace->type = 0;
    hmctrace->update = 1;
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
