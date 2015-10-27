#ifndef _EASYUI_OVERALL_LIBRARY_PAGE_H_
#define _EASYUI_OVERALL_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace eui
{
namespace overall
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage 

}
}

#endif // _EASYUI_OVERALL_LIBRARY_PAGE_H_