#include "Strip.h"
#include "Triangle.h"

#include <ee/JsonSerializer.h>
#include <ee/Math2D.h>

#include <set>
#include <algorithm>

#include <assert.h>
#include <math.h>

namespace emesh
{

Strip::Strip()
	: m_uv_offset(0)
{
}

Strip::Strip(const Strip& strip)
	: EditableMesh(strip)
	, m_left_nodes(strip.m_left_nodes)
	, m_right_nodes(strip.m_right_nodes)
{
	RefreshTriangles();
	CopyTriangles(strip);
}

Strip::Strip(const ee::Symbol* base)
	: EditableMesh(base)
{
	InitBound();
}

Strip* Strip::Clone() const
{
	return new Strip(*this);
}

void Strip::Load(const Json::Value& value)
{
	m_width  = static_cast<float>(value["width"].asDouble());
	m_height = static_cast<float>(value["height"].asDouble());

	m_left_nodes.m_ori.clear();
	m_right_nodes.m_ori.clear();
	ee::JsonSerializer::Load(value["left nodes"], m_left_nodes.m_ori);
	ee::JsonSerializer::Load(value["right nodes"], m_right_nodes.m_ori);
	m_left_nodes.m_ext = m_left_nodes.m_ori;
	m_right_nodes.m_ext = m_right_nodes.m_ori;

	RefreshTriangles();

	LoadTriangles(value["triangles"]);
}

void Strip::Store(Json::Value& value) const
{
	value["type"] = GetType();

	value["width"] = m_width;
	value["height"] = m_height;

	ee::JsonSerializer::Store(m_left_nodes.m_ori, value["left nodes"]);
	ee::JsonSerializer::Store(m_right_nodes.m_ori, value["right nodes"]);

	StoreTriangles(value["triangles"]);
}

void Strip::OffsetUV(float dx, float dy)
{
	// update uv base
	m_uv_offset += dy;
	m_uv_offset = m_uv_offset - floor(m_uv_offset);

	std::vector<std::pair<sm::vec2, sm::vec2> > trans_list;
	GetTransList(trans_list);

	// insert node
	sm::vec2 pos;
	pos.x = 0;
	pos.y = -m_height*0.5f + m_height*m_uv_offset;
	int idx_left, idx_right;
	sm::vec2 pos_left, pos_right;
	idx_left = m_left_nodes.GetNodeInsertPos(pos, pos_left);
	idx_right = m_right_nodes.GetNodeInsertPos(pos, pos_right);
	if (idx_left != -1 && idx_right != -1) {
		std::vector<sm::vec2>& left_ext = m_left_nodes.m_ext;
		std::vector<sm::vec2>& right_ext = m_right_nodes.m_ext;
		MapUV2XY(left_ext, idx_left, pos, trans_list);
		MapUV2XY(right_ext, idx_right, pos, trans_list);
		left_ext.insert(left_ext.begin() + idx_left + 1, pos_left);
		right_ext.insert(right_ext.begin() + idx_right + 1, pos_right);
	}

	// create triangles separate
	ClearTriangles();
	assert(m_left_nodes.Size() == m_right_nodes.Size());
	const std::vector<sm::vec2>& left = m_left_nodes.m_ext;
	const std::vector<sm::vec2>& right = m_right_nodes.m_ext;
	for (int i = 0, n = left.size() - 1; i < n; ++i)
	{
		if (left[i] == left[i+1] && right[i] == right[i+1]) {
			continue;
		}

		Triangle* tri = new Triangle;
		tri->nodes[0] = new Node(left[i], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[1] = new Node(right[i], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[2] = new Node(right[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		m_tris.push_back(tri);

		tri = new Triangle;
		tri->nodes[0] = new Node(left[i], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[1] = new Node(right[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[2] = new Node(left[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		m_tris.push_back(tri);
	}
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			TranslateNode(m_tris[i]->nodes[j], trans_list);
		}
	}

	// set uv between textures
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		sm::rect r;
		r.MakeEmpty();
		for (int i = 0; i < 3; ++i) {
			r.Combine(tri->nodes[i]->uv);
		}

		for (int i = 0; i < 3; ++i) 
		{
			Node* n = tri->nodes[i];
			float y = n->uv.y - m_uv_offset;
			y = y - floor(y);
			if (fabs(y - 0) < 0.0001f && n->uv.y == r.ymax) {
				y = 1;
			}
			if (fabs(y - 1) < 0.0001f && n->uv.y == r.ymin) {
				y = 0;
			}
			n->uv.y = y;
		}
	}
}

void Strip::Reset()
{
	m_uv_offset = 0;
	RefreshTriangles();
}

void Strip::Clear()
{
	InitBound();
	RefreshTriangles();
}

void Strip::InsertNode(const sm::vec2& p)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	if (!m_left_nodes.IsRegionContain(p) || !m_right_nodes.IsRegionContain(p)) {
		return;
	}

	m_left_nodes.Insert(p);
	m_right_nodes.Insert(p);

	RefreshTriangles();
}

void Strip::RemoveNode(const sm::vec2& p)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	int idx;
	idx = m_left_nodes.QueryIndex(p, m_node_radius);
	if (idx != -1) 
	{
		m_left_nodes.Remove(idx);
		m_right_nodes.Remove(idx);
		RefreshTriangles();
	} 
	else 
	{
		idx = m_right_nodes.QueryIndex(p, m_node_radius);
		if (idx != -1) 
		{
			m_left_nodes.Remove(idx);
			m_right_nodes.Remove(idx);
			RefreshTriangles();
		}
	}
}

sm::vec2* Strip::FindNode(const sm::vec2& p)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	sm::vec2* ptr = m_left_nodes.QueryPointer(p, m_node_radius);
	if (ptr == NULL) {
		ptr = m_right_nodes.QueryPointer(p, m_node_radius);
	}
	return ptr;
}

void Strip::MoveNode(sm::vec2* src, const sm::vec2& dst)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	if (!src) return;

	src->y = dst.y;

	m_left_nodes.Sort();
	m_right_nodes.Sort();

	RefreshTriangles();
}

void Strip::InitBound()
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	m_left_nodes.Reset(sm::vec2(-hw, -hh), sm::vec2(-hw,  hh));
	m_right_nodes.Reset(sm::vec2(hw, -hh), sm::vec2( hw,  hh));
}

void Strip::RefreshTriangles()
{
	ClearTriangles();

	assert(m_left_nodes.Size() == m_right_nodes.Size());
	const std::vector<sm::vec2>& left = m_left_nodes.m_ext;
	const std::vector<sm::vec2>& right = m_right_nodes.m_ext;
	Node* last_left = new Node(left[0], static_cast<int>(m_width), static_cast<int>(m_height));
	Node* last_right = new Node(right[0], static_cast<int>(m_width), static_cast<int>(m_height));
	for (int i = 0, n = left.size() - 1; i < n; ++i)
	{
		Node* next_left = new Node(left[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		Node* next_right = new Node(right[i+1], static_cast<int>(m_width), static_cast<int>(m_height));

		Triangle* tri = new Triangle;
		tri->nodes[0] = last_left;
		tri->nodes[1] = last_right;
		tri->nodes[2] = next_right;
		for (int i = 0; i < 3; ++i) {
			tri->nodes[i]->Retain();
		}
		m_tris.push_back(tri);

		tri = new Triangle;
		tri->nodes[0] = last_left;
		tri->nodes[1] = next_right;
		tri->nodes[2] = next_left;
		for (int i = 0; i < 3; ++i) {
			tri->nodes[i]->Retain();
		}
		m_tris.push_back(tri);

		last_left = next_left;
		last_right = next_right;
	}
}

void Strip::CopyTriangles(const Strip& strip)
{
	assert(m_tris.size() == strip.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* src = strip.m_tris[i];
		Triangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

void Strip::AbsorbNodeToRegion(sm::vec2& node)
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	if (fabs(node.x - (-hw)) < m_node_radius) {
		node.x = -hw;
	}
	if (fabs(node.x - hw) < m_node_radius) {
		node.x = hw;
	}
	if (fabs(node.y - (-hh)) < m_node_radius) {
		node.y = -hh;
	}
	if (fabs(node.y - hh) < m_node_radius) {
		node.y = hh;
	}
}

void Strip::GetTransList(std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list) const
{
	std::set<Node*> nodes;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			nodes.insert(tri->nodes[i]);
		}
	}
	std::set<Node*>::iterator itr = nodes.begin();
	for ( ; itr != nodes.end(); ++itr)
	{
		Node* n = *itr;
		if (n->xy != n->ori_xy) {
			trans_list.push_back(std::make_pair(n->ori_xy, n->xy));
		}
	}
}

void Strip::TranslateNode(Node* node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list)
{
	for (int i = 0, m = trans_list.size(); i < m; ++i)
	{
		float dis = ee::Math2D::GetDistanceSquare(node->ori_xy, trans_list[i].first);
		if (dis < 0.01) {
			node->xy = trans_list[i].second;
			break;
		}
	}
}

void Strip::TranslateNode(sm::vec2& node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list)
{
	for (int i = 0, m = trans_list.size(); i < m; ++i)
	{
		float dis = ee::Math2D::GetDistanceSquare(node, trans_list[i].first);
		if (dis < 0.01) {
			node = trans_list[i].second;
			break;
		}
	}
}

void Strip::MapUV2XY(const std::vector<sm::vec2>& nodes, int index, const sm::vec2& pos, 
					 std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list)
{
	const sm::vec2& s = nodes[index];
	const sm::vec2& e = nodes[index+1];

	sm::vec2 xy_s = s, xy_e = e;
	TranslateNode(xy_s, trans_list);
	TranslateNode(xy_e, trans_list);

	sm::vec2 foot;
	int st = ee::Math2D::GetFootOfPerpendicular(s, e, pos, &foot);
	
	float p = ee::Math2D::GetDistance(s, foot) / ee::Math2D::GetDistance(s, e);
 	sm::vec2 xy_p = xy_s + (xy_e - xy_s) * p;
 	trans_list.push_back(std::make_pair(foot, xy_p));
}

//////////////////////////////////////////////////////////////////////////
// class Strip::NodeList
//////////////////////////////////////////////////////////////////////////

void Strip::NodeList::
Reset(const sm::vec2& begin, const sm::vec2& end)
{
	m_ori.clear();
	m_ori.push_back(begin);
	m_ori.push_back(end);
	m_ext = m_ori;
}

void Strip::NodeList::
Insert(const sm::vec2& p)
{
	Insert(m_ori, p);
	m_ext = m_ori;
}

void Strip::NodeList::
Remove(int idx)
{
	m_ori.erase(m_ori.begin() + idx);
	m_ext.erase(m_ext.begin() + idx);
}

int Strip::NodeList::
GetNodeInsertPos(const sm::vec2& p, sm::vec2& nearest)
{
	m_ext = m_ori;

	int idx_nearest = -1;
	float dis_nearest = FLT_MAX;
	sm::vec2 foot_nearest;
	for (int i = 0, n = m_ext.size() - 1; i < n; ++i)
	{
		sm::vec2 foot;
		int st = ee::Math2D::GetFootOfPerpendicular(m_ext[i], m_ext[i+1], p, &foot);
		if (st == -1) {
			continue;
		}
		float dis = ee::Math2D::GetDistanceSquare(foot, p);
		if (dis < dis_nearest) {
			idx_nearest = i;
			dis_nearest = dis;
			foot_nearest = foot;
		}
	}

	if (idx_nearest != -1) {
		nearest = foot_nearest;
	}

	return idx_nearest;
}

int Strip::NodeList::
QueryIndex(const sm::vec2& p, float radius) const
{
	// front and back are bound
	for (int i = 1, n = m_ori.size() - 1; i < n; ++i) {
		if (ee::Math2D::GetDistance(m_ori[i], p) < radius) {
			return i;
		}
	}
	return -1;
}

sm::vec2* Strip::NodeList::
QueryPointer(const sm::vec2& p, float radius)
{
	int idx = QueryIndex(p, radius);
	if (idx != -1) {
		return &m_ori[idx];
	} else {
		return NULL;
	}
}

bool Strip::NodeList::
IsRegionContain(const sm::vec2& p) const
{
	sm::vec2 foot;
	return ee::Math2D::GetFootOfPerpendicular(m_ori.front(), m_ori.back(), p, &foot) != -1;
}

void Strip::NodeList::
Sort()
{
	std::sort(m_ori.begin(), m_ori.end(), sm::Vector2CmpY());
	std::sort(m_ext.begin(), m_ext.end(), sm::Vector2CmpY());
}

void Strip::NodeList::
Insert(std::vector<sm::vec2>& nodes, const sm::vec2& p)
{
	int idx_nearest = -1;
	float dis_nearest = FLT_MAX;
	sm::vec2 foot_nearest;
	for (int i = 0, n = nodes.size() - 1; i < n; ++i)
	{
		sm::vec2 foot;
		int st = ee::Math2D::GetFootOfPerpendicular(nodes[i], nodes[i+1], p, &foot);
		if (st == -1) {
			continue;
		}
		float dis = ee::Math2D::GetDistanceSquare(foot, p);
		if (dis < dis_nearest) {
			idx_nearest = i;
			dis_nearest = dis;
			foot_nearest = foot;
		}
	}
	assert(idx_nearest != -1);
	nodes.insert(nodes.begin() + idx_nearest + 1, foot_nearest);
}

}