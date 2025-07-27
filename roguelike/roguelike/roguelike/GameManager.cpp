#include "GameManager.h"

GameManager::GameManager() :map(), player(map.getPlayerPoint(), 10, 2, 5), currentFloor(1)
{
}

GameManager::~GameManager()
{
    for (size_t i = 0; i < monsters.size(); i++)
    {
        monsters[i].reset();
    }
    monsters.clear();
}

void GameManager::initializeGame()
{
    map.generateMap();

    player.setPlayerPoint(map.getPlayerPoint());

    monsters.clear();

    unique_ptr<MonsterFactory> sphinxFactory = make_unique<SphinxFactory>();
    unique_ptr<MonsterFactory> orcFactory = make_unique<OrcFactory>();
    POINT p;
    int mapData, hp, attack, defense;
    for (int y = 0; y < MAP_ROWS; ++y)
    {
        for (int x = 0; x < MAP_COLS; ++x)
        {
            p = { x, y };
            mapData = map.getMapData(p);
            if (mapData == 4)
            {
                hp = 10, attack = 3, defense = 2;
                monsters.push_back(sphinxFactory->createMonster(p, hp, attack, defense));
            }
            else if (mapData == 5)
            {
                hp = 8, attack = 2, defense = 1;
                monsters.push_back(orcFactory->createMonster(p, hp, attack, defense));
            }
        }
    }
    sphinxFactory.reset();
    orcFactory.reset();
}

void GameManager::nextFloor()
{
    currentFloor++;
    initializeGame();
}

GameMap& GameManager::getMap()
{
    return map;
}

Player& GameManager::getPlayer()
{
    return player;
}

vector<unique_ptr<Monster>>& GameManager::getMonsters()
{
    return monsters;
}

int GameManager::getCurrentFloor() const
{
    return currentFloor;
}

void GameManager::setCurrentFloor(int _floor)
{
    currentFloor = _floor;
}

void GameManager::monsterTurn()
{
    for (auto& monster_ptr : monsters)
    {
        if (searchPlayer(monster_ptr->getMonsterPoint()))
        {
            //if문 1칸 근접 및 공격 or 추격
        }
        else
        {
            monsterMove(*monster_ptr);
        }
    }
}

bool GameManager::searchPlayer(const POINT& _mp) const
{
    POINT playerPoint = player.getPlayerPoint(); //플레이어의 현재 위치

    //몬스터와 플레이어 사이의 거리 계산 (맨해튼 거리)
    int distanceX = abs(_mp.x - _mp.x);
    int distanceY = abs(_mp.y - _mp.y);

    //5x5 범위 (중앙 몬스터 포함)는 x, y 각각 2칸 이내를 의미
    //따라서 거리 계산 시 2 이하여야 함 (ex: -2, -1, 0, 1, 2)
    if (distanceX <= 2 && distanceY <= 2) 
    {
        return true;
    }
    return false;
}

void GameManager::monsterMove(Monster& _m)
{
    POINT currentMonsterPoint = _m.getMonsterPoint();
    POINT targetMovePoint = _m.getMovePoint();

    //몬스터가 이미 목표 지점에 도달했다면 새로운 목표 지점 설정
    if (currentMonsterPoint.x == targetMovePoint.x && currentMonsterPoint.y == targetMovePoint.y)
    {
        _m.setMovePoint(getRandomEmptyPoint());
        targetMovePoint = _m.getMovePoint(); //새로운 목표 지점으로 업데이트
    }

    //A* 알고리즘의 findPath는 GameMap 객체를 참조로 받습니다.
    vector<POINT> path = aStarAlgorithm.findPath(map, currentMonsterPoint, targetMovePoint);

    //경로가 존재하고 이동할 다음 칸이 있다면
    //path.size() > 0 이면 시작점에서 이미 도착한 경우 (경로의 길이가 1)이므로
    //path.size() > 1 이어야 실제로 한 칸 이동할 수 있습니다.
    if (path.size() > 1)
    {
        POINT nextPos = path[1]; //경로의 두 번째 노드가 다음 이동할 위치


        //다음 위치가 플레이어 현 위치랑 같으면? 일단 중지 어차피 searchPlayer->monsterChase 때문에 걱정 안 해도 됨
        if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y)
        {
            return;
        }


        //맵에서 몬스터의 이전 위치를 0
        map.setMapData(currentMonsterPoint, (int)MapDataType::EMPTY);

        //몬스터의 실제 위치 업데이트
        _m.setMonsterPoint(nextPos);

        //맵에서 몬스터의 새 위치를 해당 몬스터 타입으로 설정
        //Monster 클래스에 `monsterNum` 멤버를 사용하여 MapDataType을 저장했으므로 이를 활용합니다.
        map.setMapData(nextPos, _m.getMonsterNum());
    }
}

void GameManager::monsterChase()
{
}

POINT GameManager::getRandomEmptyPoint() const
{
    const vector<GameRoom>& rooms = map.getRooms();

    if (rooms.empty()) 
    {
        //방이 없으면 유효하지 않은 포인트 반환
        return { -1, -1 };
    }

    mt19937 tempRng(chrono::system_clock::now().time_since_epoch().count());

    while (true)
    {
        //무작위 방 선택
        uniform_int_distribution<int> roomIndexDist(0, rooms.size() - 1);
        const GameRoom& randomRoom = rooms[roomIndexDist(tempRng)];

        // 방 내부의 무작위 좌표
        uniform_int_distribution<int> xDist(randomRoom.roomPoint.x + 1, randomRoom.roomPoint.x + randomRoom.width - 2);
        uniform_int_distribution<int> yDist(randomRoom.roomPoint.y + 1, randomRoom.roomPoint.y + randomRoom.height - 2);

        POINT p = { xDist(tempRng), yDist(tempRng) };

        // 해당 위치가 빈 공간(EMPTY)인지 확인
        int mapData = map.getMapData(p);
        if (mapData == (int)MapDataType::EMPTY)
        {
            return p;
        }
    }
}
