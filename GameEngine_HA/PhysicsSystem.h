﻿#pragma once

#include "PhysicsObject.h"

#include <map>
#include <vector>

#include "GUI.h"
#include "Shapes.h"

class PhysicsSystem {
public:
	PhysicsSystem();
	~PhysicsSystem();

	PhysicsObject* CreatePhysicsObject(const glm::vec3& position, iShape* shape, std::string name);
	void UpdateStep(float duration);
	void AddTriangleToAABBCollisionCheck(int hash, Triangle* triangle);

	// 0b is binary notation for the numbers that follow
	// 0b00000010 = 2
	// 0b00000110 = 6
	bool RayCastClosest(Ray ray, PhysicsObject** hitObject, unsigned char type = 0b11111111);
	bool RayCastFirstFound(Ray ray, PhysicsObject** hitObject);
	std::vector<PhysicsObject*> RayCastAll(Ray ray);

	const std::map<int, std::vector<Triangle*>>& GetAABBStructure() {
		return m_AABBStructure;
	}

private:
	friend GUI;
	friend class PhysicsEngine;
	bool CollisionTest(const glm::vec3& posA, iShape* shapeA, const glm::vec3& posB, iShape* shapeB);
	bool CollisionTest(const glm::vec3& posA, Sphere* a, const glm::vec3& posB, Sphere* b);
	bool CollisionTest(const glm::vec3& posA, Sphere* a, const glm::vec3& posB, AABB* b);
	bool CollisionTest(const glm::vec3& posA, Sphere* a, const glm::vec3& posB, Triangle* b);

	//bool CollisionTest(AABB* a, Triangle* b);		// Not implementing
	//bool CollisionTest(AABB* a, AABB* b);			// Not implementing
	//bool CollisionTest(Triangle* a, Triangle* b);	// Not implementing

	std::vector<PhysicsObject*> m_PhysicsObjects;
	std::map<int, std::vector<Triangle*>> m_AABBStructure;
};