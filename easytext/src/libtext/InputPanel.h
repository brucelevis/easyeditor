#ifndef _EASYTEXT_INPUT_PANEL_H_
#define _EASYTEXT_INPUT_PANEL_H_

#include <wx/panel.h>

namespace ee { class EditPanelImpl; }

class wxTextCtrl;
class wxButton;

namespace etext
{

class Sprite;

class InputPanel : public wxPanel
{
public:
	InputPanel(wxWindow* parent, Sprite* spr,
		ee::EditPanelImpl* stage_impl);
	virtual ~InputPanel();

private:
	void InitLayout();

	void OnEnterPress(wxCommandEvent& event);

private:
	Sprite* m_spr;

	ee::EditPanelImpl* m_stage_impl;

	wxTextCtrl* m_text_ctrl;

	wxButton* m_enter_btn;

}; // InputPanel

}

#endif // _EASYTEXT_INPUT_PANEL_H_