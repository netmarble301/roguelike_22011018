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

//나중에 이것을 싱글톤으로 구현?
//알고리즘 리뷰 계속 하기 아직 완벽하게 이해 못함

//이 A*알고리즘은 상하좌우만(4방향)만 이동한다고 가정합니다
//맵 나중에 템플릿으로 적용
// 0 : 이동 가능한 통로
// 1 : 이동 불가능한 벽

//unordered_map을 위한 POINT의 해시 함수를 std::hash에 특수화?
namespace std {
	template <>
	struct hash<POINT> {
		size_t operator()(const POINT& p) const {
			// 간단한 해시 조합
			return hash<long>()(p.x) ^ (hash<long>()(p.y) << 1);
		}
	};
}

struct Node {
	POINT p; //현재 노드의 좌표
	int g; //시작점에서 현재 노드까지의 실제 비용
	int h; //현재 노드에서 목표점까지의 추정 비용
	int f; //g+h
	int random; //만약 f값이 같을 때 랜덤 우선순위
	Node* parent; //경로 재구성을 위한 부모 노드 포인터
	Node(POINT _p, int _g, int _h, int _random, Node* _pNode = nullptr) : p(_p), g(_g), h(_h), f(_g + _h), random(_random), parent(_pNode) {}

	//우선순위 큐를 위한 비교 연산자 오버로드
	//f값이 낮은 순서대로 정렬. f값이 같으면 random값이 낮은 순서대로 정렬
	bool operator>(const Node& _other) const
	{
		if (f != _other.f)
		{
			return f > _other.f;
		}
		//f값이 같을 경우 random값을 비교 (낮을수록 우선)
		return random > _other.random;
	}
};

class A_Star_Algorithm
{
private:
	mt19937 gen; //랜덤 넘버 제너레이터 엔진
	int calculateHeuristic(POINT _p1, POINT _p2);
	int getRandomTieBreaker();
	vector<POINT> reconstructPath(Node* _endNode);
public:
	A_Star_Algorithm();
	virtual ~A_Star_Algorithm() = default;

	vector<POINT> findPath(const GameMap& _map, POINT _start, POINT _end); //최단 경로 반환

	// 7/29 콜백함수? 템플릿, 장애물 무엇으로 기준을 잡을 건지 결정?
};

