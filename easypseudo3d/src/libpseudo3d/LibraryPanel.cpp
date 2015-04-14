#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace epseudo3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	addPage(new d2d::LibraryImagePage(nb));
}

}