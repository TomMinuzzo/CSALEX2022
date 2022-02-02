/*
 * LCD.h
 *
 *  Created on: 25Feb.,2020
 *      Author: Angus Wallace
 */

#ifndef LCD_H_DEFINED
#define LCD_H_DEFINED

#include <string>
#include <LiquidCrystal_I2C.h>
#include <map>
#include <inttypes.h>
//#include "Crutch.h"
#include <iostream>
#include <unistd.h>

class LCD
{
public:
	LCD();
	virtual ~LCD();
	void printStr(std::string str);
	void printStr(std::string str, uint8_t col, uint8_t row);
	void setup();

	void printStage(int stage);

	void printCurrState(std::string currState);
	void printNextMove(std::string nextMove);
	void clearCurrState();
	void clearNextMove();

	void flash();

	void backlightOn();
	void backlightOff();

	void commControlOn();
	void commControlOff();

	int sendNextCommand();
	int isQueueEmpty();


private:
	LiquidCrystal_I2C *lcd;
};

#endif