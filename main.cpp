#include <stdint.h>
#include <msp430.h>
#include "ledmatrix.h"
#include "clocks.h"
#include "macros.h"

void init(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Initialise Power Management Module
	PMMCTL0 = PMMPW | PMMCOREV_3;

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
	UCSCTL5 = DIVPA__32 | DIVA__1 | DIVS__4 | DIVM__1;
	UCSCTL6 = XT2DRIVE_1 /*| XT2BYPASS*/ /*| XT2OFF*/ | XT1DRIVE_0 /*| XTS*/ | XT1BYPASS | XT1OFF;
	UCSCTL8 = MODOSCREQEN | SMCLKREQEN | MCLKREQEN | ACLKREQEN;
	// UCSCTL9 is bypass settings, not available on MSP430F6659
	// Stabilise XT2
	__delay_cycles(MCLK / 1000000 * 500);
	// [UCSCTL7] Clear fault flags
	UCSCTL7 &= 0xFFF0;

	// LED output port
	P5SEL &= ~(1 << 7);
	P5DIR |= 1 << 7;
	P5DS |= 1 << 7;
	P5OUT |= 1 << 7;

	ledMatrix::init();
	__enable_interrupt();
}

int main(void)
{
	init();

	using namespace ledMatrix;
	fill(Red | Green);
	setColour(((Red | Green) & Foreground) | (Blank & Background));

	bool inv = false;
loop:
	if (!poolOneSecond())
		goto loop;
	testPattern(inv);
	inv = !inv;
	setXY(2, 4);
	drawString("HelloWorld");
	setXY(5, 12);
	drawString("-> \200\201\202 <-");
	setXY(2, 20);
	drawString("??????????");
	swapBuffer();
	P5OUT ^= 1 << 7;
	goto loop;
}