#include "stdafx.h"
#include "GameFramework.h"


WGameFramework::WGameFramework()
{
	Clear();
}

WGameFramework::~WGameFramework()
{
}

void WGameFramework::Reset()
{
}

void WGameFramework::Clear()
{
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;
}

void WGameFramework::OnDraw(HDC hdc)
{
	static int x = 10;
	x++;
	Rectangle(hdc, x + 10, 10, x + 100, 100);
}

void WGameFramework::OnUpdate(const float frameTime)
{
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_KEYDOWN:
		{
			if (wParam == VK_Q)
			{
				SendMessage(m_hWnd, WM_DESTROY, 0, 0);
				return;
			}
		}
		break;
		

		case WM_KEYUP:
		{

		}
		break;


	}

}
