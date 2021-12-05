#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "CNetMgr.h"
#include "GameData.h"

#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;


//	소켓 오류 출력 함수
void err_quit(char* msg);
void err_display(char* msg);
//	소켓 함수
int recvn(SOCKET s, char* buf, int len, int flags);

//	커스텀 패킷 송수신 함수
void SendID2Client(SOCKET _sock, SOCKADDR_IN _clientaddr);
void Recv_Packet_Assembly(char* _p, PACKET_TYPE _pType);
void Send_Packet(void* _packet, SOCKET _sock);
UINT judgePacketData(ID _id);

//	멀티쓰레드 관리 함수
DWORD WINAPI MainGameThread(LPVOID arg);
DWORD WINAPI EndGameThread(LPVOID arg);

static SOCKET listen_sock;
static HANDLE hSendEvent; // 전송 완료 이벤트
static HANDLE hRecvEvent; // 수신 완료 이벤트
static unsigned int ThreadNum = 1;
 
static ID g_clientIDManager[2]{};      //  클라이언트 ID 부여 후 이를 관리할 컨테이너, 차후에 서버 프로젝트의 mainGame 안에서 관리 할 예정
static UINT g_uiIDCnt{ 0 };            //  각 클라이언트 ID 부여를 위한 Count
static BOOL g_bGameStart{ false };     //  게임 시작 여부 확인 변수

