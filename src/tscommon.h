/*
 * tscommon.h
 *
 *  Created on: Apr 25, 2013
 *      Author: liyajie1209
 */

#ifndef TSCOMMON_H_
#define TSCOMMON_H_
#include <stdint.h>

/*every packat*/
/*The TS header*/
typedef struct {
	uint8_t sync_byte;
	uint8_t transport_error_indicator;
	uint8_t payload_unit_start_indicator;
	uint8_t transport_priority;
	uint16_t PID;
	uint8_t transport_scrambling_control;
	uint8_t adaptation_feild_control;
	uint8_t continuity_counter;
} ts_header;

/**
 * PMT
 */
typedef struct {
	uint8_t table_id;
	uint8_t section_syntax_indicator;
	uint8_t reserved0;
	uint8_t reserved1;
	uint16_t section_length;
	uint16_t program_number;
	uint8_t reserved2;
	uint8_t version_number;
	uint8_t current_next_indicator;
	uint8_t section_number;
	uint8_t last_section_number;
	uint8_t reserved3;
	uint16_t PCR_PID;
	uint8_t reserved4;
	uint16_t program_info_length;
	uint8_t stream_type;
	uint16_t elementary_PID;
	uint16_t ES_info_length;
	uint8_t stream_video_type;
	uint16_t elementary_video_PID;
	uint8_t stream_audio_type;
	uint16_t elementary_audio_PID;
	uint32_t CRC_32;
} pmt_header;

typedef struct {
	uint8_t table_id;
	uint8_t section_syntax_indicator;
	uint8_t reserved0;
	uint8_t reserved1;
	uint16_t section_length;
	uint8_t transport_stream_id;
	uint8_t reserved2;
	uint8_t version_number;
	uint8_t current_next_indicator;
	uint8_t section_number;
	uint8_t last_section_number;
	uint16_t program_number; /*我们只写一个节目*/
	uint16_t program_map_PID; /*也只有一个对应的PMT*/
	uint32_t CRC_32;
} pat_header;

typedef struct {
	uint32_t packet_start_code_prefix_24bit;
	uint8_t stream_id_8bit;
	uint16_t PES_packet_length_16bit;
	uint8_t reserved_const_bit_10;
	uint8_t PES_scrambling_control_2bit;
	uint8_t PES_priority_1bit;
	uint8_t data_alignment_indicator_1bit;
	uint8_t copyright_1bit;
	uint8_t original_or_copy_1bit;
	uint8_t PTS_DTS_flags_2bit;
	uint8_t ESCR_flag_1bit;
	uint8_t ES_rate_flag_1bit;
	uint8_t DSM_trick_mode_flag_1bit;
	uint8_t additional_copy_info_flag_1bit;
	uint8_t PES_CRC_flag_1bit;
	uint8_t PES_extension_flag_1bit;
	uint8_t PES_header_data_length_8bit;
	uint64_t ptstime;
} pes_header;
#endif /* TSCOMMON_H_ */
