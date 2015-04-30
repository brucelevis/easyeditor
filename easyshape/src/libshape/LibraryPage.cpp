#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace libshape
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Shape"))
{
	initLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = "*_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
	wxFileDialog dlg(this, wxT("choose shape files"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filepath = filenames[i];
			try {
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				m_list->insert(symbol);
				symbol->Release();
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}