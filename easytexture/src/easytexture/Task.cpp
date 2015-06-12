#include "Task.h"

#include <easytexture.h>

namespace etexture
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
	, m_toolbar(NULL)
{
	InitLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filename)
{
	m_stage->GetSymbol()->loadFromFile(filename);
}

void Task::store(const char* filename) const
{
	FileSaver::Store(filename, m_stage->GetSymbol());
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_library->Clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_vert_splitter = new wxSplitterWindow(right_splitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(left_vert_splitter);

	m_library = new LibraryPanel(left_hori_splitter);
	m_property = new d2d::PropertySettingPanel(left_hori_splitter);

	left_hori_splitter->SetSashGravity(0.5f);
	left_hori_splitter->SplitHorizontally(m_library, m_property);

	StagePanel* stage;
	m_stage = stage = new StagePanel(left_vert_splitter, m_parent, m_library);
	m_library->SetCanvas(m_stage->getCanvas());

	left_vert_splitter->SetSashGravity(0.2f);
	left_vert_splitter->SplitVertically(left_hori_splitter, m_stage);

	m_toolbar = new ToolbarPanel(right_splitter, m_stage, m_property);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_vert_splitter, m_toolbar);

	m_root = right_splitter;
}

}