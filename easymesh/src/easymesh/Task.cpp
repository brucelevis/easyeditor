#include "Task.h"

#include <easymesh.h>

namespace emesh
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filepath)
{
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_mesh)) {
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		Symbol* msymbol = static_cast<Symbol*>(symbol);
		m_stage->SetSymbol(msymbol);
		
		m_library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
		symbol->Release();
		m_stage->getCanvas()->resetViewport();
	}
}

void Task::store(const char* filepath) const
{
	FileIO::store(filepath, m_stage->GetSymbol());
	m_stage->onSave();
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

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
	wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
	wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar)
{
	library = m_library = new d2d::LibraryPanel(leftHorizontalSplitter);
	m_library->AddPage(new d2d::LibraryImagePage(m_library->GetNotebook()));

	property = m_property = new d2d::PropertySettingPanel(leftHorizontalSplitter);

	stage = m_stage = new emesh::StagePanel(leftVerticalSplitter, m_parent, m_library);
//	m_property->setPropertySetting(new complex::PropertySetting(m_stage, m_sstage->getSymbol()));

	toolbar = new emesh::ToolbarPanel(rightVerticalSplitter, m_stage, true, NULL);
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	wxWindow *library, *property, *stage, *toolbar;
	initWindows(leftHorizontalSplitter, leftVerticalSplitter, rightVerticalSplitter,
		library, property, stage, toolbar);

	if (library || property)
	{
		if (library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.5f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.2f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.85f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;
}

}