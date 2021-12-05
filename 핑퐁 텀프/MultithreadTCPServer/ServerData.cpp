#include "ServerData.h"

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

int recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        ptr += received;
    }

    return (len - left);
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

void Send_Packet(sc_packet_mainGame _packet, SOCKET _sock)
{
    int retval = send(_sock, (char*)&_packet, sizeof(sc_packet_mainGame), 0);

    if (retval == SOCKET_ERROR)
    {
        err_display((char*)"Send_Packet()");
        return;
    }
}

void SendID2Client(SOCKET _sock, PACKET_TYPE _pType)
{
    sc_packet_mainGame packet{};
    if (_pType == PACKET_TYPE::NONE)
        _pType = PACKET_TYPE::START;
    packet.pkType = _pType;
    packet.uiPlayerID = g_clientIDManager[g_uiIDCnt - 1].uiID;

    Send_Packet(packet, _sock);
    cout << g_uiIDCnt << " ��° Ŭ���̾�Ʈ ���� �� Ŭ�� ID : " << packet.uiPlayerID << " �۽�" << endl;
}

UINT judgePacketData(ID _id)
{
    if (_id.uiID == 0)
        return 1;
    else if (_id.uiID == 1)
        return 0;
    else
    {
        cout << "Client PID �Ǵ� ����" << endl;
        return 2;
    }

    return 2;
}

//void Recv_Packet_Assembly(char* _p)
//{
//    cs_packet_mainGame* packet = reinterpret_cast<cs_packet_mainGame*>(_p);
//
//    switch (packet->pkType)
//    {
//    case PACKET_TYPE::NONE:
//
//        break;
//    case PACKET_TYPE::START:
//
//        break;
//    case PACKET_TYPE::MAIN:
//        g_NetMgr.setPacketData(*packet);
//        break;
//    }
//
//}