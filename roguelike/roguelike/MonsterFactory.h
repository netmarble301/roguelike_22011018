#pragma once
#include "Monster.h"

// 7/11
#include <memory> // unique_ptr ����Ʈ�����ͷ� �����ϱ�!!!!!

class MonsterFactory //�������̽�
{
public:
	virtual ~MonsterFactory() = default;
	virtual Monster* createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const = 0;
};

class SphinxFactory : public MonsterFactory
{
public:
	Monster* createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const override;
};

class OrcFactory : public MonsterFactory
{
public:
	Monster* createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const override;
};

// 7/11
//Ŭ���̾�Ʈ �ڵ� �����ϱ�! ���丮 �޼ҵ带 ���� ��ü�� �����ϰ� ����ϴ� �κ�
//Ư�� Ŭ������ ��� �Լ��� �����ұ�? ���� �Լ��� �����ұ�? ���;;
//�ϴ� �����Լ��� GameLogic.cpp�� ��������(���߿� �Ű�)
/*
Monster* clientCode(const MonsterFactory& _factory) //_factory�� ������ sphinxfactory �Ǵ� orcfactory�� ��(�� OrcFactory orcFactory; SphinxFactory sphinxFactory; ���� ������ �� �ʿ䰡 ����)
{
    int hp, attack, defense;
    bool turn;
    POINT p;
    //���� ���丮 �����ߴ����� ���� if������ ������ �ʱ� ��ġ, �ʱ� ���� ����
    //if(sphinx) if(orc)

    // ���丮�κ��� ���� ��ü�� ���� (�޸� ���� �Ҵ�)
    Monster* monster = _factory.createMonster(p, hp, attack, defense, turn);

    return monster;

    // **�߿�: new�� �Ҵ�� �޸𸮴� �ݵ�� ���߿� delete�� ����**
    //�Ʒ�ó�� ��� �� �ݵ�� ����� ���� delete!
    //delete monster;
    //monster = nullptr;
    //���� unique_ptr(����Ʈ������) ����, �Լ� ���� �� monster ��ü�� �ڵ����� �Ҹ� ���� ���� �� �̰ɷ� ����
}
*/