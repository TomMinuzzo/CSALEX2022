/*
 * LCD.cpp
 *
 *  Created on: 25Feb.,2020
 *      Author: Angus Wallace
 */

#include "LCD.h"


#define LCD_ADDR 0x20
#define LCD_COLS 20
#define LCD_ROWS 4
#define I2C_BUS 1 // NB: Bus 2 is on port 1 on PB
#define PIN1 9	  //NB: on port 2 (i2c bus 2) of PB
#define PIN2 11	  //NB: on port 2 (i2c bus 2) of PB

LCD::LCD()
{
	lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS, I2C_BUS);
	lcd->configure_i2c_pins(PIN1, PIN2);
	lcd->init();
	std::cout << "LCD object created" << std::endl;
}


LCD::~LCD()
{
	lcd->commControlOff();
	lcd->clear();
	lcd->noCursor();
	lcd->noBacklight();
	lcd->noDisplay();
	lcd->i2c_close(lcd->getHandle());
	delete lcd;
	std::cout << "LCD object deleted" << std::endl;
}

void LCD::printStr(std::string str)
{
	lcd->writeStr(str);
}

void LCD::printStr(std::string str, uint8_t col, uint8_t row)
{
	lcd->cursor();
	lcd->setCursor(col, row);
	printStr(str);
	lcd->noCursor();
}

void LCD::setup()
{
	lcd->backlight();
	printStr("CS:", 0, 0);
	sleep(1);
	printStr("NM:", 0, 1);
	sleep(1);

	lcd->commControlOn();
}

void LCD::printCurrState(std::string currState)
{
	std::cout << "LCD CS: " << currState << std::endl;
	clearCurrState();
	printStr(currState, 3, 0);
}

void LCD::printNextMove(std::string nextMove)
{
	std::cout << "LCD LEVEL NEXT MOVE:" << nextMove << std::endl;
	clearNextMove();
	printStr(nextMove, 3, 1);
}

void LCD::clearCurrState()
{
	lcd->cursor();
	lcd->setCursor(3, 0);
	for (auto i = 0; i < LCD_COLS; i++)
	{
		lcd->writeI2C(' ');
	}
	lcd->noCursor();
}

void LCD::clearNextMove()
{
	lcd->cursor();
	lcd->setCursor(3, 1);
	for (auto i = 0; i < LCD_COLS; i++)
	{
		lcd->writeI2C(' ');
	}
	lcd->noCursor();
}

void LCD::flash()
{
	lcd->noBacklight();
	// sleep(1);
	lcd->backlight();
}

void LCD::backlightOn(){
	lcd->backlight();
}

void LCD::backlightOff(){
	lcd->noBacklight();
}

void LCD::commControlOn()
{
	lcd->commControlOn();
}

void LCD::commControlOff()
{
	lcd->commControlOff();
}

int LCD::sendNextCommand()
{
	return lcd->sendNextCommand();
}

int LCD::isQueueEmpty()
{
	return lcd->isQueueEmpty();
}

void LCD::printStage(int stage){
	lcd->setCursor(15, 0);
	lcd->writeI2C(stage);
}
