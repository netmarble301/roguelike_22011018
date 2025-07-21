#pragma once
#include "GameMap.h"

class Monster //�߻� Ŭ����
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
	virtual int monsterSkill() = 0; //���� ���� �Լ�, ��ų
	virtual void monsterMove(GameMap& _map); //���� �Լ�, �����̵�
	virtual void monsterChase(GameMap& _map); //���� �Լ�, �߰�
	virtual int monsterBasicAttack(GameMap& _map); //���� �Լ�, �⺻ ����
	virtual bool searchPlayer(GameMap& _map); //���� �Լ�, �÷��̾� Ž�� 5x5(�߰ݰ� ����)

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
	int monsterSkill() override; //�⺻ ���� or ��ų ���� ����
};

class Orc : public Monster
{
public:
	Orc(POINT _p, int _hp, int _attack, int _defense);
	int monsterSkill() override; //�⺻ ���� or ��ų ���� ����
};

