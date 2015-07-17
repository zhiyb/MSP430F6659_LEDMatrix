#include <stdint.h>
#include <msp430.h>
#include "clocks.h"
#include "macros.h"
#include "ledmatrix.h"
#include "display.h"
#include "uart.h"
#include "rtc.h"

#include "cc3000tasks.h"
#include <cc3000.h>

#include "FreeRTOSConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

using namespace ledMatrix;

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

	uart::puts(__DATE__ " " __TIME__ " | Hello, world!\r\n");
	ledMatrix::init();
	__enable_interrupt();

	initCC3000();
	xCC3000INTQueue = xQueueCreate(3, 2);
	xTaskCreate(wifiMangTask, "WiFiMang", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(cc3000INTTask, "CC3000INT", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(dispUpdTask, "DispUpd", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
}

int main(void)
{
	::init();
	vTaskStartScheduler();
	for (;;);
}
