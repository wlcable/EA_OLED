/***************************************************************************
This is a library is written to send data to an EA OLED over softSPI
    -control displays with 2x8 to 4x20 characters.
    -MISO is not used because the OLED doesn't return any data
    
https://www.lcd-module.de/fileadmin/eng/pdf/doma/olede.pdf
 
Written by Bill Cable, 2019
 ***************************************************************************/
#ifndef EA_OLED_h
#define EA_OLED_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//OLED Commands
#define FONT 0x39 // western european char set
#define CLEAR 0x01 //clear the OLED display
#define HOME 0x02  //return to home position

//*******************************
//OLED Display on/off control
#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0xFB
#define CURSOR_ON 0x02
#define CURSOR_OFF 0xFD
#define BLINK_ON 0x01
#define BLINK_OFF 0xFE

//OLED  Lines
#define HOME_LINE1 0x80
#define LINE1 0x00
#define LINE2 LINE1+0x40
#define LINE3 LINE1+0x14
#define LINE4 LINE2+0x14

class EA_OLED {
    public:
        EA_OLED(uint8_t SS_Pin, uint8_t MOSI_Pin, uint8_t CLK_Pin); //constructor

        void begin(uint8_t lines, uint8_t width);   //Init the OLED
        void pos(uint8_t line, uint8_t offset);
        void clearLine(int8_t line);    //clears the line and sets cursor to begining of line
        void clear(void);       //clears the whole display and sets the cursor to home position
        void str(const char *str);
        void DefineCharacter(uint8_t postion, uint8_t *data);
        void cmd(uint8_t cmd);        

     private:
        uint8_t _ss_pin, _mosi_pin, _clk_pin, _lines, _width;
        void softSpiTransfer(uint16_t shOut);
        void dat(uint8_t data);
};
#endif