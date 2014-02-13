#pragma once

#include "AbstractBV.h"

#include "common/Rect.h"

namespace d2d
{
	class AABB : public AbstractBV
	{
	public:
		AABB();
		AABB(const AABB& aabb);

		//
		// IObject interface
		//
		virtual AABB* clone() const;

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual void initFromRect(const Rect& rect);

		virtual bool isValid() const;
		virtual void makeInfinite();

		virtual void setTransform(const Vector& position, const Vector& offset, float angle);

		virtual void combine(const Vector& pos);
		virtual void combine(const Rect& rect);

		virtual bool isContain(const Vector& pos) const;
		virtual bool isContain(const Rect& rect) const;
		virtual bool isIntersect(const Rect& rect) const;
		
		virtual float area() const;
		virtual float width() const;
		virtual float height() const;
		virtual Vector center() const;

		virtual void getBoundPos(std::vector<Vector>& bound) const;

		const Rect& getRect() const;

	private:
		Rect m_rect;
		Vector m_position;

	}; // AABB

	inline void AABB::initFromRect(const Rect& rect)
	{
		m_rect = rect;
		m_position.set(0, 0);
	}

	inline bool AABB::isValid() const
	{
		return m_rect.isValid();
	}

	inline void AABB::makeInfinite()
	{
		m_rect.makeInfinite();
	}

	inline void AABB::combine(const Vector& pos)
	{
		m_rect.combine(pos - m_position);
	}

	inline void AABB::combine(const Rect& rect)
	{
		Rect r(rect);
		r.translate(-m_position);
		m_rect.combine(r);
	}

	inline const Rect& AABB::getRect() const
	{
		return m_rect;
	}
}

