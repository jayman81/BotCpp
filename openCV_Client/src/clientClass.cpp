/*
 * clientClass.cpp
 *
 *  Created on: Jul 20, 2018
 *      Author: jb
 */


#include "../includes/clientClass.hpp"


void Client::setOptionOne(int value) {
	if ((value != 1) && (value != 0))
		std::cerr << "Impossible Option Value!" << std::endl;
	else if (value == 1)
		_optionOne = true;
	else
		_optionOne = false;
}

int Client::writeLogFile(std::string message, std::string value, int state) {
	std::ofstream pLogFile;
	std::stringstream logData;
	std::time_t now = std::time(NULL);
	std::tm *pTM = std::localtime(&now);
	char timestampBuffer[32];
	std::strftime(timestampBuffer,32,"%Y-%m-%d %H:%M:%S", pTM);
	std::string stateLevel;

	switch (state) {
	case DEBUG:
		stateLevel = "DEBUG";
		break;
	case INFO:
		stateLevel = "INFO";
		break;
	case WARNING:
		stateLevel = "WARNING";
		break;
	case ERROR:
		stateLevel = "ERROR";
		break;
	default:
		return -3;
	}

	if (value != "") {
		logData << timestampBuffer
				<< ": "
				<< stateLevel
				<< " - "
				<< message
				<< ": "
				<< value
				<< "\n";
	} else {
		logData << timestampBuffer
				<< ": "
				<< stateLevel
				<< " - "
				<< message
				<< "\n";
	}


	pLogFile.open (Client::logFileName, std::ios::app);
	pLogFile << logData.rdbuf();
	pLogFile.close();

	return 0;
}
