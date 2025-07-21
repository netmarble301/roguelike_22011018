#pragma once
#include "GameMap.h"

class Monster //추상 클래스
{
private:
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
	int monsterNum = -1;

	mt19937 rng;
public:
	Monster(POINT _p, int _hp, int _attack, int _defense);
	virtual ~Monster() = default;
	virtual int monsterSkill() = 0; //순수 가상 함수, 스킬
	virtual void monsterMove(GameMap& _map); //가상 함수, 랜덤이동
	virtual void monsterChase(GameMap& _map); //가상 함수, 추격
	virtual int monsterBasicAttack(GameMap& _map); //가상 함수, 기본 공격
	virtual bool searchPlayer(GameMap& _map); //가상 함수, 플레이어 탐색 5x5(추격과 연계)

	int getMonstertHp() const;
	void setMonsterHp(int _hp);
	int getMonsterAttack() const;
	void setMonsterAttack(int _attack);
	int getMonsterDefense() const;
	void setMonsterDefense(int _defense);
	POINT getMonsterPoint() const;
	void setMonsterPoint(POINT _p);
	int getMonsterNum() const;
	void setMonsterNum(int _num);
};

class Sphinx : public Monster
{
public:
	Sphinx(POINT _p, int _hp, int _attack, int _defense);
	int monsterSkill() override; //기본 공격 or 스킬 랜덤 적용
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense);
	int monsterSkill() override; //기본 공격 or 스킬 랜덤 적용
};

