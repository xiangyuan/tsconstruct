/*
 * ts.h
 *
 *  Created on: Apr 25, 2013
 *      Author: liyajie1209
 */

#ifndef TS_H_
#define TS_H_
#include "tscommon.h"

uint16_t packet_get_pid(uint8_t* packet_data);
/***
 * parse the ts packet header
 * and the send the ts_header parameter
 */
uint8_t * packet_header_parse(uint8_t *packet_data,
		 ts_header *p_header);
/***
 * generate the header
 */
void packet_header_generate(uint8_t *packet_data,ts_header *p_header);
/**
 * dump the ts header infomations
 */
void packet_header_dump(ts_header *p_header);

#endif /* TS_H_ */
