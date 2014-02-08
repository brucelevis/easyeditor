#include "ISprite.h"
#include "ISymbol.h"
#include "IBody.h"

#include <Box2D/Box2D.h>

#include "common/Math.h"
#include "common/color_trans.h"
#include "dataset/AbstractBV.h"
#include "dataset/SpriteFactory.h"

namespace d2d
{

ISprite::ISprite()
	: m_body(NULL)
	, m_observer(NULL)
{
	multiCol.set(1, 1, 1, 1);
	addCol.set(0, 0, 0, 0);
	clip = false;

	visiable = editable = true;

	m_angle = 0.0f;
	m_xScale = m_yScale = 1.0f;
	m_xShear = m_yShear = 0.0f;
	m_xMirror = m_yMirror = false;
	m_bounding = NULL;
}

ISprite::ISprite(const ISprite& sprite)
	: m_observer(NULL)
{
	name = sprite.name;
	multiCol = sprite.multiCol;
	addCol = sprite.addCol;
	clip = sprite.clip;

	visiable = sprite.visiable;
	editable = sprite.editable;
	tag = sprite.tag;

	m_pos = sprite.m_pos;
	m_angle = sprite.m_angle;
	m_xScale = sprite.m_xScale;
	m_yScale = sprite.m_yScale;
	m_xShear = sprite.m_xShear;
	m_yShear = sprite.m_yShear;
	m_xMirror = sprite.m_xMirror;
	m_yMirror = sprite.m_yMirror;
	m_bounding = sprite.m_bounding->clone();
	m_body = sprite.m_body ? sprite.m_body->clone() : NULL;
}

ISprite::~ISprite()
{
	delete m_body;

	SpriteFactory::Instance()->remove(this);
}

ISprite* ISprite::clone() const
{
	return NULL;
}

void ISprite::clearUserData(bool deletePtr)
{
	delete m_userData, m_userData = NULL;
}

void ISprite::load(const Json::Value& val)
{
	name = val["name"].asString();
	tag = val["tag"].asString();
	clip = val["clip"].asBool();

	std::string str = val["multi color"].asString();
	if (str.empty())
		multiCol = Colorf(1, 1, 1, 1);
	else
		multiCol = transColor(str, PT_BGRA);
	str = val["add color"].asString();
	if (str.empty())
		addCol = Colorf(0, 0, 0, 0);
	else
		addCol = transColor(str, PT_ARGB);

	float x = val["position"]["x"].asDouble();
	float y = val["position"]["y"].asDouble();
	float angle = val["angle"].asDouble();
	setTransform(Vector(x, y), angle);

	float sx, sy;
	if (val["scale"].isNull())
	{
		sx = val["x scale"].asDouble();
		sy = val["y scale"].asDouble();
	}
	else
	{
		sx = sy = val["scale"].asDouble();
	}
	setScale(sx, sy);

	float kx, ky;
	if (!val["x shear"].isNull())
	{
		kx = val["x shear"].asDouble();
		ky = val["y shear"].asDouble();
	}
	else
	{
		kx = ky = 0;
	}
	setShear(kx, ky);

	bool mx = val["x mirror"].asBool();
	bool my = val["y mirror"].asBool();
	setMirror(mx, my);

	float ox, oy;
	if (!val["x offset"].isNull())
	{
		ox = val["x offset"].asDouble();
		oy = val["y offset"].asDouble();
	}
	else
	{
		ox = oy = 0;
	}
	setOffset(Vector(ox, oy));
}

void ISprite::store(Json::Value& val)
{
	val["name"] = name;
	val["tag"] = tag;
	val["clip"] = clip;

	val["multi color"] = transColor(multiCol, PT_BGRA);
	val["add color"] = transColor(addCol, PT_ARGB);

	val["position"]["x"] = m_pos.x;
	val["position"]["y"] = m_pos.y;

	val["angle"] = m_angle;

	val["x scale"] = m_xScale;
	val["y scale"] = m_yScale;

	val["x shear"] = m_xShear;
	val["y shear"] = m_yShear;

	val["x mirror"] = m_xMirror;
	val["y mirror"] = m_yMirror;

	val["x offset"] = m_offset.x;
	val["y offset"] = m_offset.y;
}

// todo: translate() and rotate() has no opt to m_body
void ISprite::setTransform(const Vector& position, float angle)
{
	if (m_pos != position) translate(position - m_pos);
	if (m_angle != angle) rotate(angle - m_angle);

	if (m_body)
		m_body->getBody()->SetTransform(b2Vec2(position.x / BOX2D_SCALE_FACTOR, position.y / BOX2D_SCALE_FACTOR), angle);
}

void ISprite::setScale(float xScale, float yScale)
{
	m_xScale = xScale;
	m_yScale = yScale;
	afterScaleChanged();
}

void ISprite::setScale(float scale)
{
	m_xScale = m_yScale = scale;
	afterScaleChanged();
}

void ISprite::setShear(float xShear, float yShear)
{
	m_xShear = xShear;
	m_yShear = yShear;
	// todo: change bounding
}

bool ISprite::isContain(const Vector& pos) const
{
	return m_bounding->isContain(pos);
}

bool ISprite::isIntersect(const Rect& rect) const
{
	return m_bounding->isIntersect(rect);
}

void ISprite::translate(const Vector& offset)
{
	if (m_observer)
		m_observer->translate(this, offset);

	m_pos += offset;
	m_bounding->setTransform(m_pos, m_offset, m_angle);
}

void ISprite::rotate(float delta)
{
	if (m_observer)
		m_observer->rotate(this, delta);

	m_angle += delta;

	m_bounding->setTransform(m_pos, m_offset, m_angle);
}

void ISprite::setOffset(const Vector& offset) 
{
	m_offset = offset;
	m_bounding->setTransform(m_pos, m_offset, m_angle);
}

void ISprite::updateEachFrame()
{
	if (!m_body) return;

	b2Body* body = m_body->getBody();
	if (!body) return;

	if (m_body->isAlive() && m_body->getBody()->GetType() != b2_staticBody)
	{
		const b2Vec2& pos = body->GetPosition();
		setTransform(Vector(pos.x * BOX2D_SCALE_FACTOR, pos.y * BOX2D_SCALE_FACTOR), body->GetAngle());
	}
	else
	{
		body->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
	}
}

IBody* ISprite::getBody() const
{
	return m_body;
}

void ISprite::afterScaleChanged()
{
	buildBounding();

	if (m_body)
	{
		b2BodyType type = m_body->getBody()->GetType();
		loadBodyFromFile();
		if (m_body)
		{
			m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
			m_body->getBody()->SetType(type);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class SpriteCmp
//////////////////////////////////////////////////////////////////////////

SpriteCmp::SpriteCmp(Type type /*= e_file*/)
	: m_type(type)
{
}

bool SpriteCmp::operator() (const ISprite* s0, const ISprite* s1) const
{
	switch (m_type)
	{
	case e_file:
		return s0->getSymbol().getFilepath() < s1->getSymbol().getFilepath();
	case e_x:
		return s0->getPosition().x < s1->getPosition().x;
	case e_y:
		return s0->getPosition().y < s1->getPosition().y;
	default:
		return s0->getSymbol().getFilepath() < s1->getSymbol().getFilepath();
	}
}

} // d2d