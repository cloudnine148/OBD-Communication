#include "SerialPort.h"
#include <iostream>

using namespace std;

CSerialPort::CSerialPort()
{
}

CSerialPort::~CSerialPort()
{
}

bool CSerialPort::OpenPort(CString portname)
{
	m_hComm = CreateFile(L"//./" + portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0); //�ø��� ��Ʈ�� �����Ѵ�. 
	if (m_hComm == INVALID_HANDLE_VALUE)  //���������� ��Ʈ�� ���ȴ��� Ȯ��
	{
		return false;  //������ �ʾ��� ��� false ��ȯ
	}
	else
		return true;   //����� ������ ��� true ��ȯ
}

bool CSerialPort::ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity,
	BYTE Parity, BYTE StopBits)
{
	if ((m_bPortReady = GetCommState(m_hComm, &m_dcb)) == 0) //��Ʈ�� ���¸� Ȯ��. ���������� ������ �ʾ��� ��� false ��ȯ
	{
		printf("\nGetCommState Error\n");
		//"MessageBox(L, L"Error", MB_OK + MB_ICONERROR);  
		CloseHandle(m_hComm);
		return false;
	}
	//��Ʈ�� ���� �⺻���� ����
	m_dcb.BaudRate = BaudRate;
	m_dcb.ByteSize = ByteSize;
	m_dcb.Parity = Parity;
	m_dcb.StopBits = StopBits;
	m_dcb.fBinary = true;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fParity = fParity;
	m_dcb.fOutX = false;
	m_dcb.fInX = false;
	m_dcb.fNull = false;
	m_dcb.fAbortOnError = true;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxDsrFlow = false;
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxCtsFlow = false;

	m_bPortReady = SetCommState(m_hComm, &m_dcb);  //��Ʈ ���� Ȯ��

	if (m_bPortReady == 0)  //��Ʈ�� ���¸� Ȯ��. ������ ��� true ��ȯ �ƴ� ��� false ��ȯ
	{
		//MessageBox(L"SetCommState Error");  
		printf("SetCommState Error");
		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

bool CSerialPort::SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
	DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
	DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant) //��� ��Ʈ�� ���� Timeout ����
{
	if ((m_bPortReady = GetCommTimeouts(m_hComm, &m_CommTimeouts)) == 0)
		return false;

	m_CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout; //����Ҷ� �ѹ���Ʈ�� ���� �� ���� ����Ʈ�� ���۵ɶ������� �ð�
															  //��ſ��� �����͸� ���� �� Timeout�� ����� �������� ���� ����
	m_CommTimeouts.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;
	//��ſ��� �����͸� ������ �� Timeout�� ����� �������� ���� ����
	m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;

	m_bPortReady = SetCommTimeouts(m_hComm, &m_CommTimeouts);  //��Ʈ ���� Ȯ��

	if (m_bPortReady == 0) //��Ʈ ���°� ���� ���� ��� false��ȯ. �ƴ� ��� true��ȯ
	{
		//MessageBox(L"StCommTimeouts function failed",L"Com Port Error",MB_OK+MB_ICONERROR);  
		printf("\nStCommTimeouts function failed\n");
		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

bool CSerialPort::runCommand(byte *cmd, char *data, unsigned int dataLength)
{
	m_iBytesWritten = 0;

	if (!PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR))
		cout << "Error : Cannot clear the comport output buffer" << endl;
	if (WriteFile(m_hComm, cmd, dataLength, &m_iBytesWritten, NULL) == 0)
		return false;
	else
		return true;
}
bool CSerialPort::runCommand(const char *cmd, char *data, unsigned int dataLength)
{
	m_iBytesWritten = 0;

	//clear buffer
	if (!PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR))
		cout << "Error : Cannot clear the comport output bffer" << endl;
	//write data into buffer

	if (WriteFile(m_hComm, cmd, dataLength, &m_iBytesWritten, NULL) == 0)
		return false;
	else
		return true;
}

bool CSerialPort::readResponse(char &resp)
{
	char rx;
	resp = 0;

	DWORD dwBytesTransferred = 0;
	if (ReadFile(m_hComm, &rx, 1, &dwBytesTransferred, 0)) //��Ʈ�� �����ϴ� �����͸� ReadFile�� ���� 1����Ʈ�� �о�´�.
	{
		if (dwBytesTransferred == 1) //�����͸� �о���µ� �������� ���
		{
			resp = rx;  //resp�� �����͸� �����ϰ� true ��ȯ
			return true;
		}
	}

	return false; //�������� ��� false ��ȯ
}

bool CSerialPort::readResponse(BYTE* &resp, UINT size)
{
	DWORD dwBytesTransferred = 0;

	if (ReadFile(m_hComm, resp, size, &dwBytesTransferred, 0))
	{
		if (dwBytesTransferred == size)
			return true;
	}

	return false;
}
void CSerialPort::getBytes(const char*mode, const char *chkMode, const char *pid, byte *values, unsigned int numValue)
{

	char data[64] = "";
	byte cmd[6] = "010D\r";

	char hexVal[] = "0x00";
	/*
	char cmd[6];
	cmd[0] = mode[0];
	cmd[1] = mode[1];
	cmd[2] = ' ';
	cmd[3] = pid[0];
	cmd[4] = pid[1];
	cmd[5] = '\r';
	*/
	runCommand(cmd, data, 5);
	int i;
	int counter = 0;
	bool aFlag = true;
	while (counter < 64 && aFlag == true)
	{
		readResponse(data[counter]);
		if (data[counter] == '>')
		{
			data[counter] = '\0';
			aFlag = false;
		}
		counter++;
	}

	for (i = 0; i < numValue; i++)
	{
		hexVal[2] = data[6 + (2 * i)];
		hexVal[3] = data[7 + (2 * i)];
		values[i] = strtol(hexVal, NULL, 16);
	}

}

void CSerialPort::ClosePort()
{
	CloseHandle(m_hComm); //��Ʈ�� �ݴ´�.
	return;
}