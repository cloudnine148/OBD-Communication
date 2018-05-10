#include <iostream>
#include <ctime>
#include <string>
#include "SerialComm.h"
#include <stdlib.h>
#include <windows.h>
#include <chrono>

//�ܼ�â ����� ��ġ
#define WINDOW_POSITION_RATIO_WIDTH 0.8
#define	WINDOW_POSITION_RATIO_HEIGHT 0.3 

using namespace std;

CSerialComm serialComm; //SerialComm ��ü ����
time_t curr_time;
tm *curr_tm;
FILE *fp1;

void getCurrentTime(int speed);
void writeData(int curYear, int curMonth, int curDay, int curHour, int curMin, int curSec, int speed);
void intializeConsoleWindow();
void CtrlHandler(DWORD fdwCtrlType);
int main()
{
	//�ܼ����α׷� �ʱ� ���� 
	//intializeConsoleWindow();

	int currentRPM = 0;
	int currentSpeed = 0;
	string response;

	//fopen_s(&fp1,"data.txt", "a");
	

	//Open COM5 port OBD Dongle
	if (!serialComm.connect("COM3")) 
	{
		cout << "connect faliled" << endl;
		Sleep(2000);
		return -1;
	}
	else
		cout << "connect successed" << endl;
	
	//get vehicle Speed	
	double volt = 0.0;
	while (true)
	{ 
#ifdef TIME_CHECK
		chrono::system_clock::time_point start = chrono::system_clock::now();
		//CTRL+C�̺�Ʈ�� �߻��ϸ� �����ͱ��, OBD ���� ��� ����
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
		serialComm.engineRPM(currentRPM);
		serialComm.vehicleSpeed(currentSpeed);
		cout << "RPM : " << currentRPM << "\t Speed : " << currentSpeed << "km/h" << endl;
		//getCurrentTime(currentSpeed);
		//1�ʿ� 10�� �ӵ� ����
		Sleep(100);
		//chrono::duration<double> sec = chrono::system_clock::now() - start;
		//cout << "Processing time : " << sec.count() << " s" << endl;
#elif
		//CTRL+C�̺�Ʈ�� �߻��ϸ� �����ͱ��, OBD ���� ��� ����
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
		//serialComm.engineRPM(currentRPM);
		//cout << "Current RPM : " << currentRPM <<" RPM"<< endl;
		serialComm.vehicleSpeed(currentSpeed);
		getCurrentTime(currentSpeed);
		//1�ʿ� 10�� �ӵ� ����
		Sleep(100);
#endif
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
	cout << speed << " km/h" << endl;
	writeData(curYear, curMonth, curDay, curHour, curMin, curSec,speed);
}

void writeData(int curYear, int curMonth, int curDay, int curHour, int curMin, int curSec, int speed)
{
	fprintf_s(fp1, "%d %d %d %d %d %d %d\n", curYear, curMonth, curDay, curHour, curMin, curSec,speed);
}

void intializeConsoleWindow()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hwnd = GetConsoleWindow();
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	CONSOLE_FONT_INFOEX fontInfo;

	//Ŀ�� �����
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	//���� ǥ���� �����
	style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE	| SWP_FRAMECHANGED);
	//�ܼ� ��� ������
	system("color 00");

	//��� �ؽ�Ʈ ��(�����)
	SetConsoleTextAttribute(hConsole, 14);
	//��� �ؽ�Ʈ ũ��
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = 0;
	fontInfo.dwFontSize.Y = 24;
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
	//âũ�� �ּ�ȭ / ����2, ����11
	system("mode con: lines=2 cols=11");

	//�ܼ����α׷��� ��� ��ġ
	//����� �ػ� ũ�� �ε�
	int resWidth = GetSystemMetrics(SM_CXSCREEN);
	int resHeight = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(hwnd, NULL, resWidth*WINDOW_POSITION_RATIO_WIDTH, resHeight*WINDOW_POSITION_RATIO_WIDTH,10,20,SWP_NOSIZE);
	}
void CtrlHandler(DWORD fdwCtrlType)
{
	if (fdwCtrlType == CTRL_C_EVENT)
	{
		serialComm.disconnect();
		system("exit");
	}
}