
#include <monoPro.h>

monoPro lcd;

void setup(){
    lcd.begin();
	lcd.clear();


}

void loop(){

    //Draw lines
	for (int i = 0;i <= 22;i++)
	{
		lcd.drawLine(0, 0, i * 6, 96);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<22;i++)
	{
		lcd.drawLine(i * 6, 67, 96, 0);
		lcd.drawDisplay();
		delay(200);
	}

	
	for (int i = 0;i <= 22;i++)
	{
		lcd.xdrawLine(0, 0, i * 6, 96);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<22;i++)
	{
		lcd.xdrawLine(i * 6, 67, 96, 0);
		lcd.drawDisplay();
		delay(200);
	}

    lcd.clear();
    delay(500);
    

}