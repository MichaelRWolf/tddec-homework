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
    BYTES_EQUAL(0x0000, virtualLeds);
}


TEST(LedDriver, TurnOnOneLed) 
{
    LedDriver_TurnOn(1);

    CHECK_EQUAL(1, virtualLeds)
	}


TEST(LedDriver, TurnOffOneLed)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);

    CHECK_EQUAL(0, virtualLeds)
	}


TEST(LedDriver, TurnOnMultipleLeds)
{
    LedDriver_TurnOn(8);
    LedDriver_TurnOn(9);

    BITS_EQUAL(0x0180, virtualLeds, 0xffff);
}


TEST(LedDriver, TurnOffAnyLed)
{
    LedDriver_TurnOn(8);
    LedDriver_TurnOn(9);
    LedDriver_TurnOff(8);

    BITS_EQUAL(0x0100, virtualLeds, 0xffff);
}


TEST(LedDriver, AllOn)
{
    LedDriver_TurnAllOn();

    BITS_EQUAL(0xffff, virtualLeds, 0xffff);
}


TEST(LedDriver, NextStep)
{
    FAIL("Pick up at page 55");
}

    
