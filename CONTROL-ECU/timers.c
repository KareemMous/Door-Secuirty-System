 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timers.c
 *
 * Description: Source file for the Timers
 *
 * Author: Kareem Moustafa
 *
 *******************************************************************************/

#include "timers.h"
#include "common_macros.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*Global variables to hold the addresses of the call back functions in the application*/
static volatile void (*g_t_0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_t_1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_t_2_callBackPtr)(void) = NULL_PTR;



/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_t_0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t_0_callBackPtr)();
		/* another method to call the function using pointer to function t_0_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_t_0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t_0_callBackPtr)();
		/* another method to call the function using pointer to function t_0_ctc_callBackPtr(); */
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_t_1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t_1_callBackPtr)();
		/* another method to call the function using pointer to function t_1_ovf_callBackPtr(); */
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g_t_1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t_1_callBackPtr)();
		/* another method to call the function using pointer to function t_1_ctc_callBackPtr(); */
	}
}
ISR(TIMER2_OVF_vect)
{
	if(g_t_2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t_2_callBackPtr)();
		/* another method to call the function using pointer to function t_2_ovf_callBackPtr(); */
	}
}
ISR(TIMER2_COMP_vect)
{
	if(g_t_2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t_2_callBackPtr)();
		/* another method to call the function using pointer to function t_2_ctc_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description:
 * Initializing the Timers according to the Configuration struct taken as input
 *  1. Set the required clock.
 * 	2. Set the required mode
 * 	3. Set the required Timer
 * 	4. Set the inital and compare values if needed
 */
void Timer_init(Timer_ConfigType* Config_Ptr){
	SREG &= ~(1<<7);
	/*
	 * Timer0
	 * FOC0=1
	 * CS02,CS01,CS00 according to the prescaler
	 */
	if((Config_Ptr->timer_num)==TIMER0){
		TCCR0 = (1<<FOC0);
		TCCR0 |= (Config_Ptr->scalar);
		if((Config_Ptr->mode)==OVERFLOW){
			/*
			 * OVERFLOW MODE
			 * TOIE0 = 1
			 * INITAL VALUE IN TCNT0 = 0
			 */
			SET_BIT(TIMSK,TOIE0);
			TCNT0=(Config_Ptr->s_initial);
		}else if((Config_Ptr->mode)==COMPARE){
			/*
			 * COMPARE MODE
			 * TCNT0 ( COUNTER REGISTER = 0 )
			 * WGM01=1 , COM01 =1 , COM00=1 SET OC0 ON COMPARE MATCH MODE
			 * OCIE0 = 1 THE OUTPUT COMPARE REGISTER NEEDED IN THE COMPARE MODE
			 * OCR0 add the compare value in it taken from the config_struct
			 */
			TCNT0=0;
			SET_BIT(TCCR0,WGM01);
			SET_BIT(TCCR0,COM01);
			SET_BIT(TCCR0,COM00);
			OCR0=Config_Ptr->s_comp;
			SET_BIT(TIMSK,OCIE0);
		}
	}else if((Config_Ptr->timer_num)==TIMER1){
		/*
		 * Timer1
		 * FOC1A=1,FOC1B=1;
		 * CS12,CS11,CS10 according to the prescaler
		 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);
			TCCR1B |= (Config_Ptr->scalar);
			if((Config_Ptr->mode)==OVERFLOW){
				TCNT1=(Config_Ptr->s_initial);
				/*
				 * OVERFLOW MODE
				 * TOIE1 = 1
				 * INITAL VALUE IN TCNT1 = 0
				 */
				SET_BIT(TIMSK,TOIE1);

			}else if((Config_Ptr->mode)==COMPARE){
				/*
				 * COMPARE MODE
				 * TCNT1 ( COUNTER REGISTER = 0 )
				 * WGM12=1 , COM1A1 =1 , COM1A0=1 SET OC0 ON COMPARE MATCH MODE
				 * OCIE1A = 1 THE OUTPUT COMPARE REGISTER NEEDED IN THE COMPARE MODE
				 * OCR1A add the compare value in it taken from the config_struct
				 */
				TCNT1=0;
//				CLEAR_BIT(TCCR1A,COM1A1);
//				CLEAR_BIT(TCCR1A,COM1A0);
				SET_BIT(TCCR1B,WGM12);
				OCR1A=Config_Ptr->s_comp;
				TIMSK|=(1<<OCIE1A);
			}
	}else if((Config_Ptr->timer_num)==TIMER2){
		/*
		 * Timer2
		 * FOC2=1
		 * CS22,CS21,CS20 according to the prescaler
		 */
		TCCR2 = (1<<FOC2);
		if(Config_Ptr->scalar <= CPU_8){
			TCCR2 |= (Config_Ptr->scalar);
		}
		else if ((Config_Ptr->scalar == CPU_32))
		{
			TCCR2 |= 0x03 ;
		}else if ((Config_Ptr->scalar == CPU_128)){
			TCCR2 |= 0x05;
		}else if((Config_Ptr->scalar == CPU_64)){
			TCCR2 |= 0x04;
		}else if(Config_Ptr->scalar > CPU_8){
			TCCR2 |= ((Config_Ptr->scalar) + 2);
		}
		if((Config_Ptr->mode)==OVERFLOW){
			/*
			 * OVERFLOW MODE
			 * TOIE2 = 1
			 * INITAL VALUE IN TCNT2 = 0
			 */
			SET_BIT(TIMSK,TOIE2);
			TCNT2=(Config_Ptr->s_initial);
		}else if((Config_Ptr->mode)==COMPARE){
			/*
			 * COMPARE MODE
			 * TCNT2 ( COUNTER REGISTER = 0 )
			 * WGM21=1 , COM21 =1 , CO20=1 SET OC0(PB3)pin ON COMPARE MATCH MODE
			 * OCIE2 = 1 THE OUTPUT COMPARE REGISTER NEEDED IN THE COMPARE MODE
			 * OCR2 add the compare value in it taken from the config_struct
			 */
			TCNT0=0;
			SET_BIT(TCCR2,WGM21);
			SET_BIT(TCCR2,COM21);
			SET_BIT(TCCR2,COM20);
			OCR2=Config_Ptr->s_comp;
			SET_BIT(TIMSK,OCIE2);
		}

	}
	SREG |= (1<<7); /*Enable I-bit*/
}




/*
 * Description: Function to set the Call Back function address for timer0
 */
void Timer0_setCallBack(void(*a_t0_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t_0_callBackPtr = a_t0_ptr;
}



/*
 * Description: Function to set the Call Back function address for timer1
 */
void Timer1_setCallBack(void(*a_t1_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t_1_callBackPtr = a_t1_ptr;
}



/*
 * Description: Function to set the Call Back function address for timer2
 */
void Timer2_setCallBack(void(*a_t2_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t_2_callBackPtr = a_t2_ptr;
}


/*
 * Description: Function to disable the Timer required
 */

void Timer_Deinit(Timer_ConfigType* Config_Ptr)
{
	uint8 timer_num = Config_Ptr->timer_num;
	if(timer_num==0){
		TCCR0 = 0;
	}else if(timer_num==1){
		TCCR1A=0;
		TCCR1B=0;
	}else if(timer_num==2){
		TCCR2=0;
	}
}
