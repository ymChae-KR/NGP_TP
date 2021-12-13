#include "stdafx.h"
#include "Client.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

SOCKET sock;
SOCKADDR_IN serveraddr;

//SOCKET client_sock;

HANDLE hThread;
DWORD WINAPI GameThread(LPVOID arg);
HANDLE hEvent; // 이벤트

global_variable WGameFramework gGameFramework;

PACKET_TYPE g_GameStatus{ PACKET_TYPE::NONE };
unsigned int CliendID = 0; // 서버에서 결정해주는 클라이언트 번호
char buf[BUFSIZE + 1]; // 데이터 송수신 버퍼
void recvID2Server(SOCKET sock);

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 편한 디버깅 환경을 제공하기 위해, 디버그 모드일 때, 콘솔창을 켜줍니다.
#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	// 
	//// 이벤트 생성
	//hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	//if (hEvent == NULL) return 1;

	//CreateThread(NULL, 0, GameThread, NULL, 0, NULL);

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

	gGameFramework.Clear();

	return (int)msg.wParam;
}


void init_Server_Socket() {

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit((char*)"connect()");

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit((char*)"socket()");

	// connect()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit((char*)"connect()");

	cout << "SOCK : " << sock << endl;
	cout << "SOCKADDR : " << serveraddr.sin_port << endl;

}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WARP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WARP));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, FRAME_WIDTH, FRAME_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{
		init_Server_Socket();
		gGameFramework.Create(hWnd);
		SetTimer(hWnd, MAIN_TIMER, MAIN_TIEMR_FRAME, NULL);
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC mainHDC = BeginPaint(hWnd, &ps);
		HBITMAP GLay = CreateCompatibleBitmap(mainHDC, FRAME_WIDTH, FRAME_HEIGHT);
		HDC GLayDC = CreateCompatibleDC(mainHDC);
		SelectObject(GLayDC, GLay);

		gGameFramework.OnDraw(GLayDC);
		gGameFramework.DrawScore(GLayDC);

		BitBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, GLayDC, 0, 0, SRCCOPY);
		DeleteDC(GLayDC);
		DeleteObject(GLay);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_TIMER:
	{
		//recvID2Server(sock);
		gGameFramework.OnUpdate();
		InvalidateRgn(hWnd, NULL, false);
	}
	break;


	case WM_KEYDOWN:
	{
		gGameFramework.KeyBoard(message, wParam, lParam);
	}
	break;

	case WM_KEYUP:
	{
		//gGameFramework.KeyBoard(message, wParam, lParam);
	}
	break;


	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Send_Packet(void* _packet)
{
	char* packet = reinterpret_cast<char*>(_packet);

	int retval = send(sock, (char*)&packet, sizeof(packet), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display((char*)"send()");
		return;
	}
	printf("[TCP 클라이언트] %d바이트를 보냈습니다.\r\n", retval);

	//  플레이어 움직임 테스트용 cout 로그
	cout << "x : " << reinterpret_cast<cs_packet_mainGame*>(_packet)->ptPos.x << "y : " << reinterpret_cast<cs_packet_mainGame*>(_packet)->ptPos.y << endl;

}


void Interaction()
{
	int retval;
	int len;

	bool bTemp = false;

	// 서버와 데이터 통신

	// 송신
	cs_packet_mainGame packet{};
	packet.pkType = g_GameStatus;
	packet.uiPlayerID = gGameFramework.GetID();
	packet.ptPos = gGameFramework.GetPlayerPos();
	retval = send(sock, (char*)&packet, sizeof(cs_packet_mainGame), 0);

	if (retval == SOCKET_ERROR)
	{
		err_display((char*)"send()");
		return;
	}

	// 수신
	sc_packet_mainGame recvPacket{};
	len = recvn(sock, reinterpret_cast<char*>(&recvPacket), sizeof(sc_packet_mainGame), 0);
	if (len == SOCKET_ERROR) {
		err_display((char*)"recv()");
	}
	else if (len == 0)
		return;

	if (recvPacket.pkType != PACKET_TYPE::END)
		bTemp = true;


	switch (recvPacket.pkType)
	{
	case PACKET_TYPE::NONE:
		cout << "Packet type is NONE, 패킷에 문제있음" << endl;
		return;

	case PACKET_TYPE::START:
		g_GameStatus = PACKET_TYPE::START;
		gGameFramework.SetClientID(recvPacket.uiPlayerID);
		cout << "Packet type is START, my PID is : " << gGameFramework.GetID() << endl;
		break;

	case PACKET_TYPE::READY:
		g_GameStatus = PACKET_TYPE::READY;
		cout << "Waiting for player" << endl;
		break;

	case PACKET_TYPE::MAIN:

		g_GameStatus = PACKET_TYPE::MAIN;
		sc_packet_mainGame temp;
		temp.pkType = recvPacket.pkType;
		temp.vec2Pos = recvPacket.vec2Pos;
		temp.uiPlayerID = recvPacket.uiPlayerID;
		temp.bPos = recvPacket.bPos;
		temp.uiScore = recvPacket.uiScore;
		temp.emScore = recvPacket.emScore;

		gGameFramework.SetEnemyData(temp);
		gGameFramework.SetBallPos(temp);
		gGameFramework.SetScore(temp);
		/*if (gGameFramework.GetScore() == 3)
			g_GameStatus = PACKET_TYPE::END;*/

		//cout << temp.uiPlayerID << " 번 클라 x = " << temp.ptPos.x << ", y = " << temp.ptPos.y << endl;
		cout << temp.uiPlayerID << "번 점수: " << gGameFramework.GetScore() << endl;

		break;

	case PACKET_TYPE::END: //endgame 상태 추가

		sc_packet_mainGame pck;
		pck.uiScore = recvPacket.uiScore;

		g_GameStatus = PACKET_TYPE::END;
		
		if (pck.uiScore >= 3)
		{
			int check = MessageBox(NULL, L"당신이 이겼습니다.", L"승리", MB_RETRYCANCEL);
			if (check == MB_RETRYCANCEL) {
				g_GameStatus = PACKET_TYPE::READY; //재시작이 안됨
			}
		}

		else
		{
			int check = MessageBox(NULL, L"상대가 이겼습니다.", L"패배", MB_RETRYCANCEL);
			if (check == MB_RETRYCANCEL) {
				g_GameStatus = PACKET_TYPE::READY;
			}
		}

		break;
		
		
	}

	

}
