#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <msp430.h>
#include "clocks.h"
#include "macros.h"
#include "ledmatrix.h"
#include "rtc.h"
#include "display.h"
#include "uart.h"

#include <cc3000.h>
#include <host_driver_version.h>
#include <wlan.h>
#include <hci.h>
#include <nvmem.h>
#include <netapp.h>
#include <socket.h>
#include <spi.h>

#include "FreeRTOSConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#define	NETAPP_IPCONFIG_MAC_OFFSET	(20)
#define	HCI_EVENT_MASK	(HCI_EVNT_WLAN_KEEPALIVE | HCI_EVNT_WLAN_UNSOL_INIT /*|	HCI_EVNT_WLAN_ASYNC_PING_REPORT*/)

using namespace ledMatrix;

struct cc3000_info_t cc3000;

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

void wifiMangTask(void *pvParameters)
{
#if 0
	static char ssid[] = "ZZFNB00000017_Network";
	static char key[] = "f3ei-zeb6-m35o";
#else
	static char ssid[] = "Network!";
	static char key[] = "WirelessNetwork";
#endif
loop:
	uart::puts("wifiMang(");
	if (cc3000.state == cc3000_info_t::Disconnected) {
		cc3000.state = cc3000_info_t::Connecting;
		wlan_disconnect();
		vTaskDelay(1);
		if (wlan_connect(WLAN_SEC_WPA2, ssid, strlen(ssid), 0, (uint8_t *)key, strlen(key)) != 0)
			cc3000.state = cc3000_info_t::Disconnected;
	} else if ((cc3000.state & cc3000_info_t::DHCPMask) == cc3000_info_t::DHCPSuccess) {
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
		} else if ((cc3000.state & cc3000_info_t::SocketMask) == cc3000_info_t::SocketConnected) {
			char str[] = "Hello, world!";
			if (send(cc3000.socket, str, strlen(str) + 1, 0) != 0) {
				closesocket(cc3000.socket);
				cc3000.socket = -1;
				cc3000.state = (cc3000.state & ~cc3000_info_t::SocketMask) | cc3000_info_t::SocketDisconnected;
			}
		}
	}
	uart::puts(")\r\n");
	vTaskDelay(configTICK_RATE_HZ);
	goto loop;
}

static QueueHandle_t xCC3000INTQueue = NULL;

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

void dispUpdTask(void *pvParameters)
{
	static uint16_t cnt = 0;
	setDoubleBufferEnabled(true);
loop:
	uart::puts("dispUpd(");
	clean();
	display::timeFS();
	(*buffer)[BufferGreen][0][0] = cnt++;
	swapBuffer();
	uart::puts(")\r\n");
	vTaskDelay(configTICK_RATE_HZ / 8);
	goto loop;
}

void initCC3000()
{
	cc3000.state = cc3000_info_t::Disconnected;
	cc3000.newEvent = false;
	cc3000.socket = -1;

	cc3000_init(CC3000_UsynchCallback);
	setColour((Red & Foreground) | (Blank & Background));
	clean();
	setXY(0, 0);
	drawString("W_INITED");
	wlan_start(0);

#if 0
	uint8_t	mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
	nvmem_set_mac_address(mac);
	__delay_cycles(MCLK / 1000 * 50);	// 50ms
	wlan_stop();
	__delay_cycles(MCLK / 1000 * 50);	// 50ms
	wlan_start(0);
	__delay_cycles(MCLK / 1000 * 50);	// 50ms
#endif

#if 0
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
	//setXY(0, 8);
	//drawString("W_STARTED");
}

void init(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Initialise Power Management Module
	PMMCTL0 = PMMPW | PMMCOREV_3;

	// Initialise backup system
	BAKCTL = BAKDIS;
	BAKCHCTL = 0;

	// Initialise clock sources
	PXSEL(P7) |= BIT3 | BIT2;
	// [DCO] & [MOD] modified automatically by FLL (in UCSCTL0)
	// [DCORSEL] DCO frequency range (6, 10.7-39MHz), [DISMOD] enable modulation
	UCSCTL1  = (6 << 4) | DISMOD;
	// [FLLD] FLL loop divider (0, f(DCOCLK)/1), [FLLN] DCO multiplier (19, N + 1)
	UCSCTL2 = FLLD__1 | 19;
	// [SELREF] FLL reference (5, XT2CLK), [FLLREFDIV] reference divider (4, f(FLLREFCLK)/12)
	UCSCTL3 = SELREF__XT2CLK | FLLREFDIV__12;
	// [SELA] ACLK source (2, XT1CLK), [SELS] SMCLK source (3, DCOCLK), [SELM] MCLK source (3, DCOCLK)
	UCSCTL4 = SELA__XT1CLK | SELS__DCOCLK | SELM__DCOCLK;
	// [DIVPA] (5, 32), [DIVA], [DIVS], [DIVM]
	UCSCTL5 = DIVPA__32 | DIVA__1 | DIVS__1 | DIVM__1;
	UCSCTL6 = XT2DRIVE_1 /*| XT2BYPASS*/ /*| XT2OFF*/ | XT1DRIVE_0 /*| XTS*/ | XT1BYPASS | XT1OFF;
	UCSCTL8 = MODOSCREQEN | SMCLKREQEN | MCLKREQEN | ACLKREQEN;
	// UCSCTL9 is bypass settings, not available on MSP430F6659

	rtc::init();
	uart::init();

	// Stabilise crystals (500ms)
	__delay_cycles(MCLK / 1000 * 500);
	// [UCSCTL7] Clear fault flags
	UCSCTL7 &= 0xFFF0;

	puts(__DATE__ " " __TIME__ " | Hello, world!");
	ledMatrix::init();
	__enable_interrupt();

	initCC3000();
	xCC3000INTQueue = xQueueCreate(3, 2);
	xTaskCreate(wifiMangTask, "WiFiMang", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(cc3000INTTask, "CC3000INT", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(dispUpdTask, "DispUpd", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 3, NULL);
}

int main(void)
{
	::init();
	vTaskStartScheduler();
	for (;;);
}
