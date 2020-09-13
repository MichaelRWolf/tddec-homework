//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved         
//-    For use by owners of Test-Driven Development for Embedded C,   
//-    and attendees of Renaissance Software Consulting, Co. training 
//-    classes.                                                       
//-                                                                   
//-    Available at http://pragprog.com/titles/jgade/                 
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3               
//-                                                                   
//-    Authorized users may use this source code in your own          
//-    projects, however the source code may not be used to           
//-    create training material, courses, books, articles, and        
//-    the like. We make no guarantees that this source code is       
//-    fit for any purpose.                                           
//-                                                                   
//-    www.renaissancesoftware.net james@renaissancesoftware.net      
//- ------------------------------------------------------------------

extern "C"
{
#include "LedDriver.h"
}

#include "CppUTest/TestHarness.h"

static u_int16_t virtualLeds;

TEST_GROUP(LedDriver)
{
    void setup()
    {
	LedDriver_Create(&virtualLeds);
    }

    void teardown()
    {
	LedDriver_Destroy();
    }
};

// TEST(LedDriver, Create)
// {
//   FAIL("Start here");
// }

TEST(LedDriver, LedsOffAfterCreate)
{
    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0x0000, HAL_bits);
}


TEST(LedDriver, TurnOnOneLed) 
{
    LedDriver_TurnOn(1);

    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0x0001, HAL_bits);
}


TEST(LedDriver, TurnOffOneLed)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);

    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0x0000, HAL_bits);
}


TEST(LedDriver, TurnOnMultipleLeds)
{
    LedDriver_TurnOn(8);
    LedDriver_TurnOn(9);

    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0x0180, HAL_bits);
}


TEST(LedDriver, TurnOffAnyLed)
{
    LedDriver_TurnOn(8);
    LedDriver_TurnOn(9);
    LedDriver_TurnOff(8);

    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0x0100, HAL_bits);
}


TEST(LedDriver, AllOn)
{
    LedDriver_TurnAllOn();

    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0xffff, HAL_bits);
}


TEST(LedDriver, AllOff)
{
    LedDriver_TurnOn(8);
    LedDriver_TurnAllOff();

    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0x0, HAL_bits);
}


// ================================================================
// The register that holds bits for LEDs is write-only.  
// It can NOT be read.
// Repeat:  write-only + read-never
// ================================================================

TEST(LedDriver, LedMemoryIsNotReadable)
{

    virtualLeds = 0xfa50; // If we SET or RESET bits with direct memory access ...

    u_int16_t HAL_bits = LedDriver_Image(); // ... when we query them later...
    CHECK_COMPARE(HAL_bits, !=, virtualLeds); // ... the cache is not correct.
}

// --- BUT --- HOWEVER --- ON THE OTHER HAND....


TEST(LedDriver, LedImageIsCachedForQuery)
{
    LedDriver_TurnOn(1);       // If we SET...
    LedDriver_TurnOn(12);
    LedDriver_TurnOn(13);	

    LedDriver_TurnOff(8);	// ... or RESET bits with the HAL...

    u_int16_t HAL_bits = LedDriver_Image(); // ... we can query them later...

    u_int16_t expected_bits =
    	(0x1 << ( 1 - 1)) |
    	(0x1 << (12 - 1)) |
    	(0x1 << (13 - 1)); 
    CHECK_EQUAL(HAL_bits, expected_bits); // ... because the HAL keeps track for us.

}


TEST(LedDriver, QueryLedStateWithUpperAndLowerBounds)
{
    LedDriver_TurnOn(1);	// 0x0001
    LedDriver_TurnOff(2);

    LedDriver_TurnOn(15);	// 0x4000
    LedDriver_TurnOn(16);	// 0x8000


    u_int16_t HAL_bits = LedDriver_Image();
    CHECK_EQUAL(0xC001, HAL_bits);
    CHECK_EQUAL(0xC001, virtualLeds);
}


TEST(LedDriver, OutOfBoundsTurnOffDoesNoHarm)
{
    LedDriver_TurnAllOn();

    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);	// Wrap past 16 bits?
    LedDriver_TurnOff(33);	// Wrap past 32 bits?

    CHECK_EQUAL(0xffff, virtualLeds);
}


TEST(LedDriver, OutOfBoundsTurnOnDoesNoHarm)
{
    LedDriver_TurnAllOff();

    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);	// Wrap past 16 bits?
    LedDriver_TurnOn(33);	// Wrap past 32 bits?

    CHECK_EQUAL(0xffff, virtualLeds);
}
