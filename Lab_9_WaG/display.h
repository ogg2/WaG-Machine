/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Controls the 7-segment display and the discrete LEDs
* 
* Last Modified: 16 April 2019
* 
******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

//Global Variables
extern int bcd_array[4];

//Public Function Prototypes: These functions may be called by any code
//that includes this header file

/**
* NAME: setupConfiguration
* Description: Sets up the AS1107 and prepares it before you can write to it
**/
void setupConfiguration ();

/**
* NAME: dsp_hit_miss
* Description: Takes in miss and hit values, stored in array, isolates each digit,
* and passes to dsp_bcd()
* input param: int *, array, stores miss and hit values
**/
void dsp_hit_miss(int *);

/**
* NAME: dsp_bcd
* Description: Transfers contents of bcd_array to display controller
* 
* Inputs: 
* Parameters: int *, array, bcd digits to be converted and written to display
**/
void dsp_bcd (int *);

/**
* NAME: int_to_bcd_array
* Description: Takes an input number and writes each digit it to bcd_array
* Inputs: 
* Parameters: int, num, input number to be converted
* int *, array, bcd digits to later be converted and written to display
**/
void int_to_bcd_array(int, int *);

/**
* NAME: displayTest
* Description: Illuminate all segments, decimal points, and all 8 LEDs
*   on the target board
**/
void displayTest();

/**
* NAME: targetLEDTest
* Description: Cycle through target board LEDs, turning one on at a time, loop
* 
* Inputs: int, which led to turn on 
**/
void targetLEDTest (unsigned int);

/**
* NAME: discreteLedOn
* Description: illuminate corresponding discrete LED for illuminated 
*    photransistor
* 
* Inputs: int, which discrete led to turn on 
**/
void discreteLedOn (int LED);

/**
* NAME: discreteLedsOff
* Description: turn off all the discrete LEDs
**/
void discreteLedsOff ();

#endif /*DISPLAY_H*/