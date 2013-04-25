/*
 * ts.c
 *
 *  Created on: Apr 25, 2013
 *      Author: liyajie1209
 */

#include "ts.h"
#include <stdio.h>


uint16_t packet_get_pid(uint8_t* packet_data) {
	return (packet_data[1] &~ 0xE0) << 8 | packet_data[2];
}
/***
 * parse the ts packet header
 * and the send the ts_header parameter
 */
uint8_t * packet_header_parse(uint8_t *packet_data, ts_header *p_header) {
	if (packet_data[0] != 0x47) {
		// ts_LOGf("*** TS packet do not start with sync byte 0x47 but with 0x%02x\n", ts_packet[0]);
//		goto return_error;
	}

	p_header->transport_error_indicator = (packet_data[1] & ~0x7f) >> 7; // x1111111
	p_header->payload_unit_start_indicator = (packet_data[1] & ~0xbf) >> 6;
	p_header->transport_priority = (packet_data[1] & ~0xdf) >> 5; // 11x11111
	p_header->PID = (packet_data[1] & ~0xE0) << 8 | packet_data[2]; // 111xxxxx xxxxxxxx

	p_header->transport_scrambling_control = (packet_data[3] & ~0x3F) >> 6; // xx111111
	p_header->adaptation_feild_control = (packet_data[3] & ~0xDF) >> 5; // 11x11111
//	ts_header->payload_field = (packet_data[3] & ~0xEF) >> 4; // 111x1111
	p_header->continuity_counter = (packet_data[3] & ~0xF0);      // 1111xxxx

	if (!p_header->adaptation_feild_control) {
		p_header->adaptation.adaptation_field_length = 0;
		p_header->adaptation.adaptation_field_extension_flag = 0;
//		ts_header->payload_offset = 4;
	} else {
		p_header->adaptation.adaptation_field_length = packet_data[4];
//		ts_header->adapt_len = ts_packet[4];
		if (p_header->adaptation.adaptation_field_length) {
			p_header->adaptation.adaptation_field_extension_flag = packet_data[5];
			//ts_header->adapt_flags = ts_packet[5];
		}
//		ts_header->payload_offset = 5 + ts_header->adapt_len; // 2 bytes see above
	}

//	if (!ts_header->adapt_field && !ts_header->payload_field) // Not allowed
//		goto return_error;
//
//	if (!ts_header->payload_field)
//		return NULL;

//	if (ts_header->payload_field && ts_header->adapt_len > 182) // Validity checks
//		goto return_error;
//	if (!ts_header->payload_field && ts_header->adapt_len > 183)
//		goto return_error;
//
//	if (ts_header->payload_offset > TS_MAX_PAYLOAD_SIZE) // Validity check
//		goto return_error;
//
//	ts_header->payload_size = TS_PACKET_SIZE - ts_header->payload_offset;
//
//	return ts_packet + ts_header->payload_offset;
//
//	return_error: memset(ts_header, 0, sizeof(struct ts_header));
	return 0;
}
/***
 * generate the header
 */
void packet_header_generate(uint8_t *packet_data, ts_header *p_header) {

}
/**
 * dump the ts header infomations
 */
void packet_header_dump(ts_header *ts_header) {
	printf("*** tei:%d pusi:%d prio:%d pid:%04x (%d) scramble:%d adapt:%d payload:%d adapt_len:%d adapt_flags:%d\n",
			ts_header->transport_error_indicator,
			ts_header->payload_unit_start_indicator,
			ts_header->transport_priority,
			ts_header->PID,
			ts_header->PID,
			ts_header->transport_scrambling_control,
			ts_header->adaptation_feild_control,
			ts_header->payload_unit_start_indicator,
			ts_header->adaptation.adaptation_field_length,
			ts_header->adaptation.adaptation_field_extension_flag
//			ts_header->payload_offset,
//			ts_header->payload_size
		);
}

