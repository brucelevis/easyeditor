#include "StageGLCanvas.h"
#include "StagePanel.h"
#include "dataset/Sprite.h"
#include "tools/Render.h"

namespace eanim
{

StageGLCanvas::StageGLCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
{
}

void StageGLCanvas::onDraw()
{
	drawbackground();
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(DrawVisitor(), 
		d2d::e_visible);
	m_editPanel->drawEditTemp();
}

void StageGLCanvas::drawbackground() const
{
	const float hWidth = GetSize().GetWidth() * 0.5f,
		hHeight = GetSize().GetHeight() * 0.5f;

 	glColor3f(0.0f, 0.0f, 0.0f);
 	glBegin(GL_LINES);
		glVertex2f(-hWidth, 0.0f);	glVertex2f(hWidth, 0.0f);
		glVertex2f(0.0f, -hHeight);	glVertex2f(0.0f, hHeight);
 	glEnd();
}

//////////////////////////////////////////////////////////////////////////
// class StageGLCanvas::DrawVisitor
//////////////////////////////////////////////////////////////////////////

StageGLCanvas::DrawVisitor::DrawVisitor()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void StageGLCanvas::DrawVisitor::visit(d2d::ICloneable* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);

	const d2d::Vector& pos = sprite->getPosition();

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0.0f);
	glRotatef(sprite->getAngle() * d2d::TRANS_RAD_TO_DEG, 0, 0, 1);

	Render::drawSpriteCenter(static_cast<Sprite*>(sprite));

	glPopMatrix();

	bFetchNext = true;
}

} // eanim