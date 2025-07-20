#pragma once
#include "GameMap.h"
#include "Player.h"
#include "MonsterFactory.h"

class GameManager
{
private:
    GameMap map;
    Player player;
    vector<unique_ptr<Monster>> monsters;
    int currentFloor;

public:
    GameManager();
    virtual ~GameManager();

    void initializeGame();
    void nextFloor();

    //get set
    GameMap& getMap();
    Player& getPlayer();
    vector<unique_ptr<Monster>>& getMonsters();
    int getCurrentFloor() const;
    void setCurrentFloor(int _floor);
};

