#include <monoPro.h>

monoPro lcd;

void setup(){
    lcd.begin();
	lcd.clear();


}

void loop(){
	for (int i = 0;i<16;i++)
	{
		lcd.drawCircle(48, 32, i * 3);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<16;i++)
	{
		lcd.xdrawCircle(48, 32, i * 3);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<30;i++)
	{
		lcd.fillCircle(random(96), random(64), random(10));
		lcd.drawDisplay();
		delay(200);
	}

	lcd.clear();

}