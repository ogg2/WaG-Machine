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

#ifndef SPI_H
#define SPI_H

#define SPI_NO_ID 0
#define SPI_AS1107_ID 1
#define SPI_DRV8806_ID 2

struct spi_cfg {
    int spi_id;             //ID should be unique for each slave type    
    DigitalOut spi_nss;     //The pin driving the slave's chip select    
    int spi_mode;           //SPI mode needed by this slave    
    int spi_freq;           //SPI frequency needed by this slave    
    int spi_no_bits;        //No. of data bits to be transfered each send to the slave
};

/**
* NAME: spi_send
* Description: XXXXXX
* 
* Inputs: 
* Parameters: struct to configure the spi, int, data being sent
* Globals:
* 
* Outputs:
* Parameters:
* Globals:
* Returns: void

* Notes:
* Changes: 
**/
void spi_send(struct spi_cfg spi_config, int data);

#endif /*SPI_H*/