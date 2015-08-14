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

#include "display_utils.h"
#include "display_frame.h"
#include "frames_feeder.h"
#include "xparameters.h"
#include "ximage_inverter.h"

#include "netif/xadapter.h"

#include "platform.h"
#include "platform_config.h"
#ifdef __arm__
#include "xil_printf.h"
#endif

XImage_inverter imageInverterIp;

void processFrame(u32 frameIdx, u32 displayFrameIdx)
{
	u32 *framePtr = feederGetFramePtr(frameIdx);
	u32 *displayPtr = displayUtilsGetFramePtr(displayFrameIdx);
	Xil_DCacheFlushRange((unsigned int) framePtr, DISPLAY_FRAME_SIZE * 4);
	XImage_inverter_Set_pixelsN(&imageInverterIp, DISPLAY_FRAME_SIZE);
	XImage_inverter_Set_ptrInput(&imageInverterIp, (unsigned int) framePtr);
	XImage_inverter_Set_ptrOutput(&imageInverterIp, (unsigned int) displayPtr);

	while(!XImage_inverter_IsReady(&imageInverterIp));
	XImage_inverter_Start(&imageInverterIp);
	while(!XImage_inverter_IsDone(&imageInverterIp));
}

void init_image_inverter_ip()
{
	int status = XImage_inverter_Initialize(&imageInverterIp, XPAR_XIMAGE_INVERTER_0_DEVICE_ID);
	if(status != XST_SUCCESS)
	{
		xil_printf("Error al inicializar image inverter: %d!\r\n", status);
	}
}

/* missing declaration in lwIP */
void lwip_init();

static struct netif server_netif;
struct netif *echo_netif;

void
print_ip(char *msg, struct ip_addr *ip) 
{
	print(msg);
	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip), 
			ip4_addr3(ip), ip4_addr4(ip));
}

void
print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}

#if XPAR_GIGE_PCS_PMA_CORE_PRESENT == 1
int ProgramSi5324(void);
int ProgramSfpPhy(void);
#endif


int main()
{
	struct ip_addr ipaddr, netmask, gw;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =
	{ 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	echo_netif = &server_netif;
#if XPAR_GIGE_PCS_PMA_CORE_PRESENT == 1
	ProgramSi5324();
	ProgramSfpPhy();
#endif

	init_platform();
	/* initliaze IP addresses to be used */
	IP4_ADDR(&ipaddr,  10, 0,   0, 2);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      10, 0,   0,  1);

	print_app_header();
	print_ip_settings(&ipaddr, &netmask, &gw);

	lwip_init();

	// Inicializar IP
	init_image_inverter_ip();
  	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(echo_netif, &ipaddr, &netmask,
						&gw, mac_ethernet_address,
						PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return -1;
	}
	netif_set_default(echo_netif);

	/* Create a new DHCP client for this interface.
	 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
	 * the predefined regular intervals after starting the client.
	 */
	/* dhcp_start(echo_netif); */

	/* now enable interrupts */
	platform_enable_interrupts();

	/* specify that the network if is up */
	netif_set_up(echo_netif);

	/* start the application (web server, rxtest, txtest, etc..) */
	start_application();

	init_display();
	//
	/* receive and process packets */
	int rawFrameIdx=-1;
	int displayFrameIdx=-1;
	int status = 0;
	while (1) {
		xemacif_input(echo_netif);
		switch(status)
		{
			case 0: // READY
				if(rawFrameIdx ==-1)
				{
					rawFrameIdx = feederGetCompleteFrame();
				}
				if(rawFrameIdx > -1)
				{
					displayFrameIdx = displayUtilsGetFreeFrame();
					// Procesar
					status = 1;
				}
				break;
			case 1: // Empezar a procesando
				processFrame(rawFrameIdx, displayFrameIdx);
				status = 2;
				break;
			case 2: // Wait
				status = 3;
				break;
			case 3: // DONE
				displayUtilsChangeFrame(displayFrameIdx);
				feederReleaseFrame(rawFrameIdx);
				rawFrameIdx = -1;
				displayFrameIdx = -1;
				status = 0;
				break;
		}
		//xil_printf("status: %d\r\n", status);
	}
	/* never reached */
	cleanup_platform();
	return 0;
}
