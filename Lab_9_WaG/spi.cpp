/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: SPI configuration using a struct
* 
* Last Modified: 28 February 2019
* 
******************************************************************************/

#include "mbed.h"
#include "io_pins.h"
#include "spi.h"

SPI spi (SPI_MOSI, SPI_MISO, SPI_SCLK);

void spi_send(struct spi_cfg spi_config, int spi_data){
    
    static int SPI_LAST_ID = SPI_NO_ID;  //init previous_id to SPI_NO_ID to force config of master on first send
    
    spi_config.spi_nss = 1;
    
    if (spi_config.spi_id != SPI_LAST_ID) {
        spi.format(spi_config.spi_no_bits, spi_config.spi_mode);
        spi.frequency(spi_config.spi_freq);
        SPI_LAST_ID = spi_config.spi_id;
    }
    spi_config.spi_nss = 0;    
    spi.write(spi_data);
    spi_config.spi_nss = 1;
}