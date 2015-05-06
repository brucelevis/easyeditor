#include "DrawPolygon2CMPT.h"
#include "EditPolylineOP.h"
#include "DrawPolygonEdgeOP.h"
#include "DrawPencilPolygonCMPT.h"

namespace libshape
{

DrawPolygon2CMPT::DrawPolygon2CMPT(wxWindow* parent, const wxString& name, d2d::EditPanel* editPanel, 
								   d2d::MultiShapesImpl* shapesImpl, d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel, wxT("Tools"))
{
 	// draw polygon with pen, node capture
	{
		d2d::OneFloatValueCMPT* cmpt = new d2d::OneFloatValueCMPT(this, "pen", editPanel, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditPolylineOP<DrawPolygonEdgeOP, d2d::SelectShapesOP>
			(editPanel, shapesImpl, propertyPanel, cmpt, cmpt);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
 	// draw polygon with pencil, simplify threshold
	{
		addChild(new DrawPencilPolygonCMPT(this, wxT("pencil"), editPanel, shapesImpl));
	}
}

wxSizer* DrawPolygon2CMPT::initLayout()
{
	return initChildrenLayout();
}

} // d2d