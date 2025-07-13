#include "GameLogic.h"
#include "GameMap.h"
#include "Player.h"
#include "ImageManager.h"
#include "MonsterFactory.h"
#include "framework.h"

//GDI+ ��ū
ULONG_PTR gdiplusToken;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;										//�ν��Ͻ� �ڵ�
LPCTSTR lpszClass = TEXT("��ȭ������Ʈ_����ȯ");				//���� ǥ���ٿ� ǥ�õǴ� ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{


	//GDI+ �ʱ�ȭ(ImageManager�� �����ڿ� �����ϴ� �ͺ��� ���⿡ �����ϴ� ���� �� ���� WinMain�� winapi���ø����̼��� ���������� �ֻ��� ���� �����̱� ����)
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	ImageManager::getInstance().imageInitialize(); //BlockImages �ʱ�ȭ(GDI+ �ʱ�ȭ �� BlockImages �ʱ�ȭ �ϱ� ���ؼ�)


	HWND hWnd;											//������ �ڵ� ����
	MSG Message;										//�޼��� ����ü ���� ����
	WNDCLASS WndClass;									//Windows Class ����ü ���� ����
	g_hInst = hInstance;								//hInstance���� �ܺο����� ����� �� �ֵ��� ���������� ���� ����

	WndClass.cbClsExtra = 0;							//���� ����. ������ ���X
	WndClass.cbWndExtra = 0;							//���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// �������� ��� ������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//�������� ���콺������ ����� ����
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//�������� Ŀ�� ������ ��� ����
	WndClass.hInstance = hInstance;						//������ Ŭ������ ����ϴ� ���α׷� ��ȣ
	WndClass.lpfnWndProc = WndProc;						//������ �޼��� ó�� �Լ� ����
	WndClass.lpszClassName = lpszClass;					//������ Ŭ������ �̸� ����

	WndClass.lpszMenuName = NULL;						//�޴� ����
	//WndClass.lpszMenuName = MAKEINTRESOURCE(~);				//�޴� ����

	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//�������� ��Ÿ���� ����

	RegisterClass(&WndClass);							//WNDCLASS ����ü�� ������ ����


	hWnd = CreateWindow(lpszClass, lpszClass,			//�����츦 ����
		WS_OVERLAPPED | WS_SYSMENU, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	//�޼��� ������ ����
	while (GetMessage(&Message, NULL, 0, 0)) {			//Queue�� �ִ� �޼����� �о���δ�
		TranslateMessage(&Message);						//Ű���� �Է� �޼����� �����Ͽ� ���α׷����� ���� 
		//  ����� �� �ֵ���
		DispatchMessage(&Message);						//�޼����� �������� �޼��� ó�� �Լ� WndProc�� ����
	}


	//GDI+ ����(ImageManager�� �Ҹ��ڿ� �����ϴ� �ͺ��� ���⿡ �����ϴ� ���� �� ����)
	GdiplusShutdown(gdiplusToken);


	return (int)Message.wParam;							//Ż�� �ڵ�. ���α׷� ����
}

/*
GameMap Map;
Player P(Map.getPlayerPosition(), 10, 2, 5);
POINT playerP = P.getPlayerPoint();
vector<unique_ptr<Monster>> Monsters;
int F = 1;
*/
ImageManager& ImgManager = ImageManager::getInstance();
/*
unique_ptr<Monster> createMonsterWithStats(const MonsterFactory& _factory, string _name, POINT _p)
{
	int hp = 0, attack = 0, defense = 0;
	bool turn = false;
	POINT p = _p;

	if (_name == "Sphinx")
	{
		hp = 10;
		attack = 3;
		defense = 2;
	}
	else if (_name == "Orc")
	{
		hp = 8;
		attack = 2;
		defense = 1;
	}

	unique_ptr<Monster> monster = _factory.createMonster(p, hp, attack, defense, turn);

	return monster;
}
*/

void GamePaint(HWND hWnd, PAINTSTRUCT ps)
{
	HDC hdc = BeginPaint(hWnd, &ps);
	

	// 1. ���� ��ü �ȼ� ũ�� ���
	int mapPixelWidth = Map.getMapCols() * BLOCK_SIZE;
	int mapPixelHeight = Map.getMapRows() * BLOCK_SIZE;

	// 2. ������ Ŭ���̾�Ʈ ������ ũ�� ��������
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	int windowWidth = clientRect.right - clientRect.left;
	int windowHeight = clientRect.bottom - clientRect.top;

	//������۸�
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	Graphics graphics(hMemDC);

	graphics.Clear(Color(255, 255, 255)); // ������� ��� �����
	
	// 3. ���� �߾� ��ܿ� ��ġ�ϱ� ���� ���� ��ǥ ���
	// X ��ǥ: (������ �ʺ� - �� �ʺ�) / 2
	int mapStartX = (windowWidth - mapPixelWidth) / 2;
	// Y ��ǥ: ������ ��ܿ��� �ణ ����߸��� (��: 20�ȼ�)
	int mapStartY = 20; // �Ǵ� ���ϴ� ��� ���� �ȼ� ��

	// 4. �� �׸���
	POINT p;
	for (int y = 0; y < Map.getMapRows(); ++y) {
		for (int x = 0; x < Map.getMapCols(); ++x) {
			// �� ����� ���� ȭ�� ��ǥ ���
			int blockDrawX = mapStartX + (x * BLOCK_SIZE);
			int blockDrawY = mapStartY + (y * BLOCK_SIZE);
			p = { x,y };
			// �� �����Ϳ� ���� ������ ��� �̹��� ���� �� �׸���
			if (Map.getMapData(p) == '1')
			{
				graphics.DrawImage(ImgManager.getImage('1'), blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == '0')
			{
				graphics.DrawImage(ImgManager.getImage('0'), blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == 'M')
			{
				graphics.DrawImage(ImgManager.getImage('M'), blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == 'P')
			{
				graphics.DrawImage(ImgManager.getImage('P'), blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == 'F')
			{
				graphics.DrawImage(ImgManager.getImage('F'), blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
		}
	}


	//���� ���
	//�÷��̾� ����
	wstringstream ss1; //WinAPI ȯ�濡�� �پ��� Ÿ���� �����͸� ���ڿ��� �����, Ư�� �����ڵ�(�ѱ� ��)�� �����ϰ� �ٷ�� ���� ���� �����ϰ� �������� C++ ���?
	ss1 << L"�÷��̾� ü�� : " << P.getPlayerHp();
	const wstring ws_Text1 = ss1.str(); //wstring���� ��ȯ
	const TCHAR* char_Text1 = ws_Text1.c_str(); //TCHAR* (const wchar_t* �Ǵ� const char*)�� ��ȯ
	Font font(L"Arial", 16); //��Ʈ �� ũ��
	SolidBrush textBrush(Color::Black); //RGB�� �ص� ��
	graphics.DrawString(char_Text1, -1, &font, PointF(0,50), &textBrush);
	//���� �α�
	wstringstream ss2;
	ss2 << L"���� �α�";
	const wstring ws_Text2 = ss2.str(); //wstring���� ��ȯ
	const TCHAR* char_Text2 = ws_Text2.c_str(); //TCHAR* (const wchar_t* �Ǵ� const char*)�� ��ȯ
	graphics.DrawString(char_Text2, -1, &font, PointF(680, 50), &textBrush);
	//���� ����
	wstringstream ss3;
	ss3 << L"���� ���� : " << F;
	const wstring ws_Text3 = ss3.str(); //wstring���� ��ȯ
	const TCHAR* char_Text3 = ws_Text3.c_str(); //TCHAR* (const wchar_t* �Ǵ� const char*)�� ��ȯ
	graphics.DrawString(char_Text3, -1, &font, PointF(680, 20), &textBrush);
	//���� ����, �÷��̾� ���� �� ����ϱ�


	// ��� �׸��Ⱑ �޸� DC���� �Ϸ�Ǹ�, �� ���� ȭ������ �����մϴ�.
	BitBlt(hdc, 0, 0, windowWidth, windowHeight, hMemDC, 0, 0, SRCCOPY);

	// ���ҽ� ����
	SelectObject(hMemDC, hOldBitmap); // ���� ��Ʈ������ ����
	DeleteObject(hBitmap); // ������ ��Ʈ�� ����
	DeleteDC(hMemDC); // �޸� DC ����

	EndPaint(hWnd, &ps);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {};

	switch (iMessage) {
	case WM_CREATE:
		return 0;

	case WM_PAINT:
		GamePaint(hWnd, ps);
		return 0;

	case WM_KEYDOWN:
		Map.setMapData(playerP, '0');
		switch (wParam)
		{
		case VK_LEFT: // ���� ����Ű
			--playerP.x;
			if (Map.getMapData(playerP) != '1' && Map.getMapData(playerP) != 'M')
			{
				P.setPlayerPoint(playerP);
			}
			else
			{
				++playerP.x;
			}
			break;
		case VK_RIGHT: // ������ ����Ű
			++playerP.x;
			if (Map.getMapData(playerP) != '1' && Map.getMapData(playerP) != 'M')
			{
				P.setPlayerPoint(playerP);
			}
			else
			{
				--playerP.x;
			}
			break;
		case VK_UP: // ���� ����Ű
			--playerP.y;
			if (Map.getMapData(playerP) != '1' && Map.getMapData(playerP) != 'M')
			{
				P.setPlayerPoint(playerP);
			}
			else
			{
				++playerP.y;
			}
			break;
		case VK_DOWN: // �Ʒ��� ����Ű
			++playerP.y;
			if (Map.getMapData(playerP) != '1' && Map.getMapData(playerP) != 'M')
			{
				P.setPlayerPoint(playerP);
			}
			else
			{
				--playerP.y;
			}
			break;
		}
		if (Map.getMapData(playerP) == 'F')
		{
			//����(�ؽ�Ʈ) ����
			Map.initializeMap();
			Map.generateMap();
			P.setPlayerPoint(Map.getPlayerPosition());
			playerP = P.getPlayerPoint();
			++F;
		}
		else
		{
			Map.setMapData(playerP, 'P');
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}