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

    //���� ��� ���� �� ���� random�� �Ҵ�
    Node* startNode = new Node(_start, 0, calculateHeuristic(_start, _end), getRandomTieBreaker());
    openSet.push(*startNode);
    visitedNodes[_start] = startNode; //POINT ��ü�� �ٷ� Ű�� ���

    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    vector<Node*> allocatedNodes; //���� �Ҵ�� ��带 �����ϱ� ���� ����
    allocatedNodes.push_back(startNode);

    while (!openSet.empty()) 
    {
        Node currentNode = openSet.top();
        openSet.pop();

        if (currentNode.p.x == _end.x && currentNode.p.y == _end.y) 
        {
            vector<POINT> path = reconstructPath(&currentNode);
            //��� ���� �Ҵ�� ��� �޸� ����
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

            if (visitedNodes.count(neighborPos)) //�ش� POINT�� �̹� �ִ��� Ȯ��
            {
                Node* existingNode = visitedNodes[neighborPos]; //�ִٸ� ������
                if (newG < existingNode->g) 
                {
                    existingNode->g = newG;
                    existingNode->f = newG + existingNode->h;
                    existingNode->parent = visitedNodes[currentNode.p]; //�θ� ��嵵 POINT�� ����
                    existingNode->random = getRandomTieBreaker();
                    openSet.push(*existingNode);
                }
            }
            else 
            {
                //���ο� ��� �߰�
                Node* neighborNode = new Node(neighborPos, newG, calculateHeuristic(neighborPos, _end), getRandomTieBreaker(), visitedNodes[currentNode.p]);
                openSet.push(*neighborNode);
                visitedNodes[neighborPos] = neighborNode; //���ο� POINT-Node* �� ����
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
