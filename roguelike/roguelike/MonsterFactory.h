#pragma once
#include "Monster.h"
#include <memory> //unique_ptr(독점 소유권), shared_ptr(공유 소유권, 즉 하나의 객체를 여러 포인터가 동시에 소유할 수 있도록(연속 참조 가능)), make_unique(unique_ptr 객체를 생성하는 권장되는 방법, new대신 사용)
using namespace std; //이거 쓰지 말고 std:: 이렇게 할까?

//함수(멤버함수) 뒤에 const 오해 : 자신의 속한 클래스의 멤버변수를 변경하지 않겠다~ 이게 맞음

class MonsterFactory //인터페이스
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
