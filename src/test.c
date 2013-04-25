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

int main(int argc, char **argv) {
	if(argc != 2)
		{
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
		while ((ret = read(fd,packet,188)) == 188) {
			ts_header header;
			uint16_t pid = packet_get_pid(packet);
			printf("the pid %d \n",pid);
			packet_header_parse(packet,&header);
			packet_header_dump(&header);
		}
		xfree(packet);
	return 0;
}
