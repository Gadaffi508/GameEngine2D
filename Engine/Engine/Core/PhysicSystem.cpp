#include "PhysicSystem.h"

bool PhysicSystem::Init(float ppm, float gx, float gy)
{
	m_PPM = ppm;

	m_World = new b2World({ gx,gy });
	return true;
}

void PhysicSystem::ShutDown()
{
	delete m_World;
	m_World == nullptr;
}

void PhysicSystem::Step(float dt)
{
	if (!m_World) return;

	m_World->Step(dt, 8, 3);
}

b2Body* PhysicSystem::CreateBox(float px, float py, float w, float h, bool dynamic, float density, float restitution)
{
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bd.position.Set(px / m_PPM, py / m_PPM);

	b2PolygonShape shape;
	shape.SetAsBox((w / 2) / m_PPM, (h / 2) / m_PPM);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = restitution;

	b2Body* body = m_World->CreateBody(&bd);
	body->CreateFixture(&fd);
	return body;
}