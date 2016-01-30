#include "Task.h"

#include <easyshape.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easytexture.h>

#include <wx/splitter.h>

namespace eshape
{

Task::Task(wxFrame* parent)
{
	InitLayout(parent);
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
//	delete m_root;
}

void Task::Load(const char* filepath)
{
	d2d::FileType::Type type = d2d::FileType::GetType(filepath);
	if (type == d2d::FileType::e_shape) {
		m_stage->LoadFromFile(filepath);		
	} else if (type == d2d::FileType::e_image 
		|| type == d2d::FileType::e_complex
		|| type == d2d::FileType::e_texture) {
		d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		m_stage->SetSymbolBG(symbol);
		symbol->Release();
	}
}

void Task::Store(const char* filepath) const
{
	std::string fixed = filepath;
	d2d::FileType::Type type = d2d::FileType::GetType(fixed);
	if (type != d2d::FileType::e_shape) {
		wxString tag = d2d::FileType::GetTag(d2d::FileType::e_shape);
		fixed = d2d::FileHelper::GetFilenameAddTag(fixed, tag, "json");
	}
	m_stage->StoreToFile(fixed.c_str());
}

bool Task::IsDirty() const
{
	return true;
}

void Task::GetAllSprite(std::vector<const d2d::Sprite*>& sprites) const
{

}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return NULL;
}

void Task::InitLayout(wxFrame* parent)
{
	wxSplitterWindow* right_vert = new wxSplitterWindow(parent);
	wxSplitterWindow* left_vert = new wxSplitterWindow(right_vert);
	wxSplitterWindow* left_hori = new wxSplitterWindow(left_vert);

	m_library = new d2d::LibraryPanel(left_hori);
	wxWindow* nb = m_library->GetNotebook();
	m_library->AddPage(new d2d::LibraryImagePage(nb));
	m_library->AddPage(new ecomplex::LibraryPage(nb));
	m_library->AddPage(new libanim::LibraryPage(nb));
	m_library->AddPage(new etexture::LibraryPage(nb));

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(left_hori);

	m_stage = new libshape::StagePanel(left_vert, parent, m_library);
	property->SetEditPanel(m_stage->GetStageImpl());

	d2d::ToolbarPanel* toolbar = new libshape::ToolbarPanel(right_vert, property, m_stage);

	left_hori->SetSashGravity(0.8f);
	left_hori->SplitHorizontally(m_library, property);

	left_vert->SetSashGravity(0.15f);
	left_vert->SplitVertically(left_hori, m_stage);

	right_vert->SetSashGravity(0.85f);
	right_vert->SplitVertically(left_vert, toolbar);
}

}