#include "ToolBarPanel.h"
#include "StagePanel.h"

namespace etemplate
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
}

wxSizer* ToolbarPanel::InitLayout()
{
	return NULL;
}

}