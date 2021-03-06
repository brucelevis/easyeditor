#include "LibraryPage.h"

#include <ee/SymbolMgr.h>
#include <ee/LibraryList.h>
#include <ee/StringHelper.h>
#include <ee/FileType.h>

#include <easyui.h>
#include <easycomplex.h>

namespace eui
{
namespace window
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name,
						 const std::string& filter)
	: ee::LibraryPage(parent, name)
	, m_filter(filter)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	if (m_filter == ee::FileType::GetTag(ee::FileType::e_uiwnd)) {
		return dynamic_cast<Symbol*>(symbol) != NULL;	
	} else {
		return true;
	}
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter;
	if (m_filter.empty()) {
		filter = "*.*";
	} else {
		filter = ee::StringHelper::Format("JSON files (*_%s.json)|*_%s.json", m_filter.c_str(), m_filter.c_str());
	}
	wxFileDialog dlg(this, wxT("����symbol"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->Insert(symbol);
			symbol->Release();
		}
	}
}

}
}