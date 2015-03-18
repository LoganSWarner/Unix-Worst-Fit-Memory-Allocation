/*
 * mem.h
 *
 *  Created on: Feb 7, 2014
 *      Author: LSWarner
 */

#ifndef MEM_H_
#define MEM_H_

struct myMemoryMap{
    char* freeAddr;
    char* freeSize;//0 means border of two adjacent used blocks, -1 means end of map
};

#endif /* MEM_H_ */
