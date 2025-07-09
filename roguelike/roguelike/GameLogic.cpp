#include "GameLogic.h"
#include "GameMap.h"
#include "Player.h"
#include "framework.h"

//GDI+ ��ū
ULONG_PTR gdiplusToken;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;										//�ν��Ͻ� �ڵ�
LPCTSTR lpszClass = TEXT("��ȭ������Ʈ_����ȯ");				//���� ǥ���ٿ� ǥ�õǴ� ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//GDI+ �ʱ�ȭ
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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

	//GDI+ ����
	GdiplusShutdown(gdiplusToken);

	return (int)Message.wParam;							//Ż�� �ڵ�. ���α׷� ����
}

//WndProc �ȿ� static �����ұ�? �ƴϸ� ����������?
GameMap Map;
Player P(Map.getPlayerPosition(), 10, 2, 5); //PlayerŬ������ �÷��̾� ��ǥ ���� �ʿ� ����?
//int px = P.getPointX();
//int py = P.getPointY();
POINT playerP = P.getPoint();
map<char, Image*> BlockImages;
int F = 1; //using namespace std �ȿ� int floor��� ������ ���� floor��� ������ ��� �Ұ���

void SettingImage()
{
	//Imagemanager ���� ����� ��
	BlockImages['0'] = Image::FromFile(L"sand_4.png");
	BlockImages['1'] = Image::FromFile(L"swamp_1.png");
	BlockImages['P'] = Image::FromFile(L"angel.png");
	BlockImages['F'] = Image::FromFile(L"enter_lair.png");
	BlockImages['M'] = Image::FromFile(L"sphinx.png"); //���� ������ ���� ��� �ұ�?
	
	//�̹��� �ε� ���� ���� Ȯ��(�ϴ� �� ����?)
}

void DeleteBlockImage()
{
	map<char, Image*>::iterator iter;
	for (iter = BlockImages.begin();iter != BlockImages.end();iter++)
	{
		if (iter->second)
		{
			delete iter->second; //�̹��� ����
			iter->second = nullptr; //������ nullptr�� ����(�̷��� �ϴ� �� ����?)
		}
	}
	BlockImages.clear(); //map ����
}

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
				graphics.DrawImage(BlockImages['1'], blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == '0')
			{
				graphics.DrawImage(BlockImages['0'], blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == 'P')
			{
				graphics.DrawImage(BlockImages['P'], blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == 'M')
			{
				graphics.DrawImage(BlockImages['M'], blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
			else if (Map.getMapData(p) == 'F')
			{
				graphics.DrawImage(BlockImages['F'], blockDrawX, blockDrawY, BLOCK_SIZE, BLOCK_SIZE);
			}
		}
	}


	//���� ���
	//�÷��̾� ����
	wstringstream ss1; //WinAPI ȯ�濡�� �پ��� Ÿ���� �����͸� ���ڿ��� �����, Ư�� �����ڵ�(�ѱ� ��)�� �����ϰ� �ٷ�� ���� ���� �����ϰ� �������� C++ ���?
	ss1 << L"�÷��̾� ü�� : " << P.getHp();
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
	//HDC hdc;
	PAINTSTRUCT ps = {}; //�ϴ� {} �ʱ�ȭ ���� GamePaint�Լ� ����ϱ� ���ؼ�

	switch (iMessage) {
	case WM_CREATE:
		SettingImage();
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
				P.setPoint(playerP);
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
				P.setPoint(playerP);
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
				P.setPoint(playerP);
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
				P.setPoint(playerP);
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
			P.setPoint(Map.getPlayerPosition());
			playerP = P.getPoint();
			++F;
		}
		else
		{
			Map.setMapData(playerP, 'P');
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_DESTROY:
		DeleteBlockImage();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}