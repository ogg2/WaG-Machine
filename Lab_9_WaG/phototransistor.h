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

#ifndef PHOTOTRANSISTOR_H
#define PHOTOTRANSISTOR_H

#define CHANNEL_NUM 16


/**
* NAME: muxScanTest
* Description: Print each scanned value to the serial console
* return: pointer to array, array contains values of scanned phototransistors
**/
float * muxScanTest ();

/**
* NAME: muxScanHalf
* Description: Print 8 scanned values of transistors to the serial console
* Transistors scanned is dependent on what station is indicated 
* input param: int, what station we want scanned
* return: pointer to array, array contains values of scanned phototransistors
**/
float * muxScanHalf(int station);

/**
* NAME: scanOneTransistor
* Description: Scan the value of a singular transistor
* input param: the transistor being scanned
* input param: bool, indicates if we want to scan same transistor or 
*   opposite/corresponding transistor
* return: float, the value of the scanned transistor
**/
float scanOneTransistor(int transistor, bool sameCarriage);

/**
* NAME: scanTransistors
* Description: Scan all 16 mux channels and write each value to a different index in an array

* input param: muxArray, float[], stores value of each mux channel in each index, 0-15
* input param: arrayLength, int, the length of the muxArray, 16 in our case
* input param: station, int, which station we want to read phototransistors from
*   int, 0 to indicate transistors 8-15, 1 to indicate 0-7, 
*   2 to indicate all transistors 
**/
void scanTransistors (float * muxArray, int arrayLength, int station);

#endif /*PHOTOTRANSISTOR_H*/