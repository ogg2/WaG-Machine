/******************************************************************************
* EECS 397
* 
* Assignment Name: Lab 9
*
* Author: Gibson, Owen
*
* Purpose: Control the stepper motor
* 
* Last Modified: 5 April 2019
* 
******************************************************************************/

#ifndef STEPPER_H
#define STEPPER_H

#define STP_LEFT_STP_CW 2
#define STP_RIGHT_STP_CCW 1

#define STP_MAXIMUM_POSITION 6000

extern bool calibration_status;
extern int calibration_array[8];

/**
* NAME: stp_init
* Description: Used to initilize the stepper motor
* 
* Inputs: 
* Parameters:
* Globals:
* 
* Outputs:
* Parameters:
* Globals:
* Returns: void

* Notes:
* Changes: 
**/
void stp_init();

/**
* NAME: stp_init
* Description: Used to control the motor
* 
* Inputs: 
* Parameters: int, direction to turn the motor
* Globals:
* 
* Outputs:
* Parameters: int, returns -1 if current position exceeds max position
*   returns 0, if step is successful
* Globals:
* Returns: void

* Notes:
* Changes: 
**/
int stp_step(int);

/**
* NAME: stp_calibrate
* Description: Calibrate the stepper motor, save cal status and cal positions
* return: int, returns -1 if error occurs
**/
int stp_calibrate();

/**
* NAME: find_home
* Description: Find the home sensor with the stepper motor
* Return: int, -1 to indicate fault, 0 to indicate success
**/
int find_home();

/**
* NAME: read_home_sensor
* Description: Determine if the home sensor is actuated
* Return: int, 0 to indicate not actuated, 1 to indicate actuated
**/
int read_home_sensor();

/**
* NAME: set_position
* Description: set the current position of the stepper motor
* input param: int, position, current position of motor
**/
void set_position(int position);

/**
* NAME: get_position
* Description: get the current position of the stepper motor
* return: int, position, current position of motor
**/
int get_position();

#endif /*STEPPER_H*/