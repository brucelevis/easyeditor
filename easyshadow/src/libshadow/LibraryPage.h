#ifndef _EASYSHADOW_LIBRARY_PAGE_H_
#define _EASYSHADOW_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace eshadow
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void onAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYSHADOW_LIBRARY_PAGE_H_