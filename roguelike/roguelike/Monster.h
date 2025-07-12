#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Monster //�߻� Ŭ����
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
	virtual int monsterBasicAttack() = 0; //���� ���� �Լ�1 �⺻����
	virtual void monsterSkill() = 0; //���� ���� �Լ�2 ��ų

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
	// 7/11 sphinx �⺻����, ��ų ��� ���� ����ϱ�
	int monsterBasicAttack() override;
	void monsterSkill() override;
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense, bool _turn);
	// 7/11 ��ũ �⺻����, ��ų ��� ���� ����ϱ�
	int monsterBasicAttack() override;
	void monsterSkill() override;
};

