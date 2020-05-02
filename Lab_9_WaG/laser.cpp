/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Control the laser module
* 
* Last Modified: 17 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "laser.h"
#include "io_pins.h"

#define LASER_DELAY 1

DigitalOut laser (LZR_ENABLE);

void laserTest() {
    laser.write(1);
    wait(0.5);
    laser.write(0);
    wait(0.5);
}

void laserToggle(int toggle) {
    laser.write(toggle);
    if (toggle == 1) {
        //delay to turn on laser so that it turns on pefore transistor is scanned
        wait_ms(20);
    }
}