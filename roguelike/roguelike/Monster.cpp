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
	setMonsterType("Sphinx");
}

int Sphinx::monsterBasicAttack()
{
	//게임로그 추가
	//플레이어가 이동 공격 후 바로 반응
	return 0; //나중에 수정
}

int Sphinx::monsterSkill()
{
	//게임로그 추가
	return 0; //나중에 수정
}

string Monster::getMonsterType() const
{
	return monsterType;
}

void Monster::setMonsterType(string _type)
{
	monsterType = _type;
}

Orc::Orc(POINT _p, int _hp, int _attack, int _defense) : Monster(_p, _hp, _attack, _defense)
{
	setMonsterType("Orc");
}

int Orc::monsterBasicAttack()
{
	//게임로그 추가
	return 0; //나중에 수정
}

int Orc::monsterSkill()
{
	//게임로그 추가
	return 0; //나중에 수정
}
