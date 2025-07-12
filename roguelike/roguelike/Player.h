#pragma once
#include <Windows.h>

class Player
{
private:
	POINT playerPoint;
	int playerHp;
	int playerAttack;
	int playerDefense;
	bool playerTurn;
public:
	Player(POINT _p, int _hp, int _attack, int _defense);
	int getPlayerHp() const;
	void setPlayerHp(int _hp);
	int getPlayerAttack() const;
	void setPlayerAttack(int _attack);
	int getPlayerDefense() const;
	void setPlayerDefense(int _defense);
	POINT getPlayerPoint() const;
	void setPlayerPoint(POINT _p);
	bool getPlayerTurn() const;
	void setPlayerTurn(bool _turn);

	// 7/11 �÷��̾� �⺻ ����, ��ų ����ϱ�

};

