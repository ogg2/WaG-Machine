/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Read the values of the phototransistors
* 
* Last Modified: 21 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "phototransistor.h"
#include "io_pins.h"

BusOut muxSelect (ANA_MUX_S0, ANA_MUX_S1, ANA_MUX_S2, ANA_MUX_S3); //to write the mux signal 0-15
AnalogIn signal (ANA_MUX_OUT);

float * muxScanTest() {
    static float scannedTransistorsArray[CHANNEL_NUM];
    int arrayLength = sizeof scannedTransistorsArray / sizeof scannedTransistorsArray[0];
    int station = 2; //indicator for testing all phototransistors
    scanTransistors (scannedTransistorsArray, arrayLength, station);
    
    return scannedTransistorsArray;
}

float * muxScanHalf(int station) {
    static float scannedTransistorsArray[CHANNEL_NUM / 2];
    int arrayLength = sizeof scannedTransistorsArray / sizeof scannedTransistorsArray[0];
    scanTransistors (scannedTransistorsArray, arrayLength, station);
    
    return scannedTransistorsArray;
}

float scanOneTransistor(int transistor, bool sameCarriage) {
    int offset = 0;
    if (station_select_bit.read() == STATION_B_LEFT) { //station B, upper carriage, transistors 8-15
        offset = 8;                                    //only read transistors 8-15 if station B
    }
    //if false, read corresponding transistor on other station
    if (sameCarriage == false) {
        if (station_select_bit.read() == STATION_B_LEFT) {
            offset = 0;
        } else {
            offset = 8;
        }
    }
    muxSelect = transistor + offset;
    wait_us(100);
    return signal;
}

void scanTransistors (float * muxArray, int arrayLength, int station) {
    int offset = 0;
    if (station == STATION_B_LEFT) { //station B, upper carriage, transistors 8-15
        offset = 8;                  //only read transistors 8-15 if station B
    }
    for (int i = 0; i < arrayLength; i++) {
        muxSelect = i + offset;
        wait_us(100);
        muxArray[i] = signal;        //get the mux value for all 16 channels
    }
}