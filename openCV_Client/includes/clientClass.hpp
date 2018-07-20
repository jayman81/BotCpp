/*
 * clientClass.hpp
 *
 *  Created on: Jul 20, 2018
 *      Author: jb
 */

#pragma once

#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

enum messageState {
	DEBUG = 0, INFO, WARNING, ERROR
};

enum OptionFlag {
	BOT = 0, SENTRY, FTL, BC
};



class Client {
public:

	void setOptionOne(int value);

	int getOptionOne() {
		return _optionOne;
	}

	int writeLogFile(std::string, std::string, int);
	std::string logMessage;

private:
	bool _optionOne = false;
	bool _optionTwo = false;
	bool _optionThree = false;
	bool _optionFour = false;


	std::string logFileName = "logFile.txt";

};


