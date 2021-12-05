#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <mutex>

#include "CNetMgr.h"
#include "GameData.h"

#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;


//	���� ���� ��� �Լ�
void err_quit(char* msg);
void err_display(char* msg);
//	���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags);

//	Ŀ���� ��Ŷ �ۼ��� �Լ�
void SendID2Client(SOCKET _sock, SOCKADDR_IN _clientaddr);
void Recv_Packet_Assembly(char* _p, PACKET_TYPE _pType);
void Send_Packet(void* _packet, SOCKET _sock);
UINT judgePacketData(ID _id);

//	��Ƽ������ ���� �Լ�
DWORD WINAPI MainGameThread(LPVOID arg);
DWORD WINAPI EndGameThread(LPVOID arg);

extern SOCKET listen_sock;
extern HANDLE hSendEvent; // ���� �Ϸ� �̺�Ʈ
extern HANDLE hRecvEvent; // ���� �Ϸ� �̺�Ʈ
extern UINT ThreadNum;
 
extern ID g_clientIDManager[2];      //  Ŭ���̾�Ʈ ID �ο� �� �̸� ������ �����̳�, ���Ŀ� ���� ������Ʈ�� mainGame �ȿ��� ���� �� ����
extern UINT g_uiIDCnt;            //  �� Ŭ���̾�Ʈ ID �ο��� ���� Count
extern BOOL g_bGameStart;     //  ���� ���� ���� Ȯ�� ����

