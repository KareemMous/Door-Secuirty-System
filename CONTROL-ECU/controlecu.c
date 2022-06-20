 /******************************************************************************
 *
 * Module: MC2 Application (CONTROL-ECU)
 *
 * File Name: controlecu.c
 *
 * Description: Source file for the MC2 application
 *
 * Author: Kareem Moustafa
 *
 /

 /*******************************************************************************
 *                                   INCLUDES                                  *
 *******************************************************************************/

#include "twi.h"
#include "external_eeprom.h"
#include "uart.h"
#include "timers.h"
#include "buzzer.h"
#include "motor.h"
#include "common_macros.h"

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/

#define M1_READY 0x10 /*MC1 send to MC2 when it is ready*/
#define M2_READY 0x20 /*MC2 send to MC1 when it is ready*/
#define Error_pass 0x00 /*MC2 send to MC1 while checking,indicates that the entered password is wrong*/
#define INITIALIZED_FLAG 0x01 /*Flag is saved once the one run code is done*/
#define SAVE 0x02 /*MC1 send to MC2 to begin saving the password*/
#define END_SAVING 0x03 /*MC2 send to MC1 to inform that saving password has been finished*/
#define CHECK_PASS 0x04 /*MC1 send to MC2 to begin checking the password*/
/*
 * MC2 send to MC1 while checking,to inform MC1 to send the flowing bit of password to check it
 *It means that there is no error in this bit from password
 */
#define CONTINUE_PASS 0x05
#define UNLOCK_DOOR 0x06 /*MC1 send to MC2 to begin opening the gate*/

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint16 g_tick = 0; /*Global variable indicates number of overflows*/
uint8 g_calc_sec_flag=0; /*Global variable indicates number of seconds*/

/*******************************************************************************
 *                            FUNCTIONS DEFINITIONS                            *
 *******************************************************************************/

/*
 * Function Description : calculating seconds
 */
void Calculate_time(void);


/*
 * Function Description : receiving the entered password from MC1 and saving it
 * Flag is set as indicator that the first run is executed
 */
void Receive_pass(void);


/*
 *  Function Description : checking the entered password from MC
 * If it doesn't match for less than 3 times, it asks to enter it again
 * Once 3 times are executed, Buzzer is on from 1 min
 * Input: Timer_ConfigTyper
 */
void Check_pass(const Timer_ConfigType * Config_Ptr);


/*
 * Function Description : opening the door
 * Input: Timer_ConfigTyper
 */
void Open_door(const Timer_ConfigType * Config_Ptr);


/*
 * Function Description : checking the password and handle the action that the user need
 *1.changing the password
 *2.opening the gate.
 *Input: Timer_ConfigTyper
 */
void Main_options(const Timer_ConfigType * Config_Ptr);

int main(void)
{
	/*
	 * flag_init : to know whether the first run is executed or not and does as follows depending on this.
	 */
	uint8 flag_init =0;
	Buzzer_Init();
	DcMotor_Init();

	/*
	 * Timer_Configrution struct
	 * 1. Timer_num: Timer0
	 * 2. Timer_mode: Overflow mode
	 * 3. Prescalar: clk/1024
	 * 4. Initial value: 0
	 * 5. Compare value: 0 as it is overflow mode
	 */
	Timer_ConfigType Timer_Config = {TIMER2,OVERFLOW,CPU_1024,0,0};

	/*
	 * Set the Call back function pointer in the timer driver
	 * Calculate_time: Function that calculate seconds
	 */
	Timer2_setCallBack(Calculate_time);


	/*
	 *Uart_Configrution struct
	 * 1. parity: Disabled (no parity)
	 * 2. stop_bit: BIT_1 (1 stop bit)
	 * 3. datasize: BIT_8 (8-bits data)
	 * 4. baudrate: 9600
	 */
	UART_ConfigType UART_Config = {BIT_8_MODE,DISABLED,ONE,9600};

	/* EEPROM and I2c initialization*/
	EEPROM_init();

	/*Pass the required UART configuration to UART initialization*/
	UART_init(& UART_Config);

	while(1)
	{
		while(UART_receiveByte()!= M1_READY); /*Wait till MC1 send that it is ready to begin*/

		/*Read the address where the initialized flag should be saved*/
		EEPROM_readByte(0x0420, &flag_init);
		UART_sendByte(flag_init); /*Send to MC1 the value of the address*/

		/*
		 *If the flag_init not equal to INITIALIZED_FLAG,
		 *the first run is no executed
		 */
		if(flag_init != INITIALIZED_FLAG)
		{
			/*
			 *Go to this function where it save the new password and the flag
			 *So, when it check again on the address, it finds that they have the same value
			 *So, it then go to the main options
			 */
			Receive_pass();
		}
		/*
		 *If the flag_init equals to INITIALIZED_FLAG,
		 *first run is executed goes to the main options.
		 */
		else
		{
			/*
			 *Go to the main options
			 *Input:the timer config struct
			 */
			Main_options(& Timer_Config);
		}
	}
	return 0;
}



/*
 * Function Description : calculating seconds
 */
void Calculate_time(void)
{
	g_tick ++; /*Every overflow increase ticks*/

	/*Once ticks become 30(according to the used prescalar),increase seconds flag and tick =0*/
	if(g_tick == 30)
	{
		g_calc_sec_flag ++; /*Every 30 tick, increase seconds flag*/
		g_tick =0; /*Put tick=0 to start counting new second*/
	}
}





/*
 * Function Description : receiving the entered password from MC1 and saving it
 * Flag is set as indicator that the first run is executed
 */
void Receive_pass(void)
{
	while(UART_receiveByte()!= SAVE); /*Wait till MC1 send SAVE to begin*/

	/*
	 * pass: variable used to receive the entered password from MC1
	 * count_pass: variable used as a counter to count the entered password digits
	 */
	uint8 pass,count_pass;
	for(count_pass=5; count_pass>0; count_pass--)
	{
		/*We begin from the last digit entered*/
		UART_sendByte(M2_READY); /*Send to MC1 that MC2 is ready to receive the next digit*/
		pass = UART_receiveByte(); /*Receive the entered password digit from MC1*/
		EEPROM_writeByte((0x0311+count_pass), pass); /*Save the entered digit in specific address*/
		_delay_ms(70); /*Wait for 70ms to write the value in the memory*/
	}
	/*Then write the INITIALIZED_FLAG in this address
	 * Every time we run code, we will check this address to know if the one run code is done or not*/
	EEPROM_writeByte(0x0420, INITIALIZED_FLAG);
	_delay_ms(70);/*Wait for 70ms to write the value in the memory*/
	UART_sendByte(END_SAVING); /*Send to MC1 that saving process has been finished*/
}




/*
 *  Function Description : checking the entered password from MC
 * If it doesn't match for less than 3 times, it asks to enter it again
 * Once 3 times are executed, Buzzer is on from 1 min
 * Input: Timer_ConfigTyper
 */
void Check_pass(const Timer_ConfigType * Config_Ptr)
{
	while(UART_receiveByte()!= CHECK_PASS); /*Wait till MC1 send CHECK_PASS to begin*/

	/*
	 * pass: variable used to receive the entered password from MC1
	 * pass_saved: variable used to get the right stored password
	 * error: variable used to know how many times the password was wrong
	 * count_pass: variable used as a counter to count the entered password digits
	 */
	uint8 pass=0,pass_saved=0,error,count_pass;
	for(count_pass=5; count_pass>0; count_pass--)
	{
		/*We begin from the last digit entered*/
		pass = UART_receiveByte(); /*Receive the entered password digit from MC1*/

		/*The right password is stored in the EEPROM,
		 *so we go to the address where it should be saved and read the correct value(pass_saved)*/
		EEPROM_readByte((0x0311+count_pass), &pass_saved);
		if(pass != pass_saved) /*The entered password digit not like that is stored*/
		{
			UART_sendByte(Error_pass); /*Send to MC1 that there is an error in the password*/
		}
		else /*The entered password digit is like that is stored*/
		{
			/*Send to MC1 to continue sending the remaining digits*/
			UART_sendByte(CONTINUE_PASS);
		}
	}
	error = UART_receiveByte(); /*Receive from MC1 how many times the password was wrong*/
	if(error <= 2) /*If it is less than or equal 2, repeat the check function*/
	{
		Check_pass(Config_Ptr);
	}
	else if(error == 3) /*If it is 3 times wrong*/
	{
		/*Initialize the timer and pass the required configurations to it*/
		Timer_init(Config_Ptr);

		g_tick =0;/*Initially make the tick equals zero, to start from the beginning*/
		g_calc_sec_flag =0; /*Initially make the sec_flag equals zero,to start from the beginning*/
		while (g_calc_sec_flag != 30) /*As long as it hasn't passed 1 minute,light the led */
		{
			Buzzer_ON();
		}
	}
}



/*
 * Function Description : opening  the door
 * Input: Timer_ConfigTyper
 */
void Open_door(const Timer_ConfigType * Config_Ptr)
{
	while(UART_receiveByte()!= UNLOCK_DOOR); /*Wait till MC1 send UNLOCK_DOOR to begin*/

	Timer_init(Config_Ptr); /*Initialize the timer and pass the required configurations to it*/

	g_tick =0; /*Initially make the tick equals zero, to start from the beginning*/
	g_calc_sec_flag =0; /*Initially make the sec_flag equals zero, to start from the beginning*/
	/*As long as it hasn't passed 15 seconds, rotate the motor clockwise*/
	while(g_calc_sec_flag != 15)
	{
		DcMotor_Rotate(CW);
	}

	/*Then put tick and sec_flag with zeros, to begin to count from the beginning*/
	g_tick =0;
	g_calc_sec_flag =0;
	/*As long as it hasn't passed 3 seconds, stop the motor*/
	while(g_calc_sec_flag != 3)
	{
		DcMotor_Rotate(STOP);
	}

	/*Then put tick and sec_flag with zeros, to begin to count from the beginning*/
	g_tick =0;
	g_calc_sec_flag =0;
	/*As long as it hasn't passed 15 seconds, rotate the motor anti-clockwise*/
	while(g_calc_sec_flag != 15)
	{
		DcMotor_Rotate(ACW);
	}

	/*Then stop the motor*/
	CLEAR_BIT(PORTB,PB0);
	CLEAR_BIT(PORTB,PB1);
}




/*
 *Function Description : checking the password and handle the action that the user need
 *1.changing the password
 *2.opening the gate.
 *Input: Timer_ConfigTyper
 */

void Main_options(const Timer_ConfigType * Config_Ptr)
{
	/*
	 *Call this function where it check whether the entered password is right or not
	 *and it handle all the cases
	 */
	Check_pass(Config_Ptr);

	/*This means that the password was entered right before finishing the 3 times of error*/
	if(UART_receiveByte() == 0)
	{
		if(UART_receiveByte() == '*') /*It receives the pressed key from MC1*/
		{
			/*This key means that the user will change the password
			 *SO,this function will save this new one */
			Receive_pass();
		}
		else
		{
			/*This means that the user has pressed '%',so we need to open the gate
			 *It takes the configuration of the timer to handle the motor and the led
			 */
			Open_door(Config_Ptr);
		}
	}
	/*This means that the password was entered wrong for 3 times and the led became on for 1 min*/
	else
	{
		Buzzer_OFF(); /*Led off as 1 min has passed*/
	}
}
