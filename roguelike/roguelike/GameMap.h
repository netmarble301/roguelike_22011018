#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <random> //uniform_int_distribution, shuffle()
#include <chrono> //난수 시드 생성
#include <algorithm> //max(a, b) : a와b 중 더 작은 것 반환, min(a,b) : a와b 중 더 큰 것 반환
#include <windows.h> //POINT
using namespace std;

//글로벌 클래스 만들까?

//게임 맵
//MapData[0].length() => 열
//MapData.size() => 행
#define MAP_COLS 30
#define MAP_ROWS 30
#define BLOCK_SIZE 16

struct GameRoom
{
	int x, y; //방 좌측 상단 좌표
	int width, height; //너비, 높이

	//방 중심 좌표
	int centerX() const { return x + width / 2; } //정수형만 반환
	int centerY() const { return y + height / 2; } //정수형만 반환
};

class GameMap
{
protected:
	vector<string> MapData;
	vector<GameRoom> Rooms;
	mt19937 rng; //mt19937 =>메르센 트위스터(고품질의 유사 난수 생성하는 데 사용되는 알고리즘) 기반 32비트 의사 난수 생성기)
	POINT playerPos;

public:
	GameMap();
	int getMapCols() const;
	int getMapRows() const;
	char getMapData(POINT _p);
	void setMapData(POINT _p, char _c);

	void initializeMap(); //맵을 벽으로 초기화
	void generateMap(); //맵 생성 전체 로직
	void createRoom(const GameRoom& room); //방 생성
	void createHorizontalCorridor(int x1, int x2, int y); //통로 수평 생성
	void createVerticalCorridor(int y1, int y2, int x); //통로 수직 생성
	void connectRooms(const GameRoom& r1, const GameRoom& r2); //방과 방을 통로 2개로 연결
	void placeObject(); //오브젝트 랜덤 배치('P'와 'F' 같은 방에 나오지 않도록, 'M'가 다른 오브젝트 좌표와 겹치지 않도록 '0'에만 setMapData)
	POINT getPlayerPosition() const; //P'의 좌표를 POINT 반환
};

