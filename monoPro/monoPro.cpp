/*
  Written by Mohammed Rezwanul Islam
*/

#include <Arduino.h>
#include "monoPro.h"



monoPro::monoPro(void)

{
   //set pin directions
  pinMode(_CSPin, OUTPUT);
  pinMode(_SDAPin, OUTPUT);
  pinMode(_SCKPin, OUTPUT);

  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
 
  //disable monoPro to start with 
  digitalWrite(_CSPin, HIGH);
  digitalWrite(_SDAPin, LOW);
  digitalWrite(_SCKPin, LOW);

  

 }
 

void monoPro::backlight(byte bklt)
{
analogWrite(6, bklt);
}
 
void monoPro::contrast(byte con)
{
  if(con<0) {con=0;}
  if(con>32) {con=32;}

con = con | 0x80;
monoPro::command(con);  
}


void monoPro::displayAllPointsOn()
{
  monoPro::command(0xA5);
}


void monoPro::displayAllPointsOff()
{
  monoPro::command(0xA4);
}


void monoPro::softwareReset()
{
  monoPro::command(0xE2);  //reset
}


 
void monoPro::line(byte line)
{
  if (line<0){line = 0;}
  if (line>8){line = 8;}
  
  line = 176+line;
  
  monoPro::command(line);  //set line
/*
  monoPro::command(0x10);  //set col=0 upper 3 bits
  monoPro::command(0x00);  //set col=0 lower 4 bits
*/
  monoPro::column(0x00);
}

 void monoPro::column(byte payload)
{
if(payload<0){payload=0;}
if(payload>95){payload=95;}

byte lower = 0x0F;
byte upper = 0x70;

lower = payload & 0x0F;
upper =payload &0x70;

upper = upper>>4;
 
upper = 0x10 | upper;
 
monoPro::command(upper);
monoPro::command(lower);
}

void monoPro::scroll(byte payload)
{
/*  payload = payload+64;
  if (payload >127){payload = 127;}
 */
  payload =64+ payload;
  
  if (payload <0){payload = 64;}
  if (payload >127){payload = 127;}
  monoPro::command(payload);
}

void monoPro::displayOff()
{
  monoPro::command(0xAE);
}

void monoPro::displayOn()
{
  monoPro::command(0xAF);
}

void monoPro::displayNormal()
{
  monoPro::command(0xA6);
}

void monoPro::displayInverse()
{
  monoPro::command(0xA7);
}

 void monoPro::println(char *characters)
{
  monoPro::command(0x10);  //set column=0 upper 3 bits
  monoPro::command(0x00);  //set column=0 lower 4 bits

  while (*characters)
  {
    monoPro::character(*characters++);
  }
}


 
 void monoPro::print(char *characters)
{
  while (*characters)
  {
    monoPro::character(*characters++);
  }
}

 void monoPro::print(int num)
{
char c[8];
itoa(num,c,10);
monoPro::print(c);
}

void monoPro::println(int num)
{
char c[8];
itoa(num,c,10);
monoPro::println(c);
}





void monoPro::clear()
{
  
  //display off
  monoPro::command(0xAE);
  
  for (int i = 0; i < 864; i++)
  {
    monoPro::data(0x00);  
  }

	memset(displayRAM, 0, 864);

  monoPro::command(0xB0);  //set page address
  monoPro::command(0x10);  //set col=0 upper 3 bits
  monoPro::command(0x00);  //set col=0 lower 4 bits

  monoPro::command(0x40);  //set row 0

  //display on
  monoPro::command(0xAF);

}

void monoPro::command(byte payload)
{
  
  //select monoPro
  //  digitalWrite(_CSPin, LOW);
  cbi(PORTD,0x07);

  //write data/command bit
  //0=command & 1= data 
  cbi(PORTC,0x00);

  //digitalWrite(_SCKPin, HIGH);
  sbi(PORTC,0x01);
  //digitalWrite(_SCKPin, LOW);
  cbi(PORTC,0x01);

  //write payload 8 bits
  for(int i=7;i>=0;i--)
  {
      if(((payload>>i)&1) ==1)
      {sbi(PORTC,0x00);}
      else
      {cbi(PORTC,0x00);}
      //toggle clock
    sbi(PORTC,0x01);
    cbi(PORTC,0x01);
  }

//  digitalWrite(_CSPin, HIGH);
  sbi(PORTD,0x07);
}


void monoPro::begin()
{
  monoPro::command(0xE2);  //reset
  delay(10);
  monoPro::command(0xA4);  //power save off
  monoPro::command(0x2F);  //power control set
  
  monoPro::command(0xB0);  //set page address
  monoPro::command(0xA0);  //normal operation
  monoPro::command(0x10);  //set col=0 upper 3 bits
  monoPro::command(0x00);  //set col=0 lower 4 bits


  monoPro::contrast(0x0E);  //set contrast
  
  monoPro::command(0xAF);  //monoPro display on
  delay(500);
}


void monoPro::data(byte payload)
{
  //select monoPro
  //  digitalWrite(_CSPin, LOW);
  cbi(PORTD,0x07);

  //write data/command bit
  //0=command & 1= data 
  sbi(PORTC,0x00);

  //digitalWrite(_SCKPin, HIGH);
  sbi(PORTC,0x01);
  //digitalWrite(_SCKPin, LOW);
  cbi(PORTC,0x01);

  //write payload 8 bits
  for(int i=7;i>=0;i--)
  {
      if((payload>>i)&1 ==1)
      {sbi(PORTC,0x00);}
      else
      {cbi(PORTC,0x00);}
      //toggle clock
    sbi(PORTC,0x01);
    cbi(PORTC,0x01);
  }
}



void monoPro::drawPixel(int x, int y)
{
	if (x > 95) { x = 95; }
	if (y > 67) { x = 67; }
	int   row;
	int  modRow;
	row = y / 8;

	unsigned char buffer = monoPro::displayRAM[x + 96 * row];

	modRow = y % 8;

	byte pixel = 1 << modRow;
	displayRAM[x + 96 * row] = buffer | pixel;

}

void monoPro::clearPixel(int x, int y)
{
	int   row;
	int  modRow;

	row = y / 8;

	unsigned char buffer = monoPro::displayRAM[x + 96 * row];

	modRow = y % 8;

	byte pixel = ~(1 << modRow);

	//  number &= ~(1 << x);
	displayRAM[x + 96 * row] = buffer & pixel;

}

void monoPro::drawDisplay()
{
  //bk
  line(0);
  column(0);
	for (int index = 0; index < 96 * 9; index++)
	{
		{data(monoPro::displayRAM[index]);}
	}
}

void monoPro::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0);
		}
		else {
			drawPixel(x0, y0);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void monoPro::xdrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			clearPixel(y0, x0);
		}
		else {
			clearPixel(x0, y0);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}


void monoPro::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	for (int16_t i = x; i<x + w; i++) {
		drawLine(i, y,i,(y+h-1));
    
	}
}

void monoPro::xfillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	for (int16_t i = x; i<x + w; i++) {
		xdrawLine(i, y,i,(y+h-1));
	}
}


void monoPro::fontSize(uint8_t x)
{
	monoPro::_fontSize = x;
}

void monoPro::setCurser(uint8_t x, uint8_t y)
{
	monoPro::_x_pos = x;
	monoPro::_y_pos = y;
}

void monoPro::drawFontPixel(uint8_t x, uint8_t y, uint8_t pixelSize)
{
	monoPro::fillRect(x, y, pixelSize, pixelSize);
}

void monoPro::xdrawFontPixel(uint8_t x, uint8_t y, uint8_t pixelSize)
{
	monoPro::xfillRect(x, y, pixelSize, pixelSize);

}

void monoPro::character(char character)
{
	if (_x_pos < _fontSize) { _x_pos = _fontSize; }


	for (uint8_t i = 0;i<5;i++)
	{
		for (uint8_t j = 0;j<8;j++)
		{
			byte myByte = 0;
			myByte = pgm_read_byte(&(ASCII[(byte)character - 0x20][i]));
			//myByte = (ASCII[(byte)character - 0x20][i]);

			if (((myByte >> j) & 0b00000001) == 0b00000001)

				//  if((pgm_read_byte_near(ASCII[character][i]  >> j)  & 0b00000001) == 0b00000001)
			{
				//drawPixel(_x_pos, _y_pos);
				drawFontPixel(_x_pos, _y_pos, _fontSize);
			}
			else
			{//draw back color
			 //       clearPixel(_x_pos, _y_pos);
				xdrawFontPixel(_x_pos, _y_pos, _fontSize);
			}

			_y_pos = _y_pos + _fontSize;
		}
		_y_pos = _y_pos - _fontSize * 8;
		_x_pos = _x_pos + _fontSize;

	}
	_x_pos = _x_pos + _fontSize;

	if (_x_pos > _LCD_x)
	{
		_x_pos = 0;
		_y_pos = _y_pos + _fontSize * 8;
	}


}

void monoPro::drawCircle(int16_t x0, int16_t y0, int16_t r) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r);
	drawPixel(x0, y0 - r);
	drawPixel(x0 + r, y0);
	drawPixel(x0 - r, y0);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y);
		drawPixel(x0 - x, y0 + y);
		drawPixel(x0 + x, y0 - y);
		drawPixel(x0 - x, y0 - y);
		drawPixel(x0 + y, y0 + x);
		drawPixel(x0 - y, y0 + x);
		drawPixel(x0 + y, y0 - x);
		drawPixel(x0 - y, y0 - x);
	}
}

void monoPro::xdrawCircle(int16_t x0, int16_t y0, int16_t r) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	clearPixel(x0, y0 + r);
	clearPixel(x0, y0 - r);
	clearPixel(x0 + r, y0);
	clearPixel(x0 - r, y0);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		clearPixel(x0 + x, y0 + y);
		clearPixel(x0 - x, y0 + y);
		clearPixel(x0 + x, y0 - y);
		clearPixel(x0 - x, y0 - y);
		clearPixel(x0 + y, y0 + x);
		clearPixel(x0 - y, y0 + x);
		clearPixel(x0 + y, y0 - x);
		clearPixel(x0 - y, y0 - x);
	}
}

void monoPro::drawCircleHelper(int16_t x0, int16_t y0,
	int16_t r, uint8_t cornername) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y);
			drawPixel(x0 + y, y0 + x);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y);
			drawPixel(x0 + y, y0 - x);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x);
			drawPixel(x0 - x, y0 + y);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x);
			drawPixel(x0 - x, y0 - y);
		}
	}
}

void monoPro::xdrawCircleHelper(int16_t x0, int16_t y0,
	int16_t r, uint8_t cornername) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			clearPixel(x0 + x, y0 + y);
			clearPixel(x0 + y, y0 + x);
		}
		if (cornername & 0x2) {
			clearPixel(x0 + x, y0 - y);
			clearPixel(x0 + y, y0 - x);
		}
		if (cornername & 0x8) {
			clearPixel(x0 - y, y0 + x);
			clearPixel(x0 - x, y0 + y);
		}
		if (cornername & 0x1) {
			clearPixel(x0 - y, y0 - x);
			clearPixel(x0 - x, y0 - y);
		}
	}
}

void  monoPro::drawFastHLine(int16_t x, int16_t y, int16_t w) {
	drawLine(x, y, x + w - 1, y);
}

void  monoPro::xdrawFastHLine(int16_t x, int16_t y, int16_t w) {
	xdrawLine(x, y, x + w - 1, y);
}

void monoPro::drawFastVLine(int16_t x, int16_t y, int16_t h) {
	drawLine(x, y, x, y + h - 1);
}
void monoPro::xdrawFastVLine(int16_t x, int16_t y, int16_t h) {
	xdrawLine(x, y, x, y + h - 1);
}

void  monoPro::drawRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	drawFastHLine(x, y, w);
	drawFastHLine(x, y + h - 1, w);
	drawFastVLine(x, y, h);
	drawFastVLine(x + w - 1, y, h);
}

void  monoPro::xdrawRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	xdrawFastHLine(x, y, w);
	xdrawFastHLine(x, y + h - 1, w);
	xdrawFastVLine(x, y, h);
	xdrawFastVLine(x + w - 1, y, h);
}

void  monoPro::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta)
{

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta);
		}
	}
}

void  monoPro::xfillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta)
{

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			xdrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta);
			xdrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta);
		}
		if (cornername & 0x2) {
			xdrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta);
			xdrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta);
		}
	}
}

void  monoPro::fillCircle(int16_t x0, int16_t y0, int16_t r)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1);
	fillCircleHelper(x0, y0, r, 3, 0);
}

void  monoPro::xfillCircle(int16_t x0, int16_t y0, int16_t r)
{
	xdrawFastVLine(x0, y0 - r, 2 * r + 1);
	xfillCircleHelper(x0, y0, r, 3, 0);
}

void  monoPro::fillRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	fillRect(x + r, y, w - 2 * r, h);

	// draw four corners
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1);
}

void  monoPro::xfillRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	xfillRect(x + r, y, w - 2 * r, h);

	// draw four corners
	xfillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
	xfillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1);
}

void  monoPro::drawRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	drawFastHLine(x + r, y, w - 2 * r); // Top
	drawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
	drawFastVLine(x, y + r, h - 2 * r); // Left
	drawFastVLine(x + w - 1, y + r, h - 2 * r); // Right
  // draw four corners
	drawCircleHelper(x + r, y + r, r, 1);
	drawCircleHelper(x + w - r - 1, y + r, r, 2);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
	drawCircleHelper(x + r, y + h - r - 1, r, 8);
}

void  monoPro::xdrawRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	xdrawFastHLine(x + r, y, w - 2 * r); // Top
	xdrawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
	xdrawFastVLine(x, y + r, h - 2 * r); // Left
	xdrawFastVLine(x + w - 1, y + r, h - 2 * r); // Right
	// draw four corners
	xdrawCircleHelper(x + r, y + r, r, 1);
	xdrawCircleHelper(x + w - r - 1, y + r, r, 2);
	xdrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
	xdrawCircleHelper(x + r, y + h - r - 1, r, 8);
}



