/*
 * pes.c
 *
 *  Created on: Apr 26, 2013
 *      Author: liyajie1209
 */

#include "pes.h"
#include <stdio.h>
#include "xmemory.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PES_BUFFER_SIZE 1024


/**
 * @return return the pes header struct
 */
pes_header* pes_alloc() {
	pes_header *pes = calloc(1, sizeof(pes_header));
//	pes->PES_packet_length_16bit = DEFAULT_PES_BUFFER_SIZE;
//	pes->pes_data_size = DEFAULT_PES_BUFFER_SIZE;
//	pes->pes_data = malloc(pes->pes_data_size);
//	memset(pes->pes_data, 0x33, pes->pes_data_size);
	return pes;
}

/**
 * reset the pes header data set
 * @param pes header
 */
void pes_clear_dataset(pes_header * pes_header) {
	if (!pes_header)
		return;
	// save
	memset(pes_header, 0, sizeof(pes_header));
//		uint8_t *pes_data = pes_header->pes_data;
//		uint32_t pes_data_size = pes_header->pes_data_size;
//		// clear
//		memset(pes_data, 0x33, pes_data_size);
//		memset(pes_header, 0, sizeof(struct ts_pes));
//		// restore
//		pes_header->pes_data = pes_data;
//		pes_header->pes_data_size = pes_data_size;
}
/**
 * @param destroy pes header data
 */
void pes_destroy(pes_header *pes_header) {
	if (pes_header) {
		xfree(pes_header);
	}
}

/**
 * @param remain dataset to parse of
 * @param pes_header the pes_header to store
 */
void pes_parse(uint8_t* payload_data, pes_header* pes) {
	uint8_t stream_id = 0;
	int pes_packet_len = 0;
	if (payload_data[0] == 0x00 && payload_data[1] == 0x00
			&& payload_data[2] == 0x01) { // pes_start_code_prefix
//		pes->ts_header = ts_header;
		stream_id = payload_data[3];
		pes_packet_len = (payload_data[4] << 8) | payload_data[5];
		//			ts_LOGf("NEW PES. Stream_id=%02x pes_length=%d\n", stream_id, pes_packet_len);
		if ((pes_packet_len == 0) && (pes->PES_packet_length_16bit == (uint16_t)-1)) {
			printf(
					"!!! ERROR: New pes with pes_packed_len == 0, started before the old was finished\n");
		}
		if (pes_packet_len > 0) {
			pes->PES_packet_length_16bit = pes_packet_len;
		} else {
			pes->PES_packet_length_16bit = -1;
		}
		pes->stream_id_8bit = stream_id;
		pes->PES_packet_length_16bit = pes_packet_len;
		/*remain parse*/
	} else {
		printf("[INFO]:first four data %0x %0x %0x %0x", payload_data[0],
				payload_data[1], payload_data[2], payload_data[3]);
	}
}
/**
 * @param pes header data
 */
void pes_dump(pes_header *pes_header) {
	printf("streamid %04x pes_length %d\n", pes_header->stream_id_8bit,
			pes_header->PES_packet_length_16bit);
}

