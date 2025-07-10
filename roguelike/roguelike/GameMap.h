#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <random> //uniform_int_distribution, shuffle()
#include <chrono> //���� �õ� ����
#include <algorithm> //max(a, b) : a��b �� �� ���� �� ��ȯ, min(a,b) : a��b �� �� ū �� ��ȯ
#include <windows.h> //POINT
using namespace std;

//�۷ι� Ŭ���� �����?

//���� ��
//MapData[0].length() => ��
//MapData.size() => ��
#define MAP_COLS 30
#define MAP_ROWS 30
#define BLOCK_SIZE 16

struct GameRoom
{
	int x, y; //�� ���� ��� ��ǥ
	int width, height; //�ʺ�, ����

	//�� �߽� ��ǥ
	int centerX() const { return x + width / 2; } //�������� ��ȯ
	int centerY() const { return y + height / 2; } //�������� ��ȯ
};

class GameMap
{
protected:
	vector<string> MapData;
	vector<GameRoom> Rooms;
	mt19937 rng; //mt19937 =>�޸��� Ʈ������(��ǰ���� ���� ���� �����ϴ� �� ���Ǵ� �˰���) ��� 32��Ʈ �ǻ� ���� ������)
	POINT playerPos;

public:
	GameMap();
	int getMapCols() const;
	int getMapRows() const;
	char getMapData(POINT _p);
	void setMapData(POINT _p, char _c);

	void initializeMap(); //���� ������ �ʱ�ȭ
	void generateMap(); //�� ���� ��ü ����
	void createRoom(const GameRoom& room); //�� ����
	void createHorizontalCorridor(int x1, int x2, int y); //��� ���� ����
	void createVerticalCorridor(int y1, int y2, int x); //��� ���� ����
	void connectRooms(const GameRoom& r1, const GameRoom& r2); //��� ���� ��� 2���� ����
	void placeObject(); //������Ʈ ���� ��ġ('P'�� 'F' ���� �濡 ������ �ʵ���, 'M'�� �ٸ� ������Ʈ ��ǥ�� ��ġ�� �ʵ��� '0'���� setMapData)
	POINT getPlayerPosition() const; //P'�� ��ǥ�� POINT ��ȯ
};

