#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ServerData.h"

SOCKET listen_sock;
HANDLE hSendEvent; // ���� �Ϸ� �̺�Ʈ
HANDLE hRecvEvent; // ���� �Ϸ� �̺�Ʈ
UINT ThreadNum = 1;

ID g_clientIDManager[2]{};
UINT g_uiIDCnt{ 0 };
BOOL g_bGameStart{ false };

static CNetMgr g_NetMgr;

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

DWORD WINAPI MainGameThread(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    SOCKADDR_IN clientaddr;

    int retval;
    int addrlen;
    char buf[BUFSIZE + 1]{};
    char recvBuf[sizeof(cs_packet_mainGame)]{};

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    gameData gd;                    //  Ŭ�� 1���� �۽��� Ŭ��2���� ������ �Ǵܿ� ����    
    ID PID{ clientaddr, g_uiIDCnt };
    gd.m_ID = judgePacketData(PID);
    g_clientIDManager[g_uiIDCnt].sc_Client_Address = clientaddr;
    g_clientIDManager[g_uiIDCnt].uiID = g_uiIDCnt++;

    if (g_uiIDCnt > 1)
        int a = 0;

    while (true)
    {
        //  ����
        cs_packet_mainGame recvPacket{};
        retval = recvn(client_sock, reinterpret_cast<char*>(&recvPacket), sizeof(cs_packet_mainGame), 0);
        if (retval == SOCKET_ERROR) {
            err_display((char*)"recv()");
            break;
        }
        else if (retval == 0)
            break;

        PACKET_TYPE pType = g_NetMgr.setPacketData(recvPacket);

        //  �۽�
        if (!g_bGameStart)
            SendID2Client(client_sock, pType);
        else
        {
            //  ��Ŷ ����
            sc_packet_mainGame data{};
            data.pkType = MAIN;
            data.vec2Pos = g_NetMgr.getOtherPlayerData(gd.m_ID).m_vecPos;
            data.uiPlayerID = gd.m_ID;

            retval = send(client_sock, (char*)&data, sizeof(sc_packet_mainGame), 0);
            if (retval == SOCKET_ERROR) {
                err_display((char*)"send()");
                break;
            }
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