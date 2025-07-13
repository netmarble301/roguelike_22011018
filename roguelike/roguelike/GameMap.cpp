#include "GameMap.h"

GameMap::GameMap() : rng(chrono::system_clock::now().time_since_epoch().count()) //���� �ð����� ���� �õ� ����
{
	generateMap();
}

int GameMap::getMapCols() const
{
	return MapData[0].length();
}

int GameMap::getMapRows() const
{
	return MapData.size();
}

char GameMap::getMapData(POINT _p)
{
	return MapData[_p.y][_p.x];
}

void GameMap::setMapData(POINT _p, char _c)
{
	MapData[_p.y][_p.x] = _c;
}

void GameMap::initializeMap() 
{
	MapData.assign(MAP_ROWS, string(MAP_COLS, '1')); //�� ��ü '1'��
	Rooms.clear(); //���� �� ���� �ʱ�ȭ
}

void GameMap::createRoom(const GameRoom& _rooms) 
{
    //height, width�� generateMap()�� ���� �� ����
    for (int i = 0; i < _rooms.height; ++i) 
    {
        for (int j = 0; j < _rooms.width; ++j) 
        {
            //�� ����� �ʵ��� Ȯ��
            if (_rooms.y + i >= 0 && _rooms.y + i < MAP_ROWS && _rooms.x + j >= 0 && _rooms.x + j < MAP_COLS) 
            {
                MapData[_rooms.y + i][_rooms.x + j] = '0';
            }
        }
    }
}

//���� ��θ� �����ϴ� �Լ�
void GameMap::createHorizontalCorridor(int _x1, int _x2, int _y) 
{
    //�Ķ���� _x1 : ��� ��������� x ���� ��ǥ
    //�Ķ���� _x2 : ��� ��������� x �� ��ǥ
    //�Ķ���� _y : �����̶� ����

    //y��ǥ�� _y�̸鼭 x��ǥ�� ���� _x���� ū _x������ ��� MapData�� ��� '0'����
    for (int x = min(_x1, _x2); x <= max(_x1, _x2); ++x) 
    {
        if (_y >= 0 && _y < MAP_ROWS && x >= 0 && x < MAP_COLS) 
        {
            MapData[_y][x] = '0';
        }
    }
}

//���� ��θ� �����ϴ� �Լ�
void GameMap::createVerticalCorridor(int _y1, int _y2, int _x) 
{
    //�Ķ���� _y1 : ��� ��������� y ���� ��ǥ
    //�Ķ���� _y2 : ��� ��������� y �� ��ǥ
    //�Ķ���� _x : �����̶� ����

    //x��ǥ�� _x�̸鼭 y��ǥ�� ���� _y���� ū _y������ ��� MapData�� ��� '0'����
    for (int y = min(_y1, _y2); y <= max(_y1, _y2); ++y)
    {
        if (y >= 0 && y < MAP_ROWS && _x >= 0 && _x < MAP_COLS) 
        {
            MapData[y][_x] = '0';
        }
    }
}

//�� ���� ��η� �����ϴ� �Լ�(��,�� ��� ���)
void GameMap::connectRooms(const GameRoom& _r1, const GameRoom& _r2) 
{
    //_r1�� �߽� ��ǥ
    int x1 = _r1.centerX();
    int y1 = _r1.centerY();

    //_r2�� �߽� ��ǥ
    int x2 = _r2.centerX();
    int y2 = _r2.centerY();

    uniform_int_distribution<int> coinFlip(0, 1); //uniform_int_distribution �־��� ���� �� �� ���ڰ� ���� Ȯ�� �����ϵ��� ���� ����

    //�������� ����->���� �Ǵ� ����->���� �� ���� ����
    if (coinFlip(rng) == 0) 
    {
        createHorizontalCorridor(x1, x2, y1);
        createVerticalCorridor(y1, y2, x2);
    }
    else 
    {
        createVerticalCorridor(y1, y2, x1);
        createHorizontalCorridor(x1, x2, y2);
    }
}


//��ü ���� �����ϴ� �Լ�
void GameMap::generateMap() 
{
    initializeMap(); //���� ������ �ʱ�ȭ

    uniform_int_distribution<int> numRoomsDist(4, 7); //4������ 7���� ��
    uniform_int_distribution<int> roomSizeDist(3, 6); //3x3���� 6x6 ũ���� ��

    int numRoomsToGenerate = numRoomsDist(rng);

    //�� ��ġ
    for (int i = 0; i < numRoomsToGenerate; ++i) 
    {
        //�߰�
        //�� ���� �ʺ� ��������
        int roomWidth = roomSizeDist(rng);
        int roomHeight = roomSizeDist(rng);

        //�� ���� ��� ��ǥ ���� ����(�� ����� �ʵ���)
        //�ִ� -1�ؼ� �׵θ� ����� �ʵ���
        uniform_int_distribution<int> xPosDist(1, MAP_COLS - roomWidth - 1);
        uniform_int_distribution<int> yPosDist(1, MAP_ROWS - roomHeight - 1);

        GameRoom newRoom;
        newRoom.x = xPosDist(rng);
        newRoom.y = yPosDist(rng);
        newRoom.width = roomWidth;
        newRoom.height = roomHeight;

        //���� ��ġ�� �ʵ��� Ȯ�ο� ����
        bool overlaps = false;

        //���ο� ���� ���� ���� ��ġ���� Ȯ��
        //�� ���ĵ� ��������� ���� Ȯ�� �� �ص� ��
        for (const auto& existingRoom : Rooms) //�����̳� Rooms�� ��� ��� ��ȸ ����� �̷��� ���ǽ� ����
        {
            //AABB(Axis-Aligned Bounding Box) �浹 üũ(a�� b�� x�� ���� y�� ���� ���� ��ġ���� ���� �� �� �� ��ġ�� �浹)
            //newRoom�� ������� ��ǥ�� existingRoom�� ������� ��ǥ x,y ���� ��
            //���� newRoom.x�� existingRoom.x���� Ŭ��, newRoom.x < existingRoom.x + existingRoom.width�� ���̸� ���� x�� ���� ��ģ ����
            //���� newRoom.x�� existingRoom.x���� ������, newRoom.x + newRoom.width > existingRoom.x�� ���̸� ���� x�� ���� ��ģ ����
            //�̷��� x,y ���� ���ؼ� x�� ���� y�� ���� ��� ��ġ�� overlaps�� true��
            if (newRoom.x < existingRoom.x + existingRoom.width && newRoom.x + newRoom.width > existingRoom.x && newRoom.y < existingRoom.y + existingRoom.height && newRoom.y + newRoom.height > existingRoom.y) 
            {
                overlaps = true;
                break;
            }
        }

        if (!overlaps) {
            Rooms.push_back(newRoom); //���ο� ���� ��Ͽ� �߰�
            createRoom(newRoom); //�ʿ� �� �׸���
        }
        else 
        {
            //������ ���, �ش� ���� �ǳʶٰ� �ٽ� �õ��ϱ� ���� i ����
            //�ʹ� ���� ��ġ�� ���� ������ ���� �� �����Ƿ�, ���� Ƚ�� �̻� �õ��ϸ� �ߴ�
            if (i > -MAP_COLS * MAP_ROWS)
            {
                i--; //��õ�
            }
        }
    }

    //�� ����
    if (Rooms.size() > 1) {
        for (size_t i = 0; i < Rooms.size() - 1; ++i) {
            connectRooms(Rooms[i], Rooms[i + 1]);
        }
    }

    placeObject(); //�÷��̾�, �÷ξ� ���� ��ǥ�� ����
}


//������Ʈ ���� ��ġ
void GameMap::placeObject() 
{
    if (Rooms.empty()) 
    {
        //���� ������ ��ġ �Ұ���
        return;
    }

    //�� ����� ��� �������� ���� ������ �� �ֵ���
    shuffle(Rooms.begin(), Rooms.end(), rng);


    //Rooms�� �̹� ���õ� ������
    //'P'�� 'F'�� ���� �濡 ��ġ���� �ʵ���
    //'P' ��ġ
    const GameRoom& pRoom = Rooms[0]; //������ �� ����
    //�� ������ ������ ��ǥ
    uniform_int_distribution<int> pXDist(pRoom.x + 1, pRoom.x + pRoom.width - 2);
    uniform_int_distribution<int> pYDist(pRoom.y + 1, pRoom.y + pRoom.height - 2);
    int pX = pXDist(rng);
    int pY = pYDist(rng);
    playerPos = { pX,pY };
    setMapData(playerPos, 'P');

    //'F' ��ġ
    const GameRoom& fRoom = Rooms[1]; //������ �� ����
    // �� ������ ������ ��ǥ
    uniform_int_distribution<int> fXDist(fRoom.x + 1, fRoom.x + fRoom.width - 2);
    uniform_int_distribution<int> fYDist(fRoom.y + 1, fRoom.y + fRoom.height - 2);
    int fX = fXDist(rng);
    int fY = fYDist(rng);
    POINT f = { fX,fY };
    setMapData(f, 'F');

    //���� ��ġ
    uniform_int_distribution<int> numMonstersDist(2, 4); //2������ 4���� ����
    int numMonstersToPlace = numMonstersDist(rng);
    uniform_int_distribution<int> monsterTypeDist(0, 1); //���� Ÿ�� ����
    for (int i = 0; i < numMonstersToPlace; ++i)
    {
        //��� �� �߿��� �������� ����
        uniform_int_distribution<int> roomIndexDist(0, Rooms.size() - 1);
        const GameRoom& mRoom = Rooms[roomIndexDist(rng)];

        //�� ������ ������ ��ǥ
        uniform_int_distribution<int> mXDist(mRoom.x + 1, mRoom.x + mRoom.width - 2);
        uniform_int_distribution<int> mYDist(mRoom.y + 1, mRoom.y + mRoom.height - 2);

        int mX, mY;
        POINT mPos;

        //�ٸ� ������Ʈ�� ��ǥ ��ġ�� �ʵ���
        do
        {
            mX = mXDist(rng);
            mY = mYDist(rng);
            mPos = { mX,mY };
        } while (getMapData(mPos) != '0'); //'0'�� ���� ��ġ

        int randomMonsterValue = monsterTypeDist(rng);
        char monsterChar = (randomMonsterValue == 0) ? 'O' : 'S'; //���� ���� 3���� �̻��̸� �ٸ� �������
        setMapData(mPos, monsterChar);
    }
}

//'P'�� x, y ��ǥ�� POINT�� ��ȯ�ϴ� �Լ�
POINT GameMap::getPlayerPosition() const 
{
    return playerPos;
}