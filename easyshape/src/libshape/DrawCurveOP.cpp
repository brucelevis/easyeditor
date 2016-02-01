#include "DrawCurveOP.h"
#include "DrawLineUtility.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/PrimitiveDraw.h>

namespace eshape
{

DrawCurveOP::DrawCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_startDraw(false)
	, m_straight_mode(false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	m_firstPos.SetInvalid();
}

bool DrawCurveOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPos.Set(x, y);

	return false;
}

bool DrawCurveOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_firstPos.SetInvalid();
	m_startDraw = false;

	return false;
}

bool DrawCurveOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (!m_startDraw && m_firstPos.IsValid() && 
		(m_firstPos.x != x || m_firstPos.y != y)) {
		m_startDraw = true;
	}

	if (!m_startDraw) {
		return false;
	}

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_curve, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_curve.back(), pos);
		m_curve.pop_back();
	}
	m_curve.push_back(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawCurveOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (!m_curve.empty()) {
		ee::PrimitiveDraw::DrawPolyline(m_curve, ee::Colorf(0, 0, 0), false, 1);
	}

	return false;
}

bool DrawCurveOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_curve.clear();
	m_firstPos.SetInvalid();
	m_startDraw = false;

	return false;
}

}