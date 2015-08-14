/************************************************************************/
/*																		*/
/*	display_utils.h	--	ZYBO Display demonstration 						*/
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

#ifndef DISPLAY_UTILS_H_
#define DISPLAY_UTILS_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "xil_types.h"
#include "display_ctrl.h"

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

int DisplayUtilsInitialize(DisplayCtrl *dispPtr, u16 vdmaId, u16 timerId, u32 dispCtrlAddr, int fHdmi, u32 *framePtr[DISPLAY_NUM_FRAMES]);
void DisplayUtilsPrintTest(u32 *frame, u32 width, u32 height, u32 stride);
void init_display();
int displayUtilsGetFreeFrame();
void displayUtilsChangeFrame(u32 displayFrameIdx);
u32* displayUtilsGetFramePtr(u32 frameIdx);

/* ------------------------------------------------------------ */

/************************************************************************/

#endif /* DISPLAY_UTILS_H_ */
