/*
 * _UCODE_C_
 *
 * MICROCODE ENGINE FOR GC64-DMC ENGINE
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "dmc_ucode.h"

//#define DEBUG
/*
 * Globals
 *
 */

btree root[MAX_WRITE_TIME*2];             /* btree root */
btree *free_list;             		/* free node list */
btree *free_list_last;        		/* last free node in the list */
btree *nodes;                 		/* node list */
//int total_read_bytes[MAX_WRITE_TIME*2];	/* total number of read bytes in the tree */
//int total_write_bytes[MAX_WRITE_TIME*2];  /* total number of write bytes in the tree */
uint64_t total_read_bytes[MAX_WRITE_TIME*2];	/* total number of read bytes in the tree */
uint64_t total_write_bytes[MAX_WRITE_TIME*2];  /* total number of write bytes in the tree */
int read_timeout      = 0;    		/* read timeout value */
int write_timeout     = 0;    		/* write timeout value */
int num_nodes         = 0;    		/* number of nodes in the free tree */
int num_free          = 0;    		/* number of nodes in the free list */
struct dmc __config;          		/* dmc config struct */
hmc_list *head	      = NULL;
hmc_list *tail	      = NULL;
int tree_cnt[32];
int gcount	      = 0;
struct dmc __config;
uint64_t base_addr[MAX_WRITE_TIME*2];    /* base address of the tree */
uint64_t temp_addr[MAX_WRITE_TIME*2];    /* temporary address for the logic testing*/
btree *temp[MAX_WRITE_TIME*2];           /* temp node represent the previous node been inserted*/

uint64_t accelerated_page	=	0;
static btree *get_left_leaf( btree *src );

static btree *get_right_leaf( btree *src );
/*-----------------------Sort the root nodes array according to the root address*/
static int sort_root(){
	int 	i	= 0;
	int 	j 	= 0;
	uint64_t min	= 0x0;
	btree	tmp;
	for(i = 0; i < MAX_WRITE_TIME*2-1; i++){
		if(root[i].req.addr == 0)
			break;
		min = root[i].req.addr;
		for(j = i+1; j < MAX_WRITE_TIME*2;j++ ){
			//if(root[j].req.addr == 0)
				//break;
			if(root[j].req.addr < min){
				min = root[j].req.addr;

				tmp.req.addr 	= root[j].req.addr;
				root[j].req.addr= root[i].req.addr;
				root[i].req.addr= tmp.req.addr;

				tmp.left    	= root[j].left;
				root[j].left	= root[i].left;
				root[i].left	= tmp.left;

				tmp.right    	= root[j].right;
				root[j].right	= root[i].right;
				root[i].right	= tmp.right;

				tmp.req.op	= root[j].req.op;
				root[j].req.op	= root[i].req.op;
				root[i].req.op	= tmp.req.op;
			}
		}

	}
	return 0;

}

/*------------------------Coalesce the adjacent requests across the page boundary*/
static int coal_across_pages_2(){
	int	i	= 0;
	int	j	= 0;
	btree*	left	= NULL;
	btree*	right	= NULL;

	//for(i = 0; i < MAX_WRITE_TIME*2; i++)
		//printf("root[%d].addr= %016lx\n", i, root[i].req.addr);


	for(i = 0; i < MAX_WRITE_TIME*2; i++){
		if(root[i].req.addr == 0 || root[i].left == NULL && root[i].right == NULL )
			continue;
		for(j = 0; j < MAX_WRITE_TIME*2; j++){
			if(root[j].req.addr == 0 || root[j].left == NULL && root[j].right == NULL )
				continue;

			if(root[i].req.addr + 0x1000 == root[j].req.addr||root[i].req.addr == root[j].req.addr + 0x1000 ){
					//Append tree i into tree j
					if(root[i].req.addr < root[j].req.addr){

						if(root[i].left != NULL){
							if(root[j].left != NULL){
								left 		= get_left_leaf(root[j].left);
								left->left 	= root[i].left;
							}
							else
							root[j].left = root[i].left;
						}
						root[i].left 	= NULL;

						if(root[i].right != NULL){
							if(root[j].right != NULL){
								left 		= get_left_leaf(root[j].right);
								left->left 	= root[i].right;
							}
							else
								root[j].right = root[i].right;
						}
					root[i].right 	= NULL;
					root[i].req.addr = 0;
					break;
					}

					//Append tree j into tree i
					else if(root[i].req.addr > root[j].req.addr){

						if(root[j].left != NULL){
							if(root[i].left != NULL){
								left 		= get_left_leaf(root[i].left);
								left->left 	= root[j].left;
							}
							else
							root[i].left = root[j].left;
						}
						root[j].left 	= NULL;

						if(root[j].right != NULL){
							if(root[i].right != NULL){
								left 		= get_left_leaf(root[i].right);
								left->left 	= root[j].right;
							}
							else
								root[i].right = root[j].right;
						}
					root[j].right 	= NULL;
					root[j].req.addr = 0;
					break;
					}

			}




		}
	}

	return 0;

}



/*------------------------Coalesce the adjacent requests across the page boundary*/
static int coal_across_pages(){
	int	i	= 0;
	btree*	left	= NULL;
	btree*	right	= NULL;

	/*Sort the root array*/
	sort_root();
	//for(i = 0; i < MAX_WRITE_TIME*2; i++)
		//printf("root[%d].addr= %016lx\n", i, root[i].req.addr);


	for(i = 0; i < MAX_WRITE_TIME*2-1; i++){
		if(root[i].req.addr == 0 )
			continue;
		// Check the adjacent coalescing frames are consecutive or not
		if(root[i].req.addr + 0x1000 == root[i+1].req.addr ){
			//printf("root[%d].addr = %016lx , get into the adjacent frames\n", i, root[i].req.addr);
			if(root[i].left != NULL){
				if(root[i+1].left != NULL){
					left 		= get_left_leaf(root[i+1].left);
						left->left 	= root[i].left;
				}
				else
					root[i+1].left = root[i].left;
			}
			root[i].left 	= NULL;

			if(root[i].right != NULL){
				if(root[i+1].right != NULL){
					left 		= get_left_leaf(root[i+1].right);
						left->left 	= root[i].right;
				}
				else
					root[i+1].right = root[i].right;
			}
			root[i].right 	= NULL;
			root[i].req.addr = 0;
		}
	}

	return 0;

}



/* ---------------------------------------- init the node of HMC LIST */
extern void hmc_init(hmc_list *node){
  node =(hmc_list*) malloc( sizeof( hmc_list ) );
  node->next=NULL;
}


/* ---------------------------------------- EXPIRE_TREE */
static void expire_tree(btree *p) {

  btree *new  = NULL;

  if(p == NULL) {
    return ;
  }

  /* descend into each subtree and put
   * all the nodes on the free list
   * first, get the end of the free
   * list
   */

  if( p->left != NULL ){
    expire_tree( p->left );
    p->left = NULL;
  }

  if( p->right != NULL ){
    expire_tree( p->right );
    p->right = NULL;
  }

  /*
   * put p back on the free list
   *
   */
  if( free_list == NULL ){
    free_list = p;
    free_list_last = p;
    num_free = 1;
    return ;
  }

  /*
   * existing nodes, get the last in the list
   *
   */
  new = free_list_last;

  /* insert the free node */
  new->left = p;
  p->left = NULL;
  p->right = NULL;
  free_list_last = p;
  num_free++;

}

/* ---------------------------------------- INIT Global Arrays */
static void init_global_array(){
	int i = 0;
	for(i = 0; i < MAX_WRITE_TIME*2;i++){
		total_read_bytes[i]	= 0;
		total_write_bytes[i]	= 0;
		base_addr[i]		= 0;
		temp_addr[i]		= 0;
		temp[i]			= NULL;
	}
}
/* ---------------------------------------- INIT_ROOT_NODE */
static void init_root_node(){
  int i = 0;
  for(i = 0; i < MAX_WRITE_TIME*2; i++){
  root[i].left   	= NULL;
  root[i].right 	= NULL;
  root[i].parent 	= NULL;
  root[i].req.addr 	= 0;
  }
}

/* ---------------------------------------- INIT_LEAF_NODE */
static void init_leaf_node(){
  int i =0;
  btree *tmp;

  /*
   * we only allocate the minimum number
   * of nodes; which is the maximum size
   * of a subtree or the maximum number
   * insertion cycles
   *
   */
  if( MAX_WRITE_TIME > 128 ){
    nodes = malloc( sizeof( btree ) * 256 );
    num_nodes = 256;
  }else{
    num_nodes = MAX_WRITE_TIME*2;
    nodes = malloc( sizeof( btree ) * num_nodes );
  }

#ifdef DEBUG
  printf( "num_nodes = %d\n", num_nodes );
#endif

  free_list = &(nodes[0]);
  tmp       = free_list;
  for( i=1; i<num_nodes; i++ ){
    tmp->left = &(nodes[i]);
    tmp = tmp->left;
  }

  nodes[num_nodes-1].left = NULL;
  free_list_last = &(nodes[num_nodes-1]);

  num_free = num_nodes;
}

/* ---------------------------------------- GET_LEFT_LEAF */
static btree *get_left_leaf( btree *src ){

  btree *tmp = NULL;

#ifdef DEBUG
  printf( "... in get_left_leaf\n" );
#endif

  /* if the left node is null,
   * then just return the src node
   * eg, no children present
   */
  if( src->left == NULL ){
    return src;
  }

  tmp = src->left;
  while( tmp->left != NULL ){
    tmp = tmp->left;
  }

  return tmp;
}

/* ---------------------------------------- GET_RIGHT_LEAF */
static btree *get_right_leaf( btree *src ){

  btree *tmp = NULL;

#ifdef DEBUG
  printf( "... in get_right_leaf\n" );
#endif

  /* if the right node is null,
   * then just return the src node
   * eg, no children present
   */
  if( src->right == NULL ){
    return src;
  }

  tmp = src->right;
  while( tmp->right != NULL ){
    tmp = tmp->right;
  }

  return tmp;
}
/* ---------------------------------------- GET_NEW_NODE */
static btree *get_new_node(){

  btree *tmp = free_list;
  btree *new = tmp;

#ifdef DEBUG
  printf( "... entering get_new_node\n" );
#endif

  if( num_free == 0 || free_list == NULL ){
    free_list_last = NULL;
    return NULL;
  }

  /* last node */
  if( free_list->left == NULL ){
    free_list = NULL;
    free_list_last = NULL;
    num_free  = 0;
    return tmp;
  }

  /* get the last node */
  while( new->left != NULL ){
    tmp = new;
    new = new->left;
  }

  num_free--;
  tmp->left   = NULL;
  free_list_last = tmp;
  new->left   = NULL;
  new->right  = NULL;

#ifdef DEBUG
  printf( "... exiting get_new_node\n" );
#endif

  return new;
}

/* ---------------------------------------- INSERT_CHILD */
static void insert_child( btree **parent, btree *child ){

#ifdef DEBUG
  printf( "...in insert_child\n" );
#endif

  /* if the parent is null, we're done */
  if( *parent == NULL ){
#ifdef DEBUG
    printf( "...found null parent\n" );
#endif
    *parent = child;
    return ;
  }

  /* recursively insert based upon the address */
  if( (*parent)->req.addr >= child->req.addr ){
    /* if new inserted addr is smaller
     * or equal to the parents' addr
     */
    if( (*parent)->left == NULL) {
      (*parent)->left = child;
      child->parent = (*parent);
      return ;
    }else{
      insert_child(&((*parent)->left),child);
      return ;
    }
  }else{
    /* if new inserted addr is larger
     * the parents' addr
     */
    if((*parent)->right == NULL) {
      (*parent)->right = child;
      child->parent = (*parent);
      return ;
    }else{
      insert_child(&((*parent)->right),child);
      return ;
    }
  }
}


/* ---------------------------------------- InOrderTraverse */
/* Read: io = 0
 * Write: io = 1
 */

static void InOrderTraverse(int index, btree* n, int io, btree* end){

  if(n != NULL){

    InOrderTraverse( index, n->left, io,end);
    /* -------------------------- for the case total read bytes in the whole tree doesn't exceed 256*/
    if(n == end && io == 0){
/*	temp_addr[index] = n->req.addr + n->req.op;
	total_read_bytes[index] = temp_addr[index] - base_addr[index];

	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
	if(total_read_bytes[index] <= 256){
		trace_read( total_read_bytes[index], base_addr[index] );
	}
	return;
    }*/

	if(n->req.addr + n->req.op - base_addr[index] > 256){
         /* -------------------------- build one hmc request*/
         total_read_bytes[index] = temp_addr[index] - base_addr[index];
         trace_read( total_read_bytes[index], base_addr[index] );

	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
         /* -------------------------- reset the base address, temp node and temp address*/
         temp[index]              = n;
         base_addr[index]         = n->req.addr;
         temp_addr[index]         = n->req.addr + n->req.op;
         total_read_bytes[index] = temp_addr[index] - base_addr[index];
         trace_read(total_read_bytes[index], base_addr[index]);
	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
         return;
	}
	else{
                 temp_addr[index] = n->req.addr + n->req.op;
                 total_read_bytes[index] = temp_addr[index] - base_addr[index];
                 trace_read( total_read_bytes[index], base_addr[index] );
	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
                 total_read_bytes[index]    = 0;
                 return;
         }

    }



    else if(n == end && io == 1){


	/*
	temp_addr[index] = n->req.addr + (n->req.op&0xFF);
	total_write_bytes[index] = temp_addr[index] - base_addr[index];
	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_write_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_write_bytes[index]);
	if(total_write_bytes[index] <= 256){
		trace_write( total_write_bytes[index], base_addr[index] );
	}
	return;
	*/
	if((n->req.addr +(n->req.op&0xFF) - base_addr[index]) > 256){
         /* -------------------------- build one hmc request*/
         total_write_bytes[index] = temp_addr[index] - base_addr[index];
         trace_write( total_write_bytes[index], base_addr[index] );

	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_write_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_write_bytes[index]);
         /* -------------------------- reset the base address, temp node and temp address*/
         temp[index]              = n;
         base_addr[index]         = n->req.addr;
         temp_addr[index]         = n->req.addr + (n->req.op&0xFF);
         total_write_bytes[index] = temp_addr[index] - base_addr[index];
         trace_write( total_write_bytes[index], base_addr[index] );
         return;
       }
        //temp_addr = n->req.addr + (n->req.op&0xFF);
         //total_write_bytes = temp_addr - base_addr;
         else{
                temp_addr[index] = n->req.addr + (n->req.op&0xFF);
                 total_write_bytes[index] = temp_addr[index] - base_addr[index];
                 trace_write( total_write_bytes[index], base_addr[index] );
	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_write_bytes[index]);
       	         total_write_bytes[index]    = 0;
                 return;
         }


    }




    if( io == 0 ){
      /* READ */
      /* -------------------------- check if total read bytes exceed 256 or not*/
      if( (n->req.addr + n->req.op - base_addr[index] <= 256) ||
          (n->req.addr == base_addr[index]) ){
        temp[index] 	 = n;
        temp_addr[index] = n->req.addr + n->req.op;
	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
      }else{
        /* -------------------------- build one hmc request*/
        total_read_bytes[index] = temp_addr[index] - base_addr[index];
	//printf("before trace for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
        trace_read( total_read_bytes[index], base_addr[index] );

        /* -------------------------- reset the base address, temp node and temp address*/
        temp[index] 	     	  = n;
        base_addr[index]    	  = n->req.addr;
        temp_addr[index]    	  = n->req.addr + n->req.op;
        total_read_bytes[index]   = 0;
	//printf("After trace for index: %d, temp_addr:%016lx, base_add:%016lx, total_read_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_read_bytes[index]);
      }

    }/* end read */
    else{
      /* WRITE */
      //if( ((n->req.addr + (n->req.op&0xFF) - base_addr[index] <= 256) &&
        //  (n->req.addr == temp_addr[index])) ||
          //(n->req.addr == base_addr[index])  ){
      if( ((n->req.addr + (n->req.op&0xFF) - base_addr[index] <= 256))){
          temp[index] 	  = n;
          temp_addr[index] = n->req.addr + (n->req.op&0xFF);
      }else{
        /* -------------------------- build one hmc request*/
        total_write_bytes[index] = temp_addr[index] - base_addr[index];
        trace_write( total_write_bytes[index], base_addr[index] );
	//printf("for index: %d, temp_addr:%016lx, base_add:%016lx, total_write_bytes: %016lx\n", index,temp_addr[index], base_addr[index],total_write_bytes[index]);
        /* -------------------------- reset the base address, temp node and temp address*/
        temp[index] 	     	  = n;
        base_addr[index]    	  = n->req.addr;
        temp_addr[index]    	  = n->req.addr + (n->req.op&0xFF);
        total_write_bytes[index] = 0;
      }
    }/* end write */

     InOrderTraverse(index, n->right, io,end);

  }
}


/******************************************************** insert HMC requset into the hmc list */
static void hmc_insert(int size, uint64_t addr, int flag ){
  hmc_list *node;
  hmc_init(node);
  int op;
  if( size%16 != 0 ){
    size = ((size/16)+1)*16;
    if( size>128){size=256;}
  }
  /* if flag = 0, the request is read operation*/
  if(flag == 0)
    op = size/16;
  /* if flag = 1, the request is write operation*/
  if(flag == 1)
    op = size/16+16;

  node->req.op = op;
  node->next = NULL;

  /* if the list is empty*/
  if( head == NULL )
    head = node;
  else
    tail->next = node;

  tail = node;

}

/* ---------------------------------------- expire the hmc_list */
extern void hmc_expire(){
   hmc_list *temp;
   hmc_list *node;
   temp =head;

   if(head->next == NULL){
	printf("the list is empty\n");
	return;
   }

   if(head->next == NULL){
	free(head);
	printf("the list is expired\n");
   	return;
   }

   while(temp != tail){
	node = temp->next;
	free(temp);
	temp=node;
   }

   free(tail);
   printf("the list is expired\n");
   return;
}

/* ---------------------------------------- UCODE_INSERT */
extern void ucode_insert(struct mtrace rqst, int index){

  /*
   * call the tracing function before returning
   * the trace function handles the config
   * and only prints tracing if enabled
   *
   */
  btree* tmp  = NULL;
  btree* pri  = NULL;
  btree* end  = NULL;
  int    i    = 0;
  int cnt_flag= 0;
  int count   = 0;
#ifdef DEBUG
  printf( "... in ucode_insert\n" );
#endif

  /*
   * get a new node for the insertion
   *
   */
  tmp = get_new_node();
  if( tmp == NULL ){

    /* no new nodes, expire the trees */
#ifdef DEBUG
    printf("...out of nodes\n");
#endif

    coal_across_pages();

    for(i = 0; i < MAX_WRITE_TIME*2; i++){
	if(root[i].req.addr == 0)
		continue;
	root[i].req.addr = 0;
    /* -- read nodes */
    if( root[i].left != NULL ){
			if(root[i].rqst_left == 1)
				accelerated_page++;
				root[i].rqst_left = 0;
			cnt_flag = 1;
      pri = get_left_leaf( root[i].left );
      /*find out the most right nodes (the end of the left subtree)*/
      end = get_right_leaf( root[i].left );
      base_addr[i] 	=  pri->req.addr;
      temp_addr[i] 	=  0;
      temp[i]      	=  pri;
      InOrderTraverse(i,root[i].left,0,end);
      expire_tree(root[i].left);
      /*===============reset all the global arrays after expirations*/
      root[i].left 	= NULL;
      total_read_bytes[i]  = 0;
      base_addr[i] 	= 0;
      temp_addr[i]      = 0;
      temp[i]	   	= NULL;
      read_timeout      = 0;
    }

    /* -- write nodes */
    if( root[i].right != NULL ){
	cnt_flag = 1;
			if(root[i].rqst_right == 1)
				accelerated_page++;
				root[i].rqst_right = 0;
      pri = get_left_leaf( root[i].right );
      /*find out the most right nodes (the end of the right subtree)*/
      end = get_right_leaf( root[i].right );
      base_addr[i] =  pri->req.addr;
      temp_addr[i] =  0;
      temp[i]      =  pri;
      InOrderTraverse(i, root[i].right,1,end);
      expire_tree(root[i].right);
      root[i].right = NULL;
      total_write_bytes[i] = 0;
      base_addr[i] 	= 0;
      temp_addr[i]      = 0;
      temp[i]	   	= NULL;
      write_timeout     = 0;
    }
        if(cnt_flag == 1){
		cnt_flag = 0;
		count++;
	}

   }
   //count += gcount;
   //printf("count = %d\n",count);
   if(count > 0)
   	tree_cnt[count - 1]++;
   count = 0;
   //gcount = 0;
#ifdef DEBUG
    printf("... reset the index after expirations of all the trees\n");
#endif
    index = 0;
    root[index].req.addr = (rqst.addr&0xFFFFF000);
    /* get a new node */
    tmp = get_new_node();

  }

#ifdef DEBUG
  printf( "...got a new node\n" );
#endif

  /* fill in the data */
  tmp->req.proc = rqst.proc;
  tmp->req.op   = rqst.op;
  tmp->req.addr = rqst.addr;
  //tmp->left     = NULL;
  //tmp->right	= NULL;

  /* insert read requests from RISCV cores */
  if( (tmp->req.op >= RD1) && (tmp->req.op <= RD15) ) {
#ifdef DEBUG
    printf( "...inserting read request %016lx into index: %d\n", tmp->req.addr, index );
#endif
		root[index].rqst_left++;
    insert_child( &root[index].left, tmp );
    if(root[index].left == NULL){
		printf("insertion failed \n");
	}
    total_read_bytes[index]+=tmp->req.op;
    read_timeout++;
  }

  /* insert write requests from RISCV cores */
  if( (tmp->req.op >= WR1) && (tmp->req.op <= WR15) ) {
#ifdef DEBUG
    printf( "...inserting write request into index: %d\n", index );
#endif
		root[index].rqst_right++;
    insert_child( &root[index].right, tmp );
    total_write_bytes[index]+=(tmp->req.op&0xFF);
    write_timeout++;
  }

#ifdef DEBUG
  printf( "...request inserted\n" );
#endif

  /*
   * force an HMC read request
   * set timeout to zero
   * clear read requests in the tree
   *
   */

  if( (total_read_bytes[index] >= 256) || (read_timeout >= MAX_READ_TIME) ){
#ifdef DEBUG
    printf( "...expiring read tree, index = %d\n", index );
#endif

    pri = get_left_leaf( root[index].left );
    end = get_right_leaf( root[index].left );
    /* --- add logic to build the hmc requests. */

    base_addr[index] =  pri->req.addr;
    temp_addr[index] =  0;
    temp[index]      =  pri;

    /* --- traverse the tree and build hmc requests */
    InOrderTraverse(index, root[index].left,0,end);

  /*printf("AFTER TRAVERSEIn index = %d\n:", index );
  if(root[0].left != NULL )
	 printf("root[0].left.req.addr = %016lx\n\n", get_left_leaf(root[0].left)->req.addr);
  else
	 printf("root[0].left is NULL\n\n");
  */
    expire_tree(root[index].left);
    root[index].left         = NULL;

    total_read_bytes[index]  = 0;
    read_timeout      = 0;
  }

  /*
   * force an HMC write request
   * set timeout to zero
   * clear the write requests in the tree
   */

  if( (total_write_bytes[index] >= 256) || (write_timeout >= MAX_WRITE_TIME) ){
#ifdef DEBUG
    printf( "...expiring write tree, index = %d\n",index );
#endif
    pri = get_left_leaf( root[index].right );
    end = get_right_leaf( root[index].right );

    base_addr[index] =  pri->req.addr;
    temp_addr[index] =  0;
    temp[index]      =  pri;

    InOrderTraverse(index, root[index].right,1,end);

    expire_tree(root[index].right);
    root[index].right        = NULL;

    total_write_bytes[index] = 0;
    write_timeout     = 0;

  }
  //if ((root[index].left == NULL && root[index].right == NULL)){
//	gcount++;
	//printf("gcount = %d\n", gcount);
  //}
  trace();
}


/* ---------------------------------------- UCODE_FLUSH */
extern void ucode_flush(){

  btree* pri  = NULL;
  btree* end  = NULL;
  btree tmp;
  int index   = 0;
  int count   = 0;
  int cnt_flag= 0;
  /* -- read nodes */
#ifdef DEBUG
  printf( "...ucode_flush()\n" );
#endif
 coal_across_pages();

  /*recursively flush all the coalescing tree*/
  for(index = 0; index < MAX_WRITE_TIME*2; index++){
	//root[index].req.addr = 0;
  	if( root[index].left != NULL ){
			 if(root[index].rqst_left == 1)
				 accelerated_page++;
  	   cnt_flag = 1;
#ifdef DEBUG
	   printf( "...flushing read nodes\n" );
#endif
	    pri = get_left_leaf( root[index].left );
	    end = get_right_leaf( root[index].left );
	    //printf("index = %d, root.req.addr = %016lx, pri.rqst = %016lx, end.rqst = %016lx\n",index,root[index].req.addr,pri->req.addr, end->req.addr);
	    base_addr[index] =  pri->req.addr;
	    temp_addr[index] =  0;
	    temp[index]      =  pri;
	    InOrderTraverse(index, root[index].left,0,end);
	    expire_tree(root[index].left);
	    root[index].left = NULL;
	    total_read_bytes[index]  = 0;
	    read_timeout      = 0;
	  }

  /* -- write nodes */
#ifdef DEBUG
  	printf( "...flushing write nodes\n" );
#endif
	if( root[index].right != NULL ){
  	   cnt_flag = 1;
			 if(root[index].rqst_right == 1)
				 accelerated_page++;
    	    pri = get_left_leaf( root[index].right );
      	    end = get_right_leaf( root[index].right );
	    base_addr[index] =  pri->req.addr;
	    temp_addr[index] =  0;
	    temp[index]      =  pri;
	    InOrderTraverse(index,root[index].right,1,end);
	    expire_tree(root[index].right);
	    root[index].right = NULL;
	    total_write_bytes[index] = 0;
	    write_timeout     = 0;
	  }
	root[index].req.addr = 0;
	if(cnt_flag == 1){
		count++;
		cnt_flag = 0;
	}
   }
   //count += gcount;
   tree_cnt[count-1]++;
   //gcount = 0;
}

/* ---------------------------------------- UCODE_FREE */
extern void ucode_free() {
  free(nodes);
  nodes = NULL;
}

/* ---------------------------------------- UCODE_INIT */
extern void ucode_init( uint64_t conf ) {

  /* init the structure */
  __config.conf = conf;

  /* init the root node */
  init_root_node();

  /* init the leaf nodes */
  init_leaf_node();

  /* init the global array */
  init_global_array();
}

/* EOF */
