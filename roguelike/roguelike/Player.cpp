#include "Player.h"

Player::Player(POINT _p, int _hp, int _attack, int _defense) : playerPoint(_p), playerHp(_hp), playerAttack(_attack), playerDefense(_defense)
{
}

/*
int Player::getPointX() const
{
    return playerPointX;
}

void Player::setPointX(int _x)
{
    playerPointX = _x;
}

int Player::getPointY() const
{
    return playerPointY;
}

void Player::setPointY(int _y)
{
    playerPointY = _y;
}
*/

int Player::getHp() const
{
    return playerHp;
}

void Player::setHp(int _hp)
{
    playerHp = _hp;
}

int Player::getAttack() const
{
    return playerHp;
}

void Player::setAttack(int _attack)
{
    playerAttack = _attack;
}

int Player::getDefense() const
{
    return playerDefense;
}

void Player::setDefense(int _defense)
{
    playerDefense = _defense;
}

POINT Player::getPoint() const
{
    return playerPoint;
}

void Player::setPoint(POINT _p)
{
    playerPoint = _p;
}
