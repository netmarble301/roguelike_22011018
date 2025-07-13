#include "GameManager.h"
#include "MonsterFactory.h"

GameManager::GameManager() :map(), player(map.getPlayerPosition(), 10, 2, 5), currentFloor(1)
{
}

void GameManager::initializeGame()
{
    map.generateMap();

    player.setPlayerPoint(map.getPlayerPosition());

    monsters.clear();

    //GameLogic.cpp�� createMonsterWithStats() �Լ� ���
    //���� ���� �ڵ� �ʹ� ��ȿ�����ΰ�? �ٽ� ����
    unique_ptr<MonsterFactory> sphinxFactory = make_unique<SphinxFactory>();
    unique_ptr<MonsterFactory> orcFactory = make_unique<OrcFactory>();
    char mapChar;
    for (int y = 0; y < map.getMapRows(); ++y) 
    {
        for (int x = 0; x < map.getMapCols(); ++x) 
        {
            POINT p = { x, y };
            mapChar = map.getMapData(p);
            if (mapChar == 'S')
            {
                string monsterType = "Sphinx";
                int hp = 10, attack = 3, defense = 2;
                monsters.push_back(sphinxFactory->createMonster(p, hp, attack, defense));
            }
            else if (mapChar == 'O')
            {
                string monsterType = "Orc";
                int hp = 8, attack = 2, defense = 1;
                monsters.push_back(orcFactory->createMonster(p, hp, attack, defense));
            }
        }
    }
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
