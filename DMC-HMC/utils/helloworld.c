#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv ){
  int i = 0;

  for( i=0; i<128; i++ ){
    printf( "Hello from inside RISC-V : %d\n", i );
  }

  return 0;
}
