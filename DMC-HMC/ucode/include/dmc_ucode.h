/*
 * _DMC_UCODE_H_
 *
 * GC64 DMC MICROCODE HEADER
 *
 */

#ifndef _DMC_UCODE_H_
#define _DMC_UCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "dmc_ucode_mem.h"

/*
 * struct mtrace
 * contains struct data for an individual memory trace
 */
struct mtrace{
  uint32_t proc;			/* proc id */
  MEMOP op;				/* memory operation type */
  uint64_t addr;			/* base address of the request */
};

/* FOR MEMORY POWER TRACE*/
struct mptrace {
  int type;       /* 0 = WR 1 = RD */
  int size;       /* memory operation size */
  int update;     /* 0 = NOT update, 1 = update*/
  int end;        /* 0 = NOT end, 1 = end*/
  uint64_t addr;  /* base address of the request */
};

struct hmcrqst{
  HMCOP op;				/* HMC operation */
  uint64_t addr;			/* base address of the request */
};

/*
 * struct dmc
 * contains the dmc private config and data structures
 */
struct dmc{
  uint8_t	n_hmc_valid;		/* number of valid HMC request entries */
  uint64_t conf;			/* DMC configuration */

  struct hmcrqst hmc[16];
};


/********************** the structure  of the binary tree  */
typedef struct btree{
    struct mtrace req;                  /* riscv requests */
		uint64_t rqst_left;
		uint64_t rqst_right;
    struct btree* left;       	        /* left child */
    struct btree* right;	        /* right child */
    struct btree* parent;               /* parent */
}btree;


typedef struct hmc_list{

    struct hmcrqst req;
    struct hmc_list *next;
}hmc_list;

/*
 * config macros
 *
 */
#define	CONF_TRACE	0x0000000000000001	/* -- enables tracing */
#define	DEEP_TRACE	0x0000000000000002	/* -- enables deep tracing */

/*
 * Globals
 *
 */
extern struct mptrace *dmctrace;

extern struct dmc __config;
//extern int total_read_bytes[MAX_WRITE_TIME*2];
//extern int total_write_bytes[MAX_WRITE_TIME*2];
extern uint64_t total_read_bytes[MAX_WRITE_TIME*2];
extern uint64_t total_write_bytes[MAX_WRITE_TIME*2];
extern int read_timeout;
extern int write_timeout;
extern int num_nodes;
extern int num_free;
extern btree root[MAX_WRITE_TIME*2];
extern btree *nodes;
extern btree *free_list;

/* stats */
extern uint64_t h_WR16;
extern uint64_t h_WR32;
extern uint64_t h_WR48;
extern uint64_t h_WR64;
extern uint64_t h_WR80;
extern uint64_t h_WR96;
extern uint64_t h_WR112;
extern uint64_t h_WR128;
extern uint64_t h_WR256;
extern uint64_t h_RD16;
extern uint64_t h_RD32;
extern uint64_t h_RD48;
extern uint64_t h_RD64;
extern uint64_t h_RD80;
extern uint64_t h_RD96;
extern uint64_t h_RD112;
extern uint64_t h_RD128;
extern uint64_t h_RD256;

/*
 * Functions
 *
 */
extern void trace_read( int sz, uint64_t addr);
extern void trace_write( int sz, uint64_t addr );
extern void trace();

// extern int print_power(struct hmcsim_t *hmc)

#ifdef __cplusplus
}/* extern C */
#endif

#endif
/* EOF */
