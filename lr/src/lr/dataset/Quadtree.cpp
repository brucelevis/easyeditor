#include "Quadtree.h"

#include <ee/EE_RVG.h>
#include <ee/SpriteRenderer.h>
#include <ee/Math2D.h>
#include <ee/color_config.h>
#include <ee/Triangulation.h>

#include <queue>
#include <easyshape.h>

namespace lr
{

Quadtree::Quadtree(const sm::rect& rect)
	: m_selected(NULL)
{
	m_root = new Node(rect);
}

Quadtree::~Quadtree()
{
	delete m_root;
}

void Quadtree::Insert(const ee::Sprite* spr)
{
	m_root->Insert(spr);
}

std::vector<const ee::Sprite*> Quadtree::Query(const sm::rect& rect) const
{
	std::set<const ee::Sprite*> set_sprites;

	std::queue<Node*> buffer;
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Node* node = buffer.front(); buffer.pop();
		if (node->IsLeaf()) {
			for (int i = 0, n = node->m_sprites.size(); i < n; ++i) {
				set_sprites.insert(node->m_sprites[i]);
			}
		} else {
			for (int i = 0; i < 4; ++i) {
				Node* cn = node->m_children[i];
				if (cn->IsIntersect(rect)) {
					buffer.push(cn);
				}
			}			
		}
	}

	std::vector<const ee::Sprite*> sprites;
	copy(set_sprites.begin(), set_sprites.end(), back_inserter(sprites));
	return sprites;
}

void Quadtree::DebugDraw() const
{
	std::queue<Node*> buffer;
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Node* node = buffer.front(); buffer.pop();
		ee::RVG::Color(ee::LIGHT_GREY);
		ee::RVG::LineWidth(1);
		ee::RVG::Rect(sm::vec2(node->m_rect.xmin, node->m_rect.ymin), sm::vec2(node->m_rect.xmax, node->m_rect.ymax), false);
		ee::RVG::LineWidth(2);

		if (!node->IsLeaf()) 
		{
			for (int i = 0; i < 4; ++i) {
				Node* cn = node->m_children[i];
				buffer.push(cn);
			}			
		}
	}

	if (m_selected) {
		ee::RVG::Color(ee::LIGHT_RED);
		ee::RVG::LineWidth(1);
		ee::RVG::Rect(sm::vec2(m_selected->m_rect.xmin, m_selected->m_rect.ymin), sm::vec2(m_selected->m_rect.xmax, m_selected->m_rect.ymax), false);
		ee::RVG::LineWidth(2);

		s2::RenderColor color;
		color.mul = ee::LIGHT_BLUE;
		s2::RenderParams params;
		params.color = color;
		for (int i = 0, n = m_selected->m_sprites.size(); i < n; ++i) {
			ee::SpriteRenderer::Draw(m_selected->m_sprites[i], params);
		}
	}
}

void Quadtree::SelectNode(const sm::vec2& pos)
{
	std::queue<Node*> buffer;
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Node* node = buffer.front(); buffer.pop();
		if (node->IsLeaf()) {
			m_selected = node;
		} else {
			for (int i = 0; i < 4; ++i) {
				Node* cn = node->m_children[i];
				if (cn->IsContain(pos)) {
					buffer.push(cn);
				}
			}			
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class Quadtree::Node
//////////////////////////////////////////////////////////////////////////

const int Quadtree::Node::MAX_COUNT = 5;
const float Quadtree::Node::MAX_AREA = 0.5f;
const int Quadtree::Node::MIN_GRID = 8;

Quadtree::Node::
Node(const sm::rect& rect)
	: m_rect(rect)
{
	memset(m_children, 0, sizeof(m_children));
}

Quadtree::Node::
~Node()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
	for (int i = 0; i < 4; ++i) {
		delete m_children[i];
	}
}

void Quadtree::Node::
Insert(const ee::Sprite* spr)
{
	spr->Retain();

	if (IsLeaf()) 
	{
		if (IsContain(spr)) {
			m_sprites.push_back(spr);
		}
	 	if (NeedSplit()) {
	 		Split();
	 	}
	} 
	else 
	{
		sm::rect rect = spr->GetRect();
		for (int i = 0; i < 4; ++i) {
			Node* child = m_children[i];
			if (child->IsIntersect(rect)) {
				child->Insert(spr);
			}
		}
	}
}

bool Quadtree::Node::
IsLeaf() const
{
	return m_children[0] == NULL;
}

bool Quadtree::Node::
IsIntersect(const sm::rect& rect) const
{
	return ee::Math2D::IsRectIntersectRect(m_rect, rect);
}

bool Quadtree::Node::
IsContain(const sm::vec2& pos) const
{
	return ee::Math2D::IsPointInRect(pos, m_rect);
}

bool Quadtree::Node::
IsContain(const ee::Sprite* spr) const
{
	const eshape::Symbol& shape = static_cast<const eshape::Sprite*>(spr)->GetSymbol();
	assert(shape.GetShapeType() == eshape::ST_POLYGON);

	const std::vector<ee::Shape*>& shapes = shape.GetShapes();
	assert(!shapes.empty());

	eshape::PolygonShape* poly = static_cast<eshape::PolygonShape*>(shapes[0]);

	std::vector<sm::vec2> bound;
	ee::Math2D::TransVertices(spr->GetTransMatrix(), poly->GetVertices(), bound);

	return ee::Math2D::IsPolylineIntersectRect(bound, true, m_rect);
}

bool Quadtree::Node::
NeedSplit() const
{
	if (m_sprites.empty()) {
		return false;
	}
	float a_shape = 0;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		a_shape += GetContainArea(m_sprites[i]);
	}

	sm::vec2 sz = m_rect.Size();
	float a_rect = sz.x * sz.y;
	float p = a_shape / a_rect;

	if (fabs(a_rect - 0) < 0.001f) {
		int zz = 0;
	}
	wxLogDebug("NeedSplit area %f, a %f, p %f", a_shape, a_rect, p);

	return p < 0.5f && a_shape > 100;
}

void Quadtree::Node::
Split()
{
	for (int i = 0; i < 4; ++i) {
		m_children[i] = new Node(m_rect);
	}
	sm::vec2 center = m_rect.Center();
	m_children[0]->m_rect.xmax = m_children[2]->m_rect.xmax = 
		m_children[1]->m_rect.xmin = m_children[3]->m_rect.xmin = center.x;
	m_children[2]->m_rect.ymax = m_children[3]->m_rect.ymax = 
		m_children[0]->m_rect.ymin = m_children[1]->m_rect.ymin = center.y;

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		const ee::Sprite* spr = m_sprites[i];
		for (int j = 0; j < 4; ++j) {
			Node* node = m_children[j];
			sm::rect rect = spr->GetRect();
			if (node->IsIntersect(rect)) {
				node->Insert(spr);
			}
		}
	}

	m_sprites.clear();
}

float Quadtree::Node::
GetContainArea(const ee::Sprite* spr) const
{
	const eshape::Symbol& shape = static_cast<const eshape::Sprite*>(spr)->GetSymbol();
	assert(shape.GetShapeType() == eshape::ST_POLYGON);

	const std::vector<ee::Shape*>& shapes = shape.GetShapes();
	assert(!shapes.empty());

	eshape::PolygonShape* poly = static_cast<eshape::PolygonShape*>(shapes[0]);

	std::vector<sm::vec2> bound;
	ee::Math2D::TransVertices(spr->GetTransMatrix(), poly->GetVertices(), bound);

	std::vector<sm::vec2> loop;
	loop.push_back(sm::vec2(m_rect.xmin, m_rect.ymin));
	loop.push_back(sm::vec2(m_rect.xmin, m_rect.ymax));
	loop.push_back(sm::vec2(m_rect.xmax, m_rect.ymax));
	loop.push_back(sm::vec2(m_rect.xmax, m_rect.ymin));
	std::vector<std::vector<sm::vec2> > loops;
	loops.push_back(loop);

	float area = 0.0f;

	std::vector<sm::vec2> lines;
	std::vector<sm::vec2> tris;
	ee::Triangulation::LinesAndLoops(bound, lines, loops, tris);
	for (int i = 0, n = tris.size() / 3; i < n; ++i)
	{
		sm::vec2 center = (tris[i*3] + tris[i*3+1] + tris[i*3+2]) / 3;
		if (ee::Math2D::IsPointInRect(center, m_rect)) {
			area += ee::Math2D::GetTriangleArea(tris[i*3], tris[i*3+1], tris[i*3+2]);
		}
	}

	return area;
}

}