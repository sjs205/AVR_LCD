/*
 *			AVR_LCD.h
 *		Created on:		17/02/2008
 *		Created by:		Steven Swann - swannonline@googlemail.com
 *		Completion:		09/09/2008
 *		Device:			ATmega48/162
 *		
 *		Simple header file to impliment the LCD interface
 *		LCD_Init();				initialises the LCD Module
 *		LCD_Clear();			Clears the module
 *		LCD_WCommand(char);		Writes char to the LCD module as a command
 *		LCD_WData(char);		Writes char to the LCD module as data
 *		LCD_Wait();				Waits for the LCD modules busy flag to become low
 */


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/*
 *	Macro definitions
 */
#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x

/*
 *	LCD dimensions
 */
#if !defined LCD_Width
	#define LCD_Width 0x0f
#endif

#if !defined(LCD_Lines)
	#define LCD_Lines 0x02
#endif
/*
 *	data bus definitions
 */
#if !defined(LCD_DATABO)
	#define LCD_DATABO PORTB		//databus out
#endif
#if !defined(LCD_DATABI)
	#define LCD_DATABI PINB			//databus in
#endif
#if !defined(LCD_DATADIR)	
	#define LCD_DATADIR DDRB		//data direction registers
#endif
/*
 *	control bus definitions
 */
#if !defined(LCD_CONTROLBO)
	#define LCD_CONTROLBO PORTC		//CONTROL out
#endif
#if !defined(LCD_CONTROLBI)	
	#define LCD_CONTROLBI PINC		//CONTROL in
#endif
#if !defined(LCD_CONTROLDIR)
	#define LCD_CONTROLDIR DDRC		//data direction register
#endif
/*
 *	control bus setup
 */
#if !defined(LCD_RS)
	#define LCD_RS 0x01				//RS bit
#endif
#if !defined(LCD_RW)
	#define LCD_RW 0x02				//R/W bit
#endif
#if !defined(LCD_EN)
	#define LCD_EN 0x04				//enable bit
#endif


/*
 *
 *LCD INITIALISATION
 *
 *      INITIALISATION CODE:
 *              CHARACTER ENTRY MODE COMMAND
 *                      00000000
 *                      ||||||||
 *                      |||||||L------1=DISPLAY SHIFT ON, 0=OFF
 *                      ||||||L-------1=INCREMENT DISPLAY, 0=DECREMENT DISPLAY
 *                      |||||L--------1
 *                      ||||L---------0
 *                      |||L----------0
 *                      ||L-----------0
 *                      |L------------0
 *                      L-------------0
 *              DISPLAY ON/OFF & CURSOR COMMAND
 *                      00000000
 *                      ||||||||
 *                      |||||||L------1=CURSOR BLINK ON, 0=OFF
 *                      ||||||L-------1=CURSOR UNDERLINE ON, 0=OFF
 *                      |||||L--------1=DISPLAY ON, 0=OFF
 *                      ||||L---------1
 *                      |||L----------0
 *                      ||L-----------0
 *                      |L------------0
 *                      L-------------0
 *              DISPLAY/CURSOR SHIFT COMMAND
 *                      00000000
 *                      ||||||||
 *                      |||||||L------X
 *                      ||||||L-------X
 *                      |||||L--------1=RIGHT SHIFT, 0=LEFT SHIFT
 *                      ||||L---------1=DISPLAY SHIFT, 0=CURSOR SHIFT
 *                      |||L----------1
 *                      ||L-----------0
 *                      |L------------0
 *                      L-------------0
 *              FUNCTION SET COMMAND
 *                      00000000
 *                      ||||||||
 *                      |||||||L------X
 *                      ||||||L-------X
 *                      |||||L--------1=5x10 CHARACTER DOT FORMAT, 0=5x7 FORMAT
 *                      ||||L---------1=2 LINE MODE, 0=1 LINE MODE
 *                      |||L----------1=8-BIT INTERFACE, 0=4 BIT INTERFACE
 *                      ||L-----------1
 *                      |L------------0
 *                      L-------------0
 *                                      NB THIS CODE ONLY SUPPORTS THE 8-BIT INTERFACE
 *	
 *	Simply initilises the LCD module
 */
extern void LCD_Init(void);


/*
 *	Clears LCD display using the 0x01 command, note that this takes longer
 *	then simply writing blanks to screen.
 */

extern void LCD_Clear(void);

/*
 *	Write Routines
 *	Write data to the LCD data bus as a command or as data - (char or string)
 */
extern void LCD_WCommand(char command);
extern void LCD_WData(char data);
extern void LCD_WSData(char dataS[]);

/*
 *	Busy wait routines
 *	Waits until the busy flag is clear before allowing the program execution 
 *	to continue 
 */
extern void LCD_Wait(void);


