 /******************************************************************************
 *
 * Module: Main file
 *
 * File Name: Main.c
 *
 * Description: source file for the project
 *
 * Author: Ahmed Magdy
 *
 *******************************************************************************/
# include "ultrasonic.h"
# include "lcd.h"
# include <util/delay.h>
# include <avr/interrupt.h>
 /*******************************************************************************/

int main(){

	/* Enable Global Interrupt I-Bit */
	SREG|=(1<<7);

	/* initialization */
	LCD_init();
	Ultrasonic_init();

	/* variable used to store value returned from ultrasonic */
	uint16 distance=0;

	LCD_displayString("Distance=     cm");

	while(1){

		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0, 10);
		LCD_intgerToString(distance);
		if(distance<100)
			LCD_displayCharacter(' ');
		/*
		 * delay between every trigger to make ultrasonic able to produce echo for every trigger
		 * if i don't make delay ,not all triggers produce echo
		 * (so i save power consumption) and benefit from every trigger
	     */
		_delay_ms(100);
	}

}
