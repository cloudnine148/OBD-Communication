#include "SerialComm.h"
CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}


int CSerialComm::connect(char* portNum)
{
	if (!serial.OpenPort(portNum)) //��Ʈ�� �����ϰ� ���¿� �����Ͽ����� fail�� ��ȯ�Ѵ�.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT); //��Ʈ �⺻���� �����Ѵ�.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout�� ����

	return RETURN_SUCCESS;
}

//OBD �ʱ⿬���� �⺻���� command ����
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
//OBD������ ������������ command ����
vector<int> CSerialComm::sendObdCommand(string command) //�����͸� �����ϴ� �Լ�
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
void CSerialComm::disconnect() //��Ʈ�� �� ���� ���� �ݴ� �Լ�
{
	serial.ClosePort();
}