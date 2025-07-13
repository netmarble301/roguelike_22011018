#include "MonsterFactory.h"

unique_ptr<Monster> SphinxFactory::createMonster(POINT _p, int _hp, int _attack, int _defense) const
{
	return make_unique<Sphinx>(_p, _hp, _attack, _defense);
}

unique_ptr<Monster> OrcFactory::createMonster(POINT _p, int _hp, int _attack, int _defense) const
{
	return make_unique<Orc>(_p, _hp, _attack, _defense);
}
