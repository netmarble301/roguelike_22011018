#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Monster //�߻� Ŭ����
{
private:
	string monsterType; //���� �α� �̸� ��¿�
	POINT monsterPoint;
	int monsterHp;
	int monsterAttack;
	int monsterDefense;
public:
	Monster(POINT _p, int _hp, int _attack, int _defense);
	virtual ~Monster() = default;
	virtual int monsterBasicAttack() = 0; //���� ���� �Լ�1 �⺻����
	virtual int monsterSkill() = 0; //���� ���� �Լ�2 ��ų

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
	// 7/11 sphinx �⺻����, ��ų ��� ���� ����ϱ�
	int monsterBasicAttack() override;
	int monsterSkill() override;
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense);
	// 7/11 ��ũ �⺻����, ��ų ��� ���� ����ϱ�
	int monsterBasicAttack() override;
	int monsterSkill() override;
};

