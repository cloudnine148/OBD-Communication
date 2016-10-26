//Available vehicleSpeed, engineRPM
#include "SerialComm.h"
CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}

int CSerialComm::connect(char* portNum)
{
	if (!serial.OpenPort(portNum)) //포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT); //포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout값 설정

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

	serial.runCommand(sendCommand, data, 20);

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