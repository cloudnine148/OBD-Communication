//Available vehicleSpeed, engineRPM
#include <iostream>
#include <string>
#include "SerialComm.h"
#include <stdlib.h>
#include <windows.h>
using namespace std;

int main()
{
	int currentRPM = 0;
	int currentSpeed = 0;
	string response;
	CSerialComm serialComm; //SerialComm °´Ã¼ »ý¼º

	serialComm.disconnect();
	//Open COM4 port
	if (!serialComm.connect("COM4")) 
	{
		cout << "connect faliled" << endl;
		return -1;
	}
	else
		cout << "connect successed" << endl;

	//sending the "OBD2 initialize" command
	response = serialComm.sendGenCommand("AT Z");
	cout << response << endl;
	Sleep(300);
	//sending the "echo off" command
	response = serialComm.sendGenCommand("AT E0");
	cout << response << endl;
	//sending the "protocol auto scanning" command
	Sleep(300);
	response = serialComm.sendGenCommand("AT SP 0");
	cout << response << endl;
	Sleep(300);
	
	while (true)
	{ 
		serialComm.engineRPM(currentRPM);
		cout << "Current RPM : " << currentRPM <<" RPM"<< endl;
		serialComm.vehicleSpeed(currentSpeed);
		cout << "Current Speed : " << currentSpeed << "Km/h" << endl;
		Sleep(70);
	}

	return 0;
}