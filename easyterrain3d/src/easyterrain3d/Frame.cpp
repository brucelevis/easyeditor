#include "Frame.h"

namespace eterrain3d
{

Frame::Frame(const wxString& title)
	: d2d::Frame(title, "")
{
#ifdef _DEBUG
	wxLog::SetActiveTarget(new wxLogWindow(this, _T("Log window")));
	m_log_chain = new wxLogChain(new wxLogStderr);
#endif
}

Frame::~Frame()
{
#ifdef _DEBUG
	delete m_log_chain;
#endif
}

void Frame::OnNew(wxCommandEvent& event)
{
}

void Frame::OnOpen(wxCommandEvent& event)
{
}

void Frame::OnSave(wxCommandEvent& event)
{
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
}

}