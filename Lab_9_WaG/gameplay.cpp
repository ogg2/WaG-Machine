/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Run the actual gameplay, play as gnoller/whacker
* 
* Last Modified: 24 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "stepper.h"
#include "io_pins.h"
#include "spi.h"
#include "laser.h"
#include "phototransistor.h"
#include "gameplay.h"
#include "display.h"

#define MAX_VOLLEY_COUNT 15
#define gnoll_lifetime 15  //seconds
#define threshold 2.0f     //volts

int match_count;
int hit_count;
int miss_count;

/**
* NAME: play_as_gnoller
* Description: Play this station as the gnoller
* return: int, returns -1 if error occurs
**/
int play_as_gnoller();

/**
* NAME: play_as_whacker
* Description: Play this station as the whacker
* return: int, returns -1 if error occurs
**/
int play_as_whacker();

void play_wag() {
    srand(time(NULL));              //seed random number generator
    if (match_count == 0) {
        hit_count = 0;              //left 2 digits
        miss_count = 0;             //right 2 digits
        
        int hit_miss_array[2] = {hit_count, miss_count};
        dsp_hit_miss(hit_miss_array);
        
        match_count = match_count + 1;
    }
    if ((station_select_bit.read() == STATION_A_RIGHT && match_count == 1) || 
        (station_select_bit.read() == STATION_B_LEFT && match_count == 2)) {
        pc.printf("Playing as gnoller.\n\r");
        play_as_gnoller();
    } else {
        pc.printf("Playing as whacker.\n\r");
        play_as_whacker();
    }
    
    pc.printf("End of volley.\n\r");
    
    match_count = match_count + 1;
    if (match_count == 3) {
        match_count = 0;
    }
}

int play_as_gnoller() {
    int volley_count = 0;
    while (volley_count < MAX_VOLLEY_COUNT) {
        int random_number = rand() % 8;
        int random_gnoll_location = calibration_array[random_number];
        
        //Step left until reaching goal position
        if (random_gnoll_location > get_position()) {
            while (random_gnoll_location > get_position()) {
                stp_step(STP_LEFT_STP_CW);
            }
        //Step right until reaching goal position
        } else {
            while (random_gnoll_location < get_position()) {
                stp_step(STP_RIGHT_STP_CCW);
            }
        }
        
        laserToggle(1);
        Timer timer;
        timer.start();
        bool sameCarriage = true;       //bool to determine which transistor to scan
        float illuminatedTransistor = scanOneTransistor(random_number, sameCarriage);
    
        if (illuminatedTransistor * 3.3f > threshold) {
            discreteLedOn(random_number);
        } else {
            pc.printf("Improper phototransitor illuminated.\n\r");
            return -1;
        }
                
        int whacker_selection;
        bool whacker_guessed = false;
        
        //wait for whacker to attempt "whack" 
        while (timer.read() < gnoll_lifetime && whacker_guessed == false) {
            float * scannedTransistors = muxScanHalf(!station_select_bit.read());
            int arrayLength = CHANNEL_NUM / 2;
            for (int i = 0; i < arrayLength; i++) {
                if (scannedTransistors[i] * 3.3f > threshold) {
                    whacker_selection = i;  //index of whacker pick
                    whacker_guessed = true;
                }
            }  
        }
        
        if (whacker_guessed == false) {
            miss_count = miss_count + 1;
        } else {
            if (whacker_selection == random_number) {
                hit_count = hit_count + 1;
            } else {
                miss_count = miss_count + 1;
            }
        }
        
        int hit_miss_array[2] = {hit_count, miss_count};
        dsp_hit_miss(hit_miss_array);
        
        laserToggle(0);
        volley_count = volley_count + 1;
        discreteLedsOff();
        //wait used to better handle back-to-back transistor selections
        wait(0.5);
    }
}

int play_as_whacker() {
    int volley_count = 0;
    while (volley_count < MAX_VOLLEY_COUNT) {
        Timer timer;
        
        bool gnoller_picked = false;
        int gnoller_selection;
        //wait for gnoller to pick transistor
        while (gnoller_picked == false) {
            
            float * scannedTransistors = muxScanHalf(!station_select_bit.read());
            int arrayLength = CHANNEL_NUM / 2;
            
            for (int i = 0; i < arrayLength; i++) {
                if (scannedTransistors[i] * 3.3f > threshold) {
                    gnoller_selection = i; //index of gnoller pick
                    gnoller_picked = true;
                    discreteLedsOff();
                    timer.start();
                }
            }
        }
        
        int gnoll_location = calibration_array[gnoller_selection];

        //step left until reached goal position or stop if timer expired
        if (gnoll_location > get_position()) {
            while (gnoll_location > get_position() && timer.read() < gnoll_lifetime) {
                stp_step(STP_LEFT_STP_CW);
            }
        //step right until reached goal position or stop if timer expired
        } else {
            while (gnoll_location < get_position() && timer.read() < gnoll_lifetime) {
                stp_step(STP_RIGHT_STP_CCW);
            }
        }
        //if timer not expired, "whack" transistor selection
        if (timer.read() < gnoll_lifetime) {
            laserToggle(1);
            bool sameCarriage = true;
            float illuminatedTransistor = scanOneTransistor(gnoller_selection, sameCarriage);
            
            if (illuminatedTransistor * 3.3f > threshold) {
                discreteLedOn(gnoller_selection);
            }
        }
        bool sameCarriage = false;
        //wait until gnoller moves away from selected transistor
        while (scanOneTransistor(gnoller_selection, sameCarriage) * 3.3f > threshold) {
            ;
        }
        
        laserToggle(0);
        volley_count = volley_count + 1;
    }
}