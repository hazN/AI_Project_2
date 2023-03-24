#pragma once

#include "PhysicsMath.h"
#include "Shapes.h"

int TestSphereSphere(const glm::vec3& posA, float radiusA, const glm::vec3& posB, float radiusB);
float SqDistPointAABB(glm::vec3 p, AABB b);
int TestSphereAABB(const glm::vec3& center, float radius, AABB b);
Point ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
int TestSphereTriangle(const glm::vec3& center, float radius, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3& p);
int TestRaySphere(const Point& p, const glm::vec3& d, const Point& center, float radius);
int TestRayAABB(const Point& p, const glm::vec3& d, glm::vec3 aabbMIN, glm::vec3 aabbMAX);