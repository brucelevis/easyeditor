#include "SymbolPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/Symbol.h"
#include "view/EditPanelImpl.h"
#include "view/IStageCanvas.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

SymbolPropertySetting::SymbolPropertySetting(Symbol* symbol)
	: IPropertySetting("Symbol")
	, m_symbol(symbol)
	, m_name(NULL)
{
}

SymbolPropertySetting::SymbolPropertySetting(std::string* name,
											 std::string* tag)
	: IPropertySetting("Symbol")
	, m_symbol(NULL)
	, m_name(name)
	, m_tag(tag)
{
}

void SymbolPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		if (m_symbol) {
			m_symbol->name = wxANY_AS(value, wxString);
		} else {
			*m_name = wxANY_AS(value, wxString);
		}
	}
	else if (name == "Tag") 
	{
		if (m_symbol) {
			m_symbol->tag = wxANY_AS(value, wxString);
		} else {
			*m_tag = wxANY_AS(value, wxString);
		}
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	if (m_symbol) {
		pg->GetProperty(wxT("Name"))->SetValue(m_symbol->name);
		pg->GetProperty(wxT("Tag"))->SetValue(m_symbol->tag);
	} else {
		pg->GetProperty(wxT("Name"))->SetValue(*m_name);
		pg->GetProperty(wxT("Name"))->SetValue(*m_tag);
	}
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	if (m_symbol) {
		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_symbol->name));
		pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, m_symbol->tag));
	} else {
		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, *m_name));
		pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, *m_tag));
	}
}

}