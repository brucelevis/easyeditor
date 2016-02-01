
#include "FixtureEditCmpt.h"
#include "StagePanel.h"
#include "SelectFixtureOP.h"

using namespace emodeling;

FixtureEditCmpt::FixtureEditCmpt(wxWindow* parent, const wxString& name, 
								 StagePanel* editPanel, ee::PropertySettingPanel* property)
	: ee::EditCMPT(parent, name, editPanel->GetStageImpl())
{
	m_editop = new SelectFixtureOP(editPanel, property);
}

wxSizer* FixtureEditCmpt::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	return topSizer;
}
