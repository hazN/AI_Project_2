#include "PhysicsSystem.h"

#include <iostream>
#include <ostream>

#include "globalThings.h"
#include "PhysicsUtils.h"

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
	for (int i = 0; i < m_PhysicsObjects.size(); i++) {
		delete m_PhysicsObjects[i];
	}
	m_PhysicsObjects.clear();

	// delete[] m_Particles[0];		Can check if this works (Not sure)
	//  -> edit: Nope, use this for array of pointers
}

PhysicsObject* PhysicsSystem::CreatePhysicsObject(const glm::vec3& position, iShape* shape, std::string name) {
	PhysicsObject* physicsObject = new PhysicsObject(position);
	physicsObject->pShape = shape;
	physicsObject->name = name;
	m_PhysicsObjects.push_back(physicsObject);
	return physicsObject;
}
void PhysicsSystem::UpdateStep(float duration) {
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		obj->Integrate(duration);
	}
}

void PhysicsSystem::AddTriangleToAABBCollisionCheck(int hash, Triangle* triangle)
{
	m_AABBStructure[hash].push_back(triangle);
}

bool PhysicsSystem::RayCastClosest(Ray ray, PhysicsObject** hitObject, unsigned char flags)
{
	PhysicsObject* closestPhysicsObject = nullptr;
	float closestDistance = FLT_MAX;

	// An alternate method to optimize this would be to sort the m_PhysicsObjects
	// by the distance of the object from the eye.
	// Sort objects should be done once per frame, not each call
	// This can be a "bubble sort", as objects will not be needing to be 100%
	// sorted every frame, and you are not jumping around the world really fast.
	std::cout << " " << std::endl;
	for (int i = 0; i < m_PhysicsObjects.size(); i++) {
		PhysicsObject* physicsObject = m_PhysicsObjects[i];
		if (physicsObject->pShape->GetType() == SHAPE_TYPE_SPHERE)
		{
			Sphere* pSphere = dynamic_cast<Sphere*>(physicsObject->pShape);
			float rad = pSphere->Radius;
			if (gameMode == eGameMode::Hover)
				rad = 8;
			if (TestRaySphere(ray.origin, ray.direction, pSphere->Center + physicsObject->position, rad))
			{
				float distance = glm::distance(ray.origin, physicsObject->position);
				if (distance < closestDistance) {
					closestPhysicsObject = physicsObject;
					closestDistance = distance;
				}
			}
		}
		if (physicsObject->pShape->GetType() == SHAPE_TYPE_AABB)
		{
			AABB* aabb = dynamic_cast<AABB*>(physicsObject->pShape);
			glm::vec3 min = glm::vec3(aabb->Min[0], aabb->Min[1], aabb->Min[2]);
			glm::vec3 max = glm::vec3(aabb->Max[0], aabb->Max[1], aabb->Max[2]);
			if (TestRayAABB(ray.origin, ray.direction, min * physicsObject->scale + physicsObject->position, max * physicsObject->scale + physicsObject->position))
			{
				float distance = glm::distance(ray.origin, physicsObject->position);
				if (distance < closestDistance) {
					closestPhysicsObject = physicsObject;
					closestDistance = distance;
				}
			}
		}
	}
	*hitObject = closestPhysicsObject;
	return closestPhysicsObject != nullptr;
}

bool PhysicsSystem::RayCastFirstFound(Ray ray, PhysicsObject** hitObject)
{
	for (int i = 0; i < m_PhysicsObjects.size(); i++) {
		PhysicsObject* physicsObject = m_PhysicsObjects[i];
		if (physicsObject->pShape->GetType() == SHAPE_TYPE_SPHERE)
		{
			Sphere* pSphere = dynamic_cast<Sphere*>(physicsObject->pShape);
			if (TestRaySphere(ray.origin, ray.direction, pSphere->Center + physicsObject->position, pSphere->Radius))
			{
				*hitObject = physicsObject;
				return true;
			}
		}
	}
	return false;
}

std::vector<PhysicsObject*> PhysicsSystem::RayCastAll(Ray ray)
{
	std::vector<PhysicsObject*> hitList;
	for (int i = 0; i < m_PhysicsObjects.size(); i++) {
		PhysicsObject* physicsObject = m_PhysicsObjects[i];
		if (physicsObject->pShape->GetType() == SHAPE_TYPE_SPHERE)
		{
			Sphere* pSphere = dynamic_cast<Sphere*>(physicsObject->pShape);
			if (TestRaySphere(ray.origin, ray.direction, pSphere->Center + physicsObject->position, pSphere->Radius))
			{
				hitList.push_back(physicsObject);
			}
		}
	}
	return hitList;
}

bool PhysicsSystem::CollisionTest(const glm::vec3& posA, iShape* shapeA, const glm::vec3& posB, iShape* shapeB)
{
	bool detectedCollision = false;

	// ShapeA AABB Collision test
	if (shapeA->GetType() == SHAPE_TYPE_AABB)
	{
		if (shapeB->GetType() == SHAPE_TYPE_AABB) {}

		else if (shapeB->GetType() == SHAPE_TYPE_SPHERE) {
			detectedCollision = CollisionTest(posB, dynamic_cast<Sphere*>(shapeB), posA, dynamic_cast<AABB*>(shapeA));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_TRIANGLE) {}
	}

	// ShapeA Sphere collision tests
	else if (shapeA->GetType() == SHAPE_TYPE_SPHERE)
	{
		if (shapeB->GetType() == SHAPE_TYPE_AABB) {
			detectedCollision = CollisionTest(posA, dynamic_cast<Sphere*>(shapeA), posB, dynamic_cast<AABB*>(shapeB));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_SPHERE) {
			detectedCollision = CollisionTest(posA, dynamic_cast<Sphere*>(shapeA), posB, dynamic_cast<Sphere*>(shapeB));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_TRIANGLE) {
			detectedCollision = CollisionTest(posA, dynamic_cast<Sphere*>(shapeA), posB, dynamic_cast<Triangle*>(shapeB));
		}
	}

	// ShapeA Triangle collision tests
	else if (shapeA->GetType() == SHAPE_TYPE_TRIANGLE)
	{
		if (shapeB->GetType() == SHAPE_TYPE_AABB) {}

		else if (shapeB->GetType() == SHAPE_TYPE_SPHERE) {
			detectedCollision = CollisionTest(posB, dynamic_cast<Sphere*>(shapeB), posA, dynamic_cast<Triangle*>(shapeA));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_TRIANGLE) {}
	}

	// ShapeA is unknown shape...
	else
	{
		// what is this!?!?!
	}

	return detectedCollision;
}

bool PhysicsSystem::CollisionTest(const glm::vec3& posA, Sphere* a, const glm::vec3& posB, Sphere* b)
{
	return TestSphereSphere(posA + a->Center, a->Radius, posB + b->Center, b->Radius);
}

bool PhysicsSystem::CollisionTest(const glm::vec3& posA, Sphere* a, const glm::vec3& posB, AABB* b)
{
	return TestSphereAABB(posA + a->Center, a->Radius, *b);
}

bool PhysicsSystem::CollisionTest(const glm::vec3& posA, Sphere* a, const glm::vec3& posB, Triangle* b)
{
	Point unused;
	return TestSphereTriangle(posA + a->Center, a->Radius, posB + (*b).A, posB + (*b).B, posB + (*b).C, unused);
}