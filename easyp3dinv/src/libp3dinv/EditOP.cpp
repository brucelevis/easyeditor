#include "EditOP.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace ep3dinv
{

EditOP::EditOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->Reset();

	return false;
}

}