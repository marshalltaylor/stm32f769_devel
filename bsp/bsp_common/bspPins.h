/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef BSP_PINS_H
#define BSP_PINS_H

#include <stdint.h>
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
//class bobert{
//	bobert() { return 5;};
//	uint8_t somedumbbyte = 4;
//	const uint32_t * asdfasdfasdf[] = {
//		(uint32_t *)0
//	};
//};

extern const uint32_t DxToPort[];
extern const uint32_t DxToPin[];

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
//extern const PinName digitalPin[];

#define PORTA_OFFSET 0x0000U
#define PORTB_OFFSET 0x0400U
#define PORTC_OFFSET 0x0800U
#define PORTD_OFFSET 0x0C00U
#define PORTE_OFFSET 0x1000U
#define PORTF_OFFSET 0x1400U
#define PORTG_OFFSET 0x1800U
#define PORTH_OFFSET 0x1C00U
#define PORTI_OFFSET 0x2000U
#define PORTJ_OFFSET 0x2400U
#define PORTK_OFFSET 0x2800U

#ifdef __cplusplus
}
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
