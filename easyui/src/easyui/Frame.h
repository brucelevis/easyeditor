#ifndef _EASYUI_FRAME_H_
#define _EASYUI_FRAME_H_

#include <drag2d.h>

namespace eui
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

private:
	void OnPreview(wxCommandEvent& event);
	void OnCode(wxCommandEvent& event);

private:
	enum
	{
		ID_PREVIEW = 1100,
		ID_SETING_EXTEND,
		ID_CODE,
	};

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYUI_FRAME_H_