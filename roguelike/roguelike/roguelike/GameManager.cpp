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
    for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
    {
        if (1) //그냥 랜덤 이동, 3x3범위에 플레이어 발견 및 추격, 1칸 근접한 플레이어 기본 공격 또는 스킬
        {
            //~
        }
        (*iter)->monsterMove(getMap()); //일단 실험용 모두 계속 랜덤 이동하도록
    }
}
