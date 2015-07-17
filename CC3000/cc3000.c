#include <msp430.h>
#include <stdio.h>
#include "cc3000.h"
#include "CC3000HostDriver/socket.h"
#include "Hardware/spi.h"
#include "macros.h"

long ReadWlanInterruptPin(void)
{
	return PXIN(CC3000_INT_PORT) & CC3000_INT;
}

void WlanInterruptEnable()
{
	PXIE(CC3000_INT_PORT) |= CC3000_INT;
}

void WlanInterruptDisable()
{
	PXIE(CC3000_INT_PORT) &= ~CC3000_INT;
}

void WriteWlanPin(unsigned char	val)
{
	if (val)
		PXOUT(CC3000_PORT) |= CC3000_EN;
	else
		PXOUT(CC3000_PORT) &= ~CC3000_EN;
}

char *sendPatch(unsigned long *Length)
{
	*Length	= 0;
	return NULL;
}

void cc3000_init(tWlanCB sWlanCB)
{
	WlanInterruptDisable();
	UCB2CTL1 = UCSWRST;	// Reset UCB2
	UCB2CTL0 = UCMSB | UCMST | UCSYNC | UCMODE_0;
	UCB2CTL1 = UCSWRST | UCSSEL__SMCLK;
	UCB2BRW = 2;
	UCB2IE = 0;

	PXSEL(CC3000_PORT) &= ~(CC3000_CS | CC3000_EN);
	PXSEL(CC3000_PORT) |= CC3000_MOSI | CC3000_MISO | CC3000_SCK;
	PXDIR(CC3000_PORT) |= CC3000_MOSI | CC3000_SCK | CC3000_CS | CC3000_EN;
	PXDS(CC3000_PORT) |= CC3000_MOSI | CC3000_SCK | CC3000_CS | CC3000_EN;
	PXDIR(CC3000_PORT) &= ~CC3000_MISO;
	PXREN(CC3000_PORT) |= CC3000_MISO;
	PXOUT(CC3000_PORT) |= CC3000_CS;
	PXOUT(CC3000_PORT) &= ~CC3000_EN;

	PXSEL(CC3000_INT_PORT) &= ~CC3000_INT;
	PXDIR(CC3000_INT_PORT) &= ~CC3000_INT;
	PXREN(CC3000_INT_PORT) |= CC3000_INT;
	PXOUT(CC3000_INT_PORT) |= CC3000_INT;
	PXIES(CC3000_INT_PORT) |= CC3000_INT;
	PXIFG(CC3000_INT_PORT) &= ~CC3000_INT;

	UCB2CTL1 &= ~UCSWRST;

	wlan_init(sWlanCB, sendPatch, sendPatch, sendPatch, ReadWlanInterruptPin, WlanInterruptEnable, WlanInterruptDisable, WriteWlanPin);
}

INT16 cc3000_read(const INT32 sockfd, void *buf, INT32 len)
{
	INT32 size = len;
	unsigned char *ptr = (unsigned char *)buf;
	while (size != 0) {
		INT16 ret = recv(sockfd, ptr, size, 0);
		if (ret <= 0)
			return ret;
		ptr += ret;
		size -= ret;
	}
	return len;
}

INT16 cc3000_write(const INT32 sockfd, const void *buf, INT32 len)
{
	INT32 size = len;
	const unsigned char *ptr = (const unsigned char *)buf;
	while (size != 0) {
		INT16 ret = send(sockfd, ptr, size, 0);
		if (ret == 0)
			continue;
		else if (ret < 0)
			return ret;
		ptr += ret;
		size -= ret;
	}
	return len;
}
