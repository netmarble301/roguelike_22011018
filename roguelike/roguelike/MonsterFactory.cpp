#include "MonsterFactory.h"

Monster* SphinxFactory::createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const
{
	return new Sphinx(_p, _hp, _attack, _defense, _turn);
}

Monster* OrcFactory::createMonster(POINT _p, int _hp, int _attack, int _defense, bool _turn) const
{
	return new Orc(_p, _hp, _attack, _defense, _turn);
}
