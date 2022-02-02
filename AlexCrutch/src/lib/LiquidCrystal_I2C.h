//DFRobot.com
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <inttypes.h>
#include <string>
#include <queue>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100 // Enable bit
#define Rw 0b00000010 // Read/Write bit
#define Rs 0b00000001 // Register select bit

// flags for rt control
#define SLEEP 0xff // NB: any 0xff sent will be treated as a skip/sleep if commControl is on

class LiquidCrystal_I2C
{
public:
  LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t i2c_bus);
  void configure_i2c_pins(char p1, char p2); // NB: sometimes this doesnt work and you'll have to configure manually
  int i2c_open(unsigned char bus, unsigned char addr);
  int i2c_close(int handle);
  int i2c_write_byte(int handle, unsigned char val);
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  void clear();
  void home();
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void printLeft();
  void printRight();
  void leftToRight();
  void rightToLeft();
  void shiftIncrement();
  void shiftDecrement();
  void noBacklight();
  void backlight();
  void autoscroll();
  void noAutoscroll();
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t col, uint8_t row);
  virtual void writeI2C(uint8_t);
  void writeStr(std::string str);
  void commControlOn();
  void commControlOff();
  int sendNextCommand(); // sends next command from queue

  void command(uint8_t);
  void init();
  int getHandle();
  int isQueueEmpty(); // returns 1 if queue is empty, 0 if it isn't
  int getCommControl();

  ////compatibility API function aliases
  void blink_on();                                             // alias for blink()
  void blink_off();                                            // alias for noBlink()
  void cursor_on();                                            // alias for cursor()
  void cursor_off();                                           // alias for noCursor()
  void setBacklight(uint8_t new_val);                          // alias for backlight() and nobacklight()
  void load_custom_character(uint8_t char_num, uint8_t *rows); // alias for createChar()

  ////Unsupported API functions (not implemented in this library)
  uint8_t status();
  void setContrast(uint8_t new_val);
  uint8_t keypad();
  void setDelay(int, int);
  void on();
  void off();
  uint8_t init_bargraph(uint8_t graphtype);
  void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
  void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
  //made public for compulation
  int _commControl;
  int _handle;
  std::queue<uint8_t> commandQueue; // gives control over when each command is sent, only used when _commControl == 1

private:
  void init_priv();
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);
  uint8_t _Addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightval;
  uint8_t _i2c_bus;
};

#endif
