/*
 * cc3000tasks.h
 *
 *  Created on: 17 Jul 2015
 *      Author: zhiyb
 */

#ifndef CC3000TASKS_H_
#define CC3000TASKS_H_

#include <stdint.h>
#include <cc3000.h>

#include "FreeRTOSConfig.h"
#include <FreeRTOS.h>
#include <queue.h>

extern struct cc3000_info_t {
	enum {Disconnected = 0, Connecting = 0x1000, Connected = 0x8000, ConnectionMask = 0xF000,
		DHCPSuccess = 0x0800, DHCPFailed = 0x0100, DHCPMask = 0x0F00,
		SocketDisconnected = 0, SocketConnecting = 0x0010, SocketConnected = 0x0080, SocketMask = 0x00F0,
		NoActions = 0, ActionMask = 0x000F, TimeSynced = 0x0008};
	volatile uint16_t state;
	volatile bool newEvent;
	INT32 socket;
	uint8_t ip[4];
} cc3000;

void CC3000_UsynchCallback(long	lEventType, char *data,	unsigned char length);
void wifiMangTask(void *pvParameters);
void cc3000INTTask(void *pvParameters);
void createWiFiTasks();

#endif /* CC3000TASKS_H_ */
