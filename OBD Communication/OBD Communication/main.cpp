#include <iostream>
#include <ctime>
#include <string>
#include "SerialComm.h"
#include <stdlib.h>
#include <windows.h>
using namespace std;
time_t curr_time;
tm *curr_tm;
FILE *fp1;
void getCurrentTime(int speed);
void writeData(int curYear, int curMonth, int curDay, int curHour, int curMin, int curSec, int speed);

int main()
{
	int currentRPM = 0;
	int currentSpeed = 0;
	string response;
	CSerialComm serialComm; //SerialComm 按眉 积己
	fopen_s(&fp1, "data.txt", "a");
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
		//serialComm.engineRPM(currentRPM);
		//cout << "Current RPM : " << currentRPM <<" RPM"<< endl;
		serialComm.vehicleSpeed(currentSpeed);
		getCurrentTime(currentSpeed);
		Sleep(70);
		system("cls");
	}
	return 0;
}

void getCurrentTime(int speed)
{
	curr_time = time(NULL);
	curr_tm = localtime(&curr_time);
	int curYear = curr_tm->tm_year + 1900;
	int curMonth = curr_tm->tm_mon + 1;
	int curDay = curr_tm->tm_mday;
	int curHour = curr_tm->tm_hour;
	int curMin = curr_tm->tm_min;
	int curSec = curr_tm->tm_sec;
	cout << curYear << "斥" << curMonth << "岿" << curDay << "老\t" << curHour << " : " << curMin << " : " << curSec<<"\t"<<speed<<"km/h";
	writeData(curYear, curMonth, curDay, curHour, curMin, curSec,speed);
}

void writeData(int curYear, int curMonth, int curDay, int curHour, int curMin, int curSec, int speed)
{
	fprintf_s(fp1, "%d斥%d岿%d老  %d : %d : %d\t %dkm/h\n", curYear, curMonth, curDay, curHour, curMin, curSec,speed);
}

