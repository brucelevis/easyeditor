#include "SymbolRender.h"
#include "StagePanel.h"
#include "SymbolExt.h"

namespace sg
{
	
SymbolRender* SymbolRender::m_instance = NULL;

SymbolRender::SymbolRender()
	: m_stage(NULL)
	, m_region(NULL)
	, m_region_size(0)
{
	m_grid = NULL;
	m_arrow_down = m_arrow_right = NULL;
	for (int i = 0; i < GRASS_COUNT; ++i) {
		m_grass[i] = NULL;
	}
}

SymbolRender::~SymbolRender()
{
	m_grid->Release();
	m_arrow_down->Release();
	m_arrow_right->Release();
	for (int i = 0; i < GRASS_COUNT; ++i) {
		m_grass[i]->Release();
	}
}

void SymbolRender::Init(StagePanel* stage)
{
	m_stage = stage;
}

void SymbolRender::DrawGrass(const d2d::ISymbol& symbol, 
							 const d2d::Vector& pos, 
							 bool is_flat) const
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return;
	}

	d2d::Vector p = pos;
	if (info->size % 2 == 0)
	{
		int row, col;
		m_stage->TransCoordsToGridPos(pos, row, col);

		d2d::Vector fixed;
		m_stage->TransGridPosToCoords(row - 1, col - 1, fixed);

		p = (pos + fixed) * 0.5f;
	}

	d2d::ISprite* grass = m_grass[info->size - 1];
	if (is_flat)
	{
		d2d::Rect r = grass->GetSymbol().GetSize();

		float half_edge = info->size * EDGE * 0.5f;

		d2d::Matrix mat;
		mat.translate(p.x, p.y);
		d2d::PrimitiveDraw::rect(mat, half_edge, half_edge, d2d::LIGHT_GREEN_FACE);
	}
	else
	{
		grass->SetTransform(p, 0);
		d2d::SpriteRenderer::Instance()->Draw(grass);
	}
}

void SymbolRender::DrawGrids(const d2d::ISymbol& symbol, 
							 const d2d::Vector& pos, 
							 bool valid,
							 bool is_flat) const
{
	if (!symbol.GetUserData()) {
		return;
	}

	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);

	d2d::Colorf color = valid ? d2d::Colorf(0, 1.0f, 0, 0.8f) : d2d::Colorf(1, 0, 0, 0.8f);

	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	int center = (info->size >> 1);
	if (is_flat)
	{
		d2d::Vector pos;
		m_stage->TransGridPosToCoords(row, col, pos);
		d2d::Matrix mat;
		mat.translate(pos.x, pos.y);

		float half_edge = info->size * EDGE * 0.5f;

		d2d::ShapeStyle style = d2d::LIGHT_GREEN_FACE;
		style.color = color;

 		d2d::PrimitiveDraw::rect(mat, half_edge, half_edge, style);
	}
	else
	{
		for (int i = 0; i < info->size; ++i) {
			for (int j = 0; j < info->size; ++j) {
				d2d::Vector pos;
				m_stage->TransGridPosToCoords(row + i - center, col + j - center, pos);
				m_grid->SetTransform(pos, m_grid->GetAngle());
				d2d::ColorTrans ct;
				ct.multi = color;
				d2d::SpriteRenderer::Instance()->Draw(m_grid, NULL, d2d::Matrix(), ct);
			}
		}
	}
}

void SymbolRender::DrawArrow(const d2d::ISymbol& symbol, 
							 const d2d::Vector& pos) const
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return;
	}
	int r = (info->size >> 1) + 2;

	d2d::SpriteRenderer* rd = d2d::SpriteRenderer::Instance();

	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);
	// left
	{
		d2d::Vector pos;
		m_stage->TransGridPosToCoords(row, col - r, pos);
		rd->Draw(m_arrow_down, d2d::Matrix(), pos, 0, -1);
	}
	// right
	{
		d2d::Vector pos;
		m_stage->TransGridPosToCoords(row, col + r, pos);
		rd->Draw(m_arrow_right, d2d::Matrix(), pos);
	}
	// up
	{
		d2d::Vector pos;
		m_stage->TransGridPosToCoords(row + r, col, pos);
		rd->Draw(m_arrow_right, d2d::Matrix(), pos, 0, -1);
	}
	// down
	{
		d2d::Vector pos;
		m_stage->TransGridPosToCoords(row - r, col, pos);
		rd->Draw(m_arrow_down, d2d::Matrix(), pos);
	}
}

void SymbolRender::DrawRegion(const d2d::ISymbol& symbol, const d2d::Vector& pos)
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return;
	}

	int max_reg, min_reg;
	bool find = m_stage->GetBuildingCfg().QueryAttackRegion(info->building->name, max_reg, min_reg);
	if (!find) {
		return;
	}

	float s = (float)(max_reg) / m_region_size;
	d2d::SpriteRenderer* rd = d2d::SpriteRenderer::Instance();
	d2d::ColorTrans ct;
	ct.multi = d2d::Colorf(1, 0, 0);
	rd->Draw(m_region, d2d::Matrix(), pos, 0, s, s, 0, 0, ct);
	if (min_reg != 0) {
		float s = (float)(min_reg) / m_region_size;
		rd->Draw(m_region, d2d::Matrix(), pos, 0, s, s);
	}
}

SymbolRender* SymbolRender::Instance()
{
	if (!m_instance)
	{
		m_instance = new SymbolRender();
	}
	return m_instance;
}

}