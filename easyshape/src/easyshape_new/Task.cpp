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
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
//	delete m_root;
}

void Task::load(const char* filepath)
{
	d2d::FileNameParser::Type type = d2d::FileNameParser::getFileType(filepath);
	if (type == d2d::FileNameParser::e_shape) {
		m_stage->LoadFromFile(filepath);		
	} else if (type == d2d::FileNameParser::e_image 
		|| type == d2d::FileNameParser::e_complex
		|| type == d2d::FileNameParser::e_texture) {
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		m_stage->SetSymbolBG(symbol);
		symbol->Release();
	}
}

void Task::store(const char* filepath) const
{
	std::string fixed = filepath;
	d2d::FileNameParser::Type type = d2d::FileNameParser::getFileType(fixed);
	if (type != d2d::FileNameParser::e_shape) {
		wxString tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
		fixed = d2d::FilenameTools::getFilenameAddTag(fixed, tag, "json");
	}
	m_stage->StoreToFile(fixed.c_str());
}

bool Task::isDirty() const
{
	return true;
}

void Task::clear()
{
 	m_library->clear();
 	m_stage->clear();
 	m_library->Refresh();
 	m_stage->Refresh();
}

void Task::getAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{

}

const d2d::EditPanel* Task::getEditPanel() const
{
	return NULL;
}

void Task::InitLayout(wxFrame* parent)
{
	wxSplitterWindow* right_vert = new wxSplitterWindow(parent);
	wxSplitterWindow* left_vert = new wxSplitterWindow(right_vert);
	wxSplitterWindow* left_hori = new wxSplitterWindow(left_vert);

	m_library = new d2d::LibraryPanel(left_hori);
	wxWindow* nb = m_library->getNotebook();
	m_library->addPage(new d2d::LibraryImagePage(nb));
	m_library->addPage(new ecomplex::LibraryPage(nb));
	m_library->addPage(new libanim::LibraryPage(nb));
	m_library->addPage(new etexture::LibraryPage(nb));

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(left_hori);
	m_stage = new libshape::StagePanel(left_vert, parent, m_library);
	m_library->setCanvas(m_stage->getCanvas());
	d2d::ToolbarPanel* toolbar = new libshape::ToolbarPanel(right_vert, property, m_stage);

	left_hori->SetSashGravity(0.8f);
	left_hori->SplitHorizontally(m_library, property);

	left_vert->SetSashGravity(0.15f);
	left_vert->SplitVertically(left_hori, m_stage);

	right_vert->SetSashGravity(0.85f);
	right_vert->SplitVertically(left_vert, toolbar);
}

}