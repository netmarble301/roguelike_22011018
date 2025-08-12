#pragma once
#include "GameMap.h"
//#include <vector>
#include <queue>
#include <unordered_map> //unordered_map
#include <cmath> //abs
//#include <random> //mt19937, uniform_int_distribution
//#include <chrono> //chrono::high_resolution_clock
//#include <algorithm> //reverse
#include <iostream>
#include <memory>
//using namespace std;

//���߿� �̰��� �̱������� ����?
//�˰��� ���� ��� �ϱ� ���� �Ϻ��ϰ� ���� ����

//�� A*�˰����� �����¿츸(4����)�� �̵��Ѵٰ� �����մϴ�
//�� ���߿� ���ø����� ����
// 0 : �̵� ������ ���
// 1 : �̵� �Ұ����� ��

//unordered_map�� ���� POINT�� �ؽ� �Լ��� std::hash�� Ư��ȭ?
namespace std {
	template <>
	struct hash<POINT> {
		size_t operator()(const POINT& p) const {
			// ������ �ؽ� ����
			return hash<long>()(p.x) ^ (hash<long>()(p.y) << 1);
		}
	};
}

struct Node {
	POINT p; //���� ����� ��ǥ
	int g; //���������� ���� �������� ���� ���
	int h; //���� ��忡�� ��ǥ�������� ���� ���
	int f; //g+h
	int random; //���� f���� ���� �� ���� �켱����
	Node* parent; //��� �籸���� ���� �θ� ��� ������
	Node(POINT _p, int _g, int _h, int _random, Node* _pNode = nullptr) : p(_p), g(_g), h(_h), f(_g + _h), random(_random), parent(_pNode) {}

	//�켱���� ť�� ���� �� ������ �����ε�
	//f���� ���� ������� ����. f���� ������ random���� ���� ������� ����
	bool operator>(const Node& _other) const
	{
		if (f != _other.f)
		{
			return f > _other.f;
		}
		//f���� ���� ��� random���� �� (�������� �켱)
		return random > _other.random;
	}
};

class A_Star_Algorithm
{
private:
	mt19937 gen; //���� �ѹ� ���ʷ����� ����
	int calculateHeuristic(POINT _p1, POINT _p2);
	int getRandomTieBreaker();
	vector<POINT> reconstructPath(Node* _endNode);
public:
	A_Star_Algorithm();
	virtual ~A_Star_Algorithm() = default;

	vector<POINT> findPath(const GameMap& _map, POINT _start, POINT _end); //�ִ� ��� ��ȯ

	// 7/29 �ݹ��Լ�? ���ø�, ��ֹ� �������� ������ ���� ���� ����?
};

