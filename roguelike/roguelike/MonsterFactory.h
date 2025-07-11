#pragma once
#include "Monster.h"

// 7/11
#include <memory> // unique_ptr 스마트포인터로 수정하기!!!!!

class MonsterFactory //인터페이스
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
//클라이언트 코드 구현하기! 팩토리 메소드를 통해 객체를 생성하고 사용하는 부분
//특정 클래스의 멤버 함수로 구현할까? 전역 함수로 구현할까? 고민;;
//일단 전역함수로 GameLogic.cpp에 구현하자(나중에 옮겨)
/*
Monster* clientCode(const MonsterFactory& _factory) //_factory는 생성한 sphinxfactory 또는 orcfactory가 들어감(즉 OrcFactory orcFactory; SphinxFactory sphinxFactory; 각각 생성해 줄 필요가 있음)
{
    int hp, attack, defense;
    bool turn;
    POINT p;
    //무슨 팩토리 선택했는지에 따라 if문으로 몬스터의 초기 위치, 초기 스탯 정의
    //if(sphinx) if(orc)

    // 팩토리로부터 몬스터 객체를 생성 (메모리 동적 할당)
    Monster* monster = _factory.createMonster(p, hp, attack, defense, turn);

    return monster;

    // **중요: new로 할당된 메모리는 반드시 나중에 delete로 해제**
    //아래처럼 사용 후 반드시 까먹지 말고 delete!
    //delete monster;
    //monster = nullptr;
    //만약 unique_ptr(스마트포인터) 쓰면, 함수 종료 시 monster 객체는 자동으로 소멸 다음 수정 때 이걸로 수정
}
*/