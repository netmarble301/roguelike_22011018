#pragma once
#include "Monster.h"
#include <memory> //unique_ptr(���� ������), shared_ptr(���� ������, �� �ϳ��� ��ü�� ���� �����Ͱ� ���ÿ� ������ �� �ֵ���(���� ���� ����)), make_unique(unique_ptr ��ü�� �����ϴ� ����Ǵ� ���, new��� ���)
using namespace std; //�̰� ���� ���� std:: �̷��� �ұ�?

//�Լ�(����Լ�) �ڿ� const ���� : �ڽ��� ���� Ŭ������ ��������� �������� �ʰڴ�~ �̰� ����

class MonsterFactory //�������̽�
{
public:
	virtual ~MonsterFactory() = default;
	virtual unique_ptr<Monster> createMonster(POINT _p, int _hp, int _attack, int _defense) const = 0;
};

class SphinxFactory : public MonsterFactory
{
public:
    unique_ptr<Monster> createMonster(POINT _p, int _hp, int _attack, int _defense) const override;
};

class OrcFactory : public MonsterFactory
{
public:
    unique_ptr<Monster> createMonster(POINT _p, int _hp, int _attack, int _defense) const override;
};
