#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ServerData.h"
#include "GameData.h"


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

SOCKET init_Client_Socket(SOCKET listen_sock) {

    int retval;

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    return listen_sock;
}

DWORD WINAPI MainGameThread(LPVOID arg) {

    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1]{};

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    //  Ŭ���̾�Ʈ���� PID �۽�
    SendID2Client(client_sock, clientaddr);

    while (1) 
    {
        // ������ �ޱ�
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // ������ ������
        cs_packet_mainGame* data = reinterpret_cast<cs_packet_mainGame*>(buf);
        retval = send(client_sock, (char*)data, sizeof(data), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        // ���� ������ ���
        //buf[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);
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

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // ������ ��ſ� ����� ����
    SOCKET client_sock = init_Client_Socket(listen_sock);
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

    printf("listening...\n");

    while (1) 
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
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

void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

void err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

void Send_Packet(void* _packet, SOCKET _sock)
{
    char* packet = reinterpret_cast<char*>(_packet);
    int retval;

    // ������ ������
    sc_packet_mainGame* temp = reinterpret_cast<sc_packet_mainGame*>(_packet);
    retval = send(_sock, (char*)&packet, sizeof(sc_packet_mainGame), 0);

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