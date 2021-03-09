/***************************************************************************
This is a library is written to send data to an EA OLED over softSPI
    -control displays with 2x8 to 4x20 characters.
    -MISO is not used because the OLED doesn't return any data
    
https://www.lcd-module.de/fileadmin/eng/pdf/doma/olede.pdf
 
Written by Bill Cable, 2019
 ***************************************************************************/

#include "Arduino.h"
#include "EA_OLED.h"


//EA_OLED constructor
// set the pins for communication
EA_OLED::EA_OLED(uint8_t SS_Pin, uint8_t MOSI_Pin, uint8_t CLK_Pin) {
  _ss_pin = SS_Pin;
  _mosi_pin = MOSI_Pin;
  _clk_pin = CLK_Pin;
}

/********************
   void begin(void)
   Initialize ELECTRONIC ASSEMBLY OLED Textseries EA Wxxx
 ********************/
void EA_OLED::begin(uint8_t lines, uint8_t width)
{
  _lines = lines;
  _width = width;
  pinMode (_ss_pin, OUTPUT);
  pinMode (_mosi_pin, OUTPUT);
  pinMode (_clk_pin, OUTPUT);
  digitalWrite (_ss_pin, 1);
  digitalWrite (_clk_pin, 0);

  cmd(FONT);  //Function set with font selection (european char set)
  cmd(0x08);  //Display Off, all control bits to zero
  cmd(0x06);  //entry mode set increment cursor by 1 not shifting display
  cmd(0x17);  //Character mode and internel power on (have to turn on internel power to get the best brightness)
  clear();
  cmd(0x0C);  //Display On, no cursor, no blink
}

/********************
   void clearLine(int8_t line)
   clears the line and sets the cursor at the beging of the line
 ********************/
void EA_OLED::clearLine(int8_t line) { //clears a line and sets the cursor back at the begining for that line
  char buff[_width+1];
  memset(buff, ' ',_width);
  buff[_width] = '\0';
  if (line>_lines) line = _lines;
  pos(line, 1);
  str(buff);
  pos(line, 1);
}

/********************
   void clear(void)
   Clear dislpay
   Clears whole display and sets cursor to home postion
 ********************/
void EA_OLED::clear(void)
{
  cmd(CLEAR);
  delay(2);
  cmd(HOME);
  delay(2);
}

/********************
   void str(char *str)
   Show string at current cursor position
   The string needs to have a 0x00 at the end
 ********************/
void EA_OLED::str(const char *str)
{
  while (*str)
    dat(*str++);
}

/********************
   void pos (uint8_t line, uint8_t offset)
   Set cursor position
   Home Position: line=1, offset=1
 ********************/
void EA_OLED::pos(uint8_t line, uint8_t offset)
{
  if (line>_lines) line = _lines;
  if (offset>_width) offset = _width;
  switch (line)
  {
  case 1:
    cmd(0x80 + LINE1 + (offset-1));
    break;
  case 2:
    cmd(0x80 + LINE2 + (offset-1));
    break;
  case 3:
    cmd(0x80 + LINE3 + (offset-1));
    break;
  case 4:
    cmd(0x80 + LINE4 + (offset-1));
    break;
  }
}

//-----------------------------------------------------
//Func: DefineCharacter(memory postion, character data)
//Desc: stores an own defined character
//-----------------------------------------------------
void EA_OLED::DefineCharacter(uint8_t postion, uint8_t *data)
{
	uint8_t i=0;
	cmd(0x40+8*postion);

	for (i=0; i<8; i++)
	{
		dat(data[i]);
	}
	//SetPostion(LINE1);
}
/********************
 * PRIVATE FUNCTIONS
 * ******************/

/********************
   void softSpiTransfer(uint16_t shOut)
   Software SPI
   Transfer 10 bit with Bit banging
 ********************/
void EA_OLED::softSpiTransfer(uint16_t shOut)
{
  digitalWrite (_ss_pin, LOW);

  for (uint8_t i = 0; i < 10; i++)
  {
    digitalWrite (_mosi_pin, ( (shOut & 0x200)? 1 : 0));

    delayMicroseconds(1);   //warten damit Datensignale korrekt anliegen
    digitalWrite (_clk_pin, 1);
    delayMicroseconds(1);  //steigen Flanke einlesen
    digitalWrite (_clk_pin, 0);
    delayMicroseconds(1); //warten für nächsten Clockpuls

    shOut <<= 1;
  }
  digitalWrite (_ss_pin, HIGH);
}

/********************
   void cmd(uint8_t cmd)
   Command to display
 ********************/
void EA_OLED::cmd(uint8_t cmd)
{
  softSpiTransfer((uint16_t)cmd);
}

/********************
   void dat(uint8_t data)
   Data to display
 ********************/
void EA_OLED::dat(uint8_t data)
{
  softSpiTransfer(0x0200 + (uint16_t)data);
}