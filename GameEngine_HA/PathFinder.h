#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <glad/glad.h>
#include "cMeshObject.h"
#include <map>

struct Coord
{
	int x;
	int y;
	bool operator== (const Coord& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator< (const Coord& rhs) const
	{
		return this->x < rhs.x && this->y < rhs.y;
	}
};
struct PathNode
{
	PathNode(char name)
		: name(name)
		, parent(nullptr)
	{
	}

	glm::vec3 point;
	Coord coord;
	PathNode* parent;
	int id;
	char name;
	float distanceFromStart;
	std::vector<PathNode*> neighbours;

	bool operator() (const PathNode* l, const PathNode* r) const 
	{
		return l->distanceFromStart > r->distanceFromStart;
	}
	bool operator< (const PathNode& rhs) const
	{
		return this->distanceFromStart < rhs.distanceFromStart;
	}
	PathNode() : name(' ') {}
};
struct Graph
{
	//PathNode* AddNode();
	void SetNeighbours(Coord a, Coord b);

	std::map<Coord,PathNode*> nodes;

	//std::vector<PathNode*> nodes;
	//void SetNeighbours(PathNode* a, PathNode* b);
};

class PathFinder
{
public:
	void CreatePathNode(Coord coord, const glm::vec3& position, char name);
	//void RenderConnection(PathNode* a, PathNode* b);
	//void DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color);
	bool AStarSearch(Graph* graph, PathNode* start, PathNode* end);

	std::vector<PathNode*> m_OpenSet;
	std::vector<PathNode*> m_ClosedSet;
	PathNode* m_StartNode;
	PathNode* m_EndNode;

	// Create a Graph and nodes
	Graph m_Graph;
	typedef struct sFloat4 {
		float x, y, z, w;
	} sFloat4;

	typedef struct sVertex_p4c4 {
		sFloat4 Pos;
		sFloat4 Color;
	} sVertex_p4c4;

	typedef struct sVertex_p4 {
		sFloat4 Pos;
	} sVertex_p4;


	//std::vector<cMeshObject*> m_PathNodeGameObjects;
};