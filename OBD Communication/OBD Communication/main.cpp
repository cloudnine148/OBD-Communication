#include <iostream>
#include <string>
#include "SerialComm.h"
#include <stdlib.h>
#include <windows.h>
using namespace std;

int main()
{
	int rpm = 0;
	int speed = 0;
	string buffer;
	string response;
	CSerialComm serialComm; //SerialComm 객체 생성
	serialComm.disconnect();
	if (!serialComm.connect("COM4")) //COM24번의 포트를 오픈한다. 실패할 경우 -1을 반환한다.
	{
		cout << "connect faliled" << endl;
		return -1;
	}
	else
		cout << "connect successed" << endl;

	response = serialComm.sendGenCommand("AT Z");
	cout << response << endl;
	Sleep(300);
	//sending the "echo off" command
	response = serialComm.sendGenCommand("AT E0");
	cout << response << endl;
	//sending the "protocol auto scan" command
	Sleep(300);
	response = serialComm.sendGenCommand("AT SP 0");
	cout << response << endl;
	Sleep(300);
	while (true)
	{ //오픈에 성공한 경우 sendCommand()를 통해 계속적으로 데이터를 전송한다. 전송에 실패 할 경우 failed 메시지를 출력한다.
	  //getline(cin, buffer);
		serialComm.vehicleSpeed(speed);
		cout << "Current Speed : " << speed << "km/h" << endl;
		Sleep(70);
		//serialComm.vehicleSpeed(speed);
		//cout << "Current Vehicle Speed : " << speed << " km/h" << endl;
	}

	serialComm.disconnect(); //작업이 끝나면 포트를 닫는다

	cout << "end connect" << endl;
	return 0;

}