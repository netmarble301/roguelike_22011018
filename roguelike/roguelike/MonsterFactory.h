#pragma once
#include "Monster.h"
#include <memory> //unique_ptr(���� ������), shared_ptr(���� ������, �� �ϳ��� ��ü�� ���� �����Ͱ� ���ÿ� ������ �� �ֵ���(���� ���� ����)), make_unique(unique_ptr ��ü�� �����ϴ� ����Ǵ� ���)
using namespace std; //�̰� ���� ���� std:: �̷��� �ұ�?

class MonsterFactory //�������̽�
{
public:
	virtual ~MonsterFactory() = default;
	virtual unique_ptr<Monster> createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const = 0;
};

class SphinxFactory : public MonsterFactory
{
public:
    unique_ptr<Monster> createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const override;
};

class OrcFactory : public MonsterFactory
{
public:
    unique_ptr<Monster> createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const override;
};
