#pragma once
#include <glm/vec3.hpp>

struct BoundingBox {
	glm::vec3 centerPoint;
	glm::vec3 minPoints;
	glm::vec3 maxPoints;
	glm::vec3 halfExtents;
};

struct BoundingSphere {
	glm::vec3 centerPoint;
	float radius;
};