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
HANDLE hSendEvent; // ���� �Ϸ� �̺�Ʈ
HANDLE hRecvEvent; // ���� �Ϸ� �̺�Ʈ
unsigned int ThreadNum = 1;

//MainGame::BallUpdate()
//MainGame::notifyCollisions()
//MainGame::playerUpdate(POINT p2dPosition)

ID g_clientIDManager[2]{};      //  Ŭ���̾�Ʈ ID �ο� �� �̸� ������ �����̳�, ���Ŀ� ���� ������Ʈ�� mainGame �ȿ��� ���� �� ����
UINT g_uiIDCnt{ 0 };            //  �� Ŭ���̾�Ʈ ID �ο��� ���� Count
BOOL g_bGameStart{ false };     //  ���� ���� ���� Ȯ�� ����

SOCKET init_Client_Socket(SOCKET listen_sock) { //����� ���� ����

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

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    //  Ŭ���̾�Ʈ���� PID �۽�
    //SendID2Client(client_sock, clientaddr); //�� �������� �ȳѾ�µ�?



    while (1)
    {
        sc_packet_mainGame recvPacket{};
        // ������ �ޱ�
        retval = recvn(client_sock, reinterpret_cast<char*>(&recvPacket), sizeof(recvPacket), 0);
        if (retval == SOCKET_ERROR) {
            err_display((char*)"recv()");
            break;
        }
        else if (retval == 0)
            break;

        // ���� ������ ���
        cout << "recv packet from server : x = " << recvPacket.vec2Pos.x << ", y = " << recvPacket.vec2Pos.y << ", PID = " << recvPacket.uiPlayerID << endl;


        //// ���� ������ ���
        //buf[retval] = '\0';
        //printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
        //    ntohs(clientaddr.sin_port), buf);

        // ������ ������
        cs_packet_mainGame* data = reinterpret_cast<cs_packet_mainGame*>(buf);
        //�׽�Ʈ ������
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
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    g_clientIDManager[g_uiIDCnt].sc_Client_Address = SOCKADDR_IN();
    g_clientIDManager[g_uiIDCnt].uiID = 0;
    --g_uiIDCnt;

    return 0;

}

int main(int argc, char* argv[])
{
    //���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0); //����� ���� 
    if (listen_sock == INVALID_SOCKET) err_quit((char*)"socket()");

    printf("listening...");

    // ������ ��ſ� ����� ����
    SOCKET client_sock = init_Client_Socket(listen_sock); //����� ���� ����
    SOCKADDR_IN clientaddr;
    HANDLE hThread;

    //Ŭ���̾�Ʈ ���� ���
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

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] %d�� Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            ThreadNum, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        ThreadNum++; //ClientNum

        /*g_clientIDManager[g_uiIDCnt].sc_Client_Address = clientaddr;
        g_clientIDManager[g_uiIDCnt].uiID = g_uiIDCnt++;
        sc_packet_mainGame packet{};
        packet.uiPlayerID = g_clientIDManager[g_uiIDCnt - 1].uiID;

        Send_Packet(&packet, client_sock);
        cout << g_uiIDCnt << "��° Ŭ���̾�Ʈ ���� �� Ŭ�� ID �۽�" << endl;*/

        // ������ ����
        hThread = CreateThread(NULL, 0, MainGameThread, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
    return 0;
}

void recvData(SOCKET sock) {

    int retval;

    cs_packet_mainGame CLpacket;
    int datalen = sizeof(cs_packet_mainGame);

    retval = recvn(sock, (char*)&datalen, datalen, 0); //����ü ũ�� �ޱ�
    if (retval == SOCKET_ERROR) err_display((char*)"recv()");

    printf("����ü ũ�� %d����Ʈ�� �޾ҽ��ϴ�.\r\n", retval);
    printf("[���� ������] %d\r\n", datalen);

    //���� ������
    int dataSize;
    char sBuf[BUFSIZE];
    cs_packet_mainGame* packet;

    dataSize = recvn(sock, sBuf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) err_display((char*)"recv()");

    sBuf[dataSize] = '\0';
    packet = (cs_packet_mainGame*)sBuf;
    printf("[TCP ����] %d����Ʈ�� �޾ҽ��ϴ�.\r\n", dataSize);
    printf("[���� ������] %s\r\n", (char*)&packet);

    //return *packet;

}

void Send_Packet(void* _packet, SOCKET _sock)
{
    //char* packet = reinterpret_cast<char*>(_packet);
    int retval;

    // ������ ������
    sc_packet_mainGame* temp = reinterpret_cast<sc_packet_mainGame*>(_packet);
    retval = send(_sock, (char*)&temp, sizeof(sc_packet_mainGame), 0);

    if (retval == SOCKET_ERROR)
    {
        err_display((char*)"send()");
        return;
    }

    printf("[TCP ����] %d����Ʈ�� ���½��ϴ�.\r\n", retval);

}

void SendID2Client(SOCKET _sock, SOCKADDR_IN _clientaddr)
{
    // ������ ������
    g_clientIDManager[g_uiIDCnt].sc_Client_Address = _clientaddr;
    g_clientIDManager[g_uiIDCnt].uiID = g_uiIDCnt;

    sc_packet_mainGame packet{};
    packet.pkType = PACKET_TYPE::START;
    packet.uiPlayerID = g_clientIDManager[g_uiIDCnt++].uiID;

    Send_Packet(&packet, _sock);
    cout << g_uiIDCnt << "��° Ŭ���̾�Ʈ ���� �� Ŭ�� ID �۽�" << endl;
}

void judgePacketData()
{

}