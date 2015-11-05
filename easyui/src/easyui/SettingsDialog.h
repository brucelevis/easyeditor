#ifndef _EASYUI_SETTINGS_DIALOG_H_
#define _EASYUI_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace eui
{
namespace window { class AnchorMgr; }

class SettingDialog : public wxDialog
{
public:
	SettingDialog(wxWindow* parent, window::AnchorMgr* anchor_mgr);

private:
	void InitLayout();

	void OnViewSizeChanged(wxCommandEvent& event);

private:
	window::AnchorMgr* m_anchor_mgr;

	wxTextCtrl *m_width_ctrl, *m_height_ctrl;

}; // SettingDialog

}

#endif // _EASYUI_SETTINGS_DIALOG_H_