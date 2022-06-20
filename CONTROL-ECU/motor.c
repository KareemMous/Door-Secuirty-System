 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.c
 *
 * Description: Source file for the DC Motor
 *
 * Author: Kareem Moustafa
 *
 *******************************************************************************/

#include "common_macros.h"
#include "motor.h"
#include "gpio.h"
#include <avr/io.h>


/*******************************************************************************
 *                        Function Defintions                                  *
 *******************************************************************************/
/*
 * Description:
 * Initializing the DC Motor
 * 1-By setting the Pin directions by the GPIO Driver
 * 2-Initializing the Motor to be at the STOP state
 */
void DcMotor_Init(void){

	/* Connect PB0,PB1 of the Atmega16 as output pins connecting it the L293D Chip to control the Motor*/
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID,PIN1_ID,PIN_OUTPUT);

	/*Starting the motor mode at stop state by writing 0 0 to the output pins PB0,PB1*/
	GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
	GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);


}


/*
 * Description:
 * 1-Controlling the Motor Mode according to the Input State by writing to the PINS by The GPIO driver
 */
void DcMotor_Rotate(DcMotor_State state){
		if(state==STOP)
		{
			GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
			GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);
		}
		else if(state==ACW)
		{
			GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_HIGH);
			GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);
		}else if(state==CW)
		{
			GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
			GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_HIGH);
		}
}
