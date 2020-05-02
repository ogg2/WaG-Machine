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
* Last Modified: 4 April 2019
* 
******************************************************************************/

#ifndef IO_PINS_H
#define IO_PINS_H

//Display subsystem
#define DSP_AS1107_NSS PC_7

//SPI Master
#define SPI_MOSI PA_7
#define SPI_MISO PA_6
#define SPI_SCLK PA_5

//Analog subsystem
#define ANA_MUX_S0 PC_6
#define ANA_MUX_S1 PB_15
#define ANA_MUX_S2 PB_13
#define ANA_MUX_S3 PB_12
#define ANA_MUX_OUT PB_1

//UI subsystem
#define UI_CAL_BUTTON PG_9
#define UI_START_BUTTON PG_14
#define UI_STATION_SELECT PF_15
#define UI_JOG_LEFT_BUTTON PE_13
#define UI_JOG_RIGHT_BUTTON PF_14
#define UI_SPARE PB_3

//Laser subsystem
#define LZR_ENABLE PB_5

//Stepper subsystem
#define STP_HOME_SENSOR PE_11
#define STP_DRV8806_NSS PA_15

//Expansion subsystem
#define EXP_NSS PA_4

//Serial printing 
#define BAUD_RATE 460800
#define ASCII_ESC 27
extern Serial pc;

/**
* NAME: clearSerial
* Description: Clear the serial console and home the cursor
*
**/
void clearSerial();

//Set up buttons
extern DigitalIn left_button;
extern DigitalIn right_button;
extern DigitalIn calibrate_button;
extern DigitalIn start_button;
extern DigitalIn win_button;
extern DigitalIn station_select_bit; //0(B)=left, 1(A)=right

#define STATION_A_RIGHT 1
#define STATION_B_LEFT 0

//Calibration
#define CALIBRATED true
#define NOT_CALIBRATED false

#endif /*IO_PINS_H*/