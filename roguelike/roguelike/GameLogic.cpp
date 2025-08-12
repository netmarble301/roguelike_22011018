#include "ImageManager.h"
#include "framework.h"
#include "GameManager.h"

//���� â ũ��
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700

//���� â ��ǥ
#define WINDOW_X 100
#define WINDOW_Y 50

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

ImageManager& ImgManager = ImageManager::getInstance();

GameManager manager;

void GamePaint(HWND hWnd, PAINTSTRUCT ps)
{
	HDC hdc = BeginPaint(hWnd, &ps);

	// 1. ���� ��ü �ȼ� ũ�� ���
	int mapPixelWidth = MAP_COLS * BLOCK_SIZE;
	int mapPixelHeight = MAP_ROWS * BLOCK_SIZE;

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
	for (int y = 0; y < MAP_ROWS; ++y) 
	{
		for (int x = 0; x < MAP_COLS; ++x) 
		{
			// �� ����� ���� ȭ�� ��ǥ ���
			int blockDrawX = mapStartX + (x * BLOCK_SIZE);
			int blockDrawY = mapStartY + (y * BLOCK_SIZE);
			p = { x,y };
			// �� �����Ϳ� ���� ������ ��� �̹��� ���� �� �׸���
			graphics.DrawImage(ImgManager.getImage((int)manager.getMap().getMapData(p)), blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
		}
	}

	//���� ���
	//�÷��̾� ����
	wstringstream ss1; //WinAPI ȯ�濡�� �پ��� Ÿ���� �����͸� ���ڿ��� �����, Ư�� �����ڵ�(�ѱ� ��)�� �����ϰ� �ٷ�� ���� ���� �����ϰ� �������� C++ ���?
	ss1 << L"�÷��̾� ü�� : " << manager.getPlayer().getPlayerHp();
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
	ss3 << L"���� ���� : " << manager.getCurrentFloor();
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

POINT playerPoint;
int mapData;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {};

	switch (iMessage) 
	{
	case WM_TIMER:
		manager.setTimer(wParam, hWnd);
		return 0;
	case WM_CREATE:
		manager.initializeGame();
		
		return 0;

	case WM_PAINT:
		GamePaint(hWnd, ps);
		return 0;

	case WM_KEYDOWN:

		switch(manager.UpdateKeyDown(wParam, hWnd))
		{
		case 1:
			SetTimer(hWnd, 1, 500, NULL);
			break;
		case 2:
			SetTimer(hWnd, 2, 500, NULL);
			break;
		
		default:
			break; // ����Ű�� �ƴ� �ٸ� Ű�� ����
		}

		

		
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}