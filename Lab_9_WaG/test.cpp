/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Testing Script for the WaG Machine
* 
* Last Modified: 5 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "test.h"
#include "io_pins.h"
#include "display.h"
#include "stepper.h"
#include "laser.h"
#include "phototransistor.h"

/**
* NAME: testOne
* Description: Display test
* input param: bool, ignore section of code for each test 
**/
void testOne (bool ignore);


/**
* NAME: testTwo
* Description: Target Board LED Test
* input param: bool, ignore section of code for each test 
**/
void testTwo (bool ignore);

/**
* NAME: testThree
* Description: Laser Module Test
* input param: bool, ignore section of code for each test 
**/
void testThree (bool ignore);

/**
* NAME: testFour
* Description: Phototransistor Test
* input param: bool, ignore section of code for each test 
**/
void testFour (bool ignore);

/**
* NAME: testFive
* Description: Pushbutton Test
* input param: bool, ignore section of code for each test 
**/
void testFive (bool ignore);

/**
* NAME: testSix
* Description: Stepper Motor Test
* input param: bool, ignore section of code for each test 
**/
int testSix (bool ignore);

/**
* NAME: testSeven
* Description: Calibration Test
* input param: bool, ignore section of code for each test 
**/
void testSeven (bool ignore);

/**
* NAME: testEight
* Description: Repeatability Test
* input param: bool, ignore section of code for each test 
**/
int testEight (bool ignore);

/**
* NAME: repeatability
* Description: Repeatability Test, helper function
* output param: returns -1 if a fault occurred
**/
int repeatability ();

/**
* NAME: printScannedTransistors
* Description: print phototransistor values to console
* input param: float *, the array of transistor values to be read
* input param: int, the length of the array
**/
void printScannedTransistors (float * muxArray, int arrayLength);

/**
* NAME: printDistances
* Description: print calibrated distances from home sensor
* input param: int *, the distance for each phototransistor
* input param: int, the length of the array
**/
void printDistances (int * calibration_array, int arrayLength);

/**
* NAME: printDifferenceDistances
* Description: print distances between each sensor
* input param: int *, the distance for each phototransistor
* input param: int, the length of the array
**/
void printDifferenceDistances (int * calibration_array, int arrayLength);

void testing(int testState, bool ignore) {
    //Display Test
    if (testState == 1) {
        testOne (ignore);
    }
    
    //Target Board LED Test
    if (testState == 2) {
        testTwo (ignore);
    }
    
    //Laser Module Test
    if (testState == 3) {
        testThree (ignore);
    }
    
    //Phototransistor Test
    if (testState == 4) {
        testFour (ignore);
    }
    
    //Pushbutton Test
    if (testState == 5) {
        testFive (ignore);
    }
    
    //Stepper Motor Test
    if (testState == 6) {
        testSix (ignore);
    }
    
    //Calibration Test
    if (testState == 7) {
        testSeven (ignore);
    }
    
    //Repeatability Test
    if (testState == 8) {
        if (calibration_status == CALIBRATED) {
            testEight (ignore);
        }
    }
}

//Display Test
void testOne (bool ignore) {
    if (ignore == false) {
        pc.printf("Display test has begun.\n\r");
        displayTest ();
    }
}

//Target Board LED Test
void testTwo (bool ignore) {
    if (ignore == false) {
        setupConfiguration ();
        pc.printf("Target board LED test has begun.\n\r");
    }
    for (int i = 1; i <= 128; i = i * 2) {
        int discreteLEDs = 0x0500;
        targetLEDTest(discreteLEDs + i);
    }
}

//Laser Module Test
void testThree (bool ignore) {
    if (ignore == false){
        setupConfiguration ();
        pc.printf("Laser module test has begun.\n\r");
    }
    laserTest();
}

//Phototransistor Test
void testFour (bool ignore) {
    if (ignore == false){
        setupConfiguration ();
        clearSerial();
        pc.printf("Phototransistor test has begun.\n\r");
    }
    float * scannedTransistors = muxScanTest();
    int arrayLength = CHANNEL_NUM;
    printScannedTransistors (scannedTransistors, arrayLength); 
}

//Pushbutton Test
void testFive (bool ignore) {
    if (ignore == false){
        pc.printf("Pushbutton test has begun.\n\r");
    }
    pc.printf("Jog Left: %d\n\r", !left_button.read());
    pc.printf("Jog Right: %d\n\r", !right_button.read());
    pc.printf("Cal: %d\n\r", !calibrate_button.read());
    pc.printf("Win: %d\n\r", !win_button.read());
    pc.printf("Start: %d\n\r", !start_button.read());
    pc.printf("Home Sensor: %d\n\r", read_home_sensor());
    if (station_select_bit.read() == STATION_B_LEFT) {
        pc.printf("Station: B(left)\n\r");
    }
    else {
        pc.printf("Station: A(right)\n\r");
    }
    clearSerial();
}

//Stepper Motor Test
int testSix (bool ignore) {
    if (ignore == false){
        pc.printf("Stepper motor test has begun.\n\r");
        if (find_home() == -1) {
            pc.printf("FAULT: Home sensor actuated improperly.\n\r");
            return -1;
        } else {
            pc.printf("Home sensor was located.\n\r");
            laserToggle(1);
        }
    }
    int arrayLength = CHANNEL_NUM / 2;
    
    while (!left_button.read() == 1 && !right_button.read() == 0) {
        int errorCheck = stp_step(STP_LEFT_STP_CW);
        if (errorCheck == -1) {
            return -1;
        }
        float * scannedTransistors = muxScanHalf(station_select_bit.read());
        printScannedTransistors (scannedTransistors, arrayLength);
    }
    while (!left_button.read() == 0 && !right_button.read() == 1) {
        stp_step(STP_RIGHT_STP_CCW);
        
        float * scannedTransistors = muxScanHalf(station_select_bit.read());
        printScannedTransistors (scannedTransistors, arrayLength);
    }
}

//Calibration Test
void testSeven (bool ignore) {
    if (ignore == false){
        clearSerial();
        pc.printf("Calibration test has begun.\n\r");
        stp_calibrate();

        int arrayLength = CHANNEL_NUM / 2;
        printDistances (calibration_array, arrayLength);
        printDifferenceDistances (calibration_array, arrayLength);
        pc.printf("\n\r");
    }
}

//Repeatability Test
int testEight (bool ignore) {
    if (ignore == false) {
        pc.printf("Repeatability test has begun.\n\r");
        //Find home
        if (find_home() == -1) {
            pc.printf("FAULT: Home sensor actuated improperly.\n\r");
            return -1;
        } else {
            pc.printf("Home sensor was located.\n\r");
            laserToggle(1);
        }
        
        for (int i = 0; i < 5; i++) {
            pc.printf("Sequence %d\n\r", i + 1);
            repeatability();
        }
        laserToggle(0);
    }   
}

int repeatability () {
    //Go to close transistor
    while (get_position() != calibration_array[7]) {
        stp_step(STP_LEFT_STP_CW);
    }
    float transistor_value = scanOneTransistor(7, true);
    pc.printf("PT7: %.1f\n\r", transistor_value * 3.3f);
    wait_ms(100);
    
    //Go to farthest transistor
    while (get_position() != calibration_array[0]) {
        stp_step(STP_LEFT_STP_CW);
    }
    transistor_value = scanOneTransistor(0, true);
    pc.printf("PT0: %.1f\n\r", transistor_value * 3.3f);
    wait_ms(100);

    //Move further from home
    for (int i = 0; i < 100; i++) {
        stp_step(STP_LEFT_STP_CW);
    }
    //Move back to farthest transistor
    while (get_position() != calibration_array[0]) {
        stp_step(STP_RIGHT_STP_CCW);
    }
    transistor_value = scanOneTransistor(0, true);
    pc.printf("PT0: %.1f\n\r", transistor_value * 3.3f);
    wait_ms(100);
    
    //Move back to closest transistor
    while (get_position() != calibration_array[7]) {
        stp_step(STP_RIGHT_STP_CCW);
    }
    transistor_value = scanOneTransistor(7, true);
    pc.printf("PT7: %.1f\n\r", transistor_value * 3.3f);
    wait_ms(100);
}

void printDistances (int * calibration_array, int arrayLength) {
    for (int i = 0; i < arrayLength; i++) {
        pc.printf("%c[%d;%df", ASCII_ESC, 8, i * 10);
        pc.printf("%d:%d | ", i, calibration_array[i]);
    }
}

void printDifferenceDistances (int * calibration_array, int arrayLength) {
    for (int i = 0; i < arrayLength - 1; i++) {
        pc.printf("%c[%d;%df", ASCII_ESC, 9, i * 23);
        pc.printf("Distance (%d->%d): %d | ", i, i + 1, calibration_array[i] - calibration_array[i + 1]);
    }
}

void printScannedTransistors (float * scannedTransistors, int arrayLength) {
    for (int i = 0; i < arrayLength; i++) {
        pc.printf("%c[%d;%df", ASCII_ESC, 4, i * 10);
        pc.printf("%d:%.1f V|", i, scannedTransistors[i] * 3.3f);
    }
}