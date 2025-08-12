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
        if (checkAround((*monster_ptr).getMonsterPoint())) //����
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
    POINT playerPoint = player.getPlayerPoint(); //�÷��̾��� ���� ��ġ

    //���Ϳ� �÷��̾� ������ �Ÿ� ��� (����ư �Ÿ�)
    int distanceX = abs(_mp.x - playerPoint.x);
    int distanceY = abs(_mp.y - playerPoint.y);

    //5x5 ���� (�߾� ���� ����)�� x, y ���� 2ĭ �̳��� �ǹ�
    //���� �Ÿ� ��� �� 2 ���Ͽ��� �� (ex: -2, -1, 0, 1, 2)
    if (distanceX <= 2 && distanceY <= 2) 
    {
        return true;
    }
    return false;
}

void GameManager::monsterMove(Monster& _m)
{
    //���߿� ���� �̵� ���� �ٸ� ���Ϳ� �浹 ���
    //���� ���� ��ǥ �̵� �� ��� ���Ͱ� �渷�ϸ� ���� ��ǥ �ٸ� ������

    POINT currentMonsterPoint = _m.getMonsterPoint();
    POINT targetMovePoint = _m.getMovePoint();

    //���Ͱ� �̹� ��ǥ ������ �����ߴٸ� ���ο� ��ǥ ���� ����
    if ((targetMovePoint.x == -1 && targetMovePoint.y == -1) || (currentMonsterPoint.x == targetMovePoint.x && currentMonsterPoint.y == targetMovePoint.y))
    {
        _m.setMovePoint(getRandomEmptyPoint());
        targetMovePoint = _m.getMovePoint(); // ���ο� ��ǥ �������� ������Ʈ
    }

    //A* �˰����� findPath�� GameMap ��ü�� ������ �޽��ϴ�.
    vector<POINT> path = aStarAlgorithm.findPath(getMap(), currentMonsterPoint, targetMovePoint);

    POINT nextPos = { -1, -1 };

    if (!path.empty() && path.size() > 1)
    {
        nextPos = path[1]; //����� �� ��° ��尡 ���� �̵��� ��ġ
    }
    else
    {
        _m.setMovePoint(getRandomEmptyPoint()); //��θ� ã�� ���߰ų� ��ȿ�� ��ġ�� �ƴϸ� ����
        return;
    }

    //�̵��� ��ġ�� �÷��̾ �ִ��� Ȯ��(���ͳ��� ��ġ�� �ʴ� �κ��� ���߿� ����)
    MapDataType mapDataAtNextPos = map.getMapData(nextPos);
    //if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y)
    if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y || mapDataAtNextPos == MapDataType::ORC || mapDataAtNextPos == MapDataType::SPHINX)
    {
        _m.setMovePoint(getRandomEmptyPoint());
        return; // �÷��̾ �ִ� ������ �̵� �õ����� ����
    }

    //�ʿ��� ������ ���� ��ġ�� 0
    map.setMapData(currentMonsterPoint, MapDataType::EMPTY);

    //������ ���� ��ġ ������Ʈ
    _m.setMonsterPoint(nextPos);

    //�ʿ��� ������ �� ��ġ�� �ش� ���� Ÿ������ ����
    //Monster Ŭ������ `monsterNum` ����� ����Ͽ� MapDataType�� ���������Ƿ� �̸� Ȱ���մϴ�.
    map.setMapData(nextPos, _m.getMonsterNum());
}

void GameManager::monsterChase(Monster& _m)
{
    POINT currentMonsterPoint = _m.getMonsterPoint();
    POINT playerPoint = player.getPlayerPoint();

    // ���Ϳ� �÷��̾� ���� �Ÿ� ���
    int distanceX = abs(currentMonsterPoint.x - playerPoint.x);
    int distanceY = abs(currentMonsterPoint.y - playerPoint.y);

    

    vector<POINT> path = aStarAlgorithm.findPath(getMap(), currentMonsterPoint, playerPoint);

    if (!path.empty() && path.size() > 1)
    {
        POINT nextPos = path[1]; // ����� �� ��° ��尡 ���� �̵��� ��ġ

        // �̵��� ��ġ�� �÷��̾ �ִ��� �ٽ� Ȯ�� (��� ����� nextPos�� �÷��̾� ��ġ�� �ƴ��� ����)
        // (path[0]�� ���� ���� ��ġ, path[1]�� ���� �̵��� ��ġ�̹Ƿ�, path[1]�� �÷��̾� ��ġ�� ��ġ�� �� ��)
        if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y)
        {
            return; // ���� �̵��� ��ġ�� �÷��̾� ��ġ�� ��ġ�� �̵����� ����
        }

        // �ʿ��� ������ ���� ��ġ�� EMPTY�� ����
        map.setMapData(currentMonsterPoint, MapDataType::EMPTY);

        // ������ ���� ��ġ ������Ʈ
        _m.setMonsterPoint(nextPos);

        // �ʿ��� ������ �� ��ġ�� �ش� ���� Ÿ������ ����
        map.setMapData(nextPos, _m.getMonsterNum());
    }
}

POINT GameManager::getRandomEmptyPoint() const
{
    const vector<GameRoom>& rooms = map.getRooms();

    if (rooms.empty()) 
    {
        //���� ������ ��ȿ���� ���� ����Ʈ ��ȯ
        return { -1, -1 };
    }

    mt19937 tempRng(chrono::system_clock::now().time_since_epoch().count());

    while (true)
    {
        //������ �� ����
        uniform_int_distribution<int> roomIndexDist(0, rooms.size() - 1);
        const GameRoom& randomRoom = rooms[roomIndexDist(tempRng)];

        // �� ������ ������ ��ǥ
        uniform_int_distribution<int> xDist(randomRoom.roomPoint.x + 1, randomRoom.roomPoint.x + randomRoom.width - 2);
        uniform_int_distribution<int> yDist(randomRoom.roomPoint.y + 1, randomRoom.roomPoint.y + randomRoom.height - 2);

        POINT p = { xDist(tempRng), yDist(tempRng) };

        // �ش� ��ġ�� �� ����(EMPTY)���� Ȯ��
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

    if (playerAction) //����
    {
        switch (p_param) //����
        {
        case VK_LEFT: // ���� ����Ű
            --newPlayerPoint.x;
            break;
        case VK_RIGHT: // ������ ����Ű
            ++newPlayerPoint.x;
            break;
        case VK_UP: // ���� ����Ű
            --newPlayerPoint.y;
            break;
        case VK_DOWN: // �Ʒ��� ����Ű
            ++newPlayerPoint.y;
            break;
        default:
            return 99; // ����Ű�� �ƴ� �ٸ� Ű�� ����
        }

        //���� ����
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
        case VK_LEFT: // ���� ����Ű
            --newPlayerPoint.x;
            playerMove = true;
            break;
        case VK_RIGHT: // ������ ����Ű
            ++newPlayerPoint.x;
            playerMove = true;
            break;
        case VK_UP: // ���� ����Ű
            --newPlayerPoint.y;
            playerMove = true;
            break;
        case VK_DOWN: // �Ʒ��� ����Ű
            ++newPlayerPoint.y;
            playerMove = true;
            break;
        case 'Z':
            playerAction = true;
            m_ISPlayerTurn = true;
            break;
        default:
            return 99; // ����Ű�� �ƴ� �ٸ� Ű�� ����
        }

        if (playerMove)
        {
            playerMove = false;
            // ���ο� ��ġ�� ��ȿ���� �˻�
            MapDataType mapData = getMap().getMapData(newPlayerPoint);
            if (mapData != MapDataType::WALL && mapData != MapDataType::ORC && mapData != MapDataType::SPHINX)
            {
                // ��ȿ�� �̵��� ���
                getMap().setMapData(playerPoint, MapDataType::EMPTY);
                getPlayer().setPlayerPoint(newPlayerPoint);
                getMap().setMapData(newPlayerPoint, MapDataType::PLAYER);

                if (mapData == MapDataType::FLOOR)
                {
                    nextFloor();
                }
                InvalidateRect(hWnd, NULL, FALSE);
                return 1; // �̵� ����
            }
            //m_ISPlayerTurn = true; // �̵� ���� �� ���� �ٽ� ��������
            InvalidateRect(hWnd, NULL, FALSE);
            return 99; // �̵� ����
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

        SetTimer(hWnd, 1, 500, NULL); //�ӽ�

        //���� ���� ó��
        for (size_t i = 0; i < monsters.size(); i++)
        {
            if ((*monsters[i]).getMonstertHp() <= 0)
            {
                getMap().setMapData((*monsters[i]).getMonsterPoint(), MapDataType::EMPTY);
                (*monsters[i]).setMonsterPoint(POINT(-1, -1));
            }
        }
        //���� ���� �ʿ䰡 ������? ������ ���� floor�� �� vector reset��
        //monsters.erase(remove_if(monsters.begin(), monsters.end(),[](const auto& monster_ptr) {return monster_ptr->getMonstertHp() <= 0;}),monsters.end());

        KillTimer(hWnd, 2);
        break;

    case 3:
        getMap().setMapData(getPlayer().getPlayerPoint(), MapDataType::PLAYER);
        InvalidateRect(hWnd, NULL, FALSE);

       
        KillTimer(hWnd, 3);
        if (getPlayer().getPlayerHp() <= 0)
        {
            MessageBox(NULL, L"����ϼ̽��ϴ�", L"����", MB_OK);
            PostQuitMessage(0);
            DestroyWindow(hWnd);
        }
        break;

    }
}

bool GameManager::checkAround(const POINT& p) const
{
    // �����¿� �� ������ ��Ÿ���� �迭
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    for (int i = 0; i < 4; ++i)
    {
        POINT neighbor = { p.x + dx[i], p.y + dy[i] };

        // ���� ��踦 ����� �ʴ��� Ȯ��
        if (neighbor.x >= 0 && neighbor.x < MAP_COLS && neighbor.y >= 0 && neighbor.y < MAP_ROWS)
        {
            // �����¿� 1ĭ ���� target�� �ִ��� Ȯ��
            if (map.getMapData(neighbor) == MapDataType::PLAYER)
            {
                return true; // ����� �߰ߵǸ� true ��ȯ
            }
        }
    }
    return false; // ����� ������ false ��ȯ
}

void GameManager::Test_DebugMob()
{
}
