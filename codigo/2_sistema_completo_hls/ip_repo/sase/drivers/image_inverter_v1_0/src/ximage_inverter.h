// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2014.2
// Copyright (C) 2014 Xilinx Inc. All rights reserved.
// 
// ==============================================================

#ifndef XIMAGE_INVERTER_H
#define XIMAGE_INVERTER_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "ximage_inverter_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axilites_BaseAddress;
} XImage_inverter_Config;
#endif

typedef struct {
    u32 Axilites_BaseAddress;
    u32 IsReady;
} XImage_inverter;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XImage_inverter_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XImage_inverter_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XImage_inverter_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XImage_inverter_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XImage_inverter_Initialize(XImage_inverter *InstancePtr, u16 DeviceId);
XImage_inverter_Config* XImage_inverter_LookupConfig(u16 DeviceId);
int XImage_inverter_CfgInitialize(XImage_inverter *InstancePtr, XImage_inverter_Config *ConfigPtr);
#else
int XImage_inverter_Initialize(XImage_inverter *InstancePtr, const char* InstanceName);
int XImage_inverter_Release(XImage_inverter *InstancePtr);
#endif

void XImage_inverter_Start(XImage_inverter *InstancePtr);
u32 XImage_inverter_IsDone(XImage_inverter *InstancePtr);
u32 XImage_inverter_IsIdle(XImage_inverter *InstancePtr);
u32 XImage_inverter_IsReady(XImage_inverter *InstancePtr);
void XImage_inverter_EnableAutoRestart(XImage_inverter *InstancePtr);
void XImage_inverter_DisableAutoRestart(XImage_inverter *InstancePtr);

void XImage_inverter_Set_ptrInput(XImage_inverter *InstancePtr, u32 Data);
u32 XImage_inverter_Get_ptrInput(XImage_inverter *InstancePtr);
void XImage_inverter_Set_ptrOutput(XImage_inverter *InstancePtr, u32 Data);
u32 XImage_inverter_Get_ptrOutput(XImage_inverter *InstancePtr);
void XImage_inverter_Set_pixelsN(XImage_inverter *InstancePtr, u32 Data);
u32 XImage_inverter_Get_pixelsN(XImage_inverter *InstancePtr);

void XImage_inverter_InterruptGlobalEnable(XImage_inverter *InstancePtr);
void XImage_inverter_InterruptGlobalDisable(XImage_inverter *InstancePtr);
void XImage_inverter_InterruptEnable(XImage_inverter *InstancePtr, u32 Mask);
void XImage_inverter_InterruptDisable(XImage_inverter *InstancePtr, u32 Mask);
void XImage_inverter_InterruptClear(XImage_inverter *InstancePtr, u32 Mask);
u32 XImage_inverter_InterruptGetEnabled(XImage_inverter *InstancePtr);
u32 XImage_inverter_InterruptGetStatus(XImage_inverter *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
