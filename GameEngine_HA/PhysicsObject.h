#pragma once

#include <ctime>
#include <string>

#include "BoundingBox.h"
#include "Shapes.h"

class PhysicsObject {
	// Private area
public:
	PhysicsObject();
	PhysicsObject(const glm::vec3& position);
	~PhysicsObject();
	PhysicsObject(const PhysicsObject& particle);
	PhysicsObject operator=(const PhysicsObject& particle);

	void SetMass(float mass) {
		// If the mass is 0 or less we treat it as 
		// a static 
		if (mass <= 0) {
			m_IsStatic = true;
			invMass = -1.f;
		}

		m_IsStatic = false;
		invMass = 1.f / mass;
	}

	void Integrate(float dt);
	void ApplyForce(const glm::vec3& direction);
	void KillAllForces();

	inline void SetPosition(glm::vec3 p) { position = p; }
	inline const glm::vec3& GetPosition() const { return position; }
	inline const glm::vec3& GetVelocity() const { return velocity; }
	inline const glm::vec3& GetAcceleration() const { return acceleration; }
	inline std::string GetName() const { return name; }

	BoundingBox* pBoundingBox;
	iShape* pShape;
	unsigned char type;
	glm::vec3 position;
	float scale;

private:
	friend class PhysicsSystem;
	friend class PhysicsEngine;
	float deltaTime = std::clock();
	float hoverTime = std::clock();
	float duration = 0;
	glm::vec3 prevPosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	std::string name;
	glm::vec3 force;
	float damping;
	bool m_IsStatic;
	float invMass;

	void PrintInfo();

};