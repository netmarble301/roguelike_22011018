#include "Monster.h"

Monster::Monster(POINT _p, int _hp, int _attack, int _defense) : monsterPoint(_p), monsterHp(_hp), monsterAttack(_attack), monsterDefense(_defense), rng(chrono::system_clock::now().time_since_epoch().count())
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

int Monster::getMonsterNum() const
{
	return monsterNum;
}

void Monster::setMonsterNum(int _num)
{
	monsterNum = _num;
}

Sphinx::Sphinx(POINT _p, int _hp, int _attack, int _defense) : Monster(_p, _hp, _attack, _defense)
{
	setMonsterNum(4);
}

int Sphinx::monsterSkill()
{
	return 0;
}

void Monster::monsterMove(GameMap& _map)
{
	if (getMonstertHp() <= 0)
	{
		//»ç¸Á
		return;
	}

	uniform_int_distribution<int> monsterDirection(0, 3);
	int direction;
	POINT p = getMonsterPoint();
	_map.setMapData(p, 0);
	while (true)
	{
		direction = monsterDirection(rng);
		switch (direction)
		{
		case 0: //¿ÞÂÊ
			--p.x;
			break;
		case 1: //¿À¸¥ÂÊ
			++p.x;
			break;
		case 2: //À§ÂÊ
			--p.y;
			break;
		case 3: //¾Æ·¡ÂÊ
			++p.y;
			break;
		}
		if (_map.getMapData(p) == 0)
		{
			setMonsterPoint(p);
			_map.setMapData(p, getMonsterNum());
			break;
		}
		switch (direction) //p º¹±Í
		{
		case 0:
			++p.x;
			break;
		case 1:
			--p.x;
			break;
		case 2:
			++p.y;
			break;
		case 3:
			--p.y;
			break;
		}
	}

}

void Monster::monsterChase(GameMap& _map)
{

}

int Monster::monsterBasicAttack(GameMap& _map)
{
	return 0;
}

bool Monster::searchPlayer(GameMap& _map)
{
	return (_map.getPlayerPoint().x >= getMonsterPoint().x - 2 && _map.getPlayerPoint().x <= getMonsterPoint().x + 2 && _map.getPlayerPoint().y >= getMonsterPoint().y - 2 && _map.getPlayerPoint().y <= getMonsterPoint().y + 2);
}

Orc::Orc(POINT _p, int _hp, int _attack, int _defense) : Monster(_p, _hp, _attack, _defense)
{
	setMonsterNum(5);
}

int Orc::monsterSkill()
{
	return 0;
}
