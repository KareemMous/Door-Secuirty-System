 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.h
 *
 * Description: Header file for the DC Motor
 *
 * Author: Kareem Moustafa
 *
 *******************************************************************************/



#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	STOP,CW,ACW
}DcMotor_State;


/*******************************************************************************
 *                        Function Prototypes                                  *
 *******************************************************************************/
/*
 * Description:
 * Initializing the DC Motor
 * 1-By setting the Pin directions by the GPIO Driver
 * 2-Initializing the Motor to be at the STOP state
 */
void DcMotor_Init(void);



/*
 * Description:
 * 1-Controlling the Motor Mode according to the Input State by writing to the PINS by The GPIO driver
 * 2-Sending the speed to the PWM
 */
void DcMotor_Rotate(DcMotor_State state);

#endif /* MOTOR_H_ */
