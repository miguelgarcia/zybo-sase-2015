/*
 * Copyright (c) 2009-2013 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#ifdef __arm__
#include "xil_printf.h"
#endif

#include "display_frame.h"
#include "frames_feeder.h"

#define FEEDER_FRAMES_NUMBER 3

// Frames status
#define FRAME_FREE 0
#define FRAME_COMPLETE 1
#define FRAME_USED 2

u32 frames[FEEDER_FRAMES_NUMBER][DISPLAY_FRAME_SIZE]  __aligned(64);;
u32 frames_status[FEEDER_FRAMES_NUMBER];
int currentFrame;
u8 *frameBuf;
u32 bufLen;
u32 written3;

u8 *origFrameBuf;
u32 origBufLen;
u32 lastBufLen;
u32 errCount;

struct tcp_pcb *gpcb = NULL;
char *fixed_msg = "a";

void setBuffer(u8 *buf)
{
	origFrameBuf = frameBuf = buf;
	origBufLen = lastBufLen = bufLen = DISPLAY_FRAME_SIZE*4;
	errCount = 0;
	written3=0;

	err_t err;
	err = tcp_write(gpcb, fixed_msg, 1, 0);
	if(err != ERR_OK)
	{
		xil_printf("problem asking one more frame\r\n");
	}
	err = tcp_output(gpcb);
	if(err != ERR_OK)
	{
		xil_printf("problem tcp_output\r\n");
	}

}


void sched()
{
	int i;

	if(currentFrame == -1 || frames_status[currentFrame] != FRAME_FREE)
	{	// Buscar un frame libre para llenar
		for(i=0;i<FEEDER_FRAMES_NUMBER;i++)
		{
			if(frames_status[i] == FRAME_FREE)
			{
				currentFrame = i;
				setBuffer((u8 *)frames[currentFrame]);
				return;
			}
		}
		xil_printf("no free frame to fill\r\n");
	}


}

void feederReleaseFrame(u32 frameIdx)
{
	if(frameIdx >= FEEDER_FRAMES_NUMBER)
	{
		xil_printf("Error releasing frame: %d\r\n", frameIdx);
	}

	frames_status[frameIdx] = FRAME_FREE;
	sched();
}

int feederGetCompleteFrame()
{
	u32 i;
	for(i=0; i < FEEDER_FRAMES_NUMBER; i++)
	{
		if(frames_status[i] == FRAME_COMPLETE)
		{
			frames_status[i] = FRAME_USED;
			return i;
		}
	}
	return -1;
}

u32 *feederGetFramePtr(u32 frameIdx)
{
	return frames[frameIdx];
}

err_t on_poll(void *arg, struct tcp_pcb *tpcb)
{
	if(bufLen == 0)
		return ERR_OK;

	if(lastBufLen == bufLen)
	{
		errCount++;
		if(errCount == 2)
		{
			xil_printf("Error detected: restarting frame!\r\n");
			setBuffer(origFrameBuf);
		}
	}
	else
	{
		lastBufLen = bufLen;
	}
	return ERR_OK;
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP TCP image client ------\n\r");
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	u32 i;
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		//tcp_recv(tpcb, NULL);
		return ERR_OK;
	}


	/* echo back the payload */
	/* in this case, we assume that the payload is < TCP_SND_BUF */
	u8 *data = (u8 *) p->payload;
	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	if(p->len > bufLen)
	{
		xil_printf("error: demasiados datos\r\n");
	}

	for(i=0; i< p->len && bufLen > 0; i++)
	{
		*frameBuf++ = *data++;
		bufLen--;
		written3++;
		if(written3 == 3)
		{
			written3 = 0;
			*frameBuf++ = 0;
			bufLen--;
		}

	}

	/* free the received pbuf */
	pbuf_free(p);

	if(bufLen == 0)
	{
		frames_status[currentFrame] = FRAME_COMPLETE;
		sched();
	}
	return ERR_OK;
}


err_t on_connect(void *arg, struct tcp_pcb *pcb, err_t err)
{
	if(err != ERR_OK)
	{
		return err;
	}
	xil_printf("connected\r\n");
	tcp_recv(pcb, recv_callback);

	gpcb = pcb;
	sched();
	return ERR_OK;
}

int start_application()
{
	int i;
	struct tcp_pcb *pcb;
	err_t err;

	struct ip_addr server_ipaddr;
	IP4_ADDR(&server_ipaddr,  10, 0,   0, 1);

	bufLen = 0;
	currentFrame = -1;
	for(i=0;i<FEEDER_FRAMES_NUMBER;i++)
	{
		frames_status[i] = FRAME_FREE;
	}
	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	err = tcp_connect(pcb, &server_ipaddr, 1080, on_connect);

	if (err != ERR_OK) {
		xil_printf("Unable to initialize connection: err = %d\n\r", err);
		return -2;
	}

	tcp_poll(pcb, on_poll, 1);
	return 0;
}

