#include <stdint.h>
#include <msp430.h>
#include "clocks.h"
#include "macros.h"
#include "ledmatrix.h"
#include "rtc.h"
#include "display.h"

#include <cc3000.h>
#include <host_driver_version.h>
#include <wlan.h>
#include <hci.h>
#include <nvmem.h>
#include <netapp.h>
#include <string.h>

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
			cc3000.state = cc3000_info_t::DHCPSuccess;
			volatile uint8_t *store = cc3000.ip;
			uint8_t i = 3;
			do
				*store++ = *(data + i);
			while (i-- != 0);
		} else
			cc3000.state = cc3000_info_t::DHCPFailed;
		break;
	}
}

void initCC3000()
{
	cc3000.state = cc3000_info_t::Disconnected;
	cc3000.newEvent = false;

	setColour((Red & Foreground) | (Blank & Background));
	clean();
	cc3000_init(CC3000_UsynchCallback);
	setXY(0, 0);
	drawString("W_INITED");
	wlan_start(0);
#if 0
	wlan_ioctl_set_connection_policy(0, 0, 0);
	wlan_ioctl_del_profile(255);
	wlan_stop();
	__delay_cycles(MCLK / 1000 * 5);	// 5ms
	setXY(0, 0);
	drawString("W_STARTING");
	wlan_start(0);
#endif
	wlan_set_event_mask(HCI_EVENT_MASK);
	wlan_disconnect();
	setXY(0, 8);
	drawString("W_CONNECT");
#if 0
	char ssid[] = "ZZFNB00000017_Network";
	char key[] = "f3ei-zeb6-m35o";
#else
	char ssid[] = "Network!";
	char key[] = "WirelessNetwork";
#endif
	cc3000.state = cc3000_info_t::Connecting;
	if (wlan_connect(WLAN_SEC_WPA2, ssid, strlen(ssid), 0, (uint8_t *)key, strlen(key)) != 0) {
		setXY(0, 16);
		drawString("W_CON_FAIL");
		for (;;);
	} else
		cc3000.state = cc3000_info_t::Disconnected;
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
	// [SELA] ACLK source (2, REFOCLK), [SELS] SMCLK source (3, DCOCLK), [SELM] MCLK source (3, DCOCLK)
	UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
	// [DIVPA] (5, 32), [DIVA], [DIVS], [DIVM]
	UCSCTL5 = DIVPA__32 | DIVA__1 | DIVS__1 | DIVM__1;
	UCSCTL6 = XT2DRIVE_1 /*| XT2BYPASS*/ /*| XT2OFF*/ | XT1DRIVE_0 /*| XTS*/ | XT1BYPASS | XT1OFF;
	UCSCTL8 = MODOSCREQEN | SMCLKREQEN | MCLKREQEN | ACLKREQEN;
	// UCSCTL9 is bypass settings, not available on MSP430F6659

	rtc::init();

	// Stabilise crystals (500ms)
	__delay_cycles(MCLK / 1000 * 500);
	// [UCSCTL7] Clear fault flags
	UCSCTL7 &= 0xFFF0;

	ledMatrix::init();
	__enable_interrupt();

	initCC3000();
}

int main(void)
{
	::init();

	static uint16_t cnt = 0;
	setDoubleBufferEnabled(true);
loop:
	clean();
	/*if (!pool1s())
		goto loop;*/
	display::timeFS();
	(*buffer)[BufferGreen][0][0] = cnt++;
	swapBuffer();
	goto loop;
}
