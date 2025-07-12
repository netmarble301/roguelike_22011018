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

    for (int y = 0; y < map.getMapRows(); ++y) 
    {
        for (int x = 0; x < map.getMapCols(); ++x) 
        {
            POINT p = { x, y };
            if (map.getMapData(p) == 'M') 
            {
                //GameLogic.cpp의 createMonsterWithStats() 함수 대신 여기에
                string monsterType = (rand() % 2 == 0) ? "Sphinx" : "Orc";

                unique_ptr<MonsterFactory> factory;

                int hp, attack, defense;

                if (monsterType == "Sphinx") {
                    factory = make_unique<SphinxFactory>();
                    hp = 10, attack = 3, defense = 2;
                }
                else { 
                    factory = make_unique<OrcFactory>();
                    hp = 8, attack = 2, defense = 1;
                }

                monsters.push_back(factory->createMonster(p, hp, attack, defense, false));
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
