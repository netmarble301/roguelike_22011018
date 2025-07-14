#pragma once
#include "GameMap.h"
#include "Player.h"
#include "Monster.h"
#include <vector>
#include <memory>
using namespace std;

//여기서 몬스터 종류 결정?
#include <random> //uniform_int_distribution, shuffle()
#include <chrono> //난수 시드 생성

class GameManager
{
private:
    GameMap map;
    Player player;
    vector<unique_ptr<Monster>> monsters;
    // 7/12 아이템
    int currentFloor;

    mt19937 rng;
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

