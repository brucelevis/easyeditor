#ifndef _EPHYSICS_BODY_MANAGER_H_
#define _EPHYSICS_BODY_MANAGER_H_

#include <drag2d.h>

namespace ephysics
{

class IBody;

class BodyManager
{
public:
	static BodyManager* Instance();

	IBody* LoadBody(d2d::Sprite* sprite);
	void UnloadBody(d2d::Sprite* sprite);

	const IBody* QueryBody(d2d::Sprite* sprite) const;

	void Update();

private:
	BodyManager();

private:
	static IBody* CreateBody(d2d::Sprite* sprite);

private:
	std::map<d2d::Sprite*, IBody*> m_map_body;

private:
	static BodyManager* m_instance;

}; // BodyManager

}

#endif // _EPHYSICS_BODY_MANAGER_H_