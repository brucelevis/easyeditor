#ifndef _LIBSHAPE_LIBRARY_PAGE_H_
#define _LIBSHAPE_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace libshape
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

#endif // _LIBSHAPE_LIBRARY_PAGE_H_