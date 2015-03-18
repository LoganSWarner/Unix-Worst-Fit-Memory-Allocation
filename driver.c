#include <stdio.h>
#include "./mem.h"
#include "./mem.c"


struct myMemoryMap coremap[100];

main()
{
  register int  m1, m2, m3, m4, m5, m6;
  int i;
  for (i=0;i<100;i++){
    coremap[i].freeSize = -1;
    coremap[i].freeAddr = 500;
  }
  coremap[0].freeSize = (char*)500;
  coremap[0].freeAddr = (char*)0;
  m1=mymalloc(coremap, 100);
  printf("Memory  should be allocated at address 0, allocated at %d \n", m1);
  mymdump(coremap);
  m2= mymalloc(coremap, 200);
  printf("Memory  should be allocated at address 100, allocated at %d \n", m2);
  mymdump(coremap);
  m3= mymalloc(coremap, 150);
  printf("Memory  should be allocated at address 300, allocated at %d \n", m3);
  mymdump(coremap);
  m4= mymalloc(coremap, 25);
  printf("Memory  should be allocated at address 450, allocated at %d \n", m4);
  mymdump(coremap);
  m5= mymalloc(coremap, 25);
  printf("Memory  should be allocated at address 475, allocated at %d \n", m5);
  mymdump(coremap);
  mymfree(coremap, m2);
  printf("Memory freed for  %d\n", m2);
  mymdump(coremap);
  mymfree(coremap, m4);
  printf("Memory freed for %d\n", m4);
  mymdump(coremap);
  m6=mymalloc(coremap, 24);
  printf("Memory  should be allocated at address %d if best fit, %d if worst fit, allocated at %d \n", m4, m2, m6);
  mymdump(coremap);
}
