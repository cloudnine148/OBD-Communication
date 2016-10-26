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
	CSerialComm serialComm; //SerialComm ��ü ����
	serialComm.disconnect();
	if (!serialComm.connect("COM4")) //COM24���� ��Ʈ�� �����Ѵ�. ������ ��� -1�� ��ȯ�Ѵ�.
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
	{ //���¿� ������ ��� sendCommand()�� ���� ��������� �����͸� �����Ѵ�. ���ۿ� ���� �� ��� failed �޽����� ����Ѵ�.
	  //getline(cin, buffer);
		serialComm.vehicleSpeed(speed);
		cout << "Current Speed : " << speed << "km/h" << endl;
		Sleep(70);
		//serialComm.vehicleSpeed(speed);
		//cout << "Current Vehicle Speed : " << speed << " km/h" << endl;
	}

	serialComm.disconnect(); //�۾��� ������ ��Ʈ�� �ݴ´�

	cout << "end connect" << endl;
	return 0;

}