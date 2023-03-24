#include "PhysicsEngine.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "globalThings.h"
extern glm::vec3 g_cameraEye;
extern glm::vec3 g_cameraTarget;
PhysicsEngine::PhysicsEngine() = default;

PhysicsEngine::~PhysicsEngine() = default;

void PhysicsEngine::Initialize()
{
	m_FreeMotionMouse = true;
	// Crosshair
	//glfwSetCursor(window, (GLFWcursor*)GLFW_CROSSHAIR_CURSOR);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	//glfwSetCursor(window, cursor);
	// Create targets
	//for (int i = 0; i < g_pMeshObjects.size() - 1; i++)
	//{
	//	if (g_pMeshObjects[i]->meshName == "Target")
	//	{
	//		Sphere* sphere = new Sphere(Point(0.0f, 0.0f, 0.0f), g_pMeshObjects[i]->scale);
	//		g_pMeshObjects[i]->physics_object = g_physics_system.CreatePhysicsObject(g_pMeshObjects[i]->position, sphere, g_pMeshObjects[i]->friendlyName);
	//		g_pMeshObjects[i]->hasPhysicsObject = true;
	//	}
	//	if (g_pMeshObjects[i]->meshName == "Cube")
	//	{
	//		sModelDrawInfo modelInfo;
	//		pVAOManager->FindDrawInfoByModelName("Cube", modelInfo);
	//		float min[3] = { modelInfo.minX, modelInfo.minY, modelInfo.minZ };
	//		float max[3] = { modelInfo.maxX, modelInfo.maxY, modelInfo.maxZ };
	//		AABB* cube = new AABB(min, max);
	//		g_pMeshObjects[i]->physics_object = g_physics_system.CreatePhysicsObject(glm::vec3(rand() % 700 + (-300), rand() % 80 + 1, rand() % 400 + (-350)), cube, g_pMeshObjects[i]->friendlyName);
	//		g_pMeshObjects[i]->hasPhysicsObject = true;
	//	}
	//}
}

void PhysicsEngine::Destroy()
{
}

void PhysicsEngine::RayTest()
{
	int width = 1200;
	int height = 800;
	glfwGetWindowSize(window, &width, &height);

	// 1. Cursor Position on the Screen
	glm::vec3 cursorPositionOnScreenSpace(
		(width / 2),				// X is fine from left to right
		(height / 2),	// Since Y is origin at the top, and positive as it goes down the screen
							// we need to fix it like this.
		1.f
	);
	std::cout << "Mouse pos: " << m_CurrMouseX << " " << m_CurrMouseY;

	// 2. Viewport: Window Information
	glm::vec4 viewport = glm::vec4(0, 0, width, height);

	// 3 Projection Matrix
	glm::mat4 projectionMatrix = glm::perspective(
		0.6f,			// Field of View
		(float)width / (float)height,	// Aspect Ratio
		0.1f,							// zNear plane
		10000.0f							// zFar plane
	);

	glm::mat4 viewMatrix = glm::lookAt(
		::g_cameraEye,				// Position of the Camera
		::g_cameraEye + ::g_cameraTarget,			// Target view point
		glm::vec3(0, 1, 0)				// Up direction
	);

	if (!m_FreeMotionMouse) {
		cursorPositionOnScreenSpace.x = width / 2;
		cursorPositionOnScreenSpace.y = height / 2;
	}

	// Calculate our position in world space
	glm::vec3 pointInWorldSpace = glm::unProject(
		cursorPositionOnScreenSpace,
		viewMatrix,
		projectionMatrix,
		viewport
	);

	// Make pointInWorldSpace a direction instead
	glm::vec3 direction = pointInWorldSpace - g_cameraEye;

	Ray ray(g_cameraEye, direction);

	PhysicsObject* hitObject;

	if (g_physics_system.RayCastClosest(ray, &hitObject)) {
		if (hitObject->pShape->GetType() == SHAPE_TYPE_AABB)
		{
			if (gameMode == eGameMode::Click)
			std::cout << "Hit an obstacle..." << std::endl;
		}
		else {
			if (gameMode == eGameMode::Click)
			{
				std::cout << "Hit a target!" << std::endl;
				numHitTargets++;
				// Set a new random position for the target
				hitObject->KillAllForces();
				hitObject->position = glm::vec3(rand() % 700 + (-300), rand() % 100 + 10, rand() % 10 + (-350));
				glm::vec3 target;
				if (hitObject->position.x < 0)
					target = hitObject->position + glm::vec3(-100, 0, 0);
				else target = hitObject->position + glm::vec3(100, 0, 0);
				glm::vec3 direction = glm::normalize(hitObject->position - target);
				hitObject->velocity = 1.01f * direction;
				hitObject->ApplyForce(direction);
			}
			else
			{
				hitObject->duration = (std::clock() - hitObject->hoverTime) / (double)CLOCKS_PER_SEC;
				if (hitObject->duration > 5)
				{
					std::cout << "Hit a target!" << std::endl;
					numHitTargets++;
					// Set a new random position for the target
					hitObject->KillAllForces();
					hitObject->position = glm::vec3(rand() % 700 + (-300), rand() % 100 + 10, rand() % 10 + (-350));
					glm::vec3 target;
					if (hitObject->position.x < 0)
						target = hitObject->position + glm::vec3(-100, 0, 0);
					else target = hitObject->position + glm::vec3(100, 0, 0);
					glm::vec3 direction = glm::normalize(hitObject->position - target);
					hitObject->velocity = 1.01f * direction;
					hitObject->ApplyForce(direction);
					hitObject->hoverTime = std::clock();
				}
			}
		}
	}
	else
	{
		for (PhysicsObject* obj : g_physics_system.m_PhysicsObjects)
		{
			obj->hoverTime = std::clock();
		}
	}
	// Randomize in this area
	//  400 to -300 x
	//  10  to 100 y
	// -350 to 50 z
}
bool isClicked = false;
int MouseStaticPosX = 0;
int MouseStaticPosY = 0;
int waitTime;

void PhysicsEngine::Update(double dt)
{
	//// Change waittime based on gamemode
	//if (gameMode == eGameMode::Click)
	//{
	//	waitTime = 10;
	//}
	//else waitTime = 15;
	//// Move targets
	//for (PhysicsObject* obj : g_physics_system.m_PhysicsObjects)
	//{
	//	obj->duration = (std::clock() - obj->deltaTime) / (double)CLOCKS_PER_SEC;
	//	if (obj->duration > waitTime && !menuMode)
	//	{
	//		if (obj->pShape->GetType() == SHAPE_TYPE_SPHERE)
	//		{
	//			obj->KillAllForces();
	//			obj->position = glm::vec3(rand() % 700 + (-300), rand() % 100 + 10, rand() % 400 + (-350));
	//			glm::vec3 target;
	//			if (obj->position.x < 0)
	//				target = obj->position + glm::vec3(-100, 0, 0);
	//			else target = obj->position + glm::vec3(100, 0, 0);
	//			glm::vec3 direction = glm::normalize(obj->position - target);
	//			obj->velocity = 1.01f * direction;
	//			obj->ApplyForce(direction);
	//		}
	//		obj->deltaTime = std::clock();
	//	}
	//}

	m_PrevMouseX = m_CurrMouseX;
	m_PrevMouseY = m_CurrMouseY;
	double tempX = 0;
	double tempY = 0;
	glfwGetCursorPos(window, &tempX, &tempY);
	m_CurrMouseX = floor(tempX);
	m_CurrMouseY = floor(tempY);
	int deltaMouseX = 0;
	int deltaMouseY = 0;

	deltaMouseX = m_CurrMouseX - m_PrevMouseX;
	deltaMouseY = m_CurrMouseY - m_PrevMouseY;

	m_HorizontalAngle -= deltaMouseX * rotateSpeed;

	const float moveSpeed = 5.0f;

	
	::g_cameraTarget.x = sin(m_HorizontalAngle);
	::g_cameraTarget.z = cos(m_HorizontalAngle);
	::g_cameraTarget.y -= deltaMouseY * rotateSpeed;

}

void PhysicsEngine::PhysicsUpdate(double dt)
{
	g_physics_system.UpdateStep(dt);
}

void PhysicsEngine::Render()
{
}

void PhysicsEngine::setMouseState(bool state)
{
	m_IsMouseDown = state;
}