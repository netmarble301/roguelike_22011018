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



class Monster// : public Actor //추상 클래스
{

//private:
//	Actor* m_ActorData;

private:
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
	MapDataType monsterNum;
	POINT movePoint; //랜덤 이동할 좌표

	mt19937 rng;
public:
	Monster(POINT _p, int _hp, int _attack, int _defense);
	virtual ~Monster() = default;
	virtual int monsterAction() = 0; //순수 가상 함수

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
	int monsterAction() override; //기본 공격 or 스킬 랜덤 적용
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense);
	int monsterAction() override; //기본 공격 or 스킬 랜덤 적용
};

