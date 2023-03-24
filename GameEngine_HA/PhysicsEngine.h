#pragma once

#include "PhysicsSystem.h"


class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void Initialize();
	void Destroy();

	void RayTest();
	void Update(double dt);
	void PhysicsUpdate(double dt);
	void Render();
	void setMouseState(bool state);
	bool m_FreeMotionMouse;
	bool m_Drag;
private:
int m_PrevMouseX;
int m_PrevMouseY;
int m_CurrMouseX;
int m_CurrMouseY;
int m_MouseDownX;
int m_MouseDownY;


float m_HorizontalAngle;

bool m_IsMouseDown;
bool m_WasMouseDown;
};