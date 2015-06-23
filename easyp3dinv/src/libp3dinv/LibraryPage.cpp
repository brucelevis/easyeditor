#include "LibraryPage.h"
#include "config.h"

namespace ep3dinv
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Template"))
{
	InitLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}