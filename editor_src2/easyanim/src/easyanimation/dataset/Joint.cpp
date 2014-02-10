#include "Joint.h"

namespace eanim
{
	const int Joint::REGION = 5;

	Joint::Joint(d2d::ISprite* sprite)
		: m_sprite(sprite)
		, m_parent(NULL)
	{
		m_relativeAngle = 0;
	}

	Joint::Joint(d2d::ISprite* sprite, const d2d::Vector& pos)
		: m_sprite(sprite)
		, m_parent(NULL)
	{
		setPosition(pos);
		m_relativeAngle = sprite->getAngle();
	}

	Joint::~Joint()
	{
		if (m_parent)
			m_parent->m_children.erase(this);

		std::set<Joint*>::iterator itr = m_children.begin();
		for ( ; itr != m_children.end(); ++itr)
		{
			Joint* child = *itr;
			assert(child->m_parent == this);
			child->m_parent = NULL;
		}
	}

	void Joint::draw() const
	{
		d2d::PrimitiveDraw::drawCircle(getWorldPos(), Joint::REGION, true, 2, d2d::Colorf(0.2f, 0.8f, 0.2f, 0.5f));
		d2d::PrimitiveDraw::drawCircle(getWorldPos(), Joint::REGION, false, 2, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
	}

	bool Joint::contain(const d2d::Vector& pos) const
	{
		return d2d::Math::getDistance(pos, getWorldPos()) < REGION;
	}

	bool Joint::intersect(const d2d::Vector& pos) const
	{
		return d2d::Math::getDistance(pos, getWorldPos()) < REGION * 2;
	}

	void Joint::setPosition(const d2d::Vector& pos) 
	{
		m_relative = getRelativePos(pos);
	}

	d2d::Vector Joint::getWorldPos() const
	{
		return m_sprite->getPosition() + d2d::Math::rotateVector(m_relative, m_sprite->getAngle());
	}
	
	d2d::Vector Joint::getRelativePos(const d2d::Vector& pos) const
	{
		return d2d::Math::rotateVector(pos - m_sprite->getPosition(), -m_sprite->getAngle());
	}

	void Joint::translate(const d2d::Vector& offset)
	{
		m_sprite->translate(offset);
	}

	void Joint::connect(Joint* joint)
	{
		std::set<Joint*>::iterator itr = joint->m_children.find(this);
		if (itr == joint->m_children.end())
		{
			joint->m_parent = this;
			m_children.insert(joint);
		}
	}

	void Joint::deconnect()
	{
		if (m_parent)
		{
			m_parent->m_children.erase(this);
			m_parent = NULL;
		}
	}
}