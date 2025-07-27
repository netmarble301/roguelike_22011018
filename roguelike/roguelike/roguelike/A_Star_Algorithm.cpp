#include "A_Star_Algorithm.h"

int A_Star_Algorithm::calculateHeuristic(POINT _p1, POINT _p2)
{
    return abs(_p1.x - _p2.x) + abs(_p1.y - _p2.y);
}

int A_Star_Algorithm::getRandomTieBreaker()
{
    uniform_int_distribution<> distrib(0, 1);
    return distrib(gen);
}

vector<POINT> A_Star_Algorithm::reconstructPath(Node* _endNode)
{
    vector<POINT> path;
    Node* current = _endNode;
    while (current != nullptr) 
    {
        path.push_back(current->p);
        current = current->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

A_Star_Algorithm::A_Star_Algorithm() : gen(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
}

vector<POINT> A_Star_Algorithm::findPath(const GameMap& _map, POINT _start, POINT _end)
{
    int rows = MAP_ROWS;
    int cols = MAP_COLS;

    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    unordered_map<POINT, Node*> visitedNodes;

    //시작 노드 생성 시 랜덤 random값 할당
    Node* startNode = new Node(_start, 0, calculateHeuristic(_start, _end), getRandomTieBreaker());
    openSet.push(*startNode);
    visitedNodes[_start] = startNode; //POINT 객체를 바로 키로 사용

    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    vector<Node*> allocatedNodes; //동적 할당된 노드를 추적하기 위한 벡터
    allocatedNodes.push_back(startNode);

    while (!openSet.empty()) 
    {
        Node currentNode = openSet.top();
        openSet.pop();

        if (currentNode.p.x == _end.x && currentNode.p.y == _end.y) 
        {
            vector<POINT> path = reconstructPath(&currentNode);
            //모든 동적 할당된 노드 메모리 해제
            for (Node* node : allocatedNodes) 
            {
                delete node;
            }
            return path;
        }

        for (int i = 0; i < 4; ++i) 
        {
            POINT neighborPos = { currentNode.p.x + dc[i], currentNode.p.y + dr[i] };

            if (neighborPos.x < 0 || neighborPos.x >= cols || neighborPos.y < 0 || neighborPos.y >= rows) 
            {
                continue;
            }

            if (_map.getMapData(neighborPos) == (int)MapDataType::WALL)
            {
                continue;
            }

            int newG = currentNode.g + 1;

            if (visitedNodes.count(neighborPos)) //해당 POINT가 이미 있는지 확인
            {
                Node* existingNode = visitedNodes[neighborPos]; //있다면 가져옴
                if (newG < existingNode->g) 
                {
                    existingNode->g = newG;
                    existingNode->f = newG + existingNode->h;
                    existingNode->parent = visitedNodes[currentNode.p]; //부모 노드도 POINT로 접근
                    existingNode->random = getRandomTieBreaker();
                    openSet.push(*existingNode);
                }
            }
            else 
            {
                //새로운 노드 추가
                Node* neighborNode = new Node(neighborPos, newG, calculateHeuristic(neighborPos, _end), getRandomTieBreaker(), visitedNodes[currentNode.p]);
                openSet.push(*neighborNode);
                visitedNodes[neighborPos] = neighborNode; //새로운 POINT-Node* 쌍 삽입
                allocatedNodes.push_back(neighborNode);
            }
        }
    }

    for (Node* node : allocatedNodes) 
    {
        delete node;
    }
    return {};
}
