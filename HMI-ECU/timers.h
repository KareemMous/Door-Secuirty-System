/*
 * timers.h
 *
 *  Created on: Nov 6, 2021
 *      Author: Lenovo
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <avr/io.h>
#include "util/delay.h"
#include <avr/interrupt.h>
#include "util/delay.h"
#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	TIMER0,TIMER1,TIMER2
}Timer_Num;
typedef enum
{
	OVERFLOW,COMPARE
}Timer_Mode;
typedef enum
{
	NO_CLOCK,CPU_CLOCK,CPU_8,CPU_64,CPU_256,CPU_1024,CPU_32,CPU_128
}Timer_Prescalar;

typedef struct
{
	Timer_Num timer_num;
	Timer_Mode mode;
	Timer_Prescalar scalar;
	uint16 s_initial; /*Take the initial value from the user*/
	uint16 s_comp; /*Take the compare value from user, put 0 if using overflow mode*/
}Timer_ConfigType;


/*******************************************************************************
 *                        Function Prototypes                                  *
 *******************************************************************************/
/*
 * Description:
 * Initializing the Timers according to the Configuration struct taken as input
 *  1. Set the required clock.
 * 	2. Set the required mode
 * 	3. Set the required Timer
 * 	4. Set the inital and compare values if needed
 */
void Timer_init(Timer_ConfigType* Config_Ptr);


/*
 * Description: Function to set the Call Back function address for timer0
 */
void Timer0_setCallBack(void(*a_t0_ptr)(void));


/*
 * Description: Function to set the Call Back function address for timer1
 */
void Timer1_setCallBack(void(*a_t1_ptr)(void));

/*
 * Description: Function to set the Call Back function address for timer1
 */
void Timer2_setCallBack(void(*a_t2_ptr)(void));


/*
 * Description: Function to disable the Timer required
 */

void Timer_Deinit(Timer_ConfigType*Config_Ptr);

#endif /* TIMERS_H_ */
