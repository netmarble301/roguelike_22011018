#include "GameLogic.h"
#include "GameMap.h"
#include "Player.h"
#include "ImageManager.h"
#include "MonsterFactory.h"
#include "framework.h"

//GDI+ 토큰
ULONG_PTR gdiplusToken;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;										//인스턴스 핸들
LPCTSTR lpszClass = TEXT("심화프로젝트_정주환");				//제목 표시줄에 표시되는 내용

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{


	//GDI+ 초기화(ImageManager의 생성자에 구현하는 것보다 여기에 구현하는 것이 더 좋음 WinMain이 winapi애플리케이션의 진입점이자 최상위 제어 지점이기 때문)
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	ImageManager::getInstance().imageInitialize(); //BlockImages 초기화(GDI+ 초기화 후 BlockImages 초기화 하기 위해서)


	HWND hWnd;											//윈도우 핸들 선언
	MSG Message;										//메세지 구조체 변수 선언
	WNDCLASS WndClass;									//Windows Class 구조체 변수 선언
	g_hInst = hInstance;								//hInstance값을 외부에서도 사용할 수 있도록 전역변수에 값을 저장

	WndClass.cbClsExtra = 0;							//예약 영역. 지금은 사용X
	WndClass.cbWndExtra = 0;							//예약 영역
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 윈도우의 배경 색상을 지정
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//윈도우의 마우스포인터 모양을 지정
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//윈도우의 커서 아이콘 모양 지정
	WndClass.hInstance = hInstance;						//윈도우 클래스를 등록하는 프로그램 번호
	WndClass.lpfnWndProc = WndProc;						//윈도우 메세지 처리 함수 지정
	WndClass.lpszClassName = lpszClass;					//윈도우 클래스의 이름 지정

	WndClass.lpszMenuName = NULL;						//메뉴 지정
	//WndClass.lpszMenuName = MAKEINTRESOURCE(~);				//메뉴 지정

	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//윈도우의 스타일을 정의

	RegisterClass(&WndClass);							//WNDCLASS 구조체의 번지를 전달


	hWnd = CreateWindow(lpszClass, lpszClass,			//윈도우를 생성
		WS_OVERLAPPED | WS_SYSMENU, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	//메세지 루프를 실행
	while (GetMessage(&Message, NULL, 0, 0)) {			//Queue에 있는 메세지를 읽어들인다
		TranslateMessage(&Message);						//키보드 입력 메세지를 가공하여 프로그램에서 쉽게 
		//  사용할 수 있도록
		DispatchMessage(&Message);						//메세지를 윈도우의 메세지 처리 함수 WndProc로 전달
	}


	//GDI+ 종료(ImageManager의 소멸자에 구현하는 것보다 여기에 구현하는 것이 더 좋음)
	GdiplusShutdown(gdiplusToken);


	return (int)Message.wParam;							//탈출 코드. 프로그램 종료
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
	

	// 1. 맵의 전체 픽셀 크기 계산
	int mapPixelWidth = Map.getMapCols() * BLOCK_SIZE;
	int mapPixelHeight = Map.getMapRows() * BLOCK_SIZE;

	// 2. 윈도우 클라이언트 영역의 크기 가져오기
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	int windowWidth = clientRect.right - clientRect.left;
	int windowHeight = clientRect.bottom - clientRect.top;

	//더블버퍼링
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	Graphics graphics(hMemDC);

	graphics.Clear(Color(255, 255, 255)); // 흰색으로 배경 지우기
	
	// 3. 맵을 중앙 상단에 배치하기 위한 시작 좌표 계산
	// X 좌표: (윈도우 너비 - 맵 너비) / 2
	int mapStartX = (windowWidth - mapPixelWidth) / 2;
	// Y 좌표: 윈도우 상단에서 약간 떨어뜨리기 (예: 20픽셀)
	int mapStartY = 20; // 또는 원하는 상단 여백 픽셀 값

	// 4. 맵 그리기
	POINT p;
	for (int y = 0; y < Map.getMapRows(); ++y) {
		for (int x = 0; x < Map.getMapCols(); ++x) {
			// 각 블록의 실제 화면 좌표 계산
			int blockDrawX = mapStartX + (x * BLOCK_SIZE);
			int blockDrawY = mapStartY + (y * BLOCK_SIZE);
			p = { x,y };
			// 맵 데이터에 따라 적절한 블록 이미지 선택 및 그리기
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


	//문자 출력
	//플레이어 상태
	wstringstream ss1; //WinAPI 환경에서 다양한 타입의 데이터를 문자열로 만들고, 특히 유니코드(한글 등)를 안전하게 다루기 위한 가장 유연하고 현대적인 C++ 방법?
	ss1 << L"플레이어 체력 : " << P.getPlayerHp();
	const wstring ws_Text1 = ss1.str(); //wstring으로 변환
	const TCHAR* char_Text1 = ws_Text1.c_str(); //TCHAR* (const wchar_t* 또는 const char*)로 변환
	Font font(L"Arial", 16); //폰트 및 크기
	SolidBrush textBrush(Color::Black); //RGB로 해도 됨
	graphics.DrawString(char_Text1, -1, &font, PointF(0,50), &textBrush);
	//게임 로그
	wstringstream ss2;
	ss2 << L"게임 로그";
	const wstring ws_Text2 = ss2.str(); //wstring으로 변환
	const TCHAR* char_Text2 = ws_Text2.c_str(); //TCHAR* (const wchar_t* 또는 const char*)로 변환
	graphics.DrawString(char_Text2, -1, &font, PointF(680, 50), &textBrush);
	//던전 층수
	wstringstream ss3;
	ss3 << L"던전 층수 : " << F;
	const wstring ws_Text3 = ss3.str(); //wstring으로 변환
	const TCHAR* char_Text3 = ws_Text3.c_str(); //TCHAR* (const wchar_t* 또는 const char*)로 변환
	graphics.DrawString(char_Text3, -1, &font, PointF(680, 20), &textBrush);
	//던전 층수, 플레이어 상태 등 출력하기


	// 모든 그리기가 메모리 DC에서 완료되면, 한 번에 화면으로 전송합니다.
	BitBlt(hdc, 0, 0, windowWidth, windowHeight, hMemDC, 0, 0, SRCCOPY);

	// 리소스 정리
	SelectObject(hMemDC, hOldBitmap); // 이전 비트맵으로 복원
	DeleteObject(hBitmap); // 생성한 비트맵 삭제
	DeleteDC(hMemDC); // 메모리 DC 삭제

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
		case VK_LEFT: // 왼쪽 방향키
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
		case VK_RIGHT: // 오른쪽 방향키
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
		case VK_UP: // 위쪽 방향키
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
		case VK_DOWN: // 아래쪽 방향키
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
			//층수(텍스트) 변경
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