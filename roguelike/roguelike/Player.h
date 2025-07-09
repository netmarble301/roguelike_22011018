#pragma once
#include <Windows.h>

class Player
{
protected:
	//int playerPointX;
	//int playerPointY;
	POINT playerPoint;
	int playerHp;
	int playerAttack;
	int playerDefense;
public:
	Player(POINT _p, int _hp, int _attack, int _defense);
	//int getPointX() const;
	//void setPointX(int _x);
	//int getPointY() const;
	//void setPointY(int _y);
	int getHp() const;
	void setHp(int _hp);
	int getAttack() const;
	void setAttack(int _attack);
	int getDefense() const;
	void setDefense(int _defense);

	POINT getPoint() const;
	void setPoint(POINT _p);
};

