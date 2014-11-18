#ifndef _EASYMESH_FRAME_H_
#define _EASYMESH_FRAME_H_

#include <drag2d.h>

namespace emesh
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);
	virtual ~Frame();

protected:
	virtual void onSaveAs(wxCommandEvent& event);

private:
	void OnSetBackground(wxCommandEvent& event);

private:
	enum
	{
		ID_SET_BG = 1100,
	};

#ifdef _DEBUG
private:
	wxLogChain* m_log_chain;
#endif

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYMESH_FRAME_H_