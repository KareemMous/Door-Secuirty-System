 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Source file for the Buzzer
 *
 * Author: Kareem Moustafa
 *
 *******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID          PORTD_ID
#define BUZZER_PIN_ID           PIN2_ID


/*******************************************************************************
 *                        Function Prototypes                                  *
 *******************************************************************************/
/*
 * Description:
 * Initializing the Buzzer LED
 * 1-By setting the Pin directions by the GPIO Driver as an OUTPUT
 */
void Buzzer_Init(void);


/*
 * Description:
 * By using the GPIO Driver, Write the Buzzer Pin as Low to be OFF
 */
void Buzzer_OFF(void);


/*
 * Description:
 * By using the GPIO Driver, Write the Buzzer Pin as HIGH to be ON
 */
void Buzzer_ON(void);




#endif /* BUZZER_H_ */
