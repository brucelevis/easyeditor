#include "TrapezoidalDec.h"

#include "common/Math.h"

#include <assert.h>

namespace d2d
{

TrapezoidalDec::TrapezoidalDec()
	: m_root(NULL)
{
}

TrapezoidalDec::~TrapezoidalDec()
{
	// todo
	delete m_root;
}

void TrapezoidalDec::Insert(const d2d::Vector& p0, const d2d::Vector& p1)
{
	if (p0 == p1) return;

	d2d::Vector left, right;
	if (p0.x < p1.x) {
		left = p0;
		right = p1;
	} else if (p0.x == p1.x) {
		if (p0.y < p1.y) {
			left = p0;
			right = p1;
		} else {
			left = p1;
			right = p0;
		}
	} else {
		left = p1;
		right = p0;		
	}

	if (!m_root) {
		InitRoot(left, right);
	} else {
		InsertSeg(left, right);
	}
}

void TrapezoidalDec::InitRoot(const d2d::Vector& lpos, const d2d::Vector& rpos)
{
	Node* left = new Node(lpos);
	Node* right = new Node(rpos);

	Seg* seg = new Seg(left, right);

	Area* a1 = new Area(NULL, NULL, NULL, left);
	Area* a2 = new Area(NULL, seg, left, right);
	Area* a3 = new Area(seg, NULL, left, right);
	Area* a4 = new Area(NULL, NULL, right, NULL);

	m_root = left;

	left->SetChild0(a1);
	left->SetChild1(right);

	right->SetChild0(seg);
	right->SetChild1(a4);

	seg->SetChild0(a2);
	seg->SetChild1(a3);
}

void TrapezoidalDec::InsertSeg(const d2d::Vector& lpos, const d2d::Vector& rpos)
{
// 	Area* a0 = m_root->Query(lpos);
// 	Area* a1 = m_root->Query(rpos);

	// todo split
}

//////////////////////////////////////////////////////////////////////////
// class TrapezoidalDec::Node
//////////////////////////////////////////////////////////////////////////

TrapezoidalDec::INode* TrapezoidalDec::Node::
Query(const d2d::Vector& pos)
{
	assert(m_child0 && m_child1);
	if (pos.x < m_pos.x) {
		return m_child0->Query(pos);
	} else {
		return m_child1->Query(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
// class TrapezoidalDec::Seg
//////////////////////////////////////////////////////////////////////////

TrapezoidalDec::INode* TrapezoidalDec::Seg::
Query(const d2d::Vector& pos)
{
	assert(m_child0 && m_child1);
	if (Math::isPointAtSegmentLeft(pos, m_left->m_pos, m_right->m_pos)) {
		return m_child0->Query(pos);
	} else {
		return m_child1->Query(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
// class TrapezoidalDec::Area
//////////////////////////////////////////////////////////////////////////

TrapezoidalDec::INode* TrapezoidalDec::Area::
Query(const d2d::Vector& pos)
{
	// todo assert
	return this;
}

}