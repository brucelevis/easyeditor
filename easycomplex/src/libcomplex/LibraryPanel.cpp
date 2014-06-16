#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easymesh.h>
#include <easyscale9.h>

namespace ecomplex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	addPage(new d2d::LibraryImagePage(getNotebook()));
	addPage(new LibraryPage(getNotebook()));
	addPage(new anim::LibraryPage(getNotebook()));
	addPage(new escale9::LibraryPage(getNotebook()));
	addPage(new d2d::LibraryFontBlankPage(getNotebook()));
	addPage(new emesh::LibraryPage(getNotebook()));
	addPage(new d2d::LibraryScriptsPage(getNotebook()));
	addPage(new d2d::LibraryFontPage(getNotebook()));
}

} // ecomplex