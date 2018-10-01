#include <monoPro.h>

monoPro lcd;

void setup(){
    lcd.begin();
	lcd.clear();


}

void loop(){
	for (int i = 0;i<60;i += 6)
	{
		lcd.drawRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<60;i += 6)
	{
		lcd.xdrawRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}


	for (int i = 0;i<60;i += 6)
	{
		lcd.fillRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<30;i += 6)
	{
		lcd.xfillRect(48 - i / 2, 32 - i / 2, i, i);
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