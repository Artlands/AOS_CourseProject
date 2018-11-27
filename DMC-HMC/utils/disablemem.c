/*
 * _DISABLEMEM_C_
 *
 * GC64-DMC UTILITY TO DISABLE MEMORY TRACING
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TRACEADDR 0xC000000000000000ull

int main(){

  uint64_t *ptr = (uint64_t *)(TRACEADDR);
  uint64_t val  = 0x00ll;

  val = *ptr;

  return 0;
}
