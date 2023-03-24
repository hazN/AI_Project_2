#include "PhysicsMath.h"

float Dot(glm::vec3 a, glm::vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}