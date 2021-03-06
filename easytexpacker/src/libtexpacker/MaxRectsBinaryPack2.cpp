#include "MaxRectsBinaryPack2.h"
#include "math.h"

#include <ee/std_functor.h>
#include <ee/math_common.h>

#include <assert.h>

namespace etexpacker
{

static const float SCALE = 1.0f;
static const int PADDING = 0;

MaxRectsBinaryPack2::MaxRectsBinaryPack2()
{
}

MaxRectsBinaryPack2::~MaxRectsBinaryPack2()
{
	for_each(m_roots.begin(), m_roots.end(), ee::DeletePointerFunctor<ee::TPNode>());
}

void MaxRectsBinaryPack2::GetSize(std::vector<RectSize>& sizes) const
{
	for (int i = 0, n = m_roots.size(); i < n; ++i) {
		ee::TPNode* root = m_roots[i];
		sizes.push_back(RectSize(root->GetWidth(), root->GetHeight()));
	}
}

void MaxRectsBinaryPack2::Pack(PACK_STRATEGY strategy, int static_size, int max_size,
							   int min_size, const std::vector<RectSize>& rects, std::vector<Rect>& output)
{
	if (rects.empty()) {
		return;
	}

	// prepare data
	int sz = rects.size();
	output.clear();
	output.resize(sz);

	std::vector<Sprite> sprites;
	sprites.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		sprites.push_back(Sprite(&rects[i], &output[i]));
	}
	std::sort(sprites.begin(), sprites.end(), SpriteCmp(e_max_edge));

	// compute area
	int area = 0;
	for (int i = 0; i < sz; ++i) {
		area += rects[i].width * rects[i].height;
	}

	switch(strategy)
	{
	case PACK_AUTO:
		PackAuto(sprites, area);
		break;
	case PACK_SQUARE:
		PackSquare(sprites, area);
		break;
	case PACK_SQUARE_MULTI:
		PackSquareMulti(sprites, static_size);
		break;
	case PACK_SQUARE_MULTI_AUTO:
		PackSquareMultiAuto(sprites, area, max_size, min_size);
		break;
	default:
		output.clear();
	}
}

ee::TPNode* MaxRectsBinaryPack2::NewRoot(int w, int h)
{
	ee::TPNode* root = new ee::TPNode(w, h);
	ee::TPNode* c = new ee::TPNode(w, h);
	root->SetChild(c);
	return root;
}

bool MaxRectsBinaryPack2::Insert(ee::TPNode* root, const Sprite* sprite, int tex_id) const
{
	ee::TPNode* n = NULL;
	int w = static_cast<int>(sprite->size->width * SCALE + PADDING * 2),
		h = static_cast<int>(sprite->size->height * SCALE + PADDING * 2);
	float scale = 1.0f;

	bool rot = false;
	if (h > w) {
		rot = true ;
	}

	if (rot)
	{
		if (root->IsRoomEnough(h, w)) {
			n = root->Insert(h, w);
		} else {
			rot = !rot;
			n = root->Insert(w, h);
		}
	}
	else
	{
		if (root->IsRoomEnough(w, h)) {
			n = root->Insert(w, h);
		} else {
			rot = !rot;
			n = root->Insert(h, w);
		}
	}

	if (!n) {
		return false;
	} else {
		sprite->pos->tex_id = tex_id;
		sprite->pos->x = n->GetMinX() + PADDING;
		sprite->pos->y = n->GetMinY() + PADDING;
		if (n->IsRotated() && !rot || !n->IsRotated() && rot) {
			sprite->pos->width = h - PADDING * 2;
			sprite->pos->height = w - PADDING * 2;
		} else {
			sprite->pos->width = w - PADDING * 2;
			sprite->pos->height = h - PADDING * 2;
		}
		return true;
	}
}

void MaxRectsBinaryPack2::PackAuto(const std::vector<Sprite>& sprites, int area)
{
	int edge = ee::next_p2((int)ceil(sqrt((float)area)));
	int w = edge, h = (edge >> 1);

	bool success = false;
	while (!success)
	{
		ee::TPNode* root = NewRoot(w, h);
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			success = Insert(root, &sprites[i]);
			if (!success) {
				break;
			}
		}

		if (success) {
			m_roots.push_back(root);
		} else {
			delete root;
		}

 		if (w == h) {
 			w *= 2;
 		} else if (w == h * 2) {
 			w /= 2;
 			h *= 2;
 		} else {
 			assert(h == w * 2);
 			w *= 2;
 		}
	}
}

void MaxRectsBinaryPack2::PackSquare(const std::vector<Sprite>& sprites, int area)
{
	int edge = ee::next_p2((int)ceil(sqrt((float)area)));
	bool success = false;
	while (!success)
	{
		ee::TPNode* root = NewRoot(edge, edge);
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			success = Insert(root, &sprites[i]);
			if (!success) {
				break;
			}
		}

		if (success) {
			m_roots.push_back(root);
		} else {
			delete root;
		}

		edge *= 2;
	}
}

static inline 
int CalArea(const std::vector<Sprite>& list) {
	int area = 0;
	for (int i = 0, n = list.size(); i < n; ++i) {
		area += list[i].pos->width * list[i].pos->height;
	}
	return area;
}

void MaxRectsBinaryPack2::PackSquareMulti(std::vector<Sprite>& sprites, int static_size)
{
	std::vector<Sprite> curr_list = sprites;

	int curr_tex = 0;
	while (!curr_list.empty()) 
	{
		std::vector<Sprite> success_list, fail_list;

		ee::TPNode* root = NewRoot(static_size, static_size);
		for (int i = 0, n = curr_list.size(); i < n; ++i) 
		{
			const Sprite& spr = curr_list[i];
			bool success = Insert(root, &spr, m_roots.size());
			if (success) {
				spr.pos->tex_id = curr_tex;
				success_list.push_back(spr);
			} else {
				fail_list.push_back(spr);
			}
		}

		curr_list = fail_list;
		m_roots.push_back(root);
		++curr_tex;
	}
}

void MaxRectsBinaryPack2::PackSquareMultiAuto(std::vector<Sprite>& sprites, int area,
											  int max_size, int min_size)
{
	static const float AREA_SCALE_LIMIT = 0.8f;
	static const float AREA_SCALE_LIMIT_STEP = 0.05f;

//	static const float AREA_LIMIT = 64 * 64;
	static const int EDGE_LIMIT = 128;
	static const int LEFT_AREA_LIMIT = 25000;

	float area_scale_limit = AREA_SCALE_LIMIT;

	int edge = std::max(std::min(ee::next_p2((int)ceil(sqrt((float)area))), max_size), min_size);

	std::vector<Sprite> curr_list = sprites;

	float fail_scale = -1;
	
	int curr_tex = 0;
	while (!curr_list.empty()) 
	{
		std::vector<Sprite> success_list, fail_list;

		ee::TPNode* root = NewRoot(edge, edge);
		for (int i = 0, n = curr_list.size(); i < n; ++i) 
		{
			const Sprite& spr = curr_list[i];
			bool success = Insert(root, &spr, m_roots.size());
			if (success) {
				spr.pos->tex_id = curr_tex;
				success_list.push_back(spr);
			} else {
				fail_list.push_back(spr);
			}
		}

		int used_area = CalArea(success_list);
		float used_area_rate = (float)used_area / (edge*edge);
		int left_area = edge*edge - used_area;
		if (success_list.empty() || used_area_rate < fail_scale)
		{
			if (edge * 2 <= max_size) {
				edge *= 2;
			} else {
				edge = max_size;
			}
			delete root;
		}
		else if (used_area_rate > area_scale_limit || edge < EDGE_LIMIT/* || left_area < LEFT_AREA_LIMIT*/) 
		{
			fail_scale = -1;
			area_scale_limit = std::min(area_scale_limit * (1 + AREA_SCALE_LIMIT_STEP), AREA_SCALE_LIMIT);
			curr_list = fail_list;
			area = area - used_area;
			edge = std::max(std::min(ee::next_p2((int)ceil(sqrt((float)area))), max_size), min_size);
			m_roots.push_back(root);
			++curr_tex;
		} 
		else 
		{
			fail_scale = used_area_rate;
			area_scale_limit *= (1 - AREA_SCALE_LIMIT_STEP);
			edge = std::max(edge / 2, min_size);
			delete root;
		}
	}

	MergeSquareMultiAuto(sprites, max_size);
}

bool MaxRectsBinaryPack2::MergeSquareMultiAuto(std::vector<Sprite>& sprites, int max_size)
{
	std::vector<int> ids;
	int count = 0;
	for (int i = 0, n = m_roots.size(); i < n; ++i) {
		ee::TPNode* n0 = m_roots[i];
		if (n0->GetWidth() * 2 >= max_size || n0->GetHeight() * 2 >= max_size) {
			continue;
		}
		ids.push_back(i);
		for (int j = 0, m = m_roots.size(); j < m; ++j) {
			ee::TPNode* n1 = m_roots[j];
			if (n1->GetWidth() * 2 >= max_size || n1->GetHeight() * 2 >= max_size) {
				continue;
			}
			if (j != i && n0->GetWidth() == n1->GetWidth() && n0->GetHeight() == n1->GetHeight()) {
				ids.push_back(j);
			}
		}
		if (ids.size() >= 4) {
			break;
		} else {
			ids.clear();
		}
	}
	if (ids.size() < 4) {
		return false;
	}

	std::vector<ee::TPNode*> remove_nodes;

	std::vector<int> map_id;
	ee::TPNode* new_root = NewRoot(m_roots[ids[0]]->GetWidth() * 2, m_roots[ids[0]]->GetHeight() * 2);
	std::vector<ee::TPNode*> new_roots;
	for (int i = 0, n = m_roots.size(); i < n; ++i) {
		if (i != ids[0] && i != ids[1] && i != ids[2] && i != ids[3]) {
			map_id.push_back(new_roots.size());
			new_roots.push_back(m_roots[i]);
		} else {
			remove_nodes.push_back(m_roots[i]);
			map_id.push_back(m_roots.size() - 4);
		}
	}
	new_roots.push_back(new_root);

	std::vector<Rect> tmp_rects;
	tmp_rects.reserve(sprites.size());
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		const Sprite& spr = sprites[i];
		tmp_rects.push_back(*spr.pos);
		if (spr.pos->tex_id == ids[0] || spr.pos->tex_id == ids[1] ||
			spr.pos->tex_id == ids[2] || spr.pos->tex_id == ids[3]) {
			bool success = Insert(new_root, &spr, m_roots.size() - 4);
			if (!success) {
				// todo
				for (int i = 0, n = tmp_rects.size(); i < n; ++i) {
					*sprites[i].pos = tmp_rects[i];
				}
				delete new_root;
				return false;
			}
		} else {
			spr.pos->tex_id = map_id[spr.pos->tex_id];
		}
	}

	m_roots = new_roots;
	for (int i = 0, n = remove_nodes.size(); i < n; ++i) {
		delete remove_nodes[i];
	}
	
	MergeSquareMultiAuto(sprites, max_size);
	return true;
}

void MaxRectsBinaryPack2::SortRoots(const std::vector<Sprite>& sprites)
{
	std::vector<ee::TPNode*> sorted(m_roots);
	std::sort(sorted.begin(), sorted.end(), NodeCmp());
	std::vector<int> map_id;
	for (int i = 0, n = m_roots.size(); i < n; ++i) {
		for (int j = 0, m = sorted.size(); j < m; ++j) {
			if (m_roots[i] == sorted[j]) {
				map_id.push_back(j);
				break;
			}
		}
	}
	assert(map_id.size() == m_roots.size());

	m_roots = sorted;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i].pos->tex_id = map_id[sprites[i].pos->tex_id];
	}
}

bool MaxRectsBinaryPack2::NodeCmp::operator() (const ee::TPNode* n0, ee::TPNode* n1) const
{
	return n0->GetWidth() * n0->GetHeight() > n1->GetWidth() * n1->GetHeight();
}

}