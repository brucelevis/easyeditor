#ifndef _EASYEDITOR_LIBRARY_PANEL_H_
#define _EASYEDITOR_LIBRARY_PANEL_H_

#include "Observer.h"

#include <wx/wx.h>
#include <wx/notebook.h>

#include <vector>

namespace ee
{

class Symbol;
class LibraryPage;
class SymbolMgr;
class LibraryList;
class Visitor;

class LibraryPanel : public wxPanel, public Observer
{
public:
	LibraryPanel(wxWindow* parent);

	virtual void OnPageChanged(wxBookCtrlEvent& event);
	virtual void OnPageChanging(wxBookCtrlEvent& event);

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) { return true; }

	void Clear();

	void ReloadTexture() const;

	void AddPage(LibraryPage* page, const char* name = NULL);

	Symbol* GetSymbol(int index = -1) const;

	void LoadFromSymbolMgr(const SymbolMgr& mgr);
	void LoadSymbol(Symbol* symbol);

	void Traverse(Visitor& visitor) const;

	wxWindow* GetNotebook() { return m_notebook; }

	bool AddSymbol(Symbol* symbol);

	void LoadFromConfig();
	void LoadDefaultSymbol();

	LibraryPage* GetCurrPage() { return m_selected; }
	const LibraryPage* GetCurrPage() const { return m_selected; }
	void SetCurrPage(int idx);
 
protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

protected:
	wxNotebook* m_notebook;

	std::vector<LibraryPage*> m_pages;

	LibraryPage* m_selected;

}; // LibraryPanel

}

#endif // _EASYEDITOR_LIBRARY_PANEL_H_