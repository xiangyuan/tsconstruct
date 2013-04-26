/*
 * test.c
 *
 *  Created on: Apr 25, 2013
 *      Author: liyajie1209
 */

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "tscommon.h"
#include <unistd.h>
#include "xmemory.h"
#include "pes.h"
#define IS_AUDIO_STREAM_ID(id)				((id) >= 0xc0 && (id) <= 0xdf)
#define IS_VIDEO_STREAM_ID(id)				((id) >= 0xe0 && (id) <= 0xef)

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s filename.ts\n", *argv);
		return 1;
	}

	argv++;

	int fd = open(*argv, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	int ret;
	uint8_t* packet = xmalloc(sizeof(uint8_t));
	while ((ret = read(fd, packet, 188)) == 188) {
		ts_header header;
		uint16_t pid = packet_get_pid(packet);
		printf("the pid %0x \n", pid);
		uint8_t * remain_data = packet_header_parse(packet, &header);
		packet_header_dump(&header);

		if (header.payload_unit_start_indicator == 1) {
			pes_header pes;
//			pes_alloc(&pes);
			pes_parse(remain_data, &pes);
			if (IS_VIDEO_STREAM_ID(pes.stream_id_8bit)) {
				pes_dump(&pes);
			} else if (IS_AUDIO_STREAM_ID(pes.stream_id_8bit)) {
				printf("is Audio\n");
			}
		}
	}
	xfree(packet);
	return 0;
}
