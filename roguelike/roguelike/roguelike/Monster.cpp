#include "Monster.h"

Monster::Monster(POINT _p, int _hp, int _attack, int _defense) : monsterPoint(_p), monsterHp(_hp), monsterAttack(_attack), monsterDefense(_defense)
{
}

int Monster::getMonstertHp() const
{
	return monsterHp;
}

void Monster::setMonsterHp(int _hp)
{
	monsterHp = _hp;
}

int Monster::getMonsterAttack() const
{
	return monsterAttack;
}

void Monster::setMonsterAttack(int _attack)
{
	monsterAttack = _attack;
}

int Monster::getMonsterDefense() const
{
	return monsterDefense;
}

void Monster::setMonsterDefense(int _defense)
{
	monsterDefense = _defense;
}

POINT Monster::getMonsterPoint() const
{
	return monsterPoint;
}

void Monster::setMonsterPoint(POINT _p)
{
	monsterPoint = _p;
}

Sphinx::Sphinx(POINT _p, int _hp, int _attack, int _defense) : Monster(_p, _hp, _attack, _defense)
{
}

int Sphinx::monsterAction()
{
	return 0;
}

Orc::Orc(POINT _p, int _hp, int _attack, int _defense) : Monster(_p, _hp, _attack, _defense)
{
}

int Orc::monsterAction()
{
	return 0;
}
