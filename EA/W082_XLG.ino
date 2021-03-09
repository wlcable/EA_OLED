#include <Arduino.h>      //The main-macros of c++


// Definitio of Portpins for Software SPI
#define SS_Pin   7
#define MOSI_Pin 6
#define CLK_Pin  5


void OLED_init(void);                         //Init OLED (2 LINES)
void OLED_str(char *str);                     //Output string at current cursor position
void OLED_pos (uint8_t line, uint8_t offset); //set cursor, home position = 1,1
void OLED_clear(void);                        //clear Display an return Home


void setup()
{
  OLED_init();
}

void loop()
{
   //Sample Screen: Show some texts on the EA W082-XLG
   OLED_clear();
   OLED_str(" Hello ");
   OLED_pos(2,2);
   OLED_str("World");
   delay(2000);
   OLED_clear();
   delay(1000);
   OLED_pos(1,4);
   OLED_str("EA");
   OLED_pos(2,1);
   OLED_str("W082-XLG");
   delay(2000);
}

/********************
 * void softSpiTransfer(uint16_t shOut)
 * Software SPI
 * Transfer 10 bit with Bit banging
 ********************/
void softSpiTransfer(uint16_t shOut)
{
  digitalWrite (SS_Pin,LOW);

  for(uint8_t i = 0; i < 10; i++)
  {
    digitalWrite (MOSI_Pin,( (shOut & 0x200)? 1 : 0));

    delayMicroseconds(1);   //warten damit Datensignale korrekt anliegen
    digitalWrite (CLK_Pin,1);
    delayMicroseconds(1);  //steigen Flanke einlesen
    digitalWrite (CLK_Pin,0);
    delayMicroseconds(1); //warten für nächsten Clockpuls

    shOut <<= 1;
  }
  digitalWrite (SS_Pin,HIGH);
}

/********************
 * void OLED_cmd(uint8_t cmd)
 * Command to display
 ********************/
void OLED_cmd(uint8_t cmd)
{
  softSpiTransfer((uint16_t)cmd);
}

/********************
 * void OLED_dat(uint8_t data)
 * Data to display
 ********************/
void OLED_dat(uint8_t data)
{
  softSpiTransfer(0x0200 + (uint16_t)data);
}

/********************
 * void OLED_init(void)
 * Initialize ELECTRONIC ASSEMBLY OLED Textseries EA Wxxx
 ********************/
void OLED_init(void)
{
  pinMode (SS_Pin,OUTPUT);
  pinMode (MOSI_Pin, OUTPUT);
  pinMode (CLK_Pin, OUTPUT);
  digitalWrite (SS_Pin,1);
  digitalWrite (CLK_Pin,0);

  OLED_cmd(0x39);  //Function set with font selection (european char set)
  OLED_cmd(0x08);
  OLED_cmd(0x06);
  OLED_cmd(0x17);
  OLED_cmd(0x01);
  delay(2); 
  OLED_cmd(0x02);
  delay(2); 
  OLED_cmd(0x0F);  //Display on, cursor on, blink on
}

/********************
 * void OLED_str(char *str)
 * Show string at current cursor position
 * The string needs to have a 0x00 at the end
 ********************/
void OLED_str(char *str)
{
  while(*str)
    OLED_dat(*str++);
}

/********************
 * void OLED_pos (uint8_t line, uint8_t offset)
 * Set cursor position
 * Home Position: line=1, offset=1
 ********************/
void OLED_pos (uint8_t line, uint8_t offset)
{
  uint8_t pos = 0;
  if(line == 2)
    pos = 0x40;
  else if(line == 3)
    pos = 0x14;
  else if (line == 4)
    pos = 0x54;

  pos += offset-1;
  
  OLED_cmd(0x80+pos);
}

/********************
 * void OLED_clear(void)
 * Clear dislpay
 * Clears whole display and sets cursor to home postion
 ********************/
void OLED_clear(void)
{
  OLED_cmd(0x01);
  delay(2); 
  OLED_cmd(0x02);
  delay(2);
}
