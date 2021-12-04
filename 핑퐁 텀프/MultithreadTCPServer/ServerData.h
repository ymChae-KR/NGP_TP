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

//	소켓 오류 출력 함수
void err_quit(char* msg);
void err_display(char* msg);

//	커스텀 패킷 송수신 함수
void SendID2Client(SOCKET _sock, SOCKADDR_IN _clientaddr);
void Send_Packet_Assembly(char* _p, PACKET_TYPE _pType);
void Send_Packet(void* _packet, SOCKET _sock);
UINT judgePacketData(ID _id);

//	멀티쓰레드 관리 함수
DWORD WINAPI MainGameThread(LPVOID arg);
DWORD WINAPI EndGameThread(LPVOID arg);