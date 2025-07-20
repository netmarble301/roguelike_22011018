#pragma once
#include "Monster.h"
#include <memory>
using namespace std;

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