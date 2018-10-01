#include <monoPro.h>

monoPro lcd;

void setup(){
    lcd.begin();
	lcd.clear();


}

void loop(){
	for (int i = 5;i<60;i += 6)
	{
		lcd.drawRoundRect(48 - i / 2, 32 - i / 2, i, i,5);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 5;i<60;i += 6)
	{
		lcd.xdrawRoundRect(48 - i / 2, 32 - i / 2, i, i,5);
		lcd.drawDisplay();
		delay(200);
	}


	for (int i = 5;i<60;i += 6)
	{
		lcd.fillRoundRect(48 - i / 2, 32 - i / 2, i, i,5);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 5;i<30;i += 6)
	{
		lcd.xfillRoundRect(48 - i / 2, 32 - i / 2, i, i,5);
		lcd.drawDisplay();
		delay(200);
	}

	lcd.clear();

	for (int i = 0;i<30;i++)
	{
		lcd.drawPixel(random(96), random(64));
		lcd.drawDisplay();
		delay(200);
	}

	lcd.clear();

}