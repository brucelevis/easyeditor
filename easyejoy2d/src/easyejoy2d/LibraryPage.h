#ifndef _EASYEJOY2D_LIBRARY_PAGE_H_
#define _EASYEJOY2D_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace eejoy2d
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

#endif // _EASYEJOY2D_LIBRARY_PAGE_H_