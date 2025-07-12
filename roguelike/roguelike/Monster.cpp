#include "Monster.h"

Monster::Monster(POINT _p, int _hp, int _attack, int _defense, bool _turn) : monsterPoint(_p), monsterHp(_hp), monsterAttack(_attack), monsterDefense(_defense), monsterTurn(_turn)
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

bool Monster::getMonsterTurn() const
{
	return monsterTurn;
}

void Monster::setMonsterTurn(bool _turn)
{
	monsterTurn = _turn;
}

Sphinx::Sphinx(POINT _p, int _hp, int _attack, int _defense, bool _turn) : Monster(_p, _hp, _attack, _defense, _turn)
{
	setMonsterName("Sphinx");
}

int Sphinx::monsterBasicAttack()
{
}

void Sphinx::monsterSkill()
{
}

string Monster::getMonsterName() const
{
	return monsterName;
}

void Monster::setMonsterName(string _name)
{
	monsterName = _name;
}

Orc::Orc(POINT _p, int _hp, int _attack, int _defense, bool _turn) : Monster(_p, _hp, _attack, _defense, _turn)
{
	setMonsterName("Orc");
}

int Orc::monsterBasicAttack()
{
}

void Orc::monsterSkill()
{
}
