 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the Buzzer
 *
 * Author: Kareem Moustafa
 *
 *******************************************************************************/
#include "buzzer.h"
#include "gpio.h"

/*******************************************************************************
 *                        Function Defintions                                  *
 *******************************************************************************/
/*
 * Description:
 * Initializing the Buzzer LED
 * 1-By setting the Pin directions by the GPIO Driver as an OUTPUT
 */
void Buzzer_Init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
}


/*
 * Description:
 * By using the GPIO Driver, Write the Buzzer Pin as Low to be OFF
 */
void Buzzer_OFF(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

/*
 * Description:
 * By using the GPIO Driver, Write the Buzzer Pin as HIGH to be ON
 */
void Buzzer_ON(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
