/*
             LUFA Library
     Copyright (C) Dean Camera, 2019.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2019  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the USBtoSerial project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "USBtoSerial.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber         = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint                 =
					{
						.Address                = CDC_TX_EPADDR,
						.Size                   = CDC_TXRX_EPSIZE,
						.Banks                  = 1,
					},
				.DataOUTEndpoint                =
					{
						.Address                = CDC_RX_EPADDR,
						.Size                   = CDC_TXRX_EPSIZE,
						.Banks                  = 1,
					},
				.NotificationEndpoint           =
					{
						.Address                = CDC_NOTIFICATION_EPADDR,
						.Size                   = CDC_NOTIFICATION_EPSIZE,
						.Banks                  = 1,
					},
			},
	};

/* See /usr/lib/avr/include/avr/iom32u4.h , use with _SFR_IO8() */
#define SFR_PORTB	0x05
#define SFR_PORTC	0x08
#define SFR_PORTD	0x0b
#define SFR_PORTE	0x0e
#define SFR_PORTF	0x11

#define ARDUINO_MICRO_PINMUX(__pm, __port, __off)	\
	case __pm:					\
		port = __port;				\
		pin = __off;				\
		break;					\

#define BIT(n)	(1 << (n))

const char * const PROGMEM help_text =
"Pin mapping:\r\n"
"              pin identifiers\r\n"
"                 |       |\r\n"
"                 v       v\r\n"
"            .-------- --------.\r\n"
"MOSI ---- 1 | B2 0   U   f B1 | 1 ---- SCK\r\n"
"RXLED --- 2 | B0 1       g B3 | 2 --- MISO\r\n"
"D1 ------ 3 | D3 2            | 3\r\n"
"D0 ------ 4 | D2 3            | 4\r\n"
"          5 |                 | 5\r\n"
"          6 |                 | 6\r\n"
"D2 ------ 7 | D1 4            | 7\r\n"
"D3 ------ 8 | D0 5            | 8\r\n"
"D4 ------ 9 | D4 6       h F0 | 9 ----- A5\r\n"
"D5 ----- 10 | C6 7       i F1 | 10 ---- A4\r\n"
"D6 ----- 11 | D7 8       j F4 | 11 ---- A3\r\n"
"D7 ----- 12 | E6 9       k F5 | 12 ---- A2\r\n"
"IO8 ---- 13 | B4 a       l F6 | 13 ---- A1\r\n"
"IO9 ---- 14 | B5 b       m F7 | 14 ---- A0\r\n"
"IO10 --- 15 | B6 c            | 15\r\n"
"IO11 --- 16 | B7 d  ___       | 16\r\n"
"IO12 --- 17 | D6 e |USB| n C7 | 17 -- IO13\r\n"
"            '------|___|------'\r\n"
"PD5 -- TXLED (Y)         o\r\n"
"PB0 -- RXLED (Y)         1\r\n"
"PC7 -- LED (G)           n\r\n"
"\r\n"
"List of operations:\r\n"
"   Lower-case 0..9 a..o --- select pin (see pin identifiers above)\r\n"
"   H --- Set pre-selected pin HIGH\r\n"
"   L --- Set pre-selected pin LOW\r\n"
"   t --- Set pre-selected pin LOW, wait 100 mS, HIGH\r\n"
"   T --- Set pre-selected pin HIGH, wait 100 mS, LOW\r\n"
"   u --- Set pre-selected pin LOW, wait 1000 mS, HIGH\r\n"
"   U --- Set pre-selected pin HIGH, wait 1000 mS, LOW\r\n"
"   ? --- Print this help\r\n"
"\r\n";

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	uint8_t port = 0, pin = 0;
	int chr;

	SetupHardware();

	GlobalInterruptEnable();

	for (;;) {
		/* Only try to read in bytes from the CDC interface if the transmit buffer is not full */
		chr = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		/* Store received byte into the USART transmit buffer */
		if (chr >= 0) {
			switch (chr) {
			ARDUINO_MICRO_PINMUX('0', SFR_PORTB, 2)
			ARDUINO_MICRO_PINMUX('1', SFR_PORTB, 0)
			ARDUINO_MICRO_PINMUX('2', SFR_PORTD, 3)
			ARDUINO_MICRO_PINMUX('3', SFR_PORTD, 2)
			ARDUINO_MICRO_PINMUX('4', SFR_PORTD, 1)
			ARDUINO_MICRO_PINMUX('5', SFR_PORTD, 0)
			ARDUINO_MICRO_PINMUX('6', SFR_PORTD, 4)
			ARDUINO_MICRO_PINMUX('7', SFR_PORTC, 6)
			ARDUINO_MICRO_PINMUX('8', SFR_PORTD, 7)
			ARDUINO_MICRO_PINMUX('9', SFR_PORTE, 6)
			ARDUINO_MICRO_PINMUX('a', SFR_PORTB, 4)
			ARDUINO_MICRO_PINMUX('b', SFR_PORTB, 5)
			ARDUINO_MICRO_PINMUX('c', SFR_PORTB, 6)
			ARDUINO_MICRO_PINMUX('d', SFR_PORTB, 7)
			ARDUINO_MICRO_PINMUX('e', SFR_PORTD, 6)

			ARDUINO_MICRO_PINMUX('f', SFR_PORTB, 1)
			ARDUINO_MICRO_PINMUX('g', SFR_PORTB, 3)
			ARDUINO_MICRO_PINMUX('h', SFR_PORTF, 0)
			ARDUINO_MICRO_PINMUX('i', SFR_PORTF, 1)
			ARDUINO_MICRO_PINMUX('j', SFR_PORTF, 4)
			ARDUINO_MICRO_PINMUX('k', SFR_PORTF, 5)
			ARDUINO_MICRO_PINMUX('l', SFR_PORTF, 6)
			ARDUINO_MICRO_PINMUX('m', SFR_PORTF, 7)
			ARDUINO_MICRO_PINMUX('n', SFR_PORTC, 7)

			ARDUINO_MICRO_PINMUX('o', SFR_PORTD, 5)	/* TXLED */

			case 'H':
				_SFR_IO8(port) |= BIT(pin);
				break;
			case 'L':
				_SFR_IO8(port) &= ~BIT(pin);
				break;
			case 't':
				_SFR_IO8(port) &= ~BIT(pin);
				Delay_MS(100);
				_SFR_IO8(port) |= BIT(pin);
				break;
			case 'T':
				_SFR_IO8(port) |= BIT(pin);
				Delay_MS(100);
				_SFR_IO8(port) &= ~BIT(pin);
				break;
			case 'u':
				_SFR_IO8(port) &= ~BIT(pin);
				Delay_MS(1000);
				_SFR_IO8(port) |= BIT(pin);
				break;
			case 'U':
				_SFR_IO8(port) |= BIT(pin);
				Delay_MS(1000);
				_SFR_IO8(port) &= ~BIT(pin);
				break;
			case '?':
				if (Endpoint_IsINReady())
					CDC_Device_SendString(&VirtualSerial_CDC_Interface, help_text);
				continue;
			default:
				/* Unknown command / pin */
				chr = '?';
				break;
			}

			if (Endpoint_IsINReady())
				CDC_Device_SendByte(&VirtualSerial_CDC_Interface, chr);
		}

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#endif

	/* Init GPIOs as LOW */
	PORTB = 0;
	PORTC &= ~(BIT(6) | BIT(7));
	PORTD &= 0 ;
	PORTE &= ~BIT(6);
	PORTF &= BIT(2) | BIT(3);

	/* Hardware Initialization */
	USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
ISR(USART1_RX_vect, ISR_BLOCK)
{
}

/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
}
