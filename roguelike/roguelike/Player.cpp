#include "Player.h"

Player::Player(POINT _p, int _hp, int _attack, int _defense, bool _turn) : playerPoint(_p), playerHp(_hp), playerAttack(_attack), playerDefense(_defense), playerTurn(_turn)
{
}

int Player::getPlayerHp() const
{
    return playerHp;
}

void Player::setPlayerHp(int _hp)
{
    playerHp = _hp;
}

int Player::getPlayerAttack() const
{
    return playerHp;
}

void Player::setPlayerAttack(int _attack)
{
    playerAttack = _attack;
}

int Player::getPlayerDefense() const
{
    return playerDefense;
}

void Player::setPlayerDefense(int _defense)
{
    playerDefense = _defense;
}

POINT Player::getPlayerPoint() const
{
    return playerPoint;
}

void Player::setPlayerPoint(POINT _p)
{
    playerPoint = _p;
}

bool Player::getPlayerTurn() const
{
    return playerTurn;
}

void Player::setPlayerTurn(bool _turn)
{
    playerTurn = _turn;
}
