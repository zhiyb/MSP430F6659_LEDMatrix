/*
 * cc3000tasks.cpp
 *
 *  Created on: 17 Jul 2015
 *      Author: zhiyb
 */

#include <stdio.h>
#include <string.h>
#include "rtc.h"
#include "uart.h"

#include "cc3000tasks.h"
#include <cc3000.h>
#include <host_driver_version.h>
#include <wlan.h>
#include <hci.h>
#include <nvmem.h>
#include <netapp.h>
#include <socket.h>
#include <spi.h>

#include <task.h>

#define RFC868_SERVER	"time-nw.nist.gov"
#define RFC868_PORT	37
#define TIME_ZONE	(+8)

#define	NETAPP_IPCONFIG_MAC_OFFSET	(20)
#define	HCI_EVENT_MASK	(HCI_EVNT_WLAN_KEEPALIVE | HCI_EVNT_WLAN_UNSOL_INIT /*|	HCI_EVNT_WLAN_ASYNC_PING_REPORT*/)

struct cc3000_info_t cc3000;
QueueHandle_t xCC3000INTQueue = NULL;

void doTimeSync(void)
{
	const rtc::time_t& tm = rtc::time();
	// Do time sync every hour
	//if (tm.i.sec == 0 && tm.i.min == 0)
	// Do time sync every 10 seconds (debug mode)
	if ((tm.i.sec & 0x0F) == 0)
		cc3000.state &= ~cc3000_info_t::TimeSynced;

	if (cc3000.state & cc3000_info_t::TimeSynced)
		return;
	puts("doTimeSync(");

	UINT32 ipAddr = 0;
	if (gethostbyname(RFC868_SERVER, strlen(RFC868_SERVER), &ipAddr) < 0 || ipAddr == 0)
		return;
	printf("DNS resolve: %08lX\n", ipAddr);

	INT32 sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
		return;
	UINT32 tout = 3000;	// 3 seconds recv timeout
	if (setsockopt(sockfd, SOL_SOCKET, SOCKOPT_RECV_TIMEOUT, &tout, sizeof(tout)) != 0)
		goto error;
	printf("Socket created: %08lX\n", sockfd);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(RFC868_PORT);
	addr.sin_addr.s_addr = htonl(ipAddr);
	memset(addr.sin_zero, 0, 8);
	if (connect(sockfd, (sockaddr *)&addr, sizeof(sockaddr)) != 0)
		goto error;
	puts("Socket connected");

	UINT32 timeData;
	if (cc3000_read(sockfd, &timeData, sizeof(timeData)) != sizeof(timeData))
		goto error;
	timeData = htonl(timeData);
	printf("Data received: %08lX\n", timeData);

	// TI version uses a different epoch: midnight UTC-6 Jan 1, 1900
	//timeData -= 2208988800;	// Convert to from 1970-01-01
	timeData += TIME_ZONE * 60UL * 60UL;
	printf("Time converted: %08lX\n", timeData);
	rtc::setTimeFromSecond(timeData);

	cc3000.state |= cc3000_info_t::TimeSynced;
error:
	closesocket(sockfd);
}

void doActions(void)
{
	doTimeSync();
#if 0
	return;
failed:
	closesocket(cc3000.socket);
	cc3000.socket = -1;
	cc3000.state = (cc3000.state & ~cc3000_info_t::SocketMask) | cc3000_info_t::SocketDisconnected;
#endif
}

void wifiMangTask(void *pvParameters)
{
#if 0
	static char ssid[] = "ZZFNB00000017_Network";
	static char key[] = "f3ei-zeb6-m35o";
#else
	static char ssid[] = "Network!";
	static char key[] = "WirelessNetwork";
#endif

	wlan_start(0);
#if 0	// Set MAC address
	uint8_t	mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
	nvmem_set_mac_address(mac);
	__delay_cycles(MCLK / 1000 * 50);	// 50ms
	wlan_stop();
	__delay_cycles(MCLK / 1000 * 50);	// 50ms
	wlan_start(0);
	__delay_cycles(MCLK / 1000 * 50);	// 50ms
#endif
#if 0	// Set connection policy (smart config?)
	wlan_ioctl_set_connection_policy(0, 0, 0);
	wlan_ioctl_del_profile(255);
	__delay_cycles(MCLK / 1000 * 5);	// 5ms
	wlan_stop();
	__delay_cycles(MCLK / 1000 * 5);	// 5ms
	setXY(0, 0);
	drawString("W_STARTING");
	wlan_start(0);
#endif
	wlan_set_event_mask(HCI_EVENT_MASK);
	//wlan_disconnect();

loop:
	vTaskDelay(configTICK_RATE_HZ);
	uart::puts("wifiMang(");
	if (cc3000.state == cc3000_info_t::Disconnected) {
		cc3000.state = cc3000_info_t::Connecting;
		wlan_disconnect();
		vTaskDelay(1);
		if (wlan_connect(WLAN_SEC_WPA2, ssid, strlen(ssid), 0, (uint8_t *)key, strlen(key)) != 0)
			cc3000.state = cc3000_info_t::Disconnected;
	} else if (cc3000.state == cc3000_info_t::Connecting) {
		uint16_t i;
		for (i = 10; cc3000.state == cc3000_info_t::Connecting && i != 0; i--)
			vTaskDelay(configTICK_RATE_HZ);
		if (i == 0) {	// Connection timed out
			wlan_disconnect();
			cc3000.state = cc3000_info_t::Disconnected;
		}
	} else if ((cc3000.state & cc3000_info_t::DHCPMask) == cc3000_info_t::DHCPSuccess) {
		doActions();
#if 0
		while (cc3000.socket == -1) {
			cc3000.state = (cc3000.state & ~cc3000_info_t::SocketMask) | cc3000_info_t::SocketConnecting;
			cc3000.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		}
		if ((cc3000.state & cc3000_info_t::SocketMask) != cc3000_info_t::SocketConnected) {
			sockaddr_in destAddr;
			destAddr.sin_family = AF_INET;
			destAddr.sin_port = htons(6666);
			const uint8_t ip[4] = {192, 168, 0, 12};
			destAddr.sin_addr.s_addr = *(uint32_t *)ip;
			memset(destAddr.sin_zero, 0, 8);
			if (connect(cc3000.socket, (sockaddr *)&destAddr, sizeof(sockaddr)) == 0)
				cc3000.state = (cc3000.state & ~cc3000_info_t::SocketMask) | cc3000_info_t::SocketConnected;
			else {
				cc3000.socket = -1;
				cc3000.state = (cc3000.state & ~cc3000_info_t::SocketMask) | cc3000_info_t::SocketDisconnected;
			}
		} else if ((cc3000.state & cc3000_info_t::SocketMask) == cc3000_info_t::SocketConnected)
			doActions();
#endif
	}
	uart::puts(")\r\n");
	goto loop;
}

void cc3000INTTask(void *pvParameters)
{
	// CC3000 interrupt polling
polling:
	uint16_t recvMsg;
	while (xQueueReceive(xCC3000INTQueue, &recvMsg, portMAX_DELAY) != pdTRUE);
	uart::puts("cc3000ISR(");
	cc3000ISR();
	uart::puts(")\r\n");
	goto polling;
}

__attribute__((interrupt(PORT4_VECTOR)))
__interrupt void IntSpiGPIOHandler(void)
{
	static const uint16_t cc3000Msg = P4IV_P4IFG7;
	switch (__even_in_range(P4IV, P4IV_P4IFG7)) {
	case P4IV_P4IFG7:
		if (xCC3000INTQueue == NULL)
			cc3000ISR();
		else {
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			xQueueSendFromISR(xCC3000INTQueue, &cc3000Msg, &xHigherPriorityTaskWoken);

			/* If writing to xQueue caused a task to unblock, and the unblocked task
			has a priority equal to or above the task that this interrupt interrupted,
			then xHigherPriorityTaskWoken will have been set to pdTRUE internally within
			xQueuesendFromISR(), and portEND_SWITCHING_ISR() will ensure that this
			interrupt returns directly to the higher priority unblocked task. */
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		break;
	default:
		break;
	}
}

// The function handles asynchronous events that come from CC3000
void CC3000_UsynchCallback(long	lEventType, char *data,	unsigned char length)
{
	cc3000.newEvent = true;
	switch (lEventType) {
	case HCI_EVNT_WLAN_UNSOL_CONNECT:
		cc3000.state = cc3000_info_t::Connected;
		break;
	case HCI_EVNT_WLAN_UNSOL_DISCONNECT:
		cc3000.state = cc3000_info_t::Disconnected;
		break;
	case HCI_EVNT_WLAN_UNSOL_DHCP:
		// Notes:
		// 1) IP config	parameters are received	swapped
		// 2) IP config	parameters are valid only if status is OK,
		// i.e.	ulCC3000DHCP becomes 1

		// Only	if status is OK, the flag is set to 1 and the
		// addresses are valid.
		if (*(data + NETAPP_IPCONFIG_MAC_OFFSET) == 0) {
			cc3000.state = (cc3000.state & ~cc3000_info_t::DHCPMask) | cc3000_info_t::DHCPSuccess;
			volatile uint8_t *store = cc3000.ip;
			uint8_t i = 3;
			do
				*store++ = *(data + i);
			while (i-- != 0);
		} else
			cc3000.state = (cc3000.state & ~cc3000_info_t::DHCPMask) | cc3000_info_t::DHCPFailed;
		break;
	}
}
