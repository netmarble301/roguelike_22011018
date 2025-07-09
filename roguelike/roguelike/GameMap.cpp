#include "GameMap.h"

GameMap::GameMap() : rng(chrono::system_clock::now().time_since_epoch().count()) //현재 시간으로 난수 시드 설정
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
	MapData.assign(MAP_ROWS, string(MAP_COLS, '1')); //맵 전체 '1'로
	Rooms.clear(); //기존 방 정보 초기화
}

void GameMap::createRoom(const GameRoom& _rooms) 
{
    for (int i = 0; i < _rooms.height; ++i) 
    {
        for (int j = 0; j < _rooms.width; ++j) 
        {
            // 맵 벗어나지 않도록 확인
            if (_rooms.y + i >= 0 && _rooms.y + i < MAP_ROWS && _rooms.x + j >= 0 && _rooms.x + j < MAP_COLS) 
            {
                MapData[_rooms.y + i][_rooms.x + j] = '0';
            }
        }
    }
}

// 수평 통로를 생성하는 함수
void GameMap::createHorizontalCorridor(int _x1, int _x2, int _y) 
{
    for (int x = min(_x1, _x2); x <= max(_x1, _x2); ++x) 
    {
        if (_y >= 0 && _y < MAP_ROWS && x >= 0 && x < MAP_COLS) 
        {
            MapData[_y][x] = '0';
        }
    }
}

// 수직 통로를 생성하는 함수
void GameMap::createVerticalCorridor(int _y1, int _y2, int _x) 
{
    for (int y = min(_y1, _y2); y <= max(_y1, _y2); ++y) 
    {
        if (y >= 0 && y < MAP_ROWS && _x >= 0 && _x < MAP_COLS) 
        {
            MapData[y][_x] = '0';
        }
    }
}

// 두 방을 통로로 연결하는 함수 (L자형)
void GameMap::connectRooms(const GameRoom& _r1, const GameRoom& _r2) 
{
    int x1 = _r1.centerX();
    int y1 = _r1.centerY();
    int x2 = _r2.centerX();
    int y2 = _r2.centerY();

    uniform_int_distribution<int> coinFlip(0, 1); //uniform_int_distribution 주어진 범위 내 각 숫자가 나올 확률 동일하도록 난수 생성

    // 무작위로 수평-수직 또는 수직-수평 중 선택
    if (coinFlip(rng) == 0) 
    {
        // 수평 후 수직
        createHorizontalCorridor(x1, x2, y1);
        createVerticalCorridor(y1, y2, x2);
    }
    else 
    {
        // 수직 후 수평
        createVerticalCorridor(y1, y2, x1);
        createHorizontalCorridor(x1, x2, y2);
    }
}


// 전체 맵을 생성하는 함수
void GameMap::generateMap() 
{
    initializeMap(); //맵을 벽으로 초기화

    uniform_int_distribution<int> numRoomsDist(4, 7); //4개에서 7개의 방
    uniform_int_distribution<int> roomSizeDist(3, 6); //3x3에서 6x6 크기의 방

    int numRoomsToGenerate = numRoomsDist(rng);

    //방 배치
    for (int i = 0; i < numRoomsToGenerate; ++i) 
    {
        int roomWidth = roomSizeDist(rng);
        int roomHeight = roomSizeDist(rng);

        //맵 경계 내에 방이 들어오도록 무작위 위치 설정
        //가장자리에서 1타일의 여백을 두어 통로 연결이 용이하게 함
        uniform_int_distribution<int> xPosDist(1, MAP_COLS - roomWidth - 1);
        uniform_int_distribution<int> yPosDist(1, MAP_ROWS - roomHeight - 1);

        GameRoom newRoom;
        newRoom.x = xPosDist(rng);
        newRoom.y = yPosDist(rng);
        newRoom.width = roomWidth;
        newRoom.height = roomHeight;

        //방이 겹치지 않도록 기본적인 확인
        bool overlaps = false;
        //새로운 방이 기존 방들과 겹치는지 확인
        for (const auto& existingRoom : Rooms) 
        {
            //AABB(Axis-Aligned Bounding Box) 충돌 확인
            if (newRoom.x < existingRoom.x + existingRoom.width && newRoom.x + newRoom.width > existingRoom.x && newRoom.y < existingRoom.y + existingRoom.height && newRoom.y + newRoom.height > existingRoom.y) 
            {
                overlaps = true;
                break;
            }
        }

        if (!overlaps) {
            Rooms.push_back(newRoom); //새로운 방을 목록에 추가
            createRoom(newRoom); //맵에 방 그리기
        }
        else 
        {
            //겹쳤을 경우, 해당 방은 건너뛰고 다시 시도하기 위해 i 감소
            //너무 많이 겹치면 무한 루프에 빠질 수 있으므로, 일정 횟수 이상 시도하면 중단
            if (i > -MAP_COLS * MAP_ROWS) //간단한 방지책
            {
                i--; //재시도
            }
        }
    }

    //방 연결
    if (Rooms.size() > 1) {
        for (size_t i = 0; i < Rooms.size() - 1; ++i) {
            connectRooms(Rooms[i], Rooms[i + 1]);
        }
    }

    placeObject(); //플레이어, 플로어 랜덤 좌표에 생성
}


//오브젝트 랜덤 배치
void GameMap::placeObject() 
{
    if (Rooms.empty()) 
    {
        //방이 없으면 배치할 수 없음
        return;
    }

    //방 목록을 섞어서 무작위로 방을 선택할 수 있도록 함
    shuffle(Rooms.begin(), Rooms.end(), rng);

    // 7/8일 아래 수정 Rooms의 size는 무조건 4개 이상임 아마 empty() 같은 조건문 필요없을 거임!!!

    //'P' 배치
    const GameRoom& pRoom = Rooms[0]; //첫 번째 무작위 방 선택
    // 방 내부의 무작위 좌표
    uniform_int_distribution<int> pXDist(pRoom.x + 1, pRoom.x + pRoom.width - 2); //벽과 겹치지 않게 1칸 안쪽
    uniform_int_distribution<int> pYDist(pRoom.y + 1, pRoom.y + pRoom.height - 2);
    int pX = pXDist(rng);
    int pY = pYDist(rng);
    playerPos = { pX,pY };
    setMapData(playerPos, 'P');

    //'F' 배치
    const GameRoom& fRoom = Rooms[1]; //두 번째 무작위 방 선택
    // 방 내부의 무작위 좌표
    uniform_int_distribution<int> fXDist(fRoom.x + 1, fRoom.x + fRoom.width - 2);
    uniform_int_distribution<int> fYDist(fRoom.y + 1, fRoom.y + fRoom.height - 2);
    int fX = fXDist(rng);
    int fY = fYDist(rng);
    POINT f = { fX,fY };
    setMapData(f, 'F');

    //'M' 배치
    uniform_int_distribution<int> numMonstersDist(2, 4); //2개에서 4개의 몬스터
    int numMonstersToPlace = numMonstersDist(rng);
    for (int i = 0; i < numMonstersToPlace; ++i)
    {
        //모든 방 중에서 무작위로 선택
        uniform_int_distribution<int> roomIndexDist(0, Rooms.size() - 1);
        const GameRoom& mRoom = Rooms[roomIndexDist(rng)];

        //방 내부의 무작위 좌표
        uniform_int_distribution<int> mXDist(mRoom.x + 1, mRoom.x + mRoom.width - 2);
        uniform_int_distribution<int> mYDist(mRoom.y + 1, mRoom.y + mRoom.height - 2);

        int mX, mY;
        POINT mPos;
        //다른 오브젝트와 좌표 겹치지 않도록 함
        do
        {
            mX = mXDist(rng);
            mY = mYDist(rng);
            mPos = { mX,mY };
        } while (getMapData(mPos) != '0');

        setMapData(mPos, 'M');
    }
}

//'P'의 x, y 좌표를 반환하는 함수
POINT GameMap::getPlayerPosition() const 
{
    return playerPos;
}