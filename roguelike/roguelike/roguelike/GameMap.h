#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <algorithm> //max(a, b) : a��b �� �� ���� �� ��ȯ, min(a,b) : a��b �� �� ū �� ��ȯ
#include <windows.h>
using namespace std;

//���� ��
#define MAP_COLS 30
#define MAP_ROWS 30
#define BLOCK_SIZE 16

//POINT ����ü�� ���� operator== ���� �߰�
//unordered_map�� Ű�� ���� �� �ʿ�
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
	int width, height; //�ʺ�, ����

	//�� �߽� ��ǥ
	int centerX() const { return roomPoint.x + width / 2; } //�������� ��ȯ
	int centerY() const { return roomPoint.y + height / 2; } //�������� ��ȯ
};

class GameMap
{
protected:
	//vector<string> MapData;
	//string mapDataArr[MAP_ROWS][MAP_COLS];
	//UINT mapDataArr[MAP_ROWS][MAP_COLS];
	MapDataType mapDataArr[MAP_ROWS][MAP_COLS];

	vector<GameRoom> rooms;
	mt19937 rng; //mt19937 =>�޸��� Ʈ������(��ǰ���� ���� ���� �����ϴ� �� ���Ǵ� �˰���) ��� 32��Ʈ �ǻ� ���� ������)
	POINT playerPoint;

public:
	GameMap();
	virtual ~GameMap() = default;

	int getMapData(POINT _p) const;
	void setMapData(POINT _p, int _c);

	void initializeMap(); //���� 1�� �ʱ�ȭ
	void generateMap(); //�� ���� ��ü ����
	void createRoom(const GameRoom& room); //�� ����
	void createHorizontalCorridor(int x1, int x2, int y); //��� ���� ����
	void createVerticalCorridor(int y1, int y2, int x); //��� ���� ����
	void connectRooms(const GameRoom& r1, const GameRoom& r2); //��� ���� ��� 2���� ����
	void placeObject(); //������Ʈ ���� ��ġ
	POINT getPlayerPoint() const; //�÷��̾��� ��ǥ ��ȯ

	const vector<GameRoom>& getRooms() const; //�� ��ü ����
};

