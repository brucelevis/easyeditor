#ifndef _EASYSHAPE_EDIT_POLYLINES_OP_H_
#define _EASYSHAPE_EDIT_POLYLINES_OP_H_

#include <ee/SelectShapesOP.h>

#include <map>

namespace eshape
{

class ChainShape;
class EditPolylinesCMPT;

class EditPolylinesOP : public ee::SelectShapesOP
{
public:
	EditPolylinesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		EditPolylinesCMPT* cmpt);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void simplify();
	void updateFromSimplified();

private:
	void clearBuffer();

private:
	class UpdateChainVisitor : public ee::Visitor
	{
	public:
		virtual void Visit(Object* object, bool& next);
	}; // UpdateChainVisitor

	class UpdateBufferVisitor : public ee::Visitor
	{
	public:
		UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer);

		virtual void Visit(Object* object, bool& next);

	private:
		std::map<ChainShape*, ChainShape*>& m_simplifyBuffer;

	}; // UpdateBufferVisitor

	class OffsetVisitor : public ee::Visitor
	{
	public:
		OffsetVisitor(const ee::Vector& offset);

		virtual void Visit(ee::Object* object, bool& next);

	private:
		const ee::Vector& m_offset;

	}; // OffsetVisitor

private:
	EditPolylinesCMPT* m_cmpt;

	std::map<ChainShape*, ChainShape*> m_simplifyBuffer;

	ee::Vector m_lastPos;

	bool m_bDirty;

}; // EditPolylinesOP

}

#endif // _EASYSHAPE_EDIT_POLYLINES_OP_H_