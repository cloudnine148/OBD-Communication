//Available vehicleSpeed, engineRPM
#pragma once
#pragma once
#include "SerialPort.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define RETURN_SUCCESS 1
#define RETURN_FAIL 0


class CSerialComm
{
public:
	CSerialComm();
	~CSerialComm();

	CSerialPort	serial;
	int		connect(char* _portNum);
	string sendGenCommand(string setCommand);
	void vehicleSpeed(int &speed);
	void engineRPM(int &rpm);
	void disconnect();
};