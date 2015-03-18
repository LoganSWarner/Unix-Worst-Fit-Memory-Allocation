/*
 * mem.c
 *
 *  Created on: Feb 7, 2014
 *      Author: Logan Warner
 */

#include "./mem.h"
#include <stdio.h>

int mymalloc(struct myMemoryMap* memMap, int memSize){
  register int addr;
  register struct myMemoryMap* mapPtr;

  //disallow allocating the used borders
  if(memSize <= 0)
    return 0;

  int largestSlotSize = 0;
  register struct myMemoryMap* largestAddrMapPtr;
  int newSlotSize = 0;
  for(mapPtr = memMap; mapPtr->freeSize != -1; mapPtr++){
    newSlotSize = mapPtr->freeSize;
    //worst-fit, so track the largest slot found
    if(newSlotSize >= memSize && newSlotSize > largestSlotSize){
      largestSlotSize = newSlotSize;//replace old largest slot size
      largestAddrMapPtr = mapPtr;//store address so we can get into it later
    }//if large enough spot found
  }//for

  char* borderAddr;//used for bordering used segments
  if(largestSlotSize > 0){//same as what used to be in the if in the for,
    mapPtr = largestAddrMapPtr;//except for changing mapPtr
    addr = mapPtr->freeAddr;
    borderAddr = mapPtr->freeAddr;
    mapPtr->freeAddr += memSize;
    if((mapPtr->freeSize -= memSize) == 0){
      do{
        mapPtr++;
        (mapPtr-1)->freeAddr = mapPtr->freeAddr;
      }while(((mapPtr-1)->freeSize = mapPtr->freeSize) != -1);
    }//if
    if(addr != 0){//insert 0-size entry on border between newly allocated and
      mapPtr = largestAddrMapPtr;//previously allocated
      char* curAddr = mapPtr->freeAddr;
      char* curSize = mapPtr->freeSize;
      mapPtr->freeSize = 0;
      mapPtr->freeAddr = borderAddr;
      char* nextAddr;
      char* nextSize;
      do{//bump up the rest of the array
        mapPtr++;
        nextAddr = mapPtr->freeAddr;
        nextSize = mapPtr->freeSize;
        mapPtr->freeAddr = curAddr;
        mapPtr->freeSize = curSize;
        curAddr = nextAddr;
        curSize = nextSize;
      }while(nextSize != -1);
    }//if
    return addr;
  }else
    return 0;
}//mymalloc

void mymfree(struct myMemoryMap* memMap, int memAddr){
  register struct myMemoryMap* mapPtr;
  register int tempAddr;
  register int addr;

  addr = memAddr;
  for(mapPtr = memMap; mapPtr->freeAddr <= addr && mapPtr->freeSize != -1; mapPtr++);
  //find size of allocated block...only substantial change from original code
  int size = mapPtr->freeAddr - (mapPtr-1)->freeAddr;
  if(mapPtr > memMap && (int)(mapPtr-1)->freeAddr + (int)(mapPtr-1)->freeSize == addr) {
    (mapPtr-1)->freeSize += size;
    if(addr+size == mapPtr->freeAddr){
      (mapPtr-1)->freeSize += (int)mapPtr->freeSize;
      while(mapPtr->freeSize != -1){
        mapPtr++;
        (mapPtr-1)->freeAddr = mapPtr->freeAddr;
        (mapPtr-1)->freeSize = mapPtr->freeSize;
      }//while
    }//if
  }else{
    if(addr+size == mapPtr->freeAddr && mapPtr->freeSize != -1){
      mapPtr->freeAddr -= size;
      mapPtr->freeSize += size;
    }else if(size){
      do{
        tempAddr = mapPtr->freeAddr;
        mapPtr->freeAddr = addr;
        addr = tempAddr;
        tempAddr = mapPtr->freeSize;
        mapPtr->freeSize = size;
        mapPtr++;
      }while(size == tempAddr);
    }//if else if
  }//if else
}//mymfree

void mymdump(struct myMemoryMap* memMap){
  int i=0;
  do{
    printf("Entry %d\t%d\t%d\n", i, memMap->freeAddr, memMap->freeSize);
    memMap++;
    i++;
  }while((memMap-1)->freeSize != -1);//make sure we print out the end of the map
}//mymdump
