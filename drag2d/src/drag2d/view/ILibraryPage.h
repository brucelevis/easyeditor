#ifndef _DRAG2D_I_LIBRARY_PAGE_H_
#define _DRAG2D_I_LIBRARY_PAGE_H_

#include <wx/wx.h>

#include "interfaces.h"

namespace d2d
{

class ListItem;
class Symbol;
class LibraryList;

class ILibraryPage : public wxWindow
{
public:
	ILibraryPage(wxWindow* parent, const wxString& name, bool isStatic = false);

	virtual bool IsHandleSymbol(Symbol* symbol) const = 0;

	virtual void Clear();

	virtual bool LoadFromConfig() { return false; }
	virtual void LoadDefaultSymbol() {}

	virtual void OnActive() {}

	void Traverse(Visitor& visitor) const;

	const wxString& GetPageName() const { return m_name; }

	ListItem* GetItem(int index = -1) const;
	Symbol* GetSymbol(int index = -1) const;

	void ReloadTexture() const;

	LibraryList* GetList() { return m_list; }

	void AddItem(ListItem* item);

protected:
	virtual void InitLayout(bool draggable = true);
	virtual void InitLayoutExtend(wxSizer* sizer) {}

	virtual void OnAddPress(wxCommandEvent& event);
	virtual void OnDelPress(wxCommandEvent& event);

protected:
	void InitButtons(wxSizer* sizer);

	bool LoadFromConfig(const std::string& key);

	void OnAddPress(const std::string& type);

protected:
	wxString m_name;

	bool m_is_static;

	wxButton *m_btn_add, *m_btn_del;

	LibraryList* m_list;

}; // ILibraryPage

}

#endif // _DRAG2D_I_LIBRARY_PAGE_H_