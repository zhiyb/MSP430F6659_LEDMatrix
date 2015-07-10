# MSP430F6659_LEDMatrix
Driving 64x32 double-colour LED matrix board with MSP430F6659

Crystals: 32.768kHz XTAL1 and 12MHz XTAL2.  
LED matrix board Connections:

	LED_DATA_PORT	P1
	// Data lines (R1, R2, G1, G2 (reversed?)) from
	LED_DATA_DATA	0
	LED_DATA_R1	BIT2
	LED_DATA_R2	BIT3
	LED_DATA_G1	BIT0
	LED_DATA_G2	BIT1
	// Row selection from
	LED_DATA_ROW	4

	LED_CTRL_PORT	P5
	LED_CTRL_STB	BIT5
	LED_CTRL_CLK	BIT4
	LED_CTRL_EN	BIT3

Additional modules:  
CC3000 - WiFi connectivity, using USCI_B2(SPI) at PORT9 + Interrupt P4.7  
FreeRTOS - task management  
