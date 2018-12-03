/*
 * _DRIVER_C_
 *
 * GC64-DMC TESTBENCH DRIVER
 *
 * DRIVES THE DMC TEST ENGINE
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include "dmc_ucode.h"
#include "hmc_sim.h"

//#define DEBUG
/* ----------------------------------------------- FUNCTION PROTOTYPES */
extern void ucode_init( uint64_t conf );
extern void ucode_insert();
extern void ucode_opt_insert();
extern void ucode_flush();
extern void ucode_free();
extern void hmc_init(hmc_list *node);
extern void hmc_expire();
extern hmc_list *head;
extern hmc_list *tail;
extern int tree_cnt[32];
extern uint64_t accelerated_page;
/* ----------------------------------------------- GLOBALS */
struct mptrace *dmctrace;

uint64_t page_num;

uint64_t c_RD1;
uint64_t c_RD2;
uint64_t c_RD4;
uint64_t c_RD8;
uint64_t c_RD16;
uint64_t c_WR1;
uint64_t c_WR2;
uint64_t c_WR4;
uint64_t c_WR8;
uint64_t c_WR16;

uint64_t h_WR16;
uint64_t h_WR32;
uint64_t h_WR48;
uint64_t h_WR64;
uint64_t h_WR80;
uint64_t h_WR96;
uint64_t h_WR112;
uint64_t h_WR128;
uint64_t h_WR256;
uint64_t h_RD16;
uint64_t h_RD32;
uint64_t h_RD48;
uint64_t h_RD64;
uint64_t h_RD80;
uint64_t h_RD96;
uint64_t h_RD112;
uint64_t h_RD128;
uint64_t h_RD256;

/* ----------------------------------------------- INIT_STATS */
static void init_stats(){
  c_RD1 = 0x00ll;
  c_RD2 = 0x00ll;
  c_RD4 = 0x00ll;
  c_RD8 = 0x00ll;
  c_RD16= 0x00ll;
  c_WR1 = 0x00ll;
  c_WR2 = 0x00ll;
  c_WR4 = 0x00ll;
  c_WR8 = 0x00ll;
  c_WR16= 0x00ll;

  h_WR16= 0x00ll;
  h_WR32= 0x00ll;
  h_WR48= 0x00ll;
  h_WR64= 0x00ll;
  h_WR80= 0x00ll;
  h_WR96= 0x00ll;
  h_WR112=0x00ll;
  h_WR128=0x00ll;
  h_WR256=0x00ll;
  h_RD16= 0x00ll;
  h_RD32= 0x00ll;
  h_RD48= 0x00ll;
  h_RD64= 0x00ll;
  h_RD80= 0x00ll;
  h_RD96= 0x00ll;
  h_RD112=0x00ll;
  h_RD128=0x00ll;
  h_RD256=0x00ll;
}

/* ---------------------------------------------------- ZERO_PACKET */
/* ZERO_PACKET */

static void zero_packet( uint64_t *packet ) {
	uint64_t i = 0x00ll;
	for( i=0; i<HMC_MAX_UQ_PACKET; i++ ){
		packet[i] = 0x00ll;
	}
	return ;
}

/* ----------------------------------------------- PRINT_POWER */
static int print_power(struct hmcsim_t *hmc) {

	if( hmc == NULL ){
    return -1;
  }else if( hmc->tfile == NULL ){
    return -1;
	}
  /*
   * write out the total trace values for power on this clock cycle
   *
   */
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_PHY_POWER : ",
                         hmc->power.t_link_phy );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_LOCAL_ROUTE_POWER : ",
                         hmc->power.t_link_local_route );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_REMOTE_ROUTE_POWER : ",
                         hmc->power.t_link_remote_route );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_RQST_SLOT_POWER : ",
                         hmc->power.t_xbar_rqst_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_RSP_SLOT_POWER : ",
                         hmc->power.t_xbar_rsp_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_ROUTE_EXTERN_POWER : ",
                         hmc->power.t_xbar_route_extern);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_RQST_SLOT_POWER : ",
                         hmc->power.t_vault_rqst_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_RSP_SLOT_POWER : ",
                         hmc->power.t_vault_rsp_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_CTRL_POWER : ",
                         hmc->power.t_vault_ctrl);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%"PRIu64"\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_ROW_ACCESS_POWER : ",
                         hmc->power.t_row_access);
		return 0;
};
/* ----------------------------------------------- PRINT_STATS */
static void print_stats(){
        uint64_t total_r_in = 0x00ll;
        uint64_t total_w_in = 0x00ll;
        uint64_t total_in   = 0x00ll;
        uint64_t total_r_out= 0x00ll;
        uint64_t total_w_out= 0x00ll;
        uint64_t total_out  = 0x00ll;
      	uint64_t data_in    = 0x00ll;
      	uint64_t data_out   = 0x00ll;

        total_r_in =  c_RD1 + c_RD2 + c_RD4 + c_RD8 + c_RD16;
        total_w_in =  c_WR1 + c_WR2 + c_WR4 + c_WR8 + c_WR16;
        total_in   =  total_r_in + total_w_in;

        total_r_out=  h_RD16+h_RD32+h_RD48+h_RD64+h_RD80+h_RD96+
                      h_RD112+h_RD128+h_RD256;
        total_w_out=  h_WR16+h_WR32+h_WR48+h_WR64+h_WR80+h_WR96+
                      h_WR112+h_WR128+h_WR256;
        total_out  =  total_r_out + total_w_out;

	data_in	   =  c_RD1 + c_RD2*2 + c_RD4*4 + c_RD8*8 + c_RD16*16 +
		      c_WR1 + c_WR2*2 + c_WR4*4 + c_WR8*8 + c_WR16*16;

	data_out   =  h_RD16*16+h_RD32*32+h_RD48*48+h_RD64*64+h_RD80*80
		      +h_RD96*96+h_RD112*112+h_RD128*128+h_RD256*256 +
		      h_WR16*16+h_WR32*32+h_WR48*48+h_WR64*64+h_WR80*80
                      +h_WR96*96+h_WR112*112+h_WR128*128+h_WR256*256;
	printf( "--------------------------------------------------\n" );
        printf( " Runtime Stats\n" );
	printf( "--------------------------------------------------\n" );
        printf( "RD1        %llu\n", c_RD1 );
        printf( "RD2        %llu\n", c_RD2 );
        printf( "RD4        %llu\n", c_RD4 );
        printf( "RD8        %llu\n", c_RD8 );
        printf( "RD16       %llu\n", c_RD16 );
        printf( "WR1        %llu\n", c_WR1 );
        printf( "WR2        %llu\n", c_WR2 );
        printf( "WR4        %llu\n", c_WR4 );
        printf( "WR8        %llu\n", c_WR8 );
        printf( "WR16       %llu\n", c_WR16 );
	printf( "--------------------------------------------------\n" );
        printf( "HMC_WR_16  %llu\n", h_WR16 );
        printf( "HMC_WR_32  %llu\n", h_WR32 );
        printf( "HMC_WR_48  %llu\n", h_WR48 );
        printf( "HMC_WR_64  %llu\n", h_WR64 );
        printf( "HMC_WR_80  %llu\n", h_WR80 );
        printf( "HMC_WR_96  %llu\n", h_WR96 );
        printf( "HMC_WR_112 %llu\n", h_WR112 );
        printf( "HMC_WR_128 %llu\n", h_WR128 );
        printf( "HMC_WR_256 %llu\n", h_WR256 );
        printf( "HMC_RD_16  %llu\n", h_RD16 );
        printf( "HMC_RD_32  %llu\n", h_RD32 );
        printf( "HMC_RD_48  %llu\n", h_RD48 );
        printf( "HMC_RD_64  %llu\n", h_RD64 );
        printf( "HMC_RD_80  %llu\n", h_RD80 );
        printf( "HMC_RD_96  %llu\n", h_RD96 );
        printf( "HMC_RD_112 %llu\n", h_RD112 );
        printf( "HMC_RD_128 %llu\n", h_RD128 );
        printf( "HMC_RD_256 %llu\n", h_RD256 );
	printf( "--------------------------------------------------\n" );
        printf( "TOTAL_READ_INPUT     %llu\n", total_r_in );
        printf( "TOTAL_WRITE_INPUT    %llu\n", total_w_in );
        printf( "TOTAL_INPUT          %llu\n", total_in );
        printf( "TOTAL_READ_OUTPUT    %llu\n", total_r_out );
        printf( "TOTAL_WRITE_OUTPUT   %llu\n", total_w_out );
        printf( "TOTAL_OUTPUT         %llu\n", total_out );
        printf( "READ_EFFICIENCY      %f\n",
                (1.0f-(double)(total_r_out)/(double)(total_r_in))*100 );
        printf( "WRITE_EFFICIENCY     %f\n",
                (1.0f-(double)(total_w_out)/(double)(total_w_in))*100 );
        printf( "TOTAL_EFFICIENCY     %f\n",
                (1.0f-(double)(total_out)/(double)(total_in))*100 );
	printf( "--------------------------------------------------\n" );
	printf( "TOTAL_REQUESTED_DATA_SIZE    %llu bytes\n",data_in );
	printf( "TOTAL_TRANSFERED_DATA_SIZE   %llu bytes\n",data_out );
	printf( "EFFECTIVE_DATA_RATE	      %f%%\n", (double)data_in/(double)data_out*100 );
	printf( "BANDWIDTH SAVING	      %llu bytes\n", (total_in - total_out)*32 );
	printf( "--------------------------------------------------\n" );
}

/* ----------------------------------------------- PRINT_HELP */
static void print_help(){
	printf( "--------------------------------------------------\n" );
	printf( "                   DMC_DRIVER\n" );
	printf( "--------------------------------------------------\n" );
	printf( " Usage: dmc_driver <options>\n" );
	printf( " <options>\n" );
	printf( " -h                : print this help\n" );
	printf( " -f filename       : file containing memory trace\n" );
  printf( " -I                : read trace from STDIN\n" );
  printf( " -p /path/to/pipe  : read trace from named pipe\n" );
	printf( " -t                : enable tracing\n" );
  printf( " -T                : enable deep tracing\n" );
	printf( " -n                : execute C version of ucode\n" );
	printf( " -s                : execute assembly version of ucode\n" );
  printf( " -P                : print stats at the end of the run\n" );
	printf( "--------------------------------------------------\n" );
}

/* ----------------------------------------------- GET_MEMOP */
static int get_memop( int type, int nbytes, MEMOP *op ){

	if( type == 0 ){
		switch( nbytes )
		{
			case 1 :
				*op = WR1;
        c_WR1++;
				break;
			case 2 :
				*op = WR2;
        c_WR2++;
				break;
			case 4 :
				*op = WR4;
        c_WR4++;
				break;
			case 8 :
				*op = WR8;
        c_WR8++;
				break;
			case 16:
				*op = WR15;
        c_WR16++;
				break;
			default:
				return -1;
				break;
		}
		return 0;
	}else if( type == 1 ){
		switch( nbytes )
		{
			case 1 :
				*op = RD1;
        c_RD1++;
				break;
			case 2 :
				*op = RD2;
        c_RD2++;
				break;
			case 4 :
				*op = RD4;
        c_RD4++;
				break;
			case 8 :
				*op = RD8;
        c_RD8++;
				break;
			case 16:
				*op = RD15;
        c_RD16++;
				break;
			default:
				return -1;
				break;
		}
		return 0;
	}else{
		return -1;
	}

}

/* ----------------------------------------------- READ_TRACE_PIPE */
static int read_trace_pipe( int fd, struct mtrace *otrace ){
	/* vars */
  char cmdbuf[3];
  char bytesbuf[2];
  char procbuf[2];
  char addrbuf[17];
	char *token;
	size_t len	= 0;
	int type	= -1;	/* {0=WR,1=RD} */
	int nbytes	= 0;
  int bytesread   = 0;
	uint32_t proc	= 0;
	uint64_t addr	= 0x00ull;
	MEMOP op;
	/* ---- */

  /* read a new buffer */
  if((bytesread = read(fd, cmdbuf, 3)) <= 0){
    return -1;
  }
  len = strlen( cmdbuf );
  if( cmdbuf[len] == '\n' ){
    cmdbuf[len] = '\0';
  }
  cmdbuf[2] = '\0';

  if( strcmp( cmdbuf, "WR" ) == 0 ){
    type = 0;
	}else if( strcmp( cmdbuf, "RD" ) == 0 ){
    type = 1;
  }else if( strcmp( cmdbuf, "PG" ) == 0 ){
		page_num++;
		type = -1;
  }else if( strcmp( cmdbuf, "EX" ) == 0 ){
    return 1;
  }else{
		printf( "error : erroneous message type : %s\n", cmdbuf );
		return -2;
  }

  if((bytesread = read(fd, bytesbuf, 2)) <= 0 ){
    return -1;
  }
  len = strlen( bytesbuf );
  if( bytesbuf[len] == '\n' ){
    bytesbuf[len] = '\0';
  }

  if((bytesread = read(fd, procbuf, 2)) <= 0 ){
    return -1;
  }
  len = strlen( procbuf );
  if( procbuf[len] == '\n' ){
    procbuf[len] = '\0';
  }

  if((bytesread = read(fd, addrbuf, 17)) <= 0 ){
    return -1;
  }
  len = strlen( addrbuf );
  if( addrbuf[len] == '\n' ){
    addrbuf[len] = '\0';
  }
  addrbuf[16] = '\0';


#ifdef DEBUG
        printf( "msg = %s:%s:%s:%s\n",
                cmdbuf,
                bytesbuf,
                procbuf,
                addrbuf );
#endif

	nbytes = atoi( bytesbuf );
	proc = (uint32_t)(atoi(procbuf));
	addr = (uint64_t)(strtol( addrbuf, NULL, 16 ));

	/* get the appropriate memory operand */
	if( get_memop( type, nbytes, &op) != 0 ){
		//printf( "error : erroneous message type from get_memop\n" );
		return -1;
	}

	/* fill out the data structure */
	otrace->proc 	= proc;
	otrace->op	= op;
	otrace->addr	= addr;

#ifdef DEBUG
	printf( "::DEBUG::      proc = %d, op = %d, addr = 0x%016lx\n", otrace->proc,(int)(otrace->op), otrace->addr );
	//printf( "::DEBUG::      op   = %d\n", (int)(otrace->op) );
	//printf( "::DEBUG::      addr = 0x%016lx\n", otrace->addr );
#endif

  return 0;
}

/* ----------------------------------------------- READ_TRACE */
static int read_trace( FILE *infile, struct mtrace *otrace ){

	/* vars */
	char buf[50];
	char *token;
	size_t len	= 0;
	int type	= -1;	/* {0=WR,1=RD} */
	int nbytes	= 0;
	uint32_t proc	= 0;
	uint64_t addr	= 0x00ull;
	MEMOP op;
	/* ---- */

	/* read a single entry from the trace file */
	if( feof( infile ) ){
		return 2;
	}

	if( fgets( buf, 50, infile ) == NULL ){
		return -1;
	}

	/*
	 * we have a valid buffer
	 * strip the newline and tokenize it
	 *
	 */
	len = strlen( buf );
	if( buf[len] == '\n' ){
		buf[len] = '\0';
	}

	if( (len < 23 ) || (len >29) ){
		/* return a special error code indicating
                 * that this is a bogus line
	         */
		return -1;
	}
	/*
	 * now to tokenize it
	 * Format: {WR,RD,CLOSE}:{NUM_BYTES}:{PROCID}:{0xADDR}
	 */
	token = strtok( buf, ":");
	if( strcmp( token, "WR" ) == 0 ){
		type = 0;
	}else if( strcmp( buf, "RD" ) == 0 ){
		type = 1;
  }else if ( strcmp( buf, "PG" ) == 0 ){
		page_num++;
		return -1;
	}else if( strcmp( buf, "CLOSE" ) == 0 ){
    /* recevied a CLOSE message */
    return 1;
	}else{
		printf( "error : erroneous message type : %s\n", token );
		return -1;
	}

	/* num_bytes */
	token = strtok( NULL, ":" );
	nbytes = atoi( token );

	/* procid */
	token = strtok( NULL, ":" );
	proc = (uint32_t)(atoi(token));

	/* -- first part of address = 0x */
	token = strtok( NULL, "x" );
	/* -- last part of address in hex */
	token = strtok( NULL, " " );
	addr = (uint64_t)(strtol( token, NULL, 16 ));

	/* get the appropriate memory operand */
	if( get_memop( type, nbytes, &op) != 0 ){
		printf( "error : erroneous message type from get_memop\n" );
		return -1;
	}

	/* fill out the data structure */
	otrace->proc 	= proc;
	otrace->op	= op;
	otrace->addr	= addr;

#ifdef DEBUG
	printf( "::DEBUG::      proc = %d\n", otrace->proc );
	printf( "::DEBUG::      op   = %d\n", (int)(otrace->op) );
	printf( "::DEBUG::      addr = 0x%016lx\n", otrace->addr );
#endif

  return 0;
}

/*-------------------------Find out the insertion tree index based on the address*/
int get_tree_index(uint64_t addr){
	int i 	 	= 0;
	int flag 	= -1;
	int index 	= 0;
#ifdef DEBUG
	printf( "::DEBUG::    in  get_tree_index()\n");
#endif

	/*Suppose the page size is 4KB, first search the page address to check if a match exists*/
	for(i = 0; i < MAX_WRITE_TIME*2; i++){
		if((addr >= root[i].req.addr) && (addr < root[i].req.addr + 0xFFF) ){
			flag = 1;
			break;
		}
	}
	if(flag == 1)
		return i;
	/*If no match exists, find a empty tree and update the page address in the root node*/
	for(i = 0; i < MAX_WRITE_TIME*2; i++){
		if(root[i].left == NULL && root[i].right == NULL){
			flag = 1;
			/*get the page address by setting page offset bits to 0*/
			root[i].req.addr = (addr & 0xFFFFF000);
			break;
		}
	}
#ifdef DEBUG
	printf("rqst.addr = %016lx, root[%d].addr =  %016lx\n", addr, i, root[i].req.addr);
#endif
	if(flag != -1)
		return i;

	return -1;
}



/* ----------------------------------------------- MAIN */
int main( int argc, char **argv ){

	/* vars */
	int index	= 0;
	int ret		= 0;
	int type 	= 0;
	int done	= 0;
	int done2	= 0;
  int stats = 0;
  int fd    = 0;
	int flag  = 0;
	int i 		= 0;
  int input       = -1; /* 0 for tracefile; 1 for STDIN */
	int multifile   = 0;
	uint64_t conf	= 0x00ull;
	FILE *infile	= NULL;
	FILE *infile2	= NULL;
  FILE *outfile = NULL; /*save hmcsim power evaluation to an output file */
	struct mtrace rqst;
	struct mtrace rqst2;
	int analysis	= 0;
	uint64_t a_addr = 0;
	uint64_t p_addr = 0;
	MEMOP	oper	= 0;
	int	op_flag	= 0;
	int count	= 0;
	int cnt_across	= 0;
	int page_n	= 0;
	char filename[1024];
	char filename2[1024];

  /* Init dmctrace*/
  dmctrace = malloc( sizeof( struct mptrace ) );
  dmctrace->type = -1;
  dmctrace->size = 0;
  dmctrace->update = 0;
  dmctrace->end = 0;
  dmctrace->addr = 0x00ull;

  /* vars for init hmc-sim */
	// uint32_t i		= 0;
  int simdone = 0;
	uint32_t num_devs	= 1;
	uint32_t num_links	= 4;
	uint32_t num_vaults	= 32;
	uint32_t queue_depth	= 64;
	uint32_t num_banks	= 16;
	uint32_t num_drams	= 20;
	uint32_t capacity	= 4;
	uint32_t xbar_depth	= 128;
  uint32_t bsize  = 128;
	char *cfile = NULL;
	struct hmcsim_t hmc;

  /* vars for running hmc-sim */
	uint64_t head	= 0x00ll;
  uint64_t tail	= 0x00ll;
  uint64_t payload[16]	= { 0x00ll, 0x00ll, 0x00ll, 0x00ll,
	                         	0x00ll, 0x00ll, 0x00ll, 0x00ll,
														0x00ll, 0x00ll, 0x00ll, 0x00ll,
														0x00ll, 0x00ll, 0x00ll, 0x00ll
													 };

	int simret = HMC_OK; 	// for running hmc-sim
  int recvflg = 0;
	// int rddone = 0; 			// for read_hmc_trace
	// int bidret = HMC_OK;	// for hmcsim_send_memrequest
	// uint32_t done	= 0;

	uint8_t	cub		= 0;
	uint8_t link		= 0;
	uint16_t tag	= 0;
	int stall_sig	= 0;

	uint32_t plen = 0;	// hmc memory request packet size
	uint64_t packet[HMC_MAX_UQ_PACKET];

	int *rtns	= NULL;		//record status for each link
	long all_sent	= 0;
	long all_recv	= 0;
	long trace = 0;

	uint64_t d_response_head;
  uint64_t d_response_tail;
  hmc_response_t d_type;
  uint8_t d_length;
  uint16_t d_tag;
  uint8_t d_rtn_tag;
  uint8_t d_src_link;
  uint8_t d_rrp;
  uint8_t d_frp;
  uint8_t d_seq;
  uint8_t d_dinv;
  uint8_t d_errstat;
  uint8_t d_rtc;
  uint32_t d_crc;
  /*
   * open the outfile
   *
   */
  outfile = fopen( "dmc_power.out", "w" );
  if( outfile == NULL ){
    printf( "FAILED : COULD NOT OPEN OUPUT FILE dmc_trace.out\n" );
    return -1;
  }

	/* ---- */
	page_num	= 0;
	for(i = 0; i < 32; i ++)
		tree_cnt[i] = 0;
	while(( ret = getopt( argc, argv, "f:tp:IThnsPaF:" )) != -1 ){
		switch( ret )
		{
			case 'f':
        input = 0;
				sprintf( filename, "%s", optarg );
				break;
			case 'F':
        multifile = 1;
				sprintf( filename2, "%s", optarg );
				break;
      case 'I':
        input = 1;
        break;
			case 't':
				conf |= CONF_TRACE;
				break;
      case 'p':
        input = 2;
				sprintf( filename, "%s", optarg );
        break;
      case 'T':
				conf |= CONF_TRACE;
        conf |= DEEP_TRACE;
        break;
			case 'n':
				printf( "...Utilizing C-based microcode\n" );
				type = 0;
				break;
			case 's':
				printf( "...Utilizing Assembly-based microcode\n" );
				type = 1;
				break;
      case 'P':
        init_stats();
        stats = 1;
        break;
			case 'h':
				print_help();
				return 0;
				break;
			case 'a':
				analysis = 1;
				break;
			default:
				printf( "Unknown option!\n" );
				print_help();
				return -1;
				break;
		}
	}

	/* sanity check */
  if( input == -1 ){
    printf( "error : no input method selected\n" );
    return -1;
  }

	if( (input == 0) && (strlen( filename ) == 0) ){
		printf( "error : filename is invalid\n" );
		return -1;
	}

	/* open the file */
  if( input == 0 ){
	  infile = fopen( filename, "r" );
	  if( infile == NULL ){
      printf( "error : could not open file %s\n", filename );
      return -1;
	  }
	}
	else if( input == 2 ){
          if( strlen(filename) == 0 ){
            printf( "ERROR : filename is invalid\n" );
            return -1;
          }
          fd = open( filename, O_RDONLY );
          if( fd == -1 ){
            printf( "ERROR : COULD NOT OPEN NAMED PIPE AT %s\n", filename );
            return -1;
          }
        }else{
          infile = stdin;
        }

	/* init the internal data structures */
	ucode_init(conf);
#ifdef DEBUG
	printf("ucode_init completed\n");
#endif

/* ----------------------------------------init the hmc-sim library */

  ret = hmcsim_init( &hmc,
	                   num_devs,
	                   num_links,
	                   num_vaults,
	                   queue_depth,
	                   num_banks,
	                   num_drams,
	                   capacity,
	                   xbar_depth );
  if( ret != 0 ){
    printf( "FAILED TO INIT HMCSIM\n" );
    return -1;
  }else{
    printf( "SUCCESS : INITIALIZED HMCSIM\n" );
  }

	if( cfile != NULL ) {
		if( hmcsim_read_config( &hmc, cfile ) != 0 ) {
			printf("FAILED TO READ CONFIG FILE : %s\n", cfile );
			hmcsim_free( &hmc );
			return -1;
		}
	}

	/*
   * setup the device topology
   *
   */
  if( num_devs > 1 ){
    /*
     * MULTIPLE DEVICES
     *
     */
  }else{
    /*
     * SINGLE DEVICE
     *
     */

    for( i=0; i<num_links; i++ ){
      ret = hmcsim_link_config( &hmc,
                                (num_devs+1),
                                0,
                                i,
                                i,
                                HMC_LINK_HOST_DEV );

      if( ret != 0 ){
        printf( "FAILED TO INIT LINK %d\n", i );

        hmcsim_free( &hmc );
				free(dmctrace);

        return -1;
      }else{
        printf( "SUCCESS : INITIALIZED LINK %d\n", i );
      }
    }
  }

	/*
   * init the max request block size
   *
   */
  ret = hmcsim_util_set_all_max_blocksize( &hmc, bsize );

  if( ret != 0 ){
    printf( "FAILED TO INIT MAX BLOCKSIZE\n" );
    hmcsim_free( &hmc );
		free(dmctrace);
    return -1;
  }else {
    printf( "SUCCESS : INITIALIZED MAX BLOCK SIZE\n" );
  }
/*---------------------------------------------end of init hmc-sim*/

	/* read a request from the input file */
  if( input == 2 ){
		done = read_trace_pipe( fd, &rqst );
	  while(done != 0){
			if(done == -2 ){
          close( fd );
					return 0;
			}
			done = read_trace_pipe( fd, &rqst );
		}
  }else{
	  done = read_trace( infile, &rqst );
	  while(done != 0){
	  	done = read_trace( infile, &rqst );
    }
	}
#ifdef DEBUG
	printf("::DEBUG::    first read trace completed\n");
#endif

	/* ------------------------------------begin the sim execution */

  /*
   * execute the hmc-sim
   * setup the tracing mechanisms
   */
	rtns = malloc( sizeof( int ) * hmc.num_links );
	memset( rtns, 0, sizeof( int ) * hmc.num_links );

	hmcsim_trace_handle( &hmc, outfile );
	hmcsim_trace_level( &hmc, (HMC_TRACE_POWER) );		//only trace power info
	hmcsim_trace_header( &hmc );

	printf( "SUCCESS : INITIALIZED TRACE HANDLERS\n" );

  /*
   * zero the packet
   *
   */
  zero_packet( &(packet[0]) );

  printf( "BEGINNING TEST EXECUTION\n" );


  /* hmc-sim must done after reading all traces */
	while( simdone != 1  ){

    if(done == 0) {
      /* coalescing only runs with valid trace */
      /*===========frame-based dmc starts here===============*/
  		index = get_tree_index(rqst.addr);
  #ifdef DEBUG
  	printf("::DEBUG::    Tree index get: %d\n", index);
  #endif

  		/*Coalescing Tree postions are full, need to flush all the trees*/
  		if(index == -1){
  			//printf("No free coalescing tree position, start flushing\n");
  			ucode_flush();
  			index = 0;
  			root[index].req.addr = (rqst.addr&0xfffff000);
  		}

  		while(analysis == 1){

  			if(rqst.addr + (rqst.op&0xff) >= root[index].req.addr+0x1000 )
  				cnt_across++;
  			// for the case that operation types are different, set op_flag for break;
  			if((rqst.op & oper) < 0x100 && (rqst.op | oper) >= 0x100 ){
  				a_addr	= rqst.addr;
  				p_addr 	= a_addr & 0xfffff000;
  				oper   	= rqst.op;
  				op_flag	= 0;
  			}
  			else
  				op_flag = 1;
  			//check if two pages are consecutive or not
  			if(oper<(p_addr == root[index].req.addr+0x1000) || (p_addr == root[index].req.addr - 0x1000) ){
  				page_n++;
  				if(op_flag == 0)
  					break;
  				if(a_addr > rqst.addr){

  					if(a_addr-(rqst.addr + (rqst.op&0xff)) <= 256 )
  						count++;
  				}
  				else{
  					if(rqst.addr + (rqst.op&0xff) - a_addr <= 256 )
  						count++;

  				}
  			}
  			a_addr	= rqst.addr;
  			p_addr 	= a_addr & 0xfffff000;
  			oper   	= rqst.op;
  			break;
  		}
  		/* insert it */
  		if( type == 0 ){

  #ifdef DEBUG
  			printf("::DEBUG::    Ucode_insert starts\n");
  #endif
  			ucode_insert( rqst, index );
  #ifdef DEBUG
  			printf("::DEBUG::    Ucode_insert completed\n");
  #endif
  		}else{
  			// ucode_opt_insert( rqst );
  		}

    } //end of if (done == 0)


    /*---check if dmctrace is updated
     * if simret != HMC_STALL and  dmctrace is updated, build a hmcsim request
     * else, read next trace
     */

     if (dmctrace->update == 1) {
       /* dmc trace is updated */
       /*if hmc-sim is not stall, send request*/
       simret = hmcsim_build_memrequest( &hmc,
																			cub,
																			dmctrace->addr,
																			tag,
																			dmctrace->size,
																			link,
																			&(packet[0]),
																			&head,
																			&tail );
        if( simret == 0) {
  				if( dmctrace->type == 0 ){
            // write
            // use max size 128. i.e. 9FLITs
            if ( dmctrace->size == 256 ) {
              plen = 9;
            } else {
              plen = ( dmctrace->size )/16;
            }
            // lay out packet
            packet[0] = head;
            for(i=0; i<((plen-1)*2); i++ ){
              packet[i+1] = payload[i];
            }
            packet[(plen*2)-1] = tail;
          }else{
            packet[0] = head;
            packet[1] = tail;
          }
          simret = hmcsim_send( &hmc, &(packet[0]) );
        } else {
			    // printf("ERROR : FATAL : MALFORMED PACKET\n");
  			}

        switch ( simret ) {
          case 0:
            all_sent++;
            /*
      			 * zero the packet
      			 *
      			 */
      			zero_packet( &(packet[0]) );

      			tag++;
      			if( tag == 2048 ){
      				tag = 0;
      			}

      			link++;
      			if( link == hmc.num_links ){
      				/* -- TODO : look at the number of connected links
      				 * to the host processor
      				 */
      				link = 0;
      			}
            // goto read_next_trace;
            break;
          case HMC_STALL:
            // printf( "STALLED : PACKET WAS STALLED IN SENDING\n" );
            /* Go to receive side */
            recvflg = 1;
            goto packet_recv;
            break;
          default:
            // printf( "FAILED : PACKET SEND FAILED\n" );
            goto complete_failure;
            break;
        } // end of switch

     }  // end of dmctrace == update

read_next_trace:
    if (dmctrace->update == 1) {
      /* reset the dmctrace*/
      dmctrace->type = -1;
      dmctrace->size = 0;
      dmctrace->update = 0;
      dmctrace->addr = 0x00ull;
    }

    /* read a request from the input file */
    if( input == 2 ){
		  done = read_trace_pipe( fd, &rqst );
		  /*valid rqsts should fall into the physical address space in spike linux kernel*/
		  while(done != 0 || rqst.addr >= 0xffffffffff ){
					done = read_trace_pipe(fd, &rqst);
				if(done == -2 )
					break;
		  }
    }else{
			done = read_trace( infile, &rqst );
		  	while(done == -1 || rqst.addr >= 0xffffffff){
		  		done = read_trace( infile, &rqst );
		  	}
	  }

    /* set flag showing this is the end of dmc trace*/
    if( done == 1 ) {
      dmctrace->end = 1;
      if( all_sent == all_recv ) {
        printf( "ALL_TRACES = %ld\n", trace );
        simdone = 1;
        print_power(&hmc);
      }
    }

    /* at this point, if done == 1 means finished reading all trace file */
packet_recv:
    if( recvflg == 1) {
      recvflg = 0;
      simret = HMC_OK;
      while( simret != HMC_STALL ) {
        for( i = 0; i < hmc.num_links; i++ ) {
          rtns[i] = hmcsim_recv( &hmc, cub, i, &(packet[0]) );
          if( rtns[i] == HMC_STALL ) {
            stall_sig++;
          }else {
            hmcsim_decode_memresponse(  &hmc,
	                                    &(packet[0]),
	                                    &d_response_head,
	                                    &d_response_tail,
	                                    &d_type,
	                                    &d_length,
	                                    &d_tag,
	                                    &d_rtn_tag,
	                                    &d_src_link,
	                                    &d_rrp,
	                                    &d_frp,
	                                    &d_seq,
	                                    &d_dinv,
	                                    &d_errstat,
	                                    &d_rtc,
	                                    &d_crc );
            all_recv++;
          }
          /*
  				 * zero the packet
  				 *
  				 */
  				zero_packet( &(packet[0]) );
        }
        /* count the number of stall signals received */
  			if( stall_sig == hmc.num_links ){
  				/*
  				 * if all links returned stalls,
  				 * then we're done receiving packets
  				 *
  				 */

  				// printf( "STALLED : STALLED IN RECEIVING\n" );
  				simret = HMC_STALL;
  			}

  			stall_sig = 0;
  			for( i=0; i< hmc.num_links; i++){
  				rtns[i] = HMC_OK;
  			}
      }// end of receive while
      /*
  		 * reset the return code
  		 *
  		 */
  		stall_sig = 0;
  		for( i=0; i< hmc.num_links; i++){
  			rtns[i] = HMC_OK;
  		}
  		simret = HMC_OK;

      /*
  	 	 * done with sending/receiving packets
  		 * update the clock
  		 */

  		hmcsim_clock( &hmc );

      if( dmctrace->end == 1 ) {
        if( all_sent == all_recv ) {
          printf( "ALL_TRACES = %ld\n", trace );
  				simdone = 1;
  				print_power(&hmc);
        }
      } else {
        goto read_next_trace;
      }

    }// end of if recvflg


#ifdef DEBUG
		printf( "::DEBUG::    Read_trace completed()\n");
#endif

} // end of the outside while

  /* flush any remaining requests */
  ucode_flush();

  /* free ucode memory */
  ucode_free();

complete_failure:
  hmcsim_free( &hmc );
  free(dmctrace);
  fclose( outfile );
  outfile = NULL;
  free( rtns );
  rtns = NULL;

  if( input == 0 ){
    fclose( infile );
  }else if( input == 2 ){
    close( fd );
  }

	infile = NULL;

  if( stats == 1 ){
    print_stats();
  }

	printf("Total Page request number: %llu\n",page_num);
	printf( "--------------------------------------------------\n" );
	if(analysis == 1){
		printf("Total rqsts that could be coalesced across pages:   %d\n", count);
		printf("Total rqsts that access across page boundary: 	    %d\n", cnt_across);
		printf("Total Contiguous pages accessed by close requests:  %d\n", page_n);
		printf( "--------------------------------------------------\n" );
	}
	printf( "--------------------------------------------------\n" );

/*// Statistics for the paged tree construction information
  double page_cnt = 0.;
	double sum			= 0.;
	for( i = 0; i < MAX_READ_TIME*2; i++){
		printf("%d   %d\n",i+1, tree_cnt[i]);
		page_cnt 	+= 	tree_cnt[i];
		sum			 	+=	tree_cnt[i]*i;
	}

	printf("Average pages # is %f\n", sum/page_cnt);
	printf("Average rqsts per page is %f\n", 16.0/(sum/page_cnt));
	printf("Accelerated pages # is %d, %f of the total pages\n", accelerated_page, 100*(double)accelerated_page/sum);

	printf( "--------------------------------------------------\n" );
	*/

	return 0;
}
/* EOF */
