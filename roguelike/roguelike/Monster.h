#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Monster //추상 클래스
{
private:
	string monsterType; //게임 로그 이름 출력용
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
public:
	Monster(POINT _p, int _hp, int _attack, int _defense);
	virtual ~Monster() = default;
	virtual int monsterBasicAttack() = 0; //순수 가상 함수1 기본공격
	virtual int monsterSkill() = 0; //순수 가상 함수2 스킬

	string getMonsterType() const;
	void setMonsterType(string _type);
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
	// 7/11 sphinx 기본공격, 스킬 어떻게 할지 고민하기
	int monsterBasicAttack() override;
	int monsterSkill() override;
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense);
	// 7/11 오크 기본공격, 스킬 어떻게 할지 고민하기
	int monsterBasicAttack() override;
	int monsterSkill() override;
};

