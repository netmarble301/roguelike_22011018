#pragma once
#include "GameMap.h"


class Actor
{
protected:
	POINT WorldPos;
	int HP;
	int ATK;
	int DEF;
};



class Monster// : public Actor //�߻� Ŭ����
{

//private:
//	Actor* m_ActorData;

private:
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
	MapDataType monsterNum;
	POINT movePoint; //���� �̵��� ��ǥ

	mt19937 rng;
public:
	Monster(POINT _p, int _hp, int _attack, int _defense);
	virtual ~Monster() = default;
	virtual int monsterAction() = 0; //���� ���� �Լ�

	int getMonstertHp() const;
	void setMonsterHp(int _hp);
	int getMonsterAttack() const;
	void setMonsterAttack(int _attack);
	int getMonsterDefense() const;
	void setMonsterDefense(int _defense);
	POINT getMonsterPoint() const;
	void setMonsterPoint(POINT _p);
	MapDataType getMonsterNum() const;
	void setMonsterNum(MapDataType _num);

	POINT getMovePoint() const;
	void setMovePoint(POINT _mp);
};

class Sphinx : public Monster
{
public:
	Sphinx(POINT _p, int _hp, int _attack, int _defense);
	int monsterAction() override; //�⺻ ���� or ��ų ���� ����
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense);
	int monsterAction() override; //�⺻ ���� or ��ų ���� ����
};

