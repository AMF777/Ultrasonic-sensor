 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the Ultrasonic driver
 *
 * Author: Ahmed Magdy
 *
 *******************************************************************************/
#include "ultrasonic.h"
#include "common_macros.h"
# include <util/delay.h>
/*******************************************************************************
 *                      Variables                                  *
 *******************************************************************************/
uint8 static g_edgeCount=0;
uint16 static g_echotime_High=0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description:
 * Initialize the ICU driver as required.
 * Setup the ICU call back function.
 *  Setup the direction for the trigger pin as output pin through the GPIO driver.
 */

void Ultrasonic_init(void){
	Icu_ConfigType config={F_CPU_8,RISING};
	Icu_init(&config);
	//Setup the ICU call back function.
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	//set direction for trigger pin
	//there is no need to set direction for echo pin as Icu_init() handles it
	GPIO_setupPinDirection(Ultrasonic_Trigger_Port, Ultrasonic_Trigger_Pin, PIN_OUTPUT);
	GPIO_writePin(Ultrasonic_Trigger_Port, Ultrasonic_Trigger_Pin, LOGIC_LOW);
 }
/*
 * Description:
 * Send the Trigger pulse to the ultrasonic.
 */
 void Ultrasonic_Trigger(void){
	 GPIO_writePin(Ultrasonic_Trigger_Port, Ultrasonic_Trigger_Pin, LOGIC_HIGH);
	 _delay_us(Ultrasonic_Trigger_Pulse_duration);
	 GPIO_writePin(Ultrasonic_Trigger_Port, Ultrasonic_Trigger_Pin, LOGIC_LOW);

 }
 /*
  * Description:
  * Send the trigger pulse by using Ultrasonic_Trigger function.
  * Start the measurements by the ICU from this moment.
  * Output:The measured distance in Centimeter.
  */

  uint16 Ultrasonic_readDistance(void){
	  Ultrasonic_Trigger();
	  /*
	   * Sound velocity = 340.00 m/s = 34000 cm/s
	   *The distance of Object (in cm) = 340000∗Time/2
 	 	 = 17000 * Time
	   *= 17000 x (TIMER value) x 1 x 10^-6 cm
		= 0.017 x (TIMER value) cm
		= (TIMER value) / 58.8 cm
	   *
	   */
	  return g_echotime_High*Ultrasonic_Distance_Constant;
	  /*
	   * when i tried pdf equation i found that the distance isn't accurate 2cm distance after trying some values
	   * i found that 57.5 is correct value to resolve error
	   */
	  //return g_echotime_High/57.5;
  }
  /*
   * Description:
   * This is the call back function called by the ICU driver.
   * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
   */
   void Ultrasonic_edgeProcessing(void){
	   g_edgeCount++;
	   if (g_edgeCount==1){
		   /*
		    * we start timer
		    */
		   Icu_clearTimerValue();
		   Icu_setEdgeDetectionType(FALLING);
	   }
	   if(g_edgeCount==2){
		   // store the high time value which represent timer value in the EQUATION
		   g_echotime_High=Icu_getInputCaptureValue();
		   Icu_setEdgeDetectionType(RISING);
		   //reset count
		   g_edgeCount=0;
	   }
   }