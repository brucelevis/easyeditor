#include "LibraryFontPage.h"
#include "LibraryList.h"
#include "ExceptionDlg.h"

#include "dataset/SymbolMgr.h"
#include "dataset/FontSymbol.h"
#include "common/Exception.h"

namespace d2d
{

LibraryFontPage::LibraryFontPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Font"))
{
	InitLayout();
}

bool LibraryFontPage::IsHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<FontSymbol*>(symbol) != NULL;
}

void LibraryFontPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Choose Font"), wxEmptyString, 
		wxEmptyString, wxT("*.ttf"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		try {
			ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(dlg.GetPath().ToStdString());
			m_list->insert(symbol);
			symbol->Release();
		} catch (Exception& e) {
			ExceptionDlg dlg(m_parent, e);
			dlg.ShowModal();
		}
	}
}
} // d2d