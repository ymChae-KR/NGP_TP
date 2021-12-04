#pragma once

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "GameData.h"


#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;

//	���� ���� ��� �Լ�
void err_quit(char* msg);
void err_display(char* msg);

//	Ŀ���� ��Ŷ �ۼ��� �Լ�
void SendID2Client(SOCKET _sock, SOCKADDR_IN _clientaddr);
void Send_Packet_Assembly(char* _p, PACKET_TYPE _pType);
void Send_Packet(void* _packet, SOCKET _sock);
UINT judgePacketData(ID _id);

//	��Ƽ������ ���� �Լ�
DWORD WINAPI MainGameThread(LPVOID arg);
DWORD WINAPI EndGameThread(LPVOID arg);