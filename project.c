#include<avr/io.h>
#include"buzzer.h"
#include"lm35_sensor.h"
#include"lcd.h"
#include"leds.h"
#include"motor.h"
#include"flame.h"
#include"ldr.h"
#include"adc.h"

int main(void)
{
	uint16 light_intensity;
	uint8 temp;

	ADC_init();
	LCD_init();
	LEDS_init();
	DcMotor_Init();
	FlameSensor_init();
	Buzzer_init();

	light_intensity = LDR_getLightIntensity();
	temp = LM35_getTemperature();

	LCD_displayStringRowColumn(0,3,"FAN IS OFF");
	LCD_displayStringRowColumn(1,0,"TEMP=  C");
	LCD_moveCursor(1,6);
	LCD_intgerToString(temp);

	LCD_displayStringRowColumn(1,9,"LDR=  %");
	LCD_moveCursor(1,13);
	LCD_intgerToString(light_intensity);

	for(;;)
	{
		light_intensity = LDR_getLightIntensity();
		temp = LM35_getTemperature();

		if(light_intensity == 100)
		{
			LCD_moveCursor(1,13);
			LCD_intgerToString(light_intensity);
		}
		else
		{
			LCD_moveCursor(1,13);
			LCD_intgerToString(light_intensity);
			LCD_displayCharacter(' ');
		}

		if(light_intensity <= 15)
		{
			LED_on(RED);
			LED_on(GREEN);
			LED_on(BLUE);
		}
		else if(light_intensity > 15 && light_intensity <= 50)
		{
			LED_on(RED);
			LED_on(GREEN);
			LED_off(BLUE);
		}
		else if(light_intensity > 50 && light_intensity <= 70)
		{
			LED_on(RED);
			LED_off(GREEN);
			LED_off(BLUE);
		}
		else
		{
			LED_off(RED);
			LED_off(GREEN);
			LED_off(BLUE);
		}


		if(temp == 100)
		{
			LCD_moveCursor(1,6);
			LCD_intgerToString(temp);
		}
		else
		{
			LCD_moveCursor(1,6);
			LCD_intgerToString(temp);
			LCD_displayCharacter(' ');
		}

		LCD_moveCursor(0,7);
		if(temp >= 40)
		{
			LCD_displayString("ON         ");
			DcMotor_Rotate(CW,100);
		}
		else if(temp >= 35 && temp < 40)
		{
			LCD_displayString("ON         ");
			DcMotor_Rotate(CW,75);
		}
		else if(temp >= 30 && temp < 35)
		{
			LCD_displayString("ON        ");
			DcMotor_Rotate(CW,50);
		}
		else if(temp >= 25 && temp < 30)
		{
			LCD_displayString("ON        ");
			DcMotor_Rotate(CW,25);
		}
		else
		{
			LCD_displayString("OFF  ");
			DcMotor_Rotate(CW,0);
		}

		if(1 == FlameSensor_getValue())
		{
			Buzzer_on();
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_displayString(" CRITICAL ALERT!");
			for(;FlameSensor_getValue() == 1;)
			{

			}
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			Buzzer_off();

			LCD_displayStringRowColumn(0,3,"FAN IS OFF");
			LCD_displayStringRowColumn(1,0,"TEMP=  C");
			LCD_moveCursor(1,6);
			LCD_intgerToString(temp);

			LCD_displayStringRowColumn(1,9,"LDR=  %");
			LCD_moveCursor(1,13);
			LCD_intgerToString(light_intensity);
		}
	}

}
