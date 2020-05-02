/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Control the laser module
* 
* Last Modified: 28 March 2019
* 
******************************************************************************/

#ifndef LASER_H
#define LASER_H


/**
* NAME: laserTest
* Description: Toggle the laser module on/off every half second
**/
void laserTest();

/**
* NAME: laserToggle
* Description: Toggle the laser on and off given an input
* @param: int, turn the laser on or off, 0=off, 1=on
**/
void laserToggle(int);

#endif /*LASER_H*/