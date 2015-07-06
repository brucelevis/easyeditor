#include "Frame.h"
#include "StagePanel.h"
#include "StageCanvas.h"

namespace sg
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_CHANGE_PERSPECTIVE, Frame::onChangePerspective)
	EVT_MENU(ID_SET_BG, Frame::onSetBackground)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_view_menu->Append(ID_CHANGE_PERSPECTIVE, wxT("Perspective"), wxT("Perspective"));
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));
}

void Frame::onChangePerspective(wxCommandEvent& event)
{
	const StagePanel* stage = static_cast<const StagePanel*>((m_task)->GetEditPanel());
	const_cast<StagePanel*>(stage)->SetPerspective(!stage->GetPerspective());
	const_cast<StagePanel*>(stage)->Refresh();
}

void Frame::onSetBackground(wxCommandEvent& event)
{
	wxString filter = wxT("*.png;*.jpg;*.json");
	wxFileDialog dlg(this, wxT("Choose Background"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = dlg.GetPath().ToStdString();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filename);
		const StagePanel* stage = static_cast<const StagePanel*>((m_task)->GetEditPanel());
		d2d::GLCanvas* canvas = stage->getCanvas();
		static_cast<StageCanvas*>(canvas)->SetBackground(symbol);
		symbol->Release();
	}
}

}