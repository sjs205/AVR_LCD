/*
 *				AVR_LCD.c
 *		Created on:		17/02/2008
 *		Created by:		Steven Swann - swannonline@googlemail.com
 *		Completion:		09/09/2008
 *		Device:			ATmega48/162
 *		
 *		Simple library file to implimliment an LCD interface
 *
 *		LCD_Init();				initialises the LCD Module
 *		LCD_Clear();			Clear the module
 *		LCD_WCommand(char);		Writes char to the LCD module as a command
 *		LCD_WData(char);		Writes char to the LCD module as data
 *		LCD_Wait();				Waits for the LCD modules busy flag to become low
 */

#include "AVR_LCD.h" 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define SETBIT(x,y) (x |= (y))		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x



/*
 *	Initiates LCD module by writing relevant commands the clear the display ready for writing.
 *
 *	LCD_Init(void)		initilises LCD module
 *	LCD_Clear(void)		clears LCD module	
 */
void LCD_Init(void)
{
	_delay_ms(300);								//power up delay
	
	LCD_WCommand(0x38);							//function set command
	LCD_Clear();								//Function to clear LCD
	LCD_WCommand(0x06);							//character entry mode
	LCD_WCommand(0x0C);							//display on/off & cursor command
	
	return;
}

void LCD_Clear(void)
{
	LCD_WCommand(0x01);							//clear LCD command
	_delay_ms(10);								//wait for 3ms - precourtionary
	return;
}

/*
 *	Simple functions to write commands and data to the LCD module
 *	
 *	LCD_WCommand(char)	char is written to LCD module as a command
 *	LCD_WData(char)		char is written to LCD module as data
 *	LCD_WSData(char[])	char array is written to the LCD as data using LCD_WData()
 */
void LCD_WCommand(char command)
{	
	char LCD_CONTROLDIRB = LCD_CONTROLDIR, LCD_DATADIRB = LCD_DATADIR;
												//DDR back up variables
			
	LCD_DATADIR = 0xff;							//Setup of DDRs
	LCD_CONTROLDIR = LCD_CONTROLDIRB | ((LCD_EN|LCD_RW) | LCD_RS); 

	CLEARBIT(LCD_CONTROLBO,LCD_RW);				//clear bits
	CLEARBIT(LCD_CONTROLBO,LCD_RS);
	CLEARBIT(LCD_CONTROLBO,LCD_EN);
	 	
		
	LCD_DATABO = command;						//output command to datab
	_delay_us(10);

	SETBIT(LCD_CONTROLBO,LCD_EN);				//set enable bit
	_delay_us(10);

	CLEARBIT(LCD_CONTROLBO,LCD_EN);				//clear enable bits
	_delay_us(10);

	LCD_Wait();
	
	LCD_DATADIR = LCD_DATADIRB;					//Restore DDRs
	LCD_CONTROLDIR = LCD_CONTROLDIRB; 
	///////////////////////////////////////////////////////////////////////////////////
	//added the following delay to aid testing, should be able to remove!!
	    _delay_ms(10);
	return;
}
void LCD_WData(char data)
{	
	char LCD_CONTROLDIRB = LCD_CONTROLDIR, LCD_DATADIRB = LCD_DATADIR;	
												//ddr back up variables
		
	LCD_DATADIR = 0xff;							//Setup of DDRs
	LCD_CONTROLDIR = LCD_CONTROLDIRB | ((LCD_EN|LCD_RW) | LCD_RS);  
	
	CLEARBIT(LCD_CONTROLBO,LCD_RW);				//clear bits
	SETBIT(LCD_CONTROLBO,LCD_RS);
	CLEARBIT(LCD_CONTROLBO,LCD_EN);

	LCD_DATABO = data;							//output data to datab
	_delay_us(1);

	SETBIT(LCD_CONTROLBO,LCD_EN);				//set enable bit
	_delay_us(1);

	CLEARBIT(LCD_CONTROLBO,LCD_EN);				//clear enable bits
	_delay_us(1);

	LCD_Wait();

	LCD_DATADIR = LCD_DATADIRB;					//Setup of DDRs
	LCD_CONTROLDIR = LCD_CONTROLDIRB; 
	
	///////////////////////////////////////////////////////////////////////////////////
	//added the following delay to aid testing, should be able to remove!!
	    _delay_ms(10);
	return;
}

void LCD_WSData(char dataS[])			
{	
	int index = 0;								//do_while index
	
	do
	{	
		LCD_WData(dataS[index]);				//output data to datab
		index++;
	}
	while(index < strlen(dataS));				//do while index is  < string length

	return;
}

/*
 *	Busy wait functions that simply wait until the busy flag is clear
 *
 *	LCD_Wait(void)	simply waits until bit 7 of the data bus is clear, DDRs
 *	should be saved by the the function calling this fuction
 *
 */
void LCD_Wait(void)
{
	CLEARBIT(LCD_DATADIR,0x80);		//set DDRs
	CLEARBIT(LCD_CONTROLBO,LCD_RS);		//clear bits
	SETBIT(LCD_CONTROLBO,LCD_RW);		//set to read

	do
	{
		CLEARBIT(LCD_CONTROLBO,LCD_EN);
		SETBIT(LCD_CONTROLBO,LCD_EN);	//set enable bit
		
	}
	while(!CHECKBIT(LCD_DATABI,0x80));			//wait until device ready

	CLEARBIT(LCD_CONTROLBO,LCD_EN);
	CLEARBIT(LCD_CONTROLBO,LCD_RS);				//clear bits
	CLEARBIT(LCD_CONTROLBO,LCD_RW);

	SETBIT(LCD_DATADIR,0x80);
	
	return;
	
}



