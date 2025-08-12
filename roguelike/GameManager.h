#pragma once
//#include <windows>
#include "Player.h"
#include "MonsterFactory.h"
#include "A_Star_Algorithm.h"
#include <winuser.h>

//상태 이건 나중에
enum class E_INGAME_STATETYPE
{
    IDLE,
    KEYINPUT,
    PLAYERMOVE,
    PLAYERMOVEEND,
    PLAYATTACK,
    //PLAY_

    MOBMOVE,
    MOBMOVEEND,
    MOBATTACK,

};


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

    void monsterTurn(HWND hWnd);
    bool searchPlayer(const POINT& _mp) const;
    void monsterMove(Monster& _m);
    void monsterChase(Monster& _m);

    POINT getRandomEmptyPoint() const;

    //void playerAttack();

    int UpdateKeyDown(WPARAM p_param, HWND hWnd);
    //void ResetPlayerKeyDown() { m_ISPlayerTurn = false; }
    void setTimer(WPARAM p_param, HWND hWnd);


    bool checkAround(const POINT& p) const;

protected:
    bool m_ISPlayerTurn = true;
    bool playerMove = false;
    bool playerAction = false;



protected:
    void Test_DebugMob();

};

