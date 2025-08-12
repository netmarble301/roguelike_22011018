#include "GameManager.h"

GameManager::GameManager() :map(), player(map.getPlayerPoint(), 80, 2, 4), currentFloor(1)
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
    int hp, attack, defense;
    MapDataType mapData;
    for (int y = 0; y < MAP_ROWS; ++y)
    {
        for (int x = 0; x < MAP_COLS; ++x)
        {
            p = { x, y };
            mapData = map.getMapData(p);
            if (mapData == MapDataType::SPHINX)
            {
                hp = 10, attack = 3, defense = 2;
                monsters.push_back(sphinxFactory->createMonster(p, hp, attack, defense));
            }
            else if (mapData == MapDataType::ORC)
            {
                hp = 8, attack = 2, defense = 1;
                monsters.push_back(orcFactory->createMonster(p, hp, attack, defense));
            }
        }
    }
    sphinxFactory.reset();
    orcFactory.reset();



    Test_DebugMob();
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

void GameManager::monsterTurn(HWND hWnd)
{
    int hp;
    for (auto& monster_ptr : monsters)
    {
        if (checkAround((*monster_ptr).getMonsterPoint())) //공격
        {
            hp = getPlayer().getPlayerHp();
            getPlayer().setPlayerHp(hp - (*monster_ptr).getMonsterAttack());
            getMap().setMapData(getPlayer().getPlayerPoint(), MapDataType::PLAYER_ATTACKED);

            SetTimer(hWnd, 3, 100, NULL);
        }
        else if (searchPlayer(monster_ptr->getMonsterPoint()))
        {
            monsterChase(*monster_ptr);
        }
        else
        {
            monsterMove(*monster_ptr);
        }
    }
    m_ISPlayerTurn = true;

}

bool GameManager::searchPlayer(const POINT& _mp) const
{
    POINT playerPoint = player.getPlayerPoint(); //플레이어의 현재 위치

    //몬스터와 플레이어 사이의 거리 계산 (맨해튼 거리)
    int distanceX = abs(_mp.x - playerPoint.x);
    int distanceY = abs(_mp.y - playerPoint.y);

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
    //나중에 몬스터 이동 도중 다른 몬스터와 충돌 고려
    //만약 랜덤 좌표 이동 중 통로 몬스터가 길막하면 랜덤 좌표 다른 것으로

    POINT currentMonsterPoint = _m.getMonsterPoint();
    POINT targetMovePoint = _m.getMovePoint();

    //몬스터가 이미 목표 지점에 도달했다면 새로운 목표 지점 설정
    if ((targetMovePoint.x == -1 && targetMovePoint.y == -1) || (currentMonsterPoint.x == targetMovePoint.x && currentMonsterPoint.y == targetMovePoint.y))
    {
        _m.setMovePoint(getRandomEmptyPoint());
        targetMovePoint = _m.getMovePoint(); // 새로운 목표 지점으로 업데이트
    }

    //A* 알고리즘의 findPath는 GameMap 객체를 참조로 받습니다.
    vector<POINT> path = aStarAlgorithm.findPath(getMap(), currentMonsterPoint, targetMovePoint);

    POINT nextPos = { -1, -1 };

    if (!path.empty() && path.size() > 1)
    {
        nextPos = path[1]; //경로의 두 번째 노드가 다음 이동할 위치
    }
    else
    {
        _m.setMovePoint(getRandomEmptyPoint()); //경로를 찾지 못했거나 유효한 위치가 아니면 갱신
        return;
    }

    //이동할 위치에 플레이어가 있는지 확인(몬스터끼리 겹치지 않는 부분은 나중에 구현)
    MapDataType mapDataAtNextPos = map.getMapData(nextPos);
    //if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y)
    if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y || mapDataAtNextPos == MapDataType::ORC || mapDataAtNextPos == MapDataType::SPHINX)
    {
        _m.setMovePoint(getRandomEmptyPoint());
        return; // 플레이어가 있는 곳으로 이동 시도하지 않음
    }

    //맵에서 몬스터의 이전 위치를 0
    map.setMapData(currentMonsterPoint, MapDataType::EMPTY);

    //몬스터의 실제 위치 업데이트
    _m.setMonsterPoint(nextPos);

    //맵에서 몬스터의 새 위치를 해당 몬스터 타입으로 설정
    //Monster 클래스에 `monsterNum` 멤버를 사용하여 MapDataType을 저장했으므로 이를 활용합니다.
    map.setMapData(nextPos, _m.getMonsterNum());
}

void GameManager::monsterChase(Monster& _m)
{
    POINT currentMonsterPoint = _m.getMonsterPoint();
    POINT playerPoint = player.getPlayerPoint();

    // 몬스터와 플레이어 간의 거리 계산
    int distanceX = abs(currentMonsterPoint.x - playerPoint.x);
    int distanceY = abs(currentMonsterPoint.y - playerPoint.y);

    

    vector<POINT> path = aStarAlgorithm.findPath(getMap(), currentMonsterPoint, playerPoint);

    if (!path.empty() && path.size() > 1)
    {
        POINT nextPos = path[1]; // 경로의 두 번째 노드가 다음 이동할 위치

        // 이동할 위치에 플레이어가 있는지 다시 확인 (방금 계산한 nextPos가 플레이어 위치가 아님을 보장)
        // (path[0]은 현재 몬스터 위치, path[1]은 다음 이동할 위치이므로, path[1]이 플레이어 위치와 겹치면 안 됨)
        if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y)
        {
            return; // 다음 이동할 위치가 플레이어 위치와 겹치면 이동하지 않음
        }

        // 맵에서 몬스터의 이전 위치를 EMPTY로 설정
        map.setMapData(currentMonsterPoint, MapDataType::EMPTY);

        // 몬스터의 실제 위치 업데이트
        _m.setMonsterPoint(nextPos);

        // 맵에서 몬스터의 새 위치를 해당 몬스터 타입으로 설정
        map.setMapData(nextPos, _m.getMonsterNum());
    }
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
        MapDataType mapData = map.getMapData(p);
        if (mapData == MapDataType::EMPTY)
        {
            return p;
        }
    }
}

int GameManager::UpdateKeyDown(WPARAM p_param, HWND hWnd)
{
    if (!m_ISPlayerTurn)
        return 99;
    m_ISPlayerTurn = false;

    POINT playerPoint = getPlayer().getPlayerPoint();
    POINT newPlayerPoint = playerPoint;

    if (playerAction) //공격
    {
        switch (p_param) //공격
        {
        case VK_LEFT: // 왼쪽 방향키
            --newPlayerPoint.x;
            break;
        case VK_RIGHT: // 오른쪽 방향키
            ++newPlayerPoint.x;
            break;
        case VK_UP: // 위쪽 방향키
            --newPlayerPoint.y;
            break;
        case VK_DOWN: // 아래쪽 방향키
            ++newPlayerPoint.y;
            break;
        default:
            return 99; // 방향키가 아닌 다른 키는 무시
        }

        //몬스터 공격
        int monsterHp;
        if (getMap().getMapData(newPlayerPoint) == MapDataType::SPHINX || getMap().getMapData(newPlayerPoint) == MapDataType::ORC)
        {
            for (auto& monster_ptr : monsters)
            {
                if ((*monster_ptr).getMonsterPoint() == newPlayerPoint)
                {
                    
                    monsterHp = (*monster_ptr).getMonstertHp();
                    (*monster_ptr).setMonsterHp(monsterHp - player.getPlayerAttack());

                    

                    if (getMap().getMapData(newPlayerPoint) == MapDataType::SPHINX)
                    {
                        getMap().setMapData(newPlayerPoint, MapDataType::SPHINX_ATTACKED);
                    }
                    else if (getMap().getMapData(newPlayerPoint) == MapDataType::ORC)
                    {
                        getMap().setMapData(newPlayerPoint, MapDataType::ORC_ATTACKED);
                    }
                    break;
                }
            }
        }

        playerAction = false;
        InvalidateRect(hWnd, NULL, FALSE);
        return 2;
    }
    else
    {
        switch (p_param)
        {
        case VK_LEFT: // 왼쪽 방향키
            --newPlayerPoint.x;
            playerMove = true;
            break;
        case VK_RIGHT: // 오른쪽 방향키
            ++newPlayerPoint.x;
            playerMove = true;
            break;
        case VK_UP: // 위쪽 방향키
            --newPlayerPoint.y;
            playerMove = true;
            break;
        case VK_DOWN: // 아래쪽 방향키
            ++newPlayerPoint.y;
            playerMove = true;
            break;
        case 'Z':
            playerAction = true;
            m_ISPlayerTurn = true;
            break;
        default:
            return 99; // 방향키가 아닌 다른 키는 무시
        }

        if (playerMove)
        {
            playerMove = false;
            // 새로운 위치가 유효한지 검사
            MapDataType mapData = getMap().getMapData(newPlayerPoint);
            if (mapData != MapDataType::WALL && mapData != MapDataType::ORC && mapData != MapDataType::SPHINX)
            {
                // 유효한 이동인 경우
                getMap().setMapData(playerPoint, MapDataType::EMPTY);
                getPlayer().setPlayerPoint(newPlayerPoint);
                getMap().setMapData(newPlayerPoint, MapDataType::PLAYER);

                if (mapData == MapDataType::FLOOR)
                {
                    nextFloor();
                }
                InvalidateRect(hWnd, NULL, FALSE);
                return 1; // 이동 성공
            }
            //m_ISPlayerTurn = true; // 이동 실패 시 턴을 다시 돌려놓음
            InvalidateRect(hWnd, NULL, FALSE);
            return 99; // 이동 실패
        }
    }

    return 99;
}

void GameManager::setTimer(WPARAM p_param, HWND hWnd)
{

    switch (p_param)
    {
    case 1:
        monsterTurn(hWnd);
        InvalidateRect(hWnd, NULL, FALSE);
        KillTimer(hWnd, 1);
        break;
    case 2:
        //m_ISPlayerTurn = true;
        for (auto& monster_ptr : monsters)
        {
            if (getMap().getMapData((*monster_ptr).getMonsterPoint())== MapDataType::ORC_ATTACKED)
            {
                getMap().setMapData((*monster_ptr).getMonsterPoint(), MapDataType::ORC);
            }
            else if (getMap().getMapData((*monster_ptr).getMonsterPoint()) == MapDataType::SPHINX_ATTACKED)
            {
                getMap().setMapData((*monster_ptr).getMonsterPoint(), MapDataType::SPHINX);
            }
        }
        
        
        InvalidateRect(hWnd, NULL, FALSE);

        SetTimer(hWnd, 1, 500, NULL); //임시

        //죽은 몬스터 처리
        for (size_t i = 0; i < monsters.size(); i++)
        {
            if ((*monsters[i]).getMonstertHp() <= 0)
            {
                getMap().setMapData((*monsters[i]).getMonsterPoint(), MapDataType::EMPTY);
                (*monsters[i]).setMonsterPoint(POINT(-1, -1));
            }
        }
        //굳이 지울 필요가 있을까? 어차피 다음 floor갈 때 vector reset함
        //monsters.erase(remove_if(monsters.begin(), monsters.end(),[](const auto& monster_ptr) {return monster_ptr->getMonstertHp() <= 0;}),monsters.end());

        KillTimer(hWnd, 2);
        break;

    case 3:
        getMap().setMapData(getPlayer().getPlayerPoint(), MapDataType::PLAYER);
        InvalidateRect(hWnd, NULL, FALSE);

       
        KillTimer(hWnd, 3);
        if (getPlayer().getPlayerHp() <= 0)
        {
            MessageBox(NULL, L"사망하셨습니다", L"종료", MB_OK);
            PostQuitMessage(0);
            DestroyWindow(hWnd);
        }
        break;

    }
}

bool GameManager::checkAround(const POINT& p) const
{
    // 상하좌우 네 방향을 나타내는 배열
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    for (int i = 0; i < 4; ++i)
    {
        POINT neighbor = { p.x + dx[i], p.y + dy[i] };

        // 맵의 경계를 벗어나지 않는지 확인
        if (neighbor.x >= 0 && neighbor.x < MAP_COLS && neighbor.y >= 0 && neighbor.y < MAP_ROWS)
        {
            // 상하좌우 1칸 내에 target이 있는지 확인
            if (map.getMapData(neighbor) == MapDataType::PLAYER)
            {
                return true; // 대상이 발견되면 true 반환
            }
        }
    }
    return false; // 대상이 없으면 false 반환
}

void GameManager::Test_DebugMob()
{
}
