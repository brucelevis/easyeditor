#include "ResizeBaseOP.h"
#include "ResizeAtomicOP.h"
#include "Symbol.h"

namespace escale9
{

ResizeBaseOP::ResizeBaseOP(wxWindow* wnd, d2d::EditPanelImpl* stage, Symbol* symbol)
	: d2d::ZoomViewOP(wnd, stage, true)
	, m_symbol(symbol)
	, m_status(e_null)
{
}

bool ResizeBaseOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_symbol) {
		return false;
	}

	m_firstPos = m_stage->TransPosScrToProj(x, y);

	const float hw = m_symbol->GetSize().Width() * 0.5f,
		hh = m_symbol->GetSize().Height() * 0.5f;
	if (d2d::Math2D::IsPointInRect(m_firstPos, d2d::Vector(-hw, -hh), REGION, REGION))
		m_status = e_leftlow;
	else if (d2d::Math2D::IsPointInRect(m_firstPos, d2d::Vector(hw, -hh), REGION, REGION))
		m_status = e_rightlow;
	else if (d2d::Math2D::IsPointInRect(m_firstPos, d2d::Vector(hw, hh), REGION, REGION))
		m_status = e_rightup;
	else if (d2d::Math2D::IsPointInRect(m_firstPos, d2d::Vector(-hw, hh), REGION, REGION))
		m_status = e_leftup;

	return false;
}

bool ResizeBaseOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (m_status != e_null)
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		const d2d::Vector src(fabs(m_firstPos.x)*2, fabs(m_firstPos.y)*2),
			dst(fabs(pos.x)*2, fabs(pos.y)*2);
		d2d::EditAddRecordSJ::Instance()->Add(new ResizeAtomicOP(m_symbol, src, dst));
	}

	m_status = e_null;


	return false;
}

bool ResizeBaseOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_status != e_null)
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_symbol->ResizeScale9(fabs(pos.x)*2, fabs(pos.y)*2);
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool ResizeBaseOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	d2d::SpriteRenderer::Instance()->Draw(m_symbol);

	const float hw = m_symbol->GetSize().Width() * 0.5f,
		hh = m_symbol->GetSize().Height() * 0.5f;
	const float r = REGION;
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(-hw, -hh), r, r, m_style);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector( hw, -hh), r, r, m_style);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector( hw,  hh), r, r, m_style);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(-hw,  hh), r, r, m_style);

	return false;
}
} // escale9