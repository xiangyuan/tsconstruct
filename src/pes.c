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
#define bit_on(__bit, __mask) ((__bit & __mask) ? 1 : 0)
#define bit_1 (0x01)
#define bit_2 (0x02)
#define bit_3 (0x04)
#define bit_4 (0x08)
#define bit_5 (0x10)
#define bit_6 (0x20)
#define bit_7 (0x40)
#define bit_8 (0x80)

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

void ts_decode_pts_dts(uint8_t *data, uint64_t *value) {
	uint64_t pts1 = ((unsigned int)data[0] & 0x0E) >> 1;
	uint64_t pts2 = ((unsigned int)data[1] << 7) | (((unsigned int)data[2] & 0xFE) >> 1);
	uint64_t pts3 = ((unsigned int)data[3] << 7) | (((unsigned int)data[4] & 0xFE) >> 1);
	*value = (pts1 << 30) | (pts2 << 15) | pts3;
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
		if ((pes_packet_len == 0)
				&& (pes->PES_packet_length_16bit == (uint16_t) -1)) {
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
		int dpos = 6;
		if ((payload_data[dpos] & ~0x3f) != 0x80) {	// 10xxxxxx (first two bits must be 10 eq 0x80
			printf(
					"!!! No 10 bits at PES start, expected 0x80 get 0x%02x, org: %02x! \n",
					payload_data[dpos] & ~0x3f, payload_data[dpos]);
			return;
		}
		// data[dpos] = 0xff;
//		pes->flags_1 = payload_data[dpos];
		pes->PES_scrambling_control_2bit = (payload_data[dpos] & ~0xCF) >> 4;
		pes->PES_priority_1bit = bit_on(payload_data[dpos], bit_4);
		pes->data_alignment_indicator_1bit = bit_on(payload_data[dpos], bit_3);
		pes->copyright_1bit = bit_on(payload_data[dpos], bit_2);
		pes->original_or_copy_1bit = bit_on(payload_data[dpos], bit_1);

		dpos = 7;
		// data[dpos] = 0xff;
//		pes->flags_2 = payload_data[dpos];
		pes->PTS_DTS_flags_2bit = (payload_data[dpos] & ~0x3F) >> 6;
//		pes->DTS_flag = bit_on(payload_data[dpos], bit_7);
		pes->ESCR_flag_1bit = bit_on(payload_data[dpos], bit_6);
		pes->ES_rate_flag_1bit = bit_on(payload_data[dpos], bit_5);
		pes->DSM_trick_mode_flag_1bit = bit_on(payload_data[dpos], bit_4);
		pes->additional_copy_info_flag_1bit = bit_on(payload_data[dpos], bit_3);
		pes->PES_CRC_flag_1bit = bit_on(payload_data[dpos], bit_2);
		pes->PES_extension_flag_1bit = bit_on(payload_data[dpos], bit_1);
		dpos = 8;

		pes->PES_header_data_length_8bit = payload_data[dpos];
		dpos = 9;

		if (pes->PTS_DTS_flags_2bit == 3) {
			ts_decode_pts_dts(&payload_data[dpos],&pes->ptstime);
			printf("[INFO:the ptstime %Lf\n",pes->ptstime);
		}
	} else {
		printf("[INFO]:first four data %0x %0x %0x %0x", payload_data[0],
				payload_data[1], payload_data[2], payload_data[3]);
	}
}
/**
 * @param pes header data
 */
void pes_dump(pes_header *pes_header) {
	printf("[INFO]:streamid %04x pes_length %d\n"
			"[INFO]:pes crambling %d pes_priority %d\n"
			"[INFO]:data_align_indicator %d copyrith %d\n"
			"[INFO]:pts-dts_flag %d \n"
			"[INFO]:escr_flag %d \n"
			"[INFO]:es_rate_flag %d\n"
			"[INFO]:dsm_mode_flag %d\n"
			"[INFO]:pes_extends flag %d\n"
			"[INFO]:pes_header_data_length %d\n", pes_header->stream_id_8bit,
			pes_header->PES_packet_length_16bit,
			pes_header->PES_scrambling_control_2bit,
			pes_header->PES_priority_1bit,
			pes_header->data_alignment_indicator_1bit,
			pes_header->copyright_1bit,
			pes_header->PTS_DTS_flags_2bit,
			pes_header->ESCR_flag_1bit,
			pes_header->ES_rate_flag_1bit,
			pes_header->DSM_trick_mode_flag_1bit,
			pes_header->PES_extension_flag_1bit,
			pes_header->PES_header_data_length_8bit);
}

