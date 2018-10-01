#include <monoPro.h>

monoPro lcd;


void setup() {
lcd.begin();
lcd.clear();

lcd.setCurser(0,0);
lcd.fontSize(1);
lcd.print("font 1");
delay(1000);
lcd.drawDisplay();

lcd.setCurser(10,10);
lcd.fontSize(2);
lcd.print("font 2");
delay(1000);
lcd.drawDisplay();

lcd.setCurser(0,35);
lcd.fontSize(3);
lcd.print("font3");

delay(1000);
lcd.drawDisplay();


}


void loop() {
}