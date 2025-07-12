#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Monster //추상 클래스
{
private:
	string monsterName;
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
	bool monsterTurn;
public:
	Monster(POINT _p, int _hp, int _attack, int _defense, bool _turn);
	virtual ~Monster() = default;
	virtual int monsterBasicAttack() = 0; //순수 가상 함수1 기본공격
	virtual void monsterSkill() = 0; //순수 가상 함수2 스킬

	string getMonsterName() const;
	void setMonsterName(string _name);
	int getMonstertHp() const;
	void setMonsterHp(int _hp);
	int getMonsterAttack() const;
	void setMonsterAttack(int _attack);
	int getMonsterDefense() const;
	void setMonsterDefense(int _defense);
	POINT getMonsterPoint() const;
	void setMonsterPoint(POINT _p);
	bool getMonsterTurn() const;
	void setMonsterTurn(bool _turn);
};

class Sphinx : public Monster
{
public:
	Sphinx(POINT _p, int _hp, int _attack, int _defense, bool _turn);
	// 7/11 sphinx 기본공격, 스킬 어떻게 할지 고민하기
	int monsterBasicAttack() override;
	void monsterSkill() override;
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense, bool _turn);
	// 7/11 오크 기본공격, 스킬 어떻게 할지 고민하기
	int monsterBasicAttack() override;
	void monsterSkill() override;
};

