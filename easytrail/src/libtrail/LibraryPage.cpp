#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

namespace etrail
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Trail")
{
	InitLayout();
	m_list->SetFileter(ee::FileType::GetTag(ee::FileType::e_trail));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = ee::FileType::GetTag(ee::FileType::e_trail);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����trail�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i];
			try {
				ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				m_list->Insert(symbol);
				symbol->Release();
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}