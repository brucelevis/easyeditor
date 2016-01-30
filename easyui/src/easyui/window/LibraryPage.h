#ifndef _EASYUI_WINDOW_LIBRARY_PAGE_H_
#define _EASYUI_WINDOW_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(d2d::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage 

}
}

#endif // _EASYUI_WINDOW_LIBRARY_PAGE_H_