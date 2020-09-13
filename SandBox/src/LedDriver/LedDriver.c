/***
 * Excerpted from "Test-Driven Development for Embedded C",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/jgade for more book information.
 ***/
/*- ------------------------------------------------------------------ -*/
/*-    Copyright (c) James W. Grenning -- All Rights Reserved          -*/
/*-    For use by owners of Test-Driven Development for Embedded C,    -*/
/*-    and attendees of Renaissance Software Consulting, Co. training  -*/
/*-    classes.                                                        -*/
/*-                                                                    -*/
/*-    Available at http://pragprog.com/titles/jgade/                  -*/
/*-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3                -*/
/*-                                                                    -*/
/*-    Authorized users may use this source code in your own           -*/
/*-    projects, however the source code may not be used to            -*/
/*-    create training material, courses, books, articles, and         -*/
/*-    the like. We make no guarantees that this source code is        -*/
/*-    fit for any purpose.                                            -*/
/*-                                                                    -*/
/*-    www.renaissancesoftware.net james@renaissancesoftware.net       -*/
/*- ------------------------------------------------------------------ -*/

#include "LedDriver.h"
#include <stdlib.h>
#include <memory.h>

static u_int16_t * ledsAddress;	/* Write-only memory location of memory-mapped LEDs */
static u_int16_t ledsImage;	/* Spy that caches the image to allow read on write-only location. */

enum {
  ALL_LEDS_ON = ~0x0,
  ALL_LEDS_OFF = ~ALL_LEDS_ON,
};

static u_int16_t convertLedNumberToBitMask(int ledNumber) {
  return 0x1 << (ledNumber - 1);
}

void LedDriver_Create(u_int16_t * address)
{
  ledsAddress = address;
  
  ledsImage = ALL_LEDS_OFF;
  *address = ledsImage;
}

void LedDriver_Destroy(void)
{
}

void LedDriver_TurnOn(int ledNumber) {
  ledsImage |= convertLedNumberToBitMask(ledNumber);
  *ledsAddress |= ledsImage;
}

void LedDriver_TurnOff(int ledNumber) {
  ledsImage &= ~convertLedNumberToBitMask(ledNumber);
  *ledsAddress &= ledsImage;
}

void LedDriver_TurnAllOn(void) 
{
  ledsImage = ALL_LEDS_ON;
  *ledsAddress = ledsImage;
}


u_int16_t LedDriver_Image(void) {
  return *ledsAddress;
}
