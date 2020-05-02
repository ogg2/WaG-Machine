/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 8
*
* Author: Gibson, Owen
*
* Purpose: Controls the 7-segment display and the discrete LEDs
* 
* Last Modified: 16 April 2019
* 
******************************************************************************/

#include "mbed.h"
#include "display.h"
#include "io_pins.h"
#include "spi.h"

//Private Constants
#define DSP_TST_ON 0x0f01
#define DSP_TST_OFF 0x0f00
#define DSP_NORM_OP 0x0c01       //normal operation, reset feature register
#define DSP_DECODE_MASK 0x090f   //B-hex decode on digits 0-3, no decode on others
#define DSP_SCAN_LIMIT 0x0b04    //Display digits 0-4 (five digits)
#define DSP_MAX_INTENSITY 0x0a0f //Set Display to maximum brightness

//Constants for configuring SPI talk to AS1107 controller chip
#define AS1107_SPI_MODE 0
#define AS1107_SPI_FREQ 1000000
#define AS1107_SPI_NO_BITS 16

DigitalOut dsp_nss(DSP_AS1107_NSS); //Construct DigitalOut for the NSS

struct spi_cfg spi_as1107 {
    SPI_AS1107_ID,             //ID should be unique for each slave type    
    dsp_nss,                   //The pin driving the slave's chip select    
    AS1107_SPI_MODE,           //SPI mode needed by this slave    
    AS1107_SPI_FREQ,           //SPI frequency needed by this slave    
    AS1107_SPI_NO_BITS,        //No. of data bits to be transfered each send to the slave
};

//Private Protoypes
void writeInt (unsigned int);
void additionalSetup (unsigned int);

DigitalOut slaveSelect (DSP_AS1107_NSS);

//Global Variables
int bcd_array[4];

//Public Functions: Function header comments for each function are in the .h file
void setupConfiguration () {
    spi_send(spi_as1107, DSP_TST_OFF); 
    spi_send(spi_as1107, DSP_NORM_OP);
    spi_send(spi_as1107, DSP_SCAN_LIMIT);
    spi_send(spi_as1107, DSP_DECODE_MASK);
    spi_send(spi_as1107, DSP_MAX_INTENSITY);
    
    //Initiating 0 to all digits
    spi_send(spi_as1107, 0x0100);
    spi_send(spi_as1107, 0x0200);
    spi_send(spi_as1107, 0x0300);
    spi_send(spi_as1107, 0x0400);
    spi_send(spi_as1107, 0x0500);
}

void displayTest () {
    spi_send(spi_as1107, DSP_TST_ON); 
}

void targetLEDTest (unsigned int digit) {
    spi_send(spi_as1107, digit);
    wait_ms(100);
    spi_send(spi_as1107, 0x0500);
}

void discreteLedOn (int LED) {
    unsigned int discreteLED = 0x0500;
    int ledOn = 1;
    if (LED != 0) {
        for (int i = 0; i < LED; i++) {
            ledOn = ledOn * 2;
        }
    } 
    spi_send(spi_as1107, discreteLED + ledOn);
}

void discreteLedsOff () {
    int allDiscreteLedsOff = 0x0500;
    spi_send(spi_as1107, allDiscreteLedsOff);
}

void dsp_hit_miss (int * hit_miss_array) {    
    int left_miss_digit = hit_miss_array[0] % 10;
    int right_miss_digit = hit_miss_array[0] / 10;
    int left_hit_digit = hit_miss_array[1] % 10;
    int right_hit_digit = hit_miss_array[1] / 10;
    int bcd_array[4] = {left_hit_digit, right_hit_digit, left_miss_digit, right_miss_digit};
    
    dsp_bcd(bcd_array);
}

void dsp_bcd (int * bcd_array) {
    int digit_no = 0x01;
    for (int i = 0; i < sizeof bcd_array; i++) { 
        unsigned int DSP_DIGITS = (digit_no<<8) | bcd_array[i];
                        
        spi_send(spi_as1107, DSP_DIGITS);
                     
        digit_no = digit_no + 1;
        if (digit_no == 0x05) {
            digit_no = 0x01;
        }
    }
}

void int_to_bcd_array(int num, int * array) {
    for (int i = 0; i < sizeof bcd_array; i++) {
        bcd_array[i] = 0;
    }
    
    int j = 0;
    while (num != 0) {
        bcd_array[j] = num % 10;
        num = num / 10;
        j = j + 1;
    }
}