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
            //if�� 1ĭ ���� �� ���� or �߰�
        }
        else
        {
            monsterMove(*monster_ptr);
        }
    }
}

bool GameManager::searchPlayer(const POINT& _mp) const
{
    POINT playerPoint = player.getPlayerPoint(); //�÷��̾��� ���� ��ġ

    //���Ϳ� �÷��̾� ������ �Ÿ� ��� (����ư �Ÿ�)
    int distanceX = abs(_mp.x - _mp.x);
    int distanceY = abs(_mp.y - _mp.y);

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
    POINT currentMonsterPoint = _m.getMonsterPoint();
    POINT targetMovePoint = _m.getMovePoint();

    //���Ͱ� �̹� ��ǥ ������ �����ߴٸ� ���ο� ��ǥ ���� ����
    if (currentMonsterPoint.x == targetMovePoint.x && currentMonsterPoint.y == targetMovePoint.y)
    {
        _m.setMovePoint(getRandomEmptyPoint());
        targetMovePoint = _m.getMovePoint(); //���ο� ��ǥ �������� ������Ʈ
    }

    //A* �˰����� findPath�� GameMap ��ü�� ������ �޽��ϴ�.
    vector<POINT> path = aStarAlgorithm.findPath(map, currentMonsterPoint, targetMovePoint);

    //��ΰ� �����ϰ� �̵��� ���� ĭ�� �ִٸ�
    //path.size() > 0 �̸� ���������� �̹� ������ ��� (����� ���̰� 1)�̹Ƿ�
    //path.size() > 1 �̾�� ������ �� ĭ �̵��� �� �ֽ��ϴ�.
    if (path.size() > 1)
    {
        POINT nextPos = path[1]; //����� �� ��° ��尡 ���� �̵��� ��ġ


        //���� ��ġ�� �÷��̾� �� ��ġ�� ������? �ϴ� ���� ������ searchPlayer->monsterChase ������ ���� �� �ص� ��
        if (nextPos.x == player.getPlayerPoint().x && nextPos.y == player.getPlayerPoint().y)
        {
            return;
        }


        //�ʿ��� ������ ���� ��ġ�� 0
        map.setMapData(currentMonsterPoint, (int)MapDataType::EMPTY);

        //������ ���� ��ġ ������Ʈ
        _m.setMonsterPoint(nextPos);

        //�ʿ��� ������ �� ��ġ�� �ش� ���� Ÿ������ ����
        //Monster Ŭ������ `monsterNum` ����� ����Ͽ� MapDataType�� ���������Ƿ� �̸� Ȱ���մϴ�.
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
        int mapData = map.getMapData(p);
        if (mapData == (int)MapDataType::EMPTY)
        {
            return p;
        }
    }
}
