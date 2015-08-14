/************************************************************************/
/*																		*/
/*	display_utils.c	--	ZYBO Display demonstration 						*/
/*																		*/
/************************************************************************/
/*	Author: Sam Bobrowicz												*/
/*	Copyright 2014, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*		This file contains code for running a demonstration of the		*/
/*		Video output capabilities on the ZYBO. It is a good example of  */
/*		how to properly use the display_ctrl driver.					*/
/*																		*/
/*		This module contains code from the Xilinx Demo titled			*/
/*		" xiicps_polled_master_example.c"								*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/* 																		*/
/*		2/20/2014(SamB): Created										*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "display_utils.h"
#include "display_frame.h"
#include <stdio.h>
#include "xuartps.h"
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "display_ctrl.h"
#include "xparameters.h"

// Framebuffers para la salida VGA
char _a; // align !!!
u32 vgaBuf[DISPLAY_NUM_FRAMES][DISPLAY_FRAME_SIZE]  __aligned(64);;

// Controlador VGA
DisplayCtrl vgaCtrl;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

#define VGA_BASEADDR XPAR_AXI_DISPCTRL_0_S_AXI_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define SCU_TIMER_ID XPAR_SCUTIMER_DEVICE_ID

/**
 * Inicializa el controlador de video apuntando a los frame buffers vgaBuf.
 */
void init_display()
{
	u32 *vgaPtr[DISPLAY_NUM_FRAMES];
	int i;
	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		vgaPtr[i] = vgaBuf[i];
	}
	DisplayUtilsInitialize(&vgaCtrl, VGA_VDMA_ID, SCU_TIMER_ID, VGA_BASEADDR, DISPLAY_NOT_HDMI, vgaPtr);
}

int DisplayUtilsInitialize(DisplayCtrl *dispPtr, u16 vdmaId, u16 timerId, u32 dispCtrlAddr, int fHdmi, u32 *framePtr[DISPLAY_NUM_FRAMES])
{
	int Status;


	Status = DisplayInitialize(dispPtr, vdmaId, dispCtrlAddr, fHdmi, framePtr, DISPLAY_FRAME_STRIDE);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Display Ctrl initialization failed during demo initialization%d\r\n", Status);
		return XST_FAILURE;
	}

	Status = DisplayStart(dispPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Couldn't start display during demo initialization%d\r\n", Status);
		return XST_FAILURE;
	}

	DisplayUtilsPrintTest(dispPtr->framePtr[dispPtr->curFrame], dispPtr->vMode.width, dispPtr->vMode.height, dispPtr->stride);

	return XST_SUCCESS;
}

void DisplayUtilsPrintTest(u32 *frame, u32 width, u32 height, u32 stride)
{
	xil_printf("height: %x\r\n", height);
	u32 xcoi, ycoi;
	u32 iPixelAddr;
	u32 wStride;
	u32 wRed, wBlue, wGreen, wColor;
	u32 wCurrentInt;
	double fRed, fBlue, fGreen, fColor;
	u32 xLeft, xMid, xRight, xInt;
	u32 yMid, yInt;
	double xInc, yInc;


		wStride = stride / 4; /* Find the stride in 32-bit words */

	xInt = width / 7; //Seven intervals, each with width/7 pixels
	xInc = 256.0 / ((double) xInt); //256 color intensities per interval. Notice that overflow is handled for this pattern.

	fColor = 0.0;
	wCurrentInt = 1;
	for(xcoi = 0; xcoi < width; xcoi++)
	{
		if (wCurrentInt & 0b001)
			fRed = fColor;
		else
			fRed = 0.0;

		if (wCurrentInt & 0b010)
			fBlue = fColor;
		else
			fBlue = 0.0;

		if (wCurrentInt & 0b100)
			fGreen = fColor;
		else
			fGreen = 0.0;

		/*
		 * Just draw white in the last partial interval (when width is not divisible by 7)
		 */
		if (wCurrentInt > 7)
		{
			wColor = 0x00FFFFFF;
		}
		else
		{
			wColor = ((u32) fRed << BIT_DISPLAY_RED) | ((u32) fBlue << BIT_DISPLAY_BLUE) | ( (u32) fGreen << BIT_DISPLAY_GREEN);
		}
		iPixelAddr = xcoi;

		for(ycoi = 0; ycoi < height; ycoi++)
		{
			frame[iPixelAddr] = wColor;
			/*
			 * This pattern is printed one vertical line at a time, so the address must be incremented
			 * by the stride instead of just 1.
			 */
			iPixelAddr += wStride;
		}

		fColor += xInc;
		if (fColor >= 256.0)
		{
			fColor = 0.0;
			wCurrentInt++;
		}
	}
	/*
	 * Flush the framebuffer memory range to ensure changes are written to the
	 * actual memory, and therefore accessible by the VDMA.
	 */
	Xil_DCacheFlushRange((unsigned int) frame, DISPLAY_FRAME_SIZE * 4);
}


int displayUtilsGetFreeFrame()
{	// Cola circular de frames
	int frame = vgaCtrl.curFrame + 1;
	return frame >= DISPLAY_NUM_FRAMES ? 0 : frame;
}

void displayUtilsChangeFrame(u32 frameIdx)
{
	//Xil_DCacheFlushRange((unsigned int) vgaBuf[frameIdx], DISPLAY_FRAME_SIZE * 4);
	DisplayChangeFrame(&vgaCtrl, frameIdx);
}

u32* displayUtilsGetFramePtr(u32 frameIdx)
{
	return vgaBuf[frameIdx];
}

