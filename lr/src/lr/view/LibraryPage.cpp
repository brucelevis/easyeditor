#include "LibraryPage.h"
#include "LibraryPanel.h"

#include "dataset/Layer.h"
#include "dataset/LayerFactory.h"

#include <ee/EditOP.h>
#include <ee/SymbolMgr.h>
#include <ee/panel_msg.h>
#include <ee/Symbol.h>
#include <ee/Sprite.h>
#include <ee/Shape.h>
#include <ee/LibraryList.h>

#include <algorithm>

namespace lr
{

LibraryPage::LibraryPage(LibraryPanel* library, const std::string& name, 
						 LayerType type, int id, s2::CameraMode cam)
	: ee::LibraryPage(library->GetNotebook(), name)
	, m_layer_type(type)
	, m_cam_mode(cam)
	, m_curr_op_idx(-1)
{
	m_layer = LayerFactory::Create(id, library, type, cam);
	InitLayout();
}

LibraryPage::~LibraryPage()
{
	m_layer->Release();

	for_each(m_editops.begin(), m_editops.end(), ee::ReleaseObjectFunctor<ee::EditOP>());
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return true;
}

void LibraryPage::UpdateStatusFromLayer()
{
	m_visible_ctrl->SetValue(m_layer->IsVisible());	
	m_editable_ctrl->SetValue(m_layer->IsEditable());
}

void LibraryPage::SetLayer(Layer* layer)
{
	if (m_layer != layer) {
		m_layer->Release();
		layer->Retain();
		m_layer = layer;
	}
}

void LibraryPage::AddEditOP(ee::EditOP* editop)
{
	editop->Retain();
	m_editops.push_back(editop);
	if (m_curr_op_idx < 0) {
		m_curr_op_idx = 0;
	}
}

ee::EditOP* LibraryPage::GetNextEditOP()
{
	if (m_editops.empty()) {
		return NULL;
	}

	++m_curr_op_idx;
	if (m_curr_op_idx >= m_editops.size()) {
		m_curr_op_idx = 0;
	}
	return m_editops[m_curr_op_idx];
}

void LibraryPage::InitLayoutExtend(wxSizer* sizer)
{
	{
		m_visible_ctrl = new wxCheckBox(this, wxID_ANY, wxT("可见"));
		m_visible_ctrl->SetValue(m_layer->IsVisible());
		Connect(m_visible_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnChangeVisible));
		sizer->Add(m_visible_ctrl, 0, wxALIGN_LEFT);
	}
	{
		m_editable_ctrl = new wxCheckBox(this, wxID_ANY, wxT("编辑"));
		m_editable_ctrl->SetValue(m_layer->IsEditable());
		Connect(m_editable_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnChangeEditable));
		sizer->Add(m_editable_ctrl, 0, wxALIGN_LEFT);
	}
	{
		m_reset = new wxButton(this, wxID_ANY, "重置", wxDefaultPosition, wxSize(40, 20));
		Connect(m_reset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnResetSpritesVisibleEditable));
		sizer->Add(m_reset, 0, wxALIGN_RIGHT);
	}
	sizer->AddSpacer(20);
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("导入symbol"), wxEmptyString, 
		wxEmptyString, "*.*", wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->Insert(symbol);
			symbol->Release();
		}
	}
}

void LibraryPage::OnChangeVisible(wxCommandEvent& event)
{
	m_layer->SetVisible(event.IsChecked());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void LibraryPage::OnChangeEditable(wxCommandEvent& event)
{
	m_layer->SetEditable(event.IsChecked());
}

void LibraryPage::OnResetSpritesVisibleEditable(wxCommandEvent& event)
{
	m_layer->ResetSpritesVisibleEditable();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	ee::RefreshPanelSJ::Instance()->Refresh();
}

}