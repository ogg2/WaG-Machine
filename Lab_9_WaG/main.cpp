/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Start running WaG machine, gameplay and testing
* 
* Last Modified: 17 April 2019
* 
******************************************************************************/
#include "mbed.h"
#include "test.h"
#include "display.h"
#include "io_pins.h"
#include "stepper.h"
#include "DebounceIn.h"
#include "laser.h"
#include "phototransistor.h"
#include "gameplay.h"

//Variables for the Interrupt
DigitalIn button(USER_BUTTON);

static int testState = 0;           //keep track of current test
static bool ignore = false;         //ignore section of code for each test

int main(void) {
    
    setupConfiguration ();
    stp_init();
    
    clearSerial();
    pc.printf("Press the Blue Button to test functionality.\n\r");
    pc.printf("Press the CAL button to calibrate.\n\r");
    pc.printf("Press the START button to start the game.\n\r");
        
    while (1) {
        while (!start_button.read() == 1) {
            wait_ms(20);
            if (!start_button.read() == 0) {
                wait_ms(20);
                if (calibration_status == CALIBRATED) {
                    pc.printf("Starting the game...\n\r");
                    play_wag();
                } else {
                    pc.printf("The stepper motor needs to be calibrated.\n\r");
                }
            }
        }
        
        
        while (!start_button.read() == 0) {
            while (!calibrate_button.read() == 1) {
                wait_ms(20);
                if (!calibrate_button.read() == 0) {
                    wait_ms(20);
                    pc.printf("Calibrating the WaG Station...\n\r");
                    stp_calibrate();
                }
            }
            
            while (!button == 0) {
                wait_ms(20);
                if (!button == 1) {
                    wait_ms(20);
                    testState++;
                }
                if (testState == 9) {
                    pc.printf("Press the Blue Button to test functionality.\n\r");
                    testState = 1;    
                }
                ignore = false;
            }  
            if (testState > 0) { 
                testing(testState, ignore);
                ignore = true;
            }
        }
    }
}