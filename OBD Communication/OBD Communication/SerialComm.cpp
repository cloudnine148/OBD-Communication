#include "SerialComm.h"
CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}

int CSerialComm::connect(char* portNum)
{
	string response = "";
	if (!serial.OpenPort(portNum)) //포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_38400, 8, FALSE, NOPARITY, ONESTOPBIT); //포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(10, 10, 10, 10, 10); //Timeout값 설정

	response = sendGenCommand("0100");		// just to make sure buffer is clear
	cout << "response : " << response << endl;
	Sleep(100);
	response = sendGenCommand("ATZ");		// OBD reset
	cout << "response : " << response << endl;
	Sleep(100);
	response = sendGenCommand("ATE0");		// Disable echo
	cout << "response : " << response << endl;
	Sleep(100);
	response = sendGenCommand("ATL0");		// Disable line feeds
	cout << "response : " << response << endl;
	Sleep(100);
	response = sendGenCommand("ATS0");		// Disable spaces
	cout << "response : " << response << endl;
	Sleep(100);
	response = sendGenCommand("ATSP0");	// OBD protocol auto detection mode
	cout << "response : " << response << endl;
	Sleep(100);

	return RETURN_SUCCESS;
}

//OBD 연결성공후 기본세팅 AT command 전송
string CSerialComm::sendGenCommand(string setCommand)
{
	char data[64] = "";
	char sendCommand[20] = "";
	int counter = 0;
	int i;
	string response = "";
	setCommand += "\r";
	bool aFlag = true;

	strcpy_s(sendCommand, setCommand.length() + 1, setCommand.c_str());

	serial.runCommand(sendCommand, data, setCommand.length());

	//obd2는 입력 가능상태일때 '>'송신
	//prompt character '>'가 들어올때까지 read
	while (counter < 64 && aFlag == true)
	{
		serial.readResponse(data[counter]);
		//erase prompt character '>'
		if (data[counter] == '>')
		{
			data[counter] = '\0';
			aFlag = false;
		}
		counter++;
	}

	for (i = 0; i < 64; i++)
	{
		if (data[i] != '>')
			response += data[i];
		else break;
	}
	return response;
}
void CSerialComm::vehicleSpeed(int &speed)
{
	byte sendOBDcmd[6] = "010D\r";
	byte resValues[1];

	serial.getBytes(sendOBDcmd, resValues, 1);
	speed = (int)resValues[0];
}
void CSerialComm::engineRPM(int &rpm)
{
	byte sendOBDcmd[6] = "010C\r";
	byte resValues[2];

	serial.getBytes(sendOBDcmd, resValues, 2);
	rpm = ((int)resValues[0] * 256 + (int)resValues[1]) / 4;
}
void CSerialComm::disconnect() //포트를 다 쓰고 난뒤 닫는 함수
{
	serial.ClosePort();
}