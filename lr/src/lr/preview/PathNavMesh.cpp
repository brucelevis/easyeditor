#include "PathNavMesh.h"

#include <ee/BoundingBox.h>
#include <ee/Math2D.h>
#include <ee/color_config.h>
#include <ee/EE_RVG.h>
#include <ee/Triangulation.h>

#include <easyshape.h>

namespace lr
{
namespace preview
{

PathNavMesh::PathNavMesh(const sm::rect& region)
	: m_region(region)
	, m_visited(this)
{
}

void PathNavMesh::DisableRegion(const ee::Sprite* spr, bool disable)
{
	m_tris.clear();

	// get bound
	std::vector<sm::vec2> bound;
	const eshape::Sprite* shape = dynamic_cast<const eshape::Sprite*>(spr);
	if (shape && shape->GetSymbol().GetShapeType() == eshape::ST_POLYGON)
	{
		const std::vector<ee::Shape*>& shapes = shape->GetSymbol().GetShapes();
		const eshape::PolygonShape* poly = static_cast<const eshape::PolygonShape*>(shapes[0]);
		bound = poly->GetVertices();
	}
	else
	{
		spr->GetBounding()->GetBoundPos(bound);
	}

	std::map<const ee::Sprite*, std::vector<sm::vec2> >::iterator itr = m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		std::vector<sm::vec2> fixed;
		ee::Math2D::RemoveDuplicatePoints(bound, fixed);
		sm::mat4 mat = spr->GetTransMatrix();
		for (int i = 0; i < fixed.size(); ++i) {
			fixed[i] = ee::Math2D::TransVector(fixed[i], mat);
		}
		m_bounds.insert(std::make_pair(spr, fixed));
	} else if (itr != m_bounds.end() && disable) {
		m_bounds.erase(itr);
	}

	// create nodes
	std::vector<sm::vec2> all_bound;
	all_bound.push_back(sm::vec2(m_region.xmin, m_region.ymin));
	all_bound.push_back(sm::vec2(m_region.xmin, m_region.ymax));
	all_bound.push_back(sm::vec2(m_region.xmax, m_region.ymax));
	all_bound.push_back(sm::vec2(m_region.xmax, m_region.ymin));

	std::vector<sm::vec2> lines;
	std::vector<std::vector<sm::vec2> > loops;
	for (itr = m_bounds.begin(); itr != m_bounds.end(); ++itr) {
		loops.push_back(itr->second);
	}
	ee::Triangulation::LinesAndLoops(all_bound, lines, loops, m_tris);
}

void PathNavMesh::QueryRoute(const sm::vec2& start, const sm::vec2& end)
{
	
}

void PathNavMesh::DebugDraw() const
{
	for (int i = 0, n = m_tris.size(); i < n; ) {
		std::vector<sm::vec2> polyline;
		polyline.push_back(m_tris[i++]);
		polyline.push_back(m_tris[i++]);
		polyline.push_back(m_tris[i++]);
		ee::RVG::Color(ee::LIGHT_RED);
		ee::RVG::Polyline(polyline, true);
	}
}

sm::vec2 PathNavMesh::TransIDToPos(int id) const
{
	sm::vec2 ret;

	// todo 

	return ret;
}

}
}