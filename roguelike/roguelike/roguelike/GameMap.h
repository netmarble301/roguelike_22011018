#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <algorithm> //max(a, b) : a와b 중 더 작은 것 반환, min(a,b) : a와b 중 더 큰 것 반환
#include <windows.h>
using namespace std;

//게임 맵
#define MAP_COLS 30
#define MAP_ROWS 30
#define BLOCK_SIZE 16

//POINT 구조체에 대한 operator== 정의 추가
//unordered_map이 키를 비교할 때 필요
inline bool operator==(const POINT& p1, const POINT& p2) {
	return p1.x == p2.x && p1.y == p2.y;
}


enum class MapDataType
{
	EMPTY=0,
	WALL,
	FLOOR,
	PLAYER,
	SPHINX,
	ORC
};

struct GameRoom
{
	POINT roomPoint;
	int width, height; //너비, 높이

	//방 중심 좌표
	int centerX() const { return roomPoint.x + width / 2; } //정수형만 반환
	int centerY() const { return roomPoint.y + height / 2; } //정수형만 반환
};

class GameMap
{
protected:
	//vector<string> MapData;
	//string mapDataArr[MAP_ROWS][MAP_COLS];
	//UINT mapDataArr[MAP_ROWS][MAP_COLS];
	MapDataType mapDataArr[MAP_ROWS][MAP_COLS];

	vector<GameRoom> rooms;
	mt19937 rng; //mt19937 =>메르센 트위스터(고품질의 유사 난수 생성하는 데 사용되는 알고리즘) 기반 32비트 의사 난수 생성기)
	POINT playerPoint;

public:
	GameMap();
	virtual ~GameMap() = default;

	int getMapData(POINT _p) const;
	void setMapData(POINT _p, int _c);

	void initializeMap(); //맵을 1로 초기화
	void generateMap(); //맵 생성 전체 로직
	void createRoom(const GameRoom& room); //방 생성
	void createHorizontalCorridor(int x1, int x2, int y); //통로 수평 생성
	void createVerticalCorridor(int y1, int y2, int x); //통로 수직 생성
	void connectRooms(const GameRoom& r1, const GameRoom& r2); //방과 방을 통로 2개로 연결
	void placeObject(); //오브젝트 랜덤 배치
	POINT getPlayerPoint() const; //플레이어의 좌표 반환

	const vector<GameRoom>& getRooms() const; //방 전체 정보
};

