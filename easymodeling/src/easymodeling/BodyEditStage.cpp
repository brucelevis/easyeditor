
#include "BodyEditStage.h"

#include <easymodeling.h>

using namespace emodeling;

BodyEditStage::BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame,
							 d2d::ISymbol* symbol)
	: libshape::StagePanel(parent, frame, symbol)
{
}

BodyEditStage::BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame,
							 d2d::ISprite* sprite)
	: libshape::StagePanel(parent, frame, sprite)
{
}

void BodyEditStage::removeShape(d2d::IShape* shape)
{
	libshape::StagePanel::removeShape(shape);

	if (m_sprite && m_sprite->getUserData())
	{
		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->getUserData());
		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
		{
			if (bd->fixtures[i]->shape == shape)
			{
				delete bd->fixtures[i];
				bd->fixtures.erase(bd->fixtures.begin() + i);
				break;
			}
		}
	}
}

void BodyEditStage::insertShape(d2d::IShape* shape)
{
	libshape::StagePanel::insertShape(shape);

	if (m_sprite && m_sprite->getUserData())
	{
		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->getUserData());
		libmodeling::Fixture* fixture = new libmodeling::Fixture;
		fixture->body = bd;
		shape->retain();
		fixture->shape = shape;
		bd->fixtures.push_back(fixture);
	}
}

void BodyEditStage::loadShapes()
{
	libshape::StagePanel::loadShapes();

	if (m_sprite && m_sprite->getUserData())
	{
		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->getUserData());
		m_shapes.reserve(bd->fixtures.size());
		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
		{
			d2d::IShape* shape = bd->fixtures[i]->shape;
			shape->retain();
			m_shapes.push_back(shape);
		}
	}
}
