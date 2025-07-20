#pragma once
#include <windows.h>
using namespace std;

class Monster //추상 클래스
{
private:
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
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

