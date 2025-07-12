#pragma once
#include "GameMap.h"
#include "Player.h"
#include "Monster.h"
#include <vector>
#include <memory>
using namespace std;

class GameManager
{
private:
    GameMap map;
    Player player;
    vector<unique_ptr<Monster>> monsters;
    // 7/12 æ∆¿Ã≈€
    int currentFloor;
public:
    GameManager();
    void initializeGame();
    void nextFloor();

    //get set
    GameMap& getMap();
    Player& getPlayer();
    vector<unique_ptr<Monster>>& getMonsters();
    int getCurrentFloor() const;
    void setCurrentFloor(int _floor);
};

