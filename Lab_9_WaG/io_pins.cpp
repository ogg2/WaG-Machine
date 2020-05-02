/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Complete list of pins mapped to symbolic constants needed for WaG
* machine
* 
* Last Modified: 5 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "io_pins.h"

//Set up buttons as jog left or right
DigitalIn left_button (UI_JOG_LEFT_BUTTON, PullUp);
DigitalIn right_button (UI_JOG_RIGHT_BUTTON, PullUp);
DigitalIn calibrate_button (UI_CAL_BUTTON, PullUp);
DigitalIn start_button (UI_START_BUTTON, PullUp);
DigitalIn win_button (UI_SPARE, PullUp);
DigitalIn station_select_bit (UI_STATION_SELECT); //0(B)=left, 1(A)=right

//Serial
Serial pc(USBTX, USBRX, BAUD_RATE);

void clearSerial() {
    pc.printf("%c[2J", ASCII_ESC); //clear the screen
    pc.printf("%c[2H", ASCII_ESC); //Home the cursor
}