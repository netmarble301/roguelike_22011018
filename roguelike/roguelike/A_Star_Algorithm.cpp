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
    unordered_map<POINT, unique_ptr<Node>> visitedNodes;

    //시작 노드 생성 시 랜덤 random값 할당
    auto startNode = make_unique<Node>(_start, 0, calculateHeuristic(_start, _end), getRandomTieBreaker());
    openSet.push(*startNode);
    visitedNodes[_start] = move(startNode);; //POINT 객체를 바로 키로 사용

    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    while (!openSet.empty()) 
    {
        Node topNode = openSet.top();
        openSet.pop();

        Node* currentNode = visitedNodes[topNode.p].get();

        if (topNode.g > currentNode->g)
        {
            continue;
        }

        if (currentNode->p.x == _end.x && currentNode->p.y == _end.y) 
        {
            vector<POINT> path = reconstructPath(currentNode);
            for (auto& pair : visitedNodes)
            {
                pair.second.reset();
            }
            visitedNodes.clear();
            return path;
        }

        for (int i = 0; i < 4; ++i) 
        {
            POINT neighborPos = { currentNode->p.x + dc[i], currentNode->p.y + dr[i] };

            if (neighborPos.x < 0 || neighborPos.x >= cols || neighborPos.y < 0 || neighborPos.y >= rows) 
            {
                continue;
            }

            //함수포인터,callback 7/29
            if (_map.getMapData(neighborPos) == MapDataType::WALL || _map.getMapData(neighborPos) == MapDataType::ORC || _map.getMapData(neighborPos) == MapDataType::SPHINX || _map.getMapData(neighborPos) == MapDataType::FLOOR)
            {
                continue;
            }

            int newG = currentNode->g + 1;

            auto it = visitedNodes.find(neighborPos);

            if (it != visitedNodes.end()) //해당 POINT가 이미 있는지 확인
            {
                Node* existingNode = it->second.get();
                if (newG < existingNode->g) 
                {
                    existingNode->g = newG;
                    existingNode->f = newG + existingNode->h;
                    existingNode->parent = currentNode;
                    existingNode->random = getRandomTieBreaker();
                    openSet.push(*existingNode);
                }
            }
            else 
            {
                //새로운 노드 추가
                auto neighborNode = make_unique<Node>(neighborPos, newG, calculateHeuristic(neighborPos, _end), getRandomTieBreaker(), currentNode);
                openSet.push(*neighborNode);
                visitedNodes[neighborPos] = move(neighborNode); //unique_ptr을 이동하여 소유권 이전
            }
        }
    }


    return {};
}
