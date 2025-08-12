#include "GameMap.h"

GameMap::GameMap() : rng(chrono::system_clock::now().time_since_epoch().count()) //���� �ð����� ���� �õ� ����
{
	generateMap();
}

MapDataType GameMap::getMapData(POINT _p) const
{
    return mapDataArr[_p.y][_p.x];
}

void GameMap::setMapData(POINT _p, MapDataType _d)
{
    mapDataArr[_p.y][_p.x] = _d;
}

void GameMap::initializeMap() 
{
    for (int i = 0; i < MAP_ROWS; ++i) 
    {
        for (int j = 0; j < MAP_COLS; ++j) 
        {
            mapDataArr[i][j] = MapDataType::WALL;
        }
    }
	rooms.clear();
}

void GameMap::createRoom(const GameRoom& _rooms) 
{
    //height, width�� generateMap()�� ���� �� ����
    for (int i = 0; i < _rooms.height; ++i)
    {
        for (int j = 0; j < _rooms.width; ++j) 
        {
            //�� ����� �ʵ��� Ȯ��
            if (_rooms.roomPoint.y + i >= 0 && _rooms.roomPoint.y + i < MAP_ROWS && _rooms.roomPoint.x + j >= 0 && _rooms.roomPoint.x + j < MAP_COLS)
            {
                mapDataArr[_rooms.roomPoint.y + i][_rooms.roomPoint.x + j] = MapDataType::EMPTY;
            }
        }
    }
}

void GameMap::createHorizontalCorridor(int _x1, int _x2, int _y) //���� ��� ����
{
    //�Ķ���� _x1 : ��� ��������� x ���� ��ǥ
    //�Ķ���� _x2 : ��� ��������� x �� ��ǥ
    //�Ķ���� _y : �����̶� ����

    //y��ǥ�� _y�̸鼭 x��ǥ�� ���� _x���� ū _x������ ��� MapData�� ��� 0����
    for (int x = min(_x1, _x2); x <= max(_x1, _x2); ++x) 
    {
        if (_y >= 0 && _y < MAP_ROWS && x >= 0 && x < MAP_COLS) 
        {
            mapDataArr[_y][x] = MapDataType::EMPTY;
        }
    }
}

void GameMap::createVerticalCorridor(int _y1, int _y2, int _x) //���� ��� ����
{
    //�Ķ���� _y1 : ��� ��������� y ���� ��ǥ
    //�Ķ���� _y2 : ��� ��������� y �� ��ǥ
    //�Ķ���� _x : �����̶� ����

    //x��ǥ�� _x�̸鼭 y��ǥ�� ���� _y���� ū _y������ ��� MapData�� ��� '0'����
    for (int y = min(_y1, _y2); y <= max(_y1, _y2); ++y)
    {
        if (y >= 0 && y < MAP_ROWS && _x >= 0 && _x < MAP_COLS) 
        {
            mapDataArr[y][_x] = MapDataType::EMPTY;
        }
    }
}

void GameMap::connectRooms(const GameRoom& _r1, const GameRoom& _r2) //�� ���� ��η� �����ϴ� �Լ�(��,�� ��� ���)
{
    //_r1�� �߽� ��ǥ
    int x1 = _r1.centerX();
    int y1 = _r1.centerY();

    //_r2�� �߽� ��ǥ
    int x2 = _r2.centerX();
    int y2 = _r2.centerY();

    uniform_int_distribution<int> coinFlip(0, 1); //uniform_int_distribution �־��� ���� �� �� ���ڰ� ���� Ȯ�� �����ϵ��� ���� ����

    //����->����, ����->���� �� ���� ����
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

void GameMap::generateMap() //��ü ���� �����ϴ� �Լ�
{
    initializeMap(); //���� ������ �ʱ�ȭ

    uniform_int_distribution<int> numRoomsDist(4, 7); //4������ 7���� ��
    uniform_int_distribution<int> roomSizeDist(3, 6); //3x3���� 6x6 ũ���� ��

    int numRoomsToGenerate = numRoomsDist(rng);

    //�� ��ġ
    for (int i = 0; i < numRoomsToGenerate; ++i) 
    {
        //�� ���� �ʺ� ��������
        int roomWidth = roomSizeDist(rng);
        int roomHeight = roomSizeDist(rng);

        //�� ���� ��� ��ǥ ���� ����(�� ����� �ʵ���)
        //�ִ� -1�ؼ� �׵θ� ����� �ʵ���
        uniform_int_distribution<int> xPosDist(1, MAP_COLS - roomWidth - 1);
        uniform_int_distribution<int> yPosDist(1, MAP_ROWS - roomHeight - 1);

        GameRoom newRoom;
        newRoom.roomPoint.x = xPosDist(rng);
        newRoom.roomPoint.y = yPosDist(rng);
        newRoom.width = roomWidth;
        newRoom.height = roomHeight;

        //���� ��ġ�� �ʵ��� Ȯ�ο� ����
        bool overlaps = false;

        //���ο� ���� ���� ���� ��ġ���� Ȯ��
        //�� ���ĵ� ��������� ���� Ȯ�� �� �ص� ��
        for (const auto& existingRoom : rooms) //�����̳� Rooms�� ��� ��� ��ȸ ����� �̷��� ���ǽ� ����
        {
            //AABB(Axis-Aligned Bounding Box) �浹 üũ(a�� b�� x�� ���� y�� ���� ���� ��ġ���� ���� �� �� �� ��ġ�� �浹)
            //newRoom�� ������� ��ǥ�� existingRoom�� ������� ��ǥ x,y ���� ��
            //���� newRoom.x�� existingRoom.x���� Ŭ��, newRoom.x < existingRoom.x + existingRoom.width�� ���̸� ���� x�� ���� ��ģ ����
            //���� newRoom.x�� existingRoom.x���� ������, newRoom.x + newRoom.width > existingRoom.x�� ���̸� ���� x�� ���� ��ģ ����
            //�̷��� x,y ���� ���ؼ� x�� ���� y�� ���� ��� ��ġ�� overlaps�� true��
            if (newRoom.roomPoint.x < existingRoom.roomPoint.x + existingRoom.width && newRoom.roomPoint.x + newRoom.width > existingRoom.roomPoint.x && newRoom.roomPoint.y < existingRoom.roomPoint.y + existingRoom.height && newRoom.roomPoint.y + newRoom.height > existingRoom.roomPoint.y)
            {
                overlaps = true;
                break;
            }
        }

        if (!overlaps) {
            rooms.push_back(newRoom); //���ο� ���� ��Ͽ� �߰�
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
    if (rooms.size() > 1) {
        for (size_t i = 0; i < rooms.size() - 1; ++i) {
            connectRooms(rooms[i], rooms[i + 1]);
        }
    }

    placeObject();
}

void GameMap::placeObject() //������Ʈ ���� ��ġ
{
    if (rooms.empty()) 
    {
        //���� ������ ��ġ �Ұ���
        return;
    }

    //�� ����� ��� �������� ���� ������ �� �ֵ���
    shuffle(rooms.begin(), rooms.end(), rng);

    //Rooms�� �̹� ���õ� ������
    //�÷��̾�� �÷ξ ���� �濡 ��ġ���� �ʵ���
    //�÷��̾� ��ġ
    const GameRoom& pRoom = rooms[0]; //������ �� ����
    //�� ������ ������ ��ǥ
    uniform_int_distribution<int> pXDist(pRoom.roomPoint.x + 1, pRoom.roomPoint.x + pRoom.width - 2);
    uniform_int_distribution<int> pYDist(pRoom.roomPoint.y + 1, pRoom.roomPoint.y + pRoom.height - 2);
    int pX = pXDist(rng);
    int pY = pYDist(rng);
    playerPoint = { pX,pY };
    setMapData(playerPoint, MapDataType::PLAYER);

    //�÷ξ� ��ġ
    const GameRoom& fRoom = rooms[1]; //������ �� ����
    // �� ������ ������ ��ǥ
    uniform_int_distribution<int> fXDist(fRoom.roomPoint.x + 1, fRoom.roomPoint.x + fRoom.width - 2);
    uniform_int_distribution<int> fYDist(fRoom.roomPoint.y + 1, fRoom.roomPoint.y + fRoom.height - 2);
    int fX = fXDist(rng);
    int fY = fYDist(rng);
    POINT f = { fX,fY };
    setMapData(f, MapDataType::FLOOR);

    //���� ��ġ
    uniform_int_distribution<int> numMonstersDist(2, 4); //2������ 4���� ����
    int numMonstersToPlace = numMonstersDist(rng);
    for (int i = 0; i < numMonstersToPlace; ++i)
    {
        //��� �� �߿��� �������� ����
        uniform_int_distribution<int> roomIndexDist(0, rooms.size() - 1);
        const GameRoom& mRoom = rooms[roomIndexDist(rng)];

        //�� ������ ������ ��ǥ
        uniform_int_distribution<int> mXDist(mRoom.roomPoint.x + 1, mRoom.roomPoint.x + mRoom.width - 2);
        uniform_int_distribution<int> mYDist(mRoom.roomPoint.y + 1, mRoom.roomPoint.y + mRoom.height - 2);

        int mX, mY;
        POINT mPos;

        //�ٸ� ������Ʈ�� ��ǥ ��ġ�� �ʵ���
        do
        {
            mX = mXDist(rng);
            mY = mYDist(rng);
            mPos = { mX,mY };
        } while (getMapData(mPos) != MapDataType::EMPTY); //EMPTY�� ������ ��ġ

        uniform_int_distribution<int> monsterTypeDist(4, 5); //���� ���� ����
        MapDataType randomMonsterValue = (MapDataType)monsterTypeDist(rng);
        setMapData(mPos, randomMonsterValue);
    }
}

//�÷��̾��� x, y ��ǥ�� POINT�� ��ȯ�ϴ� �Լ�
POINT GameMap::getPlayerPoint() const
{
    return playerPoint;
}

const vector<GameRoom>& GameMap::getRooms() const
{
    return rooms;
}
