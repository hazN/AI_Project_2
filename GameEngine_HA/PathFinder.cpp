#include "PathFinder.h"

void PrintSet(const std::string& name, const std::vector<PathNode*>& set)
{
	printf("%s { ", name.c_str());

	for (int i = 0; i < set.size(); i++)
	{
		printf("%c ", set[i]->name);
	}

	printf("}\n");
}

std::vector<PathNode*>::iterator GetPathNodeWithShortestDistanceFromStart(std::vector<PathNode*>& set)
{
	std::vector<PathNode*>::iterator findIt;
	std::vector<PathNode*>::iterator shortestIt;
	float shortestDistance = FLT_MAX;
	for (findIt = set.begin(); findIt != set.end(); ++findIt)
	{
		if ((*findIt)->distanceFromStart < shortestDistance)
		{
			shortestDistance = (*findIt)->distanceFromStart;
			shortestIt = findIt;
		}
	}

	return shortestIt;
}

void Graph::SetNeighbours(Coord a, Coord b)
{
	for (std::pair<Coord, PathNode*> node : nodes)
	{
		if (node.second->coord == b)
		{
			nodes.at(a)->neighbours.push_back(node.second);
			//nodes[a]->neighbours.push_back(node.second);
		}
		if (node.second->coord == a)
		{
			nodes.at(b)->neighbours.push_back(node.second);
			//nodes[b]->neighbours.push_back(node.second);
		}
	}
}

void PathFinder::CreatePathNode(Coord coord, const glm::vec3& position, char name)
{
	PathNode* newPathNode = new PathNode(name);
	newPathNode->point = position;
	newPathNode->id = m_Graph.nodes.size();
	//m_Graph.nodes.emplace(newPathNode);
	m_Graph.nodes.emplace(coord, newPathNode);

	//GameObject* ball = GDP_CreateGameObject();
	//ball->Position = position;
	//ball->Scale = glm::vec3(0.5f);
	//ball->Renderer.ShaderId = 1;
	//ball->Renderer.MeshId = m_SphereModelId;
	//ball->Renderer.MaterialId = m_UnvisitedMaterialId;
	//m_PathNodeGameObjects.push_back(ball);
}

bool PathFinder::AStarSearch(Graph* graph, PathNode* start, PathNode* end)
{
	//priority_queue<PathNode*, std::vector<PathNode*>, PathNode> open;
	std::vector<PathNode*> open;
	std::vector<PathNode*> closed;

	open.push_back(start);
	printf("Added %c to the open set!\n", start->name);
	while (!open.empty())
	{
		PrintSet("open:   ", open);
		PrintSet("closed: ", closed);

		std::vector<PathNode*>::iterator itX = GetPathNodeWithShortestDistanceFromStart(open);
		PathNode* X = *itX;
		open.erase(itX);

		printf("Removed %c from the open set!\n", X->name);

		if (X == end)
			return true;

		closed.push_back(X);
		printf("Added %c to the closed set!\n", X->name);

		printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
		for (int i = 0; i < X->neighbours.size(); i++)
		{
			PathNode* n = X->neighbours[i];
			printf("Found neighbour %c!\n", n->name);
			if (std::find(closed.begin(), closed.end(), n) == closed.end())
			{
				continue;
			}
			else if (std::find(open.begin(), open.end(), n) != open.end())
			{
				// Distance from n to it's parent + distance from parent to start
				float distanceFromStartToN = glm::distance(n->point, X->point) + X->distanceFromStart;
				if (distanceFromStartToN < n->distanceFromStart)
				{
					n->parent = X;
					n->distanceFromStart = distanceFromStartToN;
				}
			}
			else
			{
				open.insert(open.begin(), n);
				n->parent = X;
				printf("Added %c to the open set!\n", n->name);
			}
		}
	}
}