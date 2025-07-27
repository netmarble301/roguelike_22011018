#pragma once
#include "Player.h"
#include "MonsterFactory.h"
#include "A_Star_Algorithm.h"

class GameManager
{
private:
    GameMap map;
    Player player;
    vector<unique_ptr<Monster>> monsters;
    int currentFloor;

    A_Star_Algorithm aStarAlgorithm;
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

    void monsterTurn();
    bool searchPlayer(const POINT& _mp) const;
    void monsterMove(Monster& _m);
    void monsterChase();

    POINT getRandomEmptyPoint() const;
};

