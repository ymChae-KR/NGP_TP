#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ServerData.h"
#include "GameData.h"
#include "server.cpp"

void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
void Send_Packet(void* _packet, SOCKET _sock);
DWORD WINAPI MainGameThread(LPVOID arg);
DWORD WINAPI EndGameThread(LPVOID arg);

SOCKET listen_sock;
HANDLE hSendEvent; // 전송 완료 이벤트
HANDLE hRecvEvent; // 수신 완료 이벤트
unsigned int ThreadNum = 1;

//MainGame::BallUpdate()
//MainGame::notifyCollisions()
//MainGame::playerUpdate(POINT p2dPosition)

ID g_clientIDManager[2]{};      //  클라이언트 ID 부여 후 이를 관리할 컨테이너, 차후에 서버 프로젝트의 mainGame 안에서 관리 할 예정
UINT g_uiIDCnt{ 0 };            //  각 클라이언트 ID 부여를 위한 Count
BOOL g_bGameStart{ false };     //  게임 시작 여부 확인 변수

SOCKET init_Client_Socket(SOCKET listen_sock) { //연결용 소켓 생성

    int retval;

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit((char*)"bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit((char*)"listen()");

    return listen_sock;
}

DWORD WINAPI MainGameThread(LPVOID arg) {

    SOCKET client_sock = (SOCKET)arg;
    SOCKADDR_IN clientaddr;

    int retval;
    int addrlen;
    char buf[BUFSIZE + 1]{};

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    //  클라이언트에게 PID 송신
    //SendID2Client(client_sock, clientaddr); //이 다음으로 안넘어가는듯?



    while (1)
    {
        sc_packet_mainGame recvPacket{};
        // 데이터 받기
        retval = recvn(client_sock, reinterpret_cast<char*>(&recvPacket), sizeof(recvPacket), 0);
        if (retval == SOCKET_ERROR) {
            err_display((char*)"recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 받은 데이터 출력
        cout << "recv packet from server : x = " << recvPacket.vec2Pos.x << ", y = " << recvPacket.vec2Pos.y << ", PID = " << recvPacket.uiPlayerID << endl;


        //// 받은 데이터 출력
        //buf[retval] = '\0';
        //printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
        //    ntohs(clientaddr.sin_port), buf);

        // 데이터 보내기
        cs_packet_mainGame* data = reinterpret_cast<cs_packet_mainGame*>(buf);
        //테스트 데이터
        data->pkType = MAIN;
        data->ptPos.x = 500;
        data->ptPos.y = 500;
        data->uiPlayerID = 1;

        retval = send(client_sock, (char*)data, sizeof(cs_packet_mainGame), 0);
        if (retval == SOCKET_ERROR) {
            err_display((char*)"send()");
            break;
        }


    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    g_clientIDManager[g_uiIDCnt].sc_Client_Address = SOCKADDR_IN();
    g_clientIDManager[g_uiIDCnt].uiID = 0;
    --g_uiIDCnt;

    return 0;

}

int main(int argc, char* argv[])
{
    //윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0); //연결용 소켓 
    if (listen_sock == INVALID_SOCKET) err_quit((char*)"socket()");

    printf("listening...");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock = init_Client_Socket(listen_sock); //통신을 위한 소켓
    SOCKADDR_IN clientaddr;
    HANDLE hThread;

    //클라이언트 정보 얻기
    int addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1)
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display((char*)"accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] %d번 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            ThreadNum, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        ThreadNum++; //ClientNum

        /*g_clientIDManager[g_uiIDCnt].sc_Client_Address = clientaddr;
        g_clientIDManager[g_uiIDCnt].uiID = g_uiIDCnt++;
        sc_packet_mainGame packet{};
        packet.uiPlayerID = g_clientIDManager[g_uiIDCnt - 1].uiID;

        Send_Packet(&packet, client_sock);
        cout << g_uiIDCnt << "번째 클라이언트 접속 후 클라 ID 송신" << endl;*/

        // 스레드 생성
        hThread = CreateThread(NULL, 0, MainGameThread, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}

void recvData(SOCKET sock) {

    int retval;

    cs_packet_mainGame CLpacket;
    int datalen = sizeof(cs_packet_mainGame);

    retval = recvn(sock, (char*)&datalen, datalen, 0); //구조체 크기 받기
    if (retval == SOCKET_ERROR) err_display((char*)"recv()");

    printf("구조체 크기 %d바이트를 받았습니다.\r\n", retval);
    printf("[받은 데이터] %d\r\n", datalen);

    //받은 데이터
    int dataSize;
    char sBuf[BUFSIZE];
    cs_packet_mainGame* packet;

    dataSize = recvn(sock, sBuf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) err_display((char*)"recv()");

    sBuf[dataSize] = '\0';
    packet = (cs_packet_mainGame*)sBuf;
    printf("[TCP 서버] %d바이트를 받았습니다.\r\n", dataSize);
    printf("[받은 데이터] %s\r\n", (char*)&packet);

    //return *packet;

}

void Send_Packet(void* _packet, SOCKET _sock)
{
    //char* packet = reinterpret_cast<char*>(_packet);
    int retval;

    // 데이터 보내기
    sc_packet_mainGame* temp = reinterpret_cast<sc_packet_mainGame*>(_packet);
    retval = send(_sock, (char*)&temp, sizeof(sc_packet_mainGame), 0);

    if (retval == SOCKET_ERROR)
    {
        err_display((char*)"send()");
        return;
    }

    printf("[TCP 서버] %d바이트를 보냈습니다.\r\n", retval);

}

void SendID2Client(SOCKET _sock, SOCKADDR_IN _clientaddr)
{
    // 데이터 보내기
    g_clientIDManager[g_uiIDCnt].sc_Client_Address = _clientaddr;
    g_clientIDManager[g_uiIDCnt].uiID = g_uiIDCnt;

    sc_packet_mainGame packet{};
    packet.pkType = PACKET_TYPE::START;
    packet.uiPlayerID = g_clientIDManager[g_uiIDCnt++].uiID;

    Send_Packet(&packet, _sock);
    cout << g_uiIDCnt << "번째 클라이언트 접속 후 클라 ID 송신" << endl;
}

void judgePacketData()
{

}