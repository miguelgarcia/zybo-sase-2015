// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2014.2
// Copyright (C) 2014 Xilinx Inc. All rights reserved.
// 
// ==============================================================

#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "ximage_inverter.h"

extern XImage_inverter_Config XImage_inverter_ConfigTable[];

XImage_inverter_Config *XImage_inverter_LookupConfig(u16 DeviceId) {
	XImage_inverter_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XIMAGE_INVERTER_NUM_INSTANCES; Index++) {
		if (XImage_inverter_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XImage_inverter_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XImage_inverter_Initialize(XImage_inverter *InstancePtr, u16 DeviceId) {
	XImage_inverter_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XImage_inverter_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XImage_inverter_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

