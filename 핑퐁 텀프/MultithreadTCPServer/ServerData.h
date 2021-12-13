#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <mutex>

#include "GameData.h"
#include "CNetMgr.h"

#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;

class CNetMgr;
//	소켓 오류 출력 함수
void err_quit(char* msg);
void err_display(char* msg);
//	소켓 함수
int recvn(SOCKET s, char* buf, int len, int flags);

//	커스텀 패킷 송수신 함수
void SendID2Client(SOCKET _sock, PACKET_TYPE _pType);
void Recv_Packet_Assembly(char* _p, PACKET_TYPE _pType);
void Send_Packet(sc_packet_mainGame _packet, SOCKET _sock);
UINT judgePacketData(ID _id);

//	멀티쓰레드 관리 함수
DWORD WINAPI MainGameThread(LPVOID arg);
DWORD WINAPI EndGameThread(LPVOID arg);

extern SOCKET listen_sock;
extern HANDLE h_Event[3];
extern UINT ThreadNum;

extern ID g_clientIDManager[2];      //  클라이언트 ID 부여 후 이를 관리할 컨테이너, 차후에 서버 프로젝트의 mainGame 안에서 관리 할 예정
extern UINT g_uiIDCnt;            //  각 클라이언트 ID 부여를 위한 Count
extern BOOL g_bGameStart;     //  게임 시작 여부 확인 변수
extern CNetMgr g_NetMgr;

extern BOOL bIDSended;
