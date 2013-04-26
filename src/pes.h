/*
 * pes.h
 *
 *  Created on: Apr 26, 2013
 *      Author: liyajie1209
 */

#include "tscommon.h"


#ifndef PES_H_
#define PES_H_

/**
 * @return return the pes header struct
 */
pes_header* pes_alloc();

/**
 * reset the pes header data set
 * @param pes header
 */
void pes_clear_dataset(pes_header * pes_header);
/**
 * @param destroy pes header data
 */
void pes_destroy(pes_header *pes_header);
/**
 * @param pes header data
 */
void pes_dump(pes_header *pes_header);

/**
 * @param remain dataset to parse of
 * @param pes_header the pes_header to store
 */
void pes_parse(uint8_t* payload_data,pes_header* pes_header);

#endif /* PES_H_ */
