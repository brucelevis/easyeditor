#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

namespace eshadow
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Shadow")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = ee::FileType::GetTag(ee::FileType::e_shadow);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����shadow�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			try {
				ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				AddItem(symbol);
				symbol->Release();
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}