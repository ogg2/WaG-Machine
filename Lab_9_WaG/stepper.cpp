/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Control the stepper motor
* 
* Last Modified: 21 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "stepper.h"
#include "io_pins.h"
#include "spi.h"
#include "laser.h"
#include "phototransistor.h"
#include "display.h"

#define DRV8806_SPI_MODE 0
#define DRV8806_SPI_FREQ 1000000
#define DRV8806_SPI_NO_BITS 8
#define STP_DELAY 1500

static int current_position = 0;

//Calibration
bool calibration_status = NOT_CALIBRATED;
int calibration_array[8] = {0};

DigitalOut home_sensor_switch (STP_HOME_SENSOR);
DigitalOut stepper_nss(STP_DRV8806_NSS);

struct spi_cfg spi_drv8806 {
    SPI_DRV8806_ID,             //ID should be unique for each slave type    
    stepper_nss,                //The pin driving the slave's chip select    
    DRV8806_SPI_MODE,           //SPI mode needed by this slave    
    DRV8806_SPI_FREQ,           //SPI frequency needed by this slave    
    DRV8806_SPI_NO_BITS,        //No. of data bits to be transfered each send to the slave
};

void set_position(int position) {
    current_position = position;
}

int get_position() {
    return current_position;
}


void stp_init() {
    spi_send(spi_drv8806, 0x00);
    
}

int stp_step(int direction) {
    static int lookupTable[] = {0x03, 0x06, 0x0c, 0x09};
    static int index = 0;
    int cur_position = get_position();
    
    //CW
    if (direction == STP_LEFT_STP_CW) {
        if (cur_position > STP_MAXIMUM_POSITION) {
            pc.printf("WARNING: Carriage approaching maximum position.\n\r");
            return -1;
        }
        
        index = index + 1;
        if (index == 4) {
            index = 0;
        }
        spi_send(spi_drv8806, lookupTable[index]);
        int position = get_position();
        set_position(position + 1);
    }
    
    //CCW
    if (direction == STP_RIGHT_STP_CCW && !home_sensor_switch.read() == 0) {
        index = index - 1;
        if (index == -1) {
            index = 3;
        }
        spi_send(spi_drv8806, lookupTable[index]);
        int position = get_position();
        set_position(position - 1);
    }
    
    wait_us(STP_DELAY);
    return 0; //check where this is called and check return value
    //0 and continue or -1 and stop
}

int stp_calibrate() {
    
    laserToggle(0);
        
    //Find home
    if (find_home() == -1) {
        pc.printf("FAULT: Home sensor actuated improperly.\n\r");
        return -1;
    } else {
        pc.printf("Home sensor was located.\n\r");
        laserToggle(1);
    }
    calibration_status = NOT_CALIBRATED;
    int calibration_index = 7;
    float max_transistor_value = 0.0f;
    bool illuminated = false;
    while (calibration_status == NOT_CALIBRATED) {
        int errorCheck = stp_step(STP_LEFT_STP_CW);
        if (errorCheck == -1) {
            return -1;    
        }
        
        int cur_position = get_position();
        bool sameCarriage = true;
        float transistor_value = scanOneTransistor(calibration_index, sameCarriage);
        if (transistor_value * 3.3f > 0.5f) {
            illuminated = true;
            discreteLedOn(calibration_index);
        }
        if (illuminated == true) {
            if (transistor_value > max_transistor_value) {
                max_transistor_value = transistor_value;
                calibration_array[calibration_index] = cur_position;
            }
        }
        if (transistor_value * 3.3f < 0.5f && calibration_array[calibration_index] != 0) {
            calibration_index--;
            illuminated = false;
            discreteLedsOff();
            max_transistor_value = 0.0f;
            wait_ms(100);
        }
        if (calibration_index == -1) {
            calibration_status = CALIBRATED;
        }
    }
    laserToggle(0);
    find_home();
    pc.printf("Done with calibration.\n\r");
}

int find_home() {
    if (!home_sensor_switch.read() == 1) {
        for (int i = 0; i < 100; i++) {
            stp_step(STP_LEFT_STP_CW);
        }
        if (!home_sensor_switch.read() == 1) {
            return -1;
        }
    }
    while (!home_sensor_switch.read() == 0) {
        stp_step(STP_RIGHT_STP_CCW);
    }
    set_position(0);
    return 0;
}

int read_home_sensor() {
    return !home_sensor_switch.read();
}

