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

//OBD 초기연결후 기본세팅 command 전송
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


	while (counter < 64 && aFlag == true)
	{
		serial.readResponse(data[counter]);
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
	cout << response << endl;
	cout << data << endl;
	return response;
}
//OBD연결후 각종차량정보 command 전송
vector<int> CSerialComm::sendObdCommand(string command) //데이터를 전송하는 함수
{
	vector<int> res;
	//get value of vehicle speed
	if (command.compare("010D\n"))
	{
	}
	else if (command.compare("010C\n"))
	{
		int rpm;
		engineRPM(rpm);
		cout << "rpm : %d" << rpm << endl;
	}
	return res;
}
void CSerialComm::vehicleSpeed(int &speed)
{
	byte values[1];
	serial.getBytes("01", "41", "0D", values, 1);
	speed = (int)values[0];
}
void CSerialComm::engineRPM(int &rpm)
{
	byte command[2];
	command[0] = 0x01;
	command[1] = 0x0C;

	byte values[2];
	serial.getBytes("01", "41", "0C", values, 2);
	rpm = ((values[0] * 256) + values[1] / 4);


}
void CSerialComm::disconnect() //포트를 다 쓰고 난뒤 닫는 함수
{
	serial.ClosePort();
}