#include "GameMap.h"

GameMap::GameMap() : rng(chrono::system_clock::now().time_since_epoch().count()) //현재 시간으로 난수 시드 설정
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
    //height, width는 generateMap()에 설정 할 예정
    for (int i = 0; i < _rooms.height; ++i)
    {
        for (int j = 0; j < _rooms.width; ++j) 
        {
            //맵 벗어나지 않도록 확인
            if (_rooms.roomPoint.y + i >= 0 && _rooms.roomPoint.y + i < MAP_ROWS && _rooms.roomPoint.x + j >= 0 && _rooms.roomPoint.x + j < MAP_COLS)
            {
                mapDataArr[_rooms.roomPoint.y + i][_rooms.roomPoint.x + j] = MapDataType::EMPTY;
            }
        }
    }
}

void GameMap::createHorizontalCorridor(int _x1, int _x2, int _y) //수평 통로 생성
{
    //파라미터 _x1 : 통로 만들어지는 x 시작 좌표
    //파라미터 _x2 : 통로 만들어지는 x 끝 좌표
    //파라미터 _y : 수평이라 고정

    //y좌표가 _y이면서 x좌표가 작은 _x부터 큰 _x까지인 모든 MapData는 모두 0으로
    for (int x = min(_x1, _x2); x <= max(_x1, _x2); ++x) 
    {
        if (_y >= 0 && _y < MAP_ROWS && x >= 0 && x < MAP_COLS) 
        {
            mapDataArr[_y][x] = MapDataType::EMPTY;
        }
    }
}

void GameMap::createVerticalCorridor(int _y1, int _y2, int _x) //수직 통로 생성
{
    //파라미터 _y1 : 통로 만들어지는 y 시작 좌표
    //파라미터 _y2 : 통로 만들어지는 y 끝 좌표
    //파라미터 _x : 수직이라 고정

    //x좌표가 _x이면서 y좌표가 작은 _y부터 큰 _y까지인 모든 MapData는 모두 '0'으로
    for (int y = min(_y1, _y2); y <= max(_y1, _y2); ++y)
    {
        if (y >= 0 && y < MAP_ROWS && _x >= 0 && _x < MAP_COLS) 
        {
            mapDataArr[y][_x] = MapDataType::EMPTY;
        }
    }
}

void GameMap::connectRooms(const GameRoom& _r1, const GameRoom& _r2) //두 방을 통로로 연결하는 함수(ㄴ,ㄱ 모양 통로)
{
    //_r1의 중심 좌표
    int x1 = _r1.centerX();
    int y1 = _r1.centerY();

    //_r2의 중심 좌표
    int x2 = _r2.centerX();
    int y2 = _r2.centerY();

    uniform_int_distribution<int> coinFlip(0, 1); //uniform_int_distribution 주어진 범위 내 각 숫자가 나올 확률 동일하도록 난수 생성

    //수평->수직, 수직->수평 중 순서 랜덤
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

void GameMap::generateMap() //전체 맵을 생성하는 함수
{
    initializeMap(); //맵을 벽으로 초기화

    uniform_int_distribution<int> numRoomsDist(4, 7); //4개에서 7개의 방
    uniform_int_distribution<int> roomSizeDist(3, 6); //3x3에서 6x6 크기의 방

    int numRoomsToGenerate = numRoomsDist(rng);

    //방 배치
    for (int i = 0; i < numRoomsToGenerate; ++i) 
    {
        //방 높이 너비 랜덤으로
        int roomWidth = roomSizeDist(rng);
        int roomHeight = roomSizeDist(rng);

        //방 좌측 상단 좌표 랜덤 설정(맵 벗어나지 않도록)
        //최댓값 -1해서 테두리 벗어나지 않도록
        uniform_int_distribution<int> xPosDist(1, MAP_COLS - roomWidth - 1);
        uniform_int_distribution<int> yPosDist(1, MAP_ROWS - roomHeight - 1);

        GameRoom newRoom;
        newRoom.roomPoint.x = xPosDist(rng);
        newRoom.roomPoint.y = yPosDist(rng);
        newRoom.width = roomWidth;
        newRoom.height = roomHeight;

        //방이 겹치지 않도록 확인용 변수
        bool overlaps = false;

        //새로운 방이 기존 방들과 겹치는지 확인
        //방 겹쳐도 상관없으면 굳이 확인 안 해도 됨
        for (const auto& existingRoom : rooms) //컨터이너 Rooms의 모든 요소 순회 담부터 이렇게 조건식 쓰기
        {
            //AABB(Axis-Aligned Bounding Box) 충돌 체크(a와 b의 x축 변과 y축 변이 서로 겹치는지 각각 비교 둘 다 겹치면 충돌)
            //newRoom의 좌측상단 좌표와 existingRoom의 좌측상단 좌표 x,y 각각 비교
            //만약 newRoom.x가 existingRoom.x보다 클때, newRoom.x < existingRoom.x + existingRoom.width가 참이면 서로 x축 변이 겹친 거임
            //만약 newRoom.x가 existingRoom.x보다 작을때, newRoom.x + newRoom.width > existingRoom.x가 참이면 서로 x축 변이 겹친 거임
            //이렇게 x,y 각각 비교해서 x축 변과 y축 변이 모두 겹치면 overlaps를 true로
            if (newRoom.roomPoint.x < existingRoom.roomPoint.x + existingRoom.width && newRoom.roomPoint.x + newRoom.width > existingRoom.roomPoint.x && newRoom.roomPoint.y < existingRoom.roomPoint.y + existingRoom.height && newRoom.roomPoint.y + newRoom.height > existingRoom.roomPoint.y)
            {
                overlaps = true;
                break;
            }
        }

        if (!overlaps) {
            rooms.push_back(newRoom); //새로운 방을 목록에 추가
            createRoom(newRoom); //맵에 방 그리기
        }
        else 
        {
            //겹쳤을 경우, 해당 방은 건너뛰고 다시 시도하기 위해 i 감소
            //너무 많이 겹치면 무한 루프에 빠질 수 있으므로, 일정 횟수 이상 시도하면 중단
            if (i > -MAP_COLS * MAP_ROWS)
            {
                i--; //재시도
            }
        }
    }

    //방 연결
    if (rooms.size() > 1) {
        for (size_t i = 0; i < rooms.size() - 1; ++i) {
            connectRooms(rooms[i], rooms[i + 1]);
        }
    }

    placeObject();
}

void GameMap::placeObject() //오브젝트 랜덤 배치
{
    if (rooms.empty()) 
    {
        //방이 없으면 배치 불가능
        return;
    }

    //방 목록을 섞어서 무작위로 방을 선택할 수 있도록
    shuffle(rooms.begin(), rooms.end(), rng);

    //Rooms는 이미 셔플된 상태임
    //플레이어와 플로어가 같은 방에 배치되지 않도록
    //플레이어 배치
    const GameRoom& pRoom = rooms[0]; //무작위 방 선택
    //방 내부의 무작위 좌표
    uniform_int_distribution<int> pXDist(pRoom.roomPoint.x + 1, pRoom.roomPoint.x + pRoom.width - 2);
    uniform_int_distribution<int> pYDist(pRoom.roomPoint.y + 1, pRoom.roomPoint.y + pRoom.height - 2);
    int pX = pXDist(rng);
    int pY = pYDist(rng);
    playerPoint = { pX,pY };
    setMapData(playerPoint, MapDataType::PLAYER);

    //플로어 배치
    const GameRoom& fRoom = rooms[1]; //무작위 방 선택
    // 방 내부의 무작위 좌표
    uniform_int_distribution<int> fXDist(fRoom.roomPoint.x + 1, fRoom.roomPoint.x + fRoom.width - 2);
    uniform_int_distribution<int> fYDist(fRoom.roomPoint.y + 1, fRoom.roomPoint.y + fRoom.height - 2);
    int fX = fXDist(rng);
    int fY = fYDist(rng);
    POINT f = { fX,fY };
    setMapData(f, MapDataType::FLOOR);

    //몬스터 배치
    uniform_int_distribution<int> numMonstersDist(2, 4); //2개에서 4개의 몬스터
    int numMonstersToPlace = numMonstersDist(rng);
    for (int i = 0; i < numMonstersToPlace; ++i)
    {
        //모든 방 중에서 무작위로 선택
        uniform_int_distribution<int> roomIndexDist(0, rooms.size() - 1);
        const GameRoom& mRoom = rooms[roomIndexDist(rng)];

        //방 내부의 무작위 좌표
        uniform_int_distribution<int> mXDist(mRoom.roomPoint.x + 1, mRoom.roomPoint.x + mRoom.width - 2);
        uniform_int_distribution<int> mYDist(mRoom.roomPoint.y + 1, mRoom.roomPoint.y + mRoom.height - 2);

        int mX, mY;
        POINT mPos;

        //다른 오브젝트와 좌표 겹치지 않도록
        do
        {
            mX = mXDist(rng);
            mY = mYDist(rng);
            mPos = { mX,mY };
        } while (getMapData(mPos) != MapDataType::EMPTY); //EMPTY인 블럭에만 배치

        uniform_int_distribution<int> monsterTypeDist(4, 5); //몬스터 종류 결정
        MapDataType randomMonsterValue = (MapDataType)monsterTypeDist(rng);
        setMapData(mPos, randomMonsterValue);
    }
}

//플레이어의 x, y 좌표를 POINT로 반환하는 함수
POINT GameMap::getPlayerPoint() const
{
    return playerPoint;
}

const vector<GameRoom>& GameMap::getRooms() const
{
    return rooms;
}
