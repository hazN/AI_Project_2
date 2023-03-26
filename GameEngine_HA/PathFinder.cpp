#include "PathFinder.h"
#include <iostream>
#include <queue>
#include <unordered_map>

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
		//if (node.second->coord == a)
		//{
		//	nodes.at(b)->neighbours.push_back(node.second);
		//	//nodes[b]->neighbours.push_back(node.second);
		//}
	}
}

void PathFinder::CreatePathNode(Coord coord, const glm::vec3& position, char name)
{
	PathNode* newPathNode = new PathNode(name);
	newPathNode->point = position;
	newPathNode->id = m_Graph.nodes.size();
	//m_Graph.nodes.emplace(newPathNode);
	newPathNode->coord = coord;
	m_Graph.nodes.emplace(coord, newPathNode);
}
void PathFinder::CreatePathNode(Coord coord, const glm::vec3& position, char name, cMeshObject* mesh)
{
    PathNode* newPathNode = new PathNode(name);
    newPathNode->point = position;
    newPathNode->id = m_Graph.nodes.size();
    //m_Graph.nodes.emplace(newPathNode);
    newPathNode->coord = coord;
    newPathNode->mesh = mesh;
    m_Graph.nodes.emplace(coord, newPathNode);
}

// Euclidean distance
// Get distance between two, using this over manhattan since manhattan doesn't take diagonals into account
int getDistance(Coord a, Coord b) {
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    return sqrt(dx * dx + dy * dy);
}
//https://www.geeksforgeeks.org/a-search-algorithm/
std::vector<PathNode*> PathFinder::AStarSearch(Graph* graph, PathNode* start, PathNode* end)
{
    // Open and closed sets
    std::priority_queue<PathNode*, std::vector<PathNode*>, ComparePathNode> open;
    std::map<Coord, PathNode*> closed;

    // Add the start node
    start->distanceFromStart = 0;
    start->distanceFromEnd = getDistance(start->coord, end->coord);
    start->parent = nullptr;
    open.push(start);

    // Loop until end is found
    while (!open.empty())
    {
        // Pop lowest f node
        PathNode* curNode = open.top();
        open.pop();

        // Check if it has reached the end
        if (curNode == end)
        {
            // Create the path for the agent to follow
            std::vector<PathNode*> rPath;
            while (curNode != nullptr)
            {
                curNode->mesh->bUse_RGBA_colour = true;
                curNode->mesh->RGBA_colour = glm::vec4(0.f, 0.f, 1.f, 1.f);
                rPath.push_back(curNode);
                curNode = curNode->parent;
            }
            // Reset start/finish colours incase they got changed
           // m_StartNode->mesh->RGBA_colour = glm::vec4(0.f, 1.f, 0.f, 1.f);
           //// m_StartNode->mesh->RGBA_colour = glm::vec4(0.f, 1.f, 0.f, 1.f);
           // m_EndNode->mesh->RGBA_colour = glm::vec4(1.f, 0.f, 0.f, 1.f);
            // Reverse the order so it's start to finish
            std::vector<PathNode*> path(rPath.rbegin(), rPath.rend());
            return path;
        }

        // Check neighbours
        for (PathNode* neighbour : curNode->neighbours)
        {
            // Set it as the child of the current node
            if (closed.find(neighbour->coord) == closed.end())
            {
                neighbour->parent = curNode;
            }

            // Calculate distance from start and finish
            neighbour->distanceFromStart = curNode->distanceFromStart + getDistance(curNode->coord, neighbour->coord);
            neighbour->distanceFromEnd = getDistance(neighbour->coord, end->coord);

            // Check if it's in the closed set
            if (closed.find(neighbour->coord) != closed.end())
            {
                continue;
            }

            // Otherwise add it to the open
            open.push(neighbour);
        }

        // Add the current node to the closed set
        closed.emplace(curNode->coord, curNode);
    }

    // Return an empty path if the end node is not found
    return std::vector<PathNode*>();
}
