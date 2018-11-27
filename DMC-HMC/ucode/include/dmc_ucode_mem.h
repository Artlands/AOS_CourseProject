/*
 * _DMC_UCODE_MEM_H_
 *
 * GC64 DMC MICROCODE MEMORY HEADER
 *
 */

#ifndef _DMC_UCODE_MEM_H_
#define _DMC_UCODE_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifndef MAX_READ_TIME
#define MAX_READ_TIME   16
#endif

#ifndef MAX_WRITE_TIME
#define MAX_WRITE_TIME  16
#endif


/*
 * Enums for memory operations
 * least significant 16 bits indicate the number of bytes (slots)
 * in the request
 * the most significant 8 bits indicates whether this is a read
 * request (0) or a write request (1)
 *
 */
typedef enum{
	RD1	= 0x001,	/* -- 1-byte read */
	RD2	= 0x002,	/* -- 2-byte read */
	RD4	= 0x004,	/* -- 4-byte read */
	RD8	= 0x008,	/* -- 8-byte read */
	RD16	= 0x010,	/* -- 16-byte read */
	RD32,
	RD64,
	RD80,
	RD96,
	RD112,
	RD128,

	WR1	= 0x101,	/* -- 1-byte write */
	WR2	= 0x102,	/* -- 2-byte write */
	WR4	= 0x104,	/* -- 4-byte write */
	WR8	= 0x108,	/* -- 8-byte write */
	WR16	= 0x110,	/* -- 16-byte write */
	WR32,
	WR64,
	WR80,
	WR96,
	WR112,
	WR128,

	FLOW_NULL

}MEMOP;

/*
 * Enums for the HMC memory operations
 *
 */
typedef enum{           /**** HMCOP value times 16 = the number of bytes operated */
	HMC_RD16	= 0x01,		/* -- HMC 16-byte read */
	HMC_RD32	= 0x02,		/* -- HMC 32-byte read */
	HMC_RD48	= 0x03,		/* -- HMC 48-byte read */
	HMC_RD64	= 0x04,		/* -- HMC 64-byte read */
	HMC_RD80	= 0x05,		/* -- HMC 80-byte read */
	HMC_RD96	= 0x06,		/* -- HMC 96-byte read */
	HMC_RD112	= 0x07,		/* -- HMC 112-byte read */
	HMC_RD128	= 0x08,		/* -- HMC 128-byte read */
	HMC_WR16	= 0x11,		/* -- HMC 16-byte write */
	HMC_WR32	= 0x12,		/* -- HMC 32-byte write */
	HMC_WR48	= 0x13,		/* -- HMC 48-byte write */
	HMC_WR64	= 0x14,		/* -- HMC 64-byte write */
	HMC_WR80	= 0x15,		/* -- HMC 80-byte write */
	HMC_WR96	= 0x16,		/* -- HMC 96-byte write */
	HMC_WR112	= 0x17,		/* -- HMC 112-byte write */
	HMC_WR128	= 0x18,		/* -- HMC 128-byte write */
	HMC_NULL	= 0x00,		/* -- NULL HMC OP */
}HMCOP;

#ifdef __cplusplus
}/* extern C */
#endif

#endif
/* EOF */
