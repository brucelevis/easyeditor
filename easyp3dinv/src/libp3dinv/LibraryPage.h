#ifndef _EASYP3DINV_LIBRARY_PAGE_H_
#define _EASYP3DINV_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace ep3dinv
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYP3DINV_LIBRARY_PAGE_H_