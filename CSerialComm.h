#pragma once
#include <windows.h>
#include <thread>
#include <iostream>

class CSerialComm {
public:
    CSerialComm();
    ~CSerialComm();

    BOOL OpenPort(CString portName, DWORD nBaudRate, DWORD nByteSize);
    void ClosePort();
    BOOL WriteData(const char* data, DWORD size);
    void StartAsyncRead();
    void StopAsyncRead();
    void SetReadCallback(void (*callback)(const char*, DWORD));

private:
    HANDLE m_hComm;         // �ø��� ��Ʈ �ڵ�
    BOOL m_bOpened;         // ��Ʈ�� ���ȴ��� ����
    OVERLAPPED m_osWrite;   // ���� �۾��� OVERLAPPED ����ü
    OVERLAPPED m_osRead;    // �б� �۾��� OVERLAPPED ����ü
    std::thread m_readThread; // �б� ������
    BOOL m_stopRead;        // �б� ���� �÷���
    void (*m_readCallback)(const char*, DWORD); // �б� �ݹ� �Լ� ������

    void ReadThreadFunction(); // �б� ������ �Լ�
};
